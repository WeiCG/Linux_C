#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("[%d]: Begin!\n", getpid());

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork()");
        return 1;
    }

    if (pid == 0) {
        printf("[%d]: Child is working\n", getpid());
    } else {
        printf("[%d]: Parent is working!\n", getpid());
    }

    printf("[%d]: End!\n", getpid());

    return 0;
}