// pthread & circular queue

#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

#define BUFSIZE 12

struct final_buffer {
	size_t count;
	size_t start, end;
	int data[BUFSIZE+1];
};

struct final_buffer *fb_init(struct final_buffer *src) {
	assert(src != NULL);

	src->count = src->start = 0;
	src->end = 0;
	memset((void *)src->data, 0, BUFSIZE+1);
	return src;
}

size_t fb_count(struct final_buffer *src) {
	assert(src != NULL);
	assert(src->count == (src->start <= src-> end ? (src->end - src->start) :
							(BUFSIZE + 1 - src->start + src->end)));

	return src->count;
}

void fb_put(struct final_buffer *src, int value) {
	assert(src != NULL);
	assert(fb_count(src) != BUFSIZE);

	src->data[src->end] = value;
	src->end = (src->end + 1) % (BUFSIZE+1);
	src->count++;
}

int fb_get(struct final_buffer *src) {
	assert(src != NULL);
	assert(fb_count(src) != 0);

	src->count--;
	int ret = src->data[src->start];
	src->start = (src->start + 1) % (BUFSIZE+1);
	return ret;
}

void fb_destroy(struct final_buffer *src) {
	(void) src; // to supress "unused parameter" warning.
}

void fb_test() {
	struct final_buffer buf;
	fb_init(&buf);

	for (size_t i = 0; i < 9; i++) {
		fb_put(&buf, i);
		printf("len: %lu\n", fb_count(&buf));
	}
	for (size_t i = 0; i < 6; i++) {
		int t = fb_get(&buf);
		printf("%d, len: %lu\n", t, fb_count(&buf));
	}
	for (size_t i = 0; i < 9; i++) {
		fb_put(&buf, i);
		printf("len: %lu\n", fb_count(&buf));
	}
}

typedef int WORKER_TYPE;

#define READER 1
#define EATER 2

struct thread_worker_input {
	unsigned int id;
	WORKER_TYPE type;
	const char *path;
	struct final_buffer * buf;
};

struct final_buffer buf1, buf2;
pthread_mutex_t mutexes[2];

int thread_running[2] = { 0 };
struct thread_worker_input array_args[] = { { 0, READER, "1.dat", &buf1 },
										{ 1, READER, "2.dat", &buf2 }, { 2, EATER, NULL, NULL } };

void *thread_worker(void *arg_raw);

int main() {

	fb_init(&buf1);
	fb_init(&buf2);
	for (size_t i = 0; i < 2; i++)
		pthread_mutex_init(&mutexes[i], NULL);

	pthread_t workers[3];
	for (size_t i = 0; i < 2; i++) // set running flag
		thread_running[i] = 1;
	for (size_t i = 0; i < 3; i++) // spawn threads
		pthread_create(&workers[i], NULL, thread_worker, (void *)&array_args[i]);
	for (size_t i = 0; i < 3; i++)
		pthread_join(workers[i], NULL);

	for (size_t i = 0; i < 2; i++)
		pthread_mutex_destroy(&mutexes[i]);
	fb_destroy(&buf1);
	fb_destroy(&buf2);

	return 0;
}

void *thread_worker(void *arg_raw) {
	struct thread_worker_input *args = (struct thread_worker_input *)arg_raw;

	if (args->type == READER) {

		assert(args->path != NULL);
		assert(args->buf != NULL);

		FILE *fp = fopen(args->path, "r");
		assert(fp != NULL);

		int t;
		while (1) {
			if (fscanf(fp, "%d", &t) == EOF)
				break;
			pthread_mutex_lock(&mutexes[args->id]); {
				fb_put(args->buf, t);
				// printf("%d putting %d ...\n", args->id, t);
			}
			pthread_mutex_unlock(&mutexes[args->id]);
		}

		thread_running[args->id] = 0;

	} else {

		while (1) {
			int time_to_leave = 0;

			for (size_t i = 0; i < 2; i++)
				pthread_mutex_lock(&mutexes[i]); {
				while (fb_count(&buf1) && fb_count(&buf2)) {
					int a = fb_get(&buf1), b = fb_get(&buf2);
					printf("%d + %d = %d, %d * %d = %d.\n", a, b, a + b, a, b, a * b);
				}
				if (!(thread_running[0] || thread_running[1]))
					time_to_leave = 1;
			}
			for (size_t i = 0; i < 2; i++)
				pthread_mutex_unlock(&mutexes[i]);

			if (time_to_leave)
				break;
		}

	}

	return NULL;
}
