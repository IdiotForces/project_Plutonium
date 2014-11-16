#include <stdio.h>

int max(int x, int y);

int main() {
	int a = 10, b = 20;

	int c = 0;

	c = max(a, b);

	printf("Max(a, b) = %d.\n", c);

	return 0;
}

int max(int x, int y) {
	if (x > y) return x;
	else return y;
}