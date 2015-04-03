
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

#include "Platform.h"
#include "threads.h"

threads mx_pool(8);

namespace {
	uint64_t CUBEMUTEW_NOT_USED rate = -1; // not guaranteed to be available
	double rate_d = -1.; }

#ifdef CUBE_PLATFORM_DARWIN
#include <mach/mach_time.h>
#elif defined(CUBE_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(CUBE_PLATFORM_POSIX)
#include <time.h>
#endif

void timer_init() {

#ifdef CUBE_PLATFORM_DARWIN
	mach_timebase_info_data_t rate_mach;
	mach_timebase_info(&rate_mach);
	rate_d = 1000000. * (rate_mach.numer / (double)rate_mach.denom);
#elif defined(CUBE_PLATFORM_WINDOWS)
	LARGE_INTEGER f;
 
	QueryPerformanceFrequency(&f);
	rate = f.QuadPart;
	rate_d = (double)rate;
#elif defined(CUBE_PLATFORM_POSIX)
	
#endif
}

double time_in_millisec() {
	uint64_t now = -1;
	
#ifdef CUBE_PLATFORM_DARWIN
	now = mach_absolute_time();
#elif defined(CUBE_PLATFORM_WINDOWS)
	LARGE_INTEGER f;
	QueryPerformanceCounter(&f);
	now = f.QuadPart;
#elif defined(CUBE_PLATFORM_POSIX)
	
#endif
	
	return (now / rate_d);
}

class qs_pivot_policy {
	public:
		inline std::size_t operator() (std::size_t begin, std::size_t end)
			{ return begin; }
};

using qs_pivot_policy_default = qs_pivot_policy;

namespace _impl {

template <typename T>
void insertion_sort(std::vector<T>& src, std::size_t begin, std::size_t end) {
	for (std::size_t i = 0, sptr = 0; i < end; i++, sptr++)
		for (std::size_t j = sptr+1; j > 0 && src[j] < src[j-1]; j--)
			std::swap(src[j], src[j-1]);
}

template <typename ContainerT>
void quick_sort(ContainerT& src, std::size_t begin, std::size_t end) {
	if (begin >= end) return;

	std::size_t pivotf = end;
	for (std::size_t i = end; i > begin; i--)
		if (src[i] > src[begin])
			std::swap(src[i], src[pivotf--]);

	std::swap(src[begin], src[pivotf]);

	if (pivotf) quick_sort(src, begin, pivotf-1);
	return quick_sort(src, pivotf+1, end);
}

template <typename ContainerT, std::size_t threshold = 131072>
void quick_sort_parallel_thread(ContainerT *src, std::size_t begin, std::size_t end) {
	if (begin >= end) return;

	if (end - begin <= threshold)
		return quick_sort(*src, begin, end);

	std::size_t pivotf = end;
	for (std::size_t i = end; i > begin; i--)
		if ((*src)[i] > (*src)[begin])
			std::swap((*src)[i], (*src)[pivotf--]);

	std::swap((*src)[begin], (*src)[pivotf]);

	if (pivotf)
		mx_pool.add_work([src, begin, pivotf] { quick_sort_parallel_thread(src, begin, pivotf-1); });
	mx_pool.add_work([src, end, pivotf] { quick_sort_parallel_thread(src, pivotf+1, end); });
}

template <typename ContainerT>
void quick_sort_parallel(ContainerT& src, std::size_t begin, std::size_t end) {
	mx_pool.add_work([&src, begin, end] { quick_sort_parallel_thread(&src, begin, end); });
	mx_pool.wait_until_finish();
}

template <typename ContainerT, class pivot_policy = qs_pivot_policy_default, std::size_t threshold = 8>
void quick_sorte(ContainerT& src, std::size_t begin, std::size_t end) {
	if (begin >= end) return;

	if (end - begin <= threshold)
		return insertion_sort(src, begin, end);

	std::size_t p = pivot_policy()(begin, end);
	std::swap(src[p], src[begin]);

	std::size_t pivotf = end;
	for (std::size_t i = end; i > begin; i--)
		if (src[i] > src[begin])
			std::swap(src[i], src[pivotf--]);

	std::swap(src[begin], src[pivotf]);

	if (pivotf) quick_sort(src, begin, pivotf-1);
	return quick_sort(src, pivotf+1, end);
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

template <typename ContainerT>
inline void quick_sort(ContainerT& src) {
	_impl::quick_sort<ContainerT>(src, 0, src.size()-1); }

template <typename ContainerT, class pivot_policy = qs_pivot_policy_default>
inline void quick_sorte(ContainerT& src) {
	_impl::quick_sorte<ContainerT, pivot_policy>(src, 0, src.size()-1); }

template <typename ContainerT>
inline void quick_sort_parallel(ContainerT& src) {
	_impl::quick_sort_parallel<ContainerT>(src, 0, src.size()-1); }

int main() {
	timer_init();

	int input_t = 0;

	printf("loading file ...\n");
	FILE *fp = fopen("a.txt", "r");

	std::vector<int> t { };
	while (fscanf(fp, "%d", &input_t) != EOF)
		t.push_back(input_t);
	printf("content length: %lu\n", t.size());
	printf("sorted: %d\n", is_sorted(t));

	double time_start = time_in_millisec();

	// insertion_sort(t);
//	quick_sort_parallel(t);
	quick_sort(t);
//	 std::sort(begin(t), end(t));

	double time_ellpased = time_in_millisec() - time_start;
	printf("sorted: %d, time: %.3f ms.\n", is_sorted(t), time_ellpased);

	mx_pool.finish_and_destroy();

	return 0;
}