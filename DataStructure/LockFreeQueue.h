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
			// ��Ȼÿ��if֮��û������ԭ����, ����ֻҪһ���߳���ռ��CAS, �����̶߳���ʧ��
			while(true) {
				oldTail = m_Tail.load(std::memory_order_relaxed);
				oldNext = oldTail->Next.load(std::memory_order_relaxed);
				
				if (oldTail != m_Tail.load(std::memory_order_acquire))
					continue;

				// (1)�����߳̿��ܹ���,�����Ѿ����µ�next������,����tail��û���ü�����
				if (oldNext != nullptr) {
					m_Tail.compare_exchange_strong(oldTail, oldNext);
					continue;
				}
				 
				// ������������һ��CAS�жϾ���ʵ��double CAS
				if (oldTail->Next.compare_exchange_strong(oldNext, newNode))
					break;
			}

			// (2)���ｫ��λtail,�����п�����whileѭ��break֮��,�����̲߳��������,��ô�п����������߳��оͰ�æ��λtail��(��(1)��)
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

				// tail��û��,����Ͱ�������
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