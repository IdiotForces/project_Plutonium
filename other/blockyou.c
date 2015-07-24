
#include <stdio.h>

#ifdef _WIN32

#include <windows.h>
#include <conio.h>
#define SLEEP_SECONDS(second) Sleep((second) * 1000)

#else

#include <unistd.h>
#include <sys/fcntl.h>
#include <stdlib.h>

#define SLEEP_SECONDS(second) sleep(second)
#define STTY(arg) system("/bin/stty " arg)

#endif

// www.perlmonks.org/?node_id=175348
// www.gamedev.net/topic/285863-non-blocking-stdin-read-on-win32/

int main() {

	printf("Going to sleep ... just 3 seconds and do not bother me!\n");
	SLEEP_SECONDS(3);
	printf("Waked up!\n");

	{
#ifdef _WIN32
		if (_kbhit()) {
			printf("Seems you are not a good guy, look at what you've done: \n");
			while (_kbhit()) {
				_getche(); }
			putchar('\n');
		}
#else
		STTY("raw");
		fcntl(0, F_SETFL, O_NONBLOCK);
		STTY("cooked -echo");

		char t[2] = { '\0' };
		int l = read(0, t, 1);
		if (l != 0 && l != -1) {
			printf("Seems you are not a good guy, look at what you've done: \n");
			write(1, t, l);
			while (1) {
				l = read(0, t, 1);
				if (l == 0 || l == -1) break; else write(1, t, l);
			}
			putchar('\n');
		}
		
		STTY("cooked");
		fcntl(0, F_SETFL, 0);
#endif
	}

	printf("Now type something! ");
	char buf[128] = { '\0' };
	scanf("%s", buf);
	printf("What you've typed: %s\n", buf);

	return 0;
}
