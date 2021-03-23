#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

void int_handler(int s)
{
    write(STDOUT_FILENO, "!", 1);
}

int main(int argc, char const *argv[])
{
    sigset_t set, oset;
    signal(SIGINT, int_handler);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    sigprocmask(SIG_BLOCK, &set, &oset);
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 5; j++) {
            write(STDOUT_FILENO, "*", 1);
            sleep(1);
        }
        write(STDOUT_FILENO, "\n", 1);
        sigsuspend(oset);
        // sigprocmask(SIG_UNBLOCK, &set, NULL);
        // pause();
    }
    putchar('\n');

    return 0;
}