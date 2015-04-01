#include <stdio.h>

int main(int argc, const char *argv[]) {
	int a = 1;
	int b = scanf("%d", &a);
	char c[2333];
	scanf("%s", c);
	printf("%d %d", a, b);
	printf("%s\n", c);
	return 0;
}
