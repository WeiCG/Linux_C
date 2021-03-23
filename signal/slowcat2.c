/**
 * 根据令牌桶来实现
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

#define CPS 10
#define BUFSIZE CPS
#define BURST 100

static volatile int token = 0;
static void alrm_handler(int signo)
{
    alarm(1);
    token++;
    if (token > BURST)
        token = BURST;
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
    alarm(1);

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
        while(token <= 0) pause();
        token--;

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