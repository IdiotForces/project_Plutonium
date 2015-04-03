#ifndef HEADER_THREADS
#define HEADER_THREADS

#include <pthread.h>

#include <vector>
#include <functional>
#include <queue>

class threads;

struct worker_info {
	threads *parent = nullptr;
};

void *worker_func(void *data);

class threads {

	friend void *worker_func(void *data);

	public:

 		threads(std::size_t n_threads) : m_count_worker(n_threads), m_count_running(0), m_is_closing(false) {
			this->m_info.parent = this;
			for (std::size_t i = 0; i < n_threads; i++) {
				pthread_t t;
				pthread_create(&t, nullptr, worker_func, reinterpret_cast<void *>(&this->m_info));
				this->m_threads.push_back(t);
			}

			pthread_mutex_init(&this->m_task_mutex, nullptr);
			pthread_cond_init(&this->m_task_cond, nullptr);

			pthread_mutex_init(&this->m_meta_mutex, nullptr);
			pthread_cond_init(&this->m_meta_cond, nullptr);
		}
	
		~threads() {
			pthread_cond_destroy(&this->m_meta_cond);
			pthread_mutex_destroy(&this->m_meta_mutex);
			
			pthread_cond_destroy(&this->m_task_cond);
			pthread_mutex_destroy(&this->m_task_mutex);
		}

		void add_work(std::function<void ()> work) {
			pthread_mutex_lock(&this->m_task_mutex);
			this->m_task_queue.push(work);
			pthread_mutex_unlock(&this->m_task_mutex);
			pthread_cond_signal(&this->m_task_cond);
		}

		void wait_until_finish() {
			while (true) {
				pthread_mutex_lock(&this->m_meta_mutex);
				pthread_mutex_lock(&this->m_task_mutex);
				if ((this->m_task_queue.size() == 0) && (this->m_count_running == 0)) break;
				pthread_mutex_unlock(&this->m_task_mutex);
				pthread_cond_wait(&this->m_meta_cond, &this->m_meta_mutex);
				pthread_mutex_unlock(&this->m_meta_mutex);
			}
			pthread_mutex_unlock(&this->m_task_mutex);
			pthread_mutex_unlock(&this->m_meta_mutex);
		}

		void finish_and_destroy() {
			this->m_is_closing = true;
			wait_until_finish();
//			printf("destroying threads\n");

			void *t;
			pthread_cond_broadcast(&this->m_task_cond);
			for (std::size_t i = 0; i < m_count_worker; i++) {
				pthread_join(this->m_threads[i], &t); }
		}

		std::size_t job_in_queue() {
			pthread_mutex_lock(&this->m_task_mutex);
			std::size_t ret = this->m_task_queue.size();
			pthread_mutex_unlock(&this->m_task_mutex);
			return ret;
		}

		std::size_t job_on_the_run() {
			pthread_mutex_lock(&this->m_meta_mutex);
			std::size_t ret = this->m_count_running;
			pthread_mutex_unlock(&this->m_meta_mutex);
			return ret;
		}

	private:

		std::vector<pthread_t> m_threads;
		worker_info m_info;

		std::queue<std::function<void ()>> m_task_queue;
		pthread_mutex_t m_task_mutex;
		pthread_cond_t m_task_cond;

		pthread_mutex_t m_meta_mutex;
		pthread_cond_t m_meta_cond;

		std::size_t m_count_worker;
		std::size_t m_count_running;

		bool m_is_closing;

};

void *worker_func(void *data) {
	
	threads *parent = reinterpret_cast<worker_info *>(data)->parent;

	while (!parent->m_is_closing) {
		pthread_mutex_lock(&parent->m_task_mutex);

		while (parent->m_task_queue.empty() && !parent->m_is_closing)
			pthread_cond_wait(&parent->m_task_cond, &parent->m_task_mutex);

		if (parent->m_is_closing) {
			pthread_mutex_unlock(&parent->m_task_mutex);
			break;
		} else {
//			printf("executing task...\n");
			std::function<void ()> task = parent->m_task_queue.front();
			parent->m_task_queue.pop();
			pthread_mutex_unlock(&parent->m_task_mutex);

			pthread_mutex_lock(&parent->m_meta_mutex);
			parent->m_count_running++;
			pthread_mutex_unlock(&parent->m_meta_mutex);

			task();

			pthread_mutex_lock(&parent->m_meta_mutex);
			parent->m_count_running--;
			pthread_mutex_unlock(&parent->m_meta_mutex);
			pthread_cond_broadcast(&parent->m_meta_cond);
		}

	}

	return nullptr;
}

#endif