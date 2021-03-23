#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

static volatile bool loop = true;
static void alrm_handler(int signo)
{
    loop = false;
}

int main(int argc, char const *argv[])
{
    int64_t count = 0;

    alarm(5);
    signal(SIGALRM, alrm_handler);

    while (loop)
        count++;

    printf("%lld\n", count);

    return 0;
}