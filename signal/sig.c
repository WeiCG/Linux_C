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
    // signal(SIGINT, SIG_IGN);
    signal(SIGINT, int_handler);

    for (int i = 0; i < 10; i++)
    {
        write(STDOUT_FILENO, "*", 1);
        sleep(1);
    }
    putchar('\n');

    return 0;
}