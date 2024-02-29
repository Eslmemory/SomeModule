#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>

namespace DS {
	template<class T>
	class MutexQueue {
	private:
		struct QueueNode {
			T Value;
			QueueNode* Next;

			QueueNode()
				: Value(0), Next(nullptr)
			{}

			QueueNode(T& value)
				: Value(value), Next(nullptr)
			{}

			std::ostream& operator<< (const std::ostream& cout) {
				cout << Value;
				return cout;
			}
		};

		typedef QueueNode Node;

	public:
		MutexQueue() {
			m_Head = new Node();
			m_Tail = m_Head;
		}

		~MutexQueue() {
		}
		
		void Push(T& val) {
			Node* newNode = new Node(val);
			std::unique_lock<std::mutex> lock(m_Mtx);

			while (m_QueueSize == MAX_SIZE) {
				m_NotFull.wait(lock);
			}

			m_Tail->Next = newNode;
			m_Tail = newNode;
			m_QueueSize++;
			m_NotEmpty.notify_all();
			lock.unlock();
		}

		Node* Pop() {
			Node* res;
			std::unique_lock<std::mutex> lock(m_Mtx);

			// m_NotEmpty.wait(lock);
			while (m_QueueSize == 0) {
				printf("queue is empty\n");
				m_NotEmpty.wait(lock);
			}

			m_QueueSize--;
			Node* tmp = m_Head;
			res = m_Head->Next;   // m_Head是一个为了方便而构造的虚假结点
			m_Head = m_Head->Next;
			delete tmp;
			m_NotFull.notify_all();
			lock.unlock();
			return res;
		}

	private:
		int MAX_SIZE = 100000;
		int m_QueueSize = 0;
		Node* m_Head;
		Node* m_Tail;
		std::mutex m_Mtx;
		std::mutex m_ProducerCountMtx;
		std::mutex m_ConsumerCountMtx;

		std::condition_variable m_NotFull;
		std::condition_variable m_NotEmpty;
	};
}