
#include <cstdio>

#include <cstddef>
#include <cassert>
#include <cstdlib>

template <std::size_t block_size>
class mx_free_list_naive {

	private:
		struct listnode {
			listnode *next;
			void *block; };

	public:

		mx_free_list_naive(std::size_t pre_alloc_count) : m_heap_alloc_count(0), m_freecount(0) {
			this->m_list_head = new listnode();
			this->m_list_head->next = nullptr;
			this->m_list_head->block = nullptr;

			for (std::size_t i = 0; i < pre_alloc_count; i++)
				this->attach_node(this->new_node());
		}

		~mx_free_list_naive() {

			// memory leak detected
			assert(this->m_heap_alloc_count == this->m_freecount);

			while(this->m_freecount) {
				listnode *node_to_free = this->fetch_node();
				::free(node_to_free);
			}

			delete this->m_list_head;
		}

		void *alloc() {
			if (this->m_freecount) {
				listnode *ret = this->fetch_node();
				return ret->block;
			} else {
				listnode *ret = this->new_node();
				return ret->block;
			}
		}

		void free(void *raw_block) {
			assert(raw_block != nullptr);

			listnode *node_to_attach = reinterpret_cast<listnode *>(((char *)raw_block) - sizeof(listnode));
			assert(node_to_attach->block != nullptr);
			
			this->attach_node(node_to_attach);
		}

	private:

		void attach_node(listnode *node_to_attach) {
			listnode *nn = m_list_head->next;
			assert(node_to_attach->block != nullptr);

			m_list_head->next = node_to_attach;
			node_to_attach->next = nn;
			this->m_freecount++;
		}

		listnode *fetch_node() {
			listnode *nn = this->m_list_head->next;
			this->m_list_head->next = nn->next;
			nn->next = nullptr;
			this->m_freecount--;
			return nn;
		}

		listnode *new_node() {
			listnode *node = reinterpret_cast<listnode *>(malloc(sizeof(listnode) + block_size));
			assert(node != nullptr);

			node->next = nullptr;
			node->block = reinterpret_cast<void *>((char *)node + sizeof(listnode));
			this->m_heap_alloc_count++;

			return node;
		}

	// private data
	private:

		std::size_t m_heap_alloc_count;
		std::size_t m_freecount;
		listnode *m_list_head;
};

struct test_struct {
	int a;
	char b;
};

int main(int argc, char const *argv[]) {
	
	printf("size of test struct: %lu\n", sizeof(test_struct));

	mx_free_list_naive<sizeof(test_struct)> pool(10);

	printf("-------\n");

	void *pa = pool.alloc();
	printf("alloc pa: 0x%lx\n", (unsigned long)pa);

	pool.free(pa);
	pa = nullptr;
	printf("pa freed: 0x%lx\n", (unsigned long)pa);

	pa = pool.alloc();
	printf("alloc pa: 0x%lx\n", (unsigned long)pa);
	pool.free(pa);

	pa = pool.alloc();
	printf("alloc pa: 0x%lx\n", (unsigned long)pa);
	pool.free(pa);

	pa = pool.alloc();
	printf("alloc pa: 0x%lx\n", (unsigned long)pa);
	pool.free(pa);


	return 0;
}
