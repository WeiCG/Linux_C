/**
 * 根据漏桶来实现
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/time.h>

#define CPS 10
#define BUFSIZE CPS

static volatile bool loop = true;
static void alrm_handler(int signo)
{
    loop = false;
}

int main(int argc, char const *argv[])
{
    int sfd;
    char buf[BUFSIZE];
    int len, ret, pos;

    if (argc < 2) {
        fprintf(stderr, "Usage: slowcat <filename>\n");
        return 1;
    }

    signal(SIGALRM, alrm_handler);
    // alarm(1);
    struct itimerval itv;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itv, NULL) < 0) {
        perror("setitimer()");
        return 1;
    }


    do {
        sfd = open(argv[1], O_RDONLY);
        if (sfd < 0) {
            if (errno != EINTR) {
                perror("open():");
                return 1;
            }
        }
    } while (sfd < 0);

    while (true) {
        while(loop) pause();
        loop = true;

        while ((len = read(sfd, buf, BUFSIZE)) < 0) {
            if (errno == EINTR) continue;
            perror("read()");
            break;
        }
        if (len == 0) break;

        pos = 0;
        while (len > 0) {
            ret = write(STDOUT_FILENO, buf, len);
            if (ret < 0) {
                if (errno == EINTR) continue;
                perror("write()");
                return 1;
            }
            pos += ret;
            len -= ret;
        }
    }

    close(sfd);

    return 0;
}