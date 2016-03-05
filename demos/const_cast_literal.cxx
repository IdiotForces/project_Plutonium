
#include <stdio.h>

int main () {
	const int c = 1;
	const int *cp = &c;
	int *p = const_cast<int *>(cp);
	printf("%d %d %d\n", c, *cp, *p);
	*p = 2;
	int v = *p;
	printf("%d %d %d %d\n", c, *cp, *p, v);

	return 0;
}
