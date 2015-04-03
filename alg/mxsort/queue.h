#ifndef HEADER_THREAD_SAFE_QUEUE
#define HEADER_THREAD_SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class pc_queue {
	public:

		void enqueue(const T& value) {
			std::unique_lock<std::mutex> lock(this->m_mutex);
			this->m_internal.push(value);
			lock.unlock();
			m_cond.notify_one();
		}

		T top() {
			
		}

		T dequeue() {
			std::unique_lock<std::mutex> lock(this->m_mutex);
			while (this->m_internal.empty())
				this->m_cond.wait(lock);
			T ret = this->m_internal.front();
			lock.unlock();
			return ret;
		}

	private:

		std::condition_variable m_cond;
		std::mutex m_mutex;
		std::queue<T> m_internal;

};

#endif