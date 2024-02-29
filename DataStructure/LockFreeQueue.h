#pragma once
#include <atomic>
#include <optional>

namespace DS {
	
	#define EMPTY_QUEUE nullptr

	template<class T>
	class LockFreeQueue {
	private:
		struct QueueNode {
			T Value;
			std::atomic<QueueNode*> Next;

			QueueNode()
				: Value(0), Next(nullptr)
			{}

			QueueNode(T& value)
				: Value(value), Next(nullptr)
			{}
		};

		typedef QueueNode Node;

	public:
		LockFreeQueue() {
			Node* node = new Node();
			m_Head = m_Tail = node;
		}

		~LockFreeQueue() {
			while (Node* deleteNode = m_Head.load()) {
				m_Head.store(deleteNode->Next.load());
				delete deleteNode;
			}
		}

		void Push(T& value) {
			Node* newNode = new Node(value);

			Node* oldTail = nullptr;
			Node* oldNext = nullptr;
			// 虽然每个if之间没有做到原子性, 但是只要一个线程抢占了CAS, 其他线程都会失败
			while(true) {
				oldTail = m_Tail.load(std::memory_order_relaxed);
				oldNext = oldTail->Next.load(std::memory_order_relaxed);
				
				if (oldTail != m_Tail.load(std::memory_order_acquire))
					continue;

				// (1)其他线程可能挂了,导致已经有新的next被加入,但是tail还没来得及更新
				if (oldNext != nullptr) {
					m_Tail.compare_exchange_strong(oldTail, oldNext);
					continue;
				}
				 
				// 在这里额外加入一个CAS判断就能实现double CAS
				if (oldTail->Next.compare_exchange_strong(oldNext, newNode))
					break;
			}

			// (2)这里将置位tail,但是有可能在while循环break之后,其他线程插入阻断了,那么有可能在其他线程中就帮忙置位tail了(在(1)中)
			m_Tail.compare_exchange_strong(oldTail, newNode);
		}

		Node* Pop() {
			Node* oldHead = nullptr;
			Node* oldTail = nullptr;
			Node* oldNext = nullptr;
			while (true) {
				oldHead = m_Head.load(std::memory_order_relaxed);
				oldTail = m_Tail.load(std::memory_order_relaxed);
				oldNext = oldHead->Next.load(std::memory_order_relaxed);

				if (oldHead != m_Head.load(std::memory_order_acquire))
					continue;

				if (oldHead == oldTail && oldNext == nullptr)
					return EMPTY_QUEUE;

				// tail还没动,这里就帮他动了
				if (oldHead == oldTail && oldNext != nullptr) {
					m_Tail.compare_exchange_strong(oldTail, oldNext);
					continue;
				}

				if (m_Head.compare_exchange_strong(oldHead, oldNext)) {
					// delete oldHead;
					return oldHead;
				}
			}
		}
	
	private:
		std::atomic<Node*> m_Head;
		std::atomic<Node*> m_Tail;
	};
}