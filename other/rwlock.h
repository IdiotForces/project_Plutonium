
#include <cstddef>
#include <pthread.h>

// #define MX_RWLOCK_READER_TAKE_PRIORITY

class mx_rwlock {
	public:

	mx_rwlock() : m_writing(false), m_creader(0), 
			m_creader_waiting(0), m_cwriter_waiting(0) {
		pthread_mutex_init(&this->m_mut, nullptr);

		pthread_cond_init(&this->m_cond_read, nullptr);
		pthread_cond_init(&this->m_cond_write, nullptr);
	}

	~mx_rwlock() {
		pthread_mutex_destroy(&this->m_mut);

		pthread_cond_destroy(&this->m_cond_read);
		pthread_cond_destroy(&this->m_cond_write);
	}

	void read_lock() {
		pthread_mutex_lock(&this->m_mut);
		if (this->m_writing) {
			this->m_creader_waiting++;
			pthread_cond_wait(&this->m_cond_read, &this->m_mut);
			this->m_creader_waiting--;
		}

		this->m_creader++;
		pthread_mutex_unlock(&this->m_mut);
	}

	void write_lock() {
		pthread_mutex_lock(&this->m_mut);
		if (this->m_writing) {
			this->m_cwriter_waiting++;
			pthread_cond_wait(&this->m_cond_write, &this->m_mut);
			this->m_cwriter_waiting--;
		}

		this->m_writing = true;
		pthread_mutex_unlock(&this->m_mut);
	}

	void unlock() {
		pthread_mutex_lock(&this->m_mut);

		if (this->m_writing) {
			this->m_writing = false; }

		#ifndef MX_RWLOCK_READER_TAKE_PRIORITY

		if (this->m_cwriter_waiting) {
			pthread_cond_signal(&this->m_cond_write);
		} else if (this->m_creader_waiting) {
			pthread_cond_signal(&this->m_cond_read); }

		#else

		if (this->m_creader_waiting) {
			pthread_cond_signal(&this->m_cond_read);
		} else if (this->m_cwriter_waiting) {
			pthread_cond_signal(&this->m_cond_write); }
			
		#endif
		
		pthread_mutex_unlock(&this->m_mut);
	}

	private:
		pthread_mutex_t m_mut;

		pthread_cond_t m_cond_read;
		pthread_cond_t m_cond_write;

		bool m_writing;
		std::size_t m_creader;
		std::size_t m_creader_waiting;
		std::size_t m_cwriter_waiting;

};