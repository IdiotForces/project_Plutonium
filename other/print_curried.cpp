
#include <iostream>
#include <functional>

template <typename ... Args>
class print_f { };

template <typename ArgT, typename ... Args>
struct print_f<ArgT, Args ...> {

	print_f<Args ...> operator() (ArgT n) {
		std::cout << n << ' ';
		return print_f<Args ...>();
	}

};

int main(int argc, const char *argv[]) {

	print_f<int, int, int>()(1)(2)(3);

	return 0;
}