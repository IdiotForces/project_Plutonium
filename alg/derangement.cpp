// by seCOnDatkE 2014.
//
// including:
//
//   * an algorithm to list all permutations
//   * filter derangements from permutations
//
// dependency:
//
//   * C++ 11
//
// changelog:
//
//   * 2014.10.26 initial commit.
//
//   http://github.com/secondwtq/project_Plutonium
//
// reference:
//
//   * http://www.cppblog.com/goal00001111/articles/67230.html

#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;

template <typename ContainerT>
void print_array(const ContainerT& src) {
	for (auto item : src)
		cout << item << ' ';
	cout << endl;
}

bool check_is_derange (const vector<unsigned int>& src) {
	for (int i = 0; i < src.size(); i++)
		if (src[i] == i) return false;
	return true;
 }

void generate_perm (vector<vector<unsigned int>> &dest, vector<unsigned int>& tmp, unsigned int ustart) {
	if (tmp.size() - ustart <= 1) {
		if (check_is_derange(tmp))
			dest.push_back(tmp);
		return;
	} else {
		for (int i = ustart; i < tmp.size(); i++) {
			swap(tmp[ustart], tmp[i]);
			generate_perm(dest, tmp, ustart+1);
			swap(tmp[ustart], tmp[i]);
		}
	}
}

template <typename T>
vector<vector<T> > derangement(vector<T>& src) {
	vector<unsigned int> tmp(src.size());
	iota(begin(tmp), end(tmp), 0);

	vector<vector<unsigned int>> tmp_dest;
	generate_perm(tmp_dest, tmp, 0);

	vector<vector<T> > ret;

	for (auto item : tmp_dest) {
		vector<T> vec_to_push;
		for (unsigned int element_idx : item) {
			vec_to_push.push_back(src[element_idx]);
		}
		ret.push_back(vec_to_push);
	}

	return ret;

}

int main(int argc, char const *argv[]) {
	vector<int> test = { 1, 2, 3, 4, 5, 6, 7, 8 };

	auto dest = derangement(test);

	for (auto item : dest)
		print_array(item);

	return 0;
}