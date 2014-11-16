#include <vector>
#include <cmath>
#include <cstdio>
using namespace std;

vector<vector<bool> > generate_bin_seq(unsigned int bits) {
	vector<vector<bool> > ret;

	std::size_t count = pow(2, bits);

	vector<int> div;

	for (unsigned int i = 0, t = 1; i < bits; i++, t *= 2)
		div.push_back(t);

	for (std::size_t i = 0; i < count; i++) {
		vector<bool> tv;
		for (unsigned int j = 0; j < bits; j++)
			tv.push_back((bool)((i / div[j]) % 2));
		ret.push_back(tv);
	}

	return ret;
}

int main(int argc, const char *argv[]) {
	unsigned int bits = 8;
	auto t = generate_bin_seq(bits);
	for (int i = 0; i < pow(2, bits); i++) {
		for (int j = bits-1; j >= 0; j--)
			printf("%d", (int)t[i][j]);
		printf("\n");
	}
	return 0;
}
