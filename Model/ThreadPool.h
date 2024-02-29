#pragma once
#include <thread>
#include <future>
#include <queue>
#include <functional>
#include <condition_variable>

namespace MD {
	class ThreadPool {
	public:
		ThreadPool(size_t threadNum) {
			for (size_t i = 0; i < threadNum; i++) {
				m_Threads.emplace_back([this]() {
					for (;;) {
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(m_Mtx);
							while (m_Tasks.empty() && !m_Stop)
								m_Condition.wait(lock);
							if (m_Tasks.empty() && m_Stop)
								return;
							task = std::move(m_Tasks.front());
							m_Tasks.pop();
						}
						task();
					}
				});
			}
		}
		
		template<typename F, typename... Args>
		auto Push(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
			using returnType = typename std::result_of<F(Args...)>::type;
			// std::packaged_task的模板中要传入函数变量,例如void(),这里传入的就是returnType()
			auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
			std::future<returnType> res = task->get_future();
			{
				if (m_Stop)
					throw std::runtime_error("queue on stopped threadPool");
				std::unique_lock<std::mutex>(m_Mtx);
				m_Tasks.emplace([task]() { (*task)(); });
			}

			m_Condition.notify_one();
			return res;
		}

		~ThreadPool() {
			{
				std::unique_lock<std::mutex> lock(m_Mtx);
				m_Stop = true;
			}
			m_Condition.notify_all();
			for (std::thread& thr : m_Threads)
				thr.join();
		}

	private:
		bool m_Stop = false;
		std::mutex m_Mtx;
		std::vector<std::thread> m_Threads;
		std::queue<std::function<void()>> m_Tasks;
		std::condition_variable m_Condition;
	};
}