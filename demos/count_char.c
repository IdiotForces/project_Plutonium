#include <stdio.h>
#include <string.h>

int count(const char *a, const char *b) {
	int ret = 0;

	for (int i = 0; i < strlen(a); i++) {
		for (int j = 0; j < strlen(b); j++) {

			if (a[i] == b[j]) {
				ret++;
			}

		}
	}

	return ret;
}

int main(int argc, const char *argv[]) {
	
	char str_1[256];
	char str_2[256];

	scanf("%s", str_1);
	scanf("%s", str_2);

	printf("%d\n", count(str_1, str_2));

	return 0;
}