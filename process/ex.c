#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * 实现打印时间戳的命令
 * date +%s
 */
int main(void)
{
    puts("Begin!");
    fflush(NULL);

    execl("/bin/date", "date", "+%s", NULL);
    perror("execl(): ");

    puts("End!");
    return 0;
}