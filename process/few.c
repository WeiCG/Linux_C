#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    puts("Begin!");

    fflush(NULL);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork()");
        return 1;
    }

    if (pid == 0) {
        execl("/bin/date", "date", "+%s", NULL);
        perror("execl()");
        return 2;
    }

    wait(NULL);

    puts("End!");
    return 0;
}