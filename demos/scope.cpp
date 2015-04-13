#include <stdio.h>

int a = -1;

int main(int argc, char const *argv[])
{

	printf("-1: %d\n", a);

	int a = 0;
	printf("0: %d\n", a);

	{
		int a = 1;
		printf("1: %d\n", a);

		if (a == 1) {
			int a = 2;
			printf("2: %d\n", a);
			
		}
	}

	if (a == 0) {
		int a = 3;
		printf("3: %d\n", a);
	}

	return 0;
}
