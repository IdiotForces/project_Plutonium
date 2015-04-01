
#include <iostream>
#include <utility>
#include <vector>

class qs_pivot_policy {
	public:
		inline std::size_t operator() (std::size_t begin, std::size_t end)
			{ return end; }
};

using qs_pivot_policy_default = qs_pivot_policy;

namespace _impl {

template <typename T>
void insertion_sort(std::vector<T>& src, std::size_t begin, std::size_t end) {
	for (std::size_t i = 0, sptr = 0; i < end; i++, sptr++)
		for (std::size_t j = sptr+1; j > 0 && src[j] < src[j-1]; j--)
			std::swap(src[j], src[j-1]);
}

template <typename ContainerT, class pivot_policy = qs_pivot_policy_default, std::size_t threshold = 8>
void quick_sort(ContainerT& src, std::size_t begin, std::size_t end) {
	if (begin >= end) return;

	std::size_t p = pivot_policy()(begin, end);
	std::swap(src[p], src[begin]);

	int pivotf = end;
	for (int i = end; i > begin; i--)
		if (src[i] > src[begin])
			std::swap(src[i], src[pivotf--]);

	std::swap(src[begin], src[pivotf]);

	if (pivotf) quick_sort(src, begin, pivotf-1);
	quick_sort(src, pivotf+1, end);
}

}

template <typename T>
void print_container(const T& src) {
	for (auto i : src)
		std::cout << i << ' ';
	std::cout << std::endl;
}

template <typename ContainerT>
inline bool is_sorted(const ContainerT& src) {
	return std::is_sorted(std::begin(src), std::end(src)); }

template <typename ContainerT>
inline void insertion_sort(ContainerT& src) {
	_impl::insertion_sort(src, 0, src.size()-1); }

template <typename ContainerT, class pivot_policy = qs_pivot_policy_default>
inline void quick_sort(ContainerT& src) {
	_impl::quick_sort<ContainerT, pivot_policy>(src, 0, src.size()-1); }

int main() {

	std::vector<int> t { 79, 15, 35, 66, 178, 241, 190, 1, 95, 10, 227, 168, 16, 228, 11, 19, 16, 117, 197, 144, 189, 229, 153, 150, 191, 30, 160, 36, 6, 21, 223, 250, 133, 218, 104, 123, 176, 96, 47, 93, 182, 13, 173, 22, 126, 1, 54, 6, 100, 38, 27, 101, 176, 252, 127, 235, 218, 33, 250, 65, 128, 163, 239, 42, 35, 6, 173, 113, 165, 20, 72, 145, 131, 103, 54, 39, 210, 183, 169, 24, 251, 228, 174, 130, 54, 194, 10, 99, 199, 168, 207, 11, 251, 183, 194, 67, 225, 235, 108, 59, 201, 227, 2, 52, 126, 112, 246, 242, 67, 223, 89, 223, 142, 7, 247, 255, 134, 17, 49, 152, 62, 10, 152, 157, 175, 42, 78, 222 };
	print_container(t);
	printf("sorted: %d\n", is_sorted(t));
	// insertion_sort(t);
	quick_sort(t);
	print_container(t);
	printf("sorted: %d\n", is_sorted(t));

	return 0;
}