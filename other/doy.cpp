
#ifndef HAS_CPP0X
	#ifdef _MSC_VER
		// Visual Studio doesnot support constexpr AS OF 2013!
		// 	while 2015 version only has partial support.
		#error("Microsoft Visual Studio is not supported since it only has partial C++11 support yet.")
	#elif __cplusplus >= 201103L
		#define HAS_CPP0X
		#pragma GCC warning "Compiling with C++11 ..."
	#else
		#pragma GCC error "The source must be compiled with C++11 support!"
	#endif
#endif

#include <iostream>
#include <string>
#include <type_traits>

const int NUM_MONTHS = 12;

// 'doy' for computing 'countOfDaysEachMonth' and 
// 'daysAtEndOfMonth' statically at the compile time
namespace doy {

	namespace details {

		// template to check whether a certain value (v)
		// 	is inside a list of value of the same type
		//
		// this is the variadic version
		//	maybe we'll need an index_pack/std::tuple version in the future
		//
		// this template is wrapped inside 'doy' structure
		template <typename T, T v, T ... list>
		struct any;

		template <typename T, T v, T item1, T ... list>
		struct any<T, v, item1, list ...> {
			static const bool val = (v == item1) || any<T, v, list ...>::val;
		};

		// specialization as the end of recursion
		// 	the list has no item so it just returns false
		template <typename T, T v>
		struct any<T, v> {
			static const bool val = false;
		};

		// 'day_for_month' template
		// 	specifically, this is for months with 30 days
		// 	see declarations for other months below
		//
		// this template is wrapped inside 'doy' structure
		template <typename T, size_t n, typename is31 = void>
		struct day_for_month {
			static const T val = 30;
		};

		// specialization for months with 31 days
		template <typename T, size_t n>
		struct day_for_month<T, n, typename std::enable_if<any<T, n, 1, 3, 5, 7, 8, 10, 12>::val>::type> {
			static const T val = 31;
		};

		// specialization for February
		template <typename T>
		struct day_for_month<T, 2> {
			static const T val = 28;
		};

	}

	// interfaces wrapped for external use
	// template parameter: T for the type of number to compute
	template <typename T>
	struct doy {

		template <T v, T ... list>
		struct any : details::any<T, v, list ...> { };

		template <size_t n>
		struct day_for_month : details::day_for_month<T, n> { };

		// a simple adder
		template <T a, T b>
		struct add {
			static const T val = a + b;
		};

		template <template <size_t> class accessor, size_t start, size_t end>
		struct accumulate {
			static const T val = add<accessor<start>::val, accumulate<accessor, start+1, end>::val>::val;
		};

		// specialization as the end of recursion
		template <template <size_t> class accessor, size_t start>
		struct accumulate<accessor, start, start> {
			static const T val = accessor<start>::val;
		};

	};

}

// DayOfYear class declaration
class DayOfYear {
private:
	// helpers for doy
	template <size_t n>
	using month = doy::doy<int>::day_for_month<n>;

	// hold the count of days of each month, i.e.: 31 of January,
	// 28 of Feburary, etc.
	// computes statically
	static constexpr const int countOfDaysEachMonth[NUM_MONTHS] = {
		month<1>::val, month<2>::val, month<3>::val, month<4>::val,
		month<5>::val, month<6>::val, month<7>::val, month<8>::val,
		month<9>::val, month<10>::val, month<11>::val, month<12>::val,
	};

	template <size_t idx>
	struct accessor {
		static const int val = DayOfYear::countOfDaysEachMonth[idx];
	};

	template <size_t idx>
	using accu_i = doy::doy<int>::accumulate<accessor, 0, idx>;

public:

	// interfaces

	// hold the last days of each month, i.e.: 31 of January,
	// 59 of Feburary, etc.
	// computes statically
	static constexpr const int daysAtEndOfMonth[NUM_MONTHS] = {
		accu_i<0>::val, accu_i<1>::val, accu_i<2>::val,
		accu_i<3>::val, accu_i<4>::val, accu_i<5>::val,
		accu_i<6>::val, accu_i<7>::val, accu_i<8>::val,
		accu_i<9>::val, accu_i<10>::val, accu_i<11>::val,
	};

	// hold the name for each month, i.e.: 'January', etc.
	// see the definition below
	static const std::string monthName[NUM_MONTHS];

	// constructor
	// accepts an integer parameter as the dat
	DayOfYear(unsigned short d) : day(d) { }

	// print the day in the month–day format
	void print();
	// mutator for 'day'
	void setDay(int day) {
		this->day = day; }

private:
	// private data
	int day;
};

// definitions for static variables
constexpr const int DayOfYear::countOfDaysEachMonth[NUM_MONTHS];
constexpr const int DayOfYear::daysAtEndOfMonth[NUM_MONTHS];

const std::string DayOfYear::monthName[NUM_MONTHS] = {
	"January", "February", "March",
	"April", "May", "June",
	"July", "Augest", "September",
	"October", "November", "December"
};

// prints the day in the month–day format
void DayOfYear::print() {
	unsigned char i_month = 0, rem_day = 0;
	// for every month
	for (size_t i = 0; i < NUM_MONTHS; i++) {
		// if we find which month the day locates on
		if (DayOfYear::daysAtEndOfMonth[i] >= this->day) {
			i_month = i;
			rem_day = day - ((i == 0) ? 0 : DayOfYear::daysAtEndOfMonth[i-1]);
			break;
		}
	}
	// output the date
	std::cout << "Day " << this->day << " would be " <<
		DayOfYear::monthName[i_month] << ' ' << static_cast<unsigned int>(rem_day) << std::endl;
}

int main() {

	// print some instruction messages
	std::cout << "Enter day (0 < day <= 365) to see which month it belongs to."
		<< std::endl << "Enter 0 to exit." << std::endl;

	while (true) {
		unsigned short day;
		// enter a day
		std::cout << "Enter a day: ";
		std::cin >> day;
		if (!day) {	// if user enters 0, we exit
			break; }
		// one 'year' in this context
		// cannot have more than 365 days!
		if (day > 365) {
			std::cout << "Invalid day." << std::endl;
			continue;
		}
		// call the 'print' function
		DayOfYear(day).print();
	}

	return 0;
}
