#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void prompt(void)
{
    printf("mysh-0.1$ ");
}

static parse(char *line)
{

}

int main(void)
{
    char *linebuf = NULL;
    size_t linebuf_size = 0;

    while(1) {
        prompt();  // 打印提示符
        if (getline(&linebuf, &linebuf_size, stdin) < 0)
            break;

        parse(linebuf);  // 解析用户输入
        if (0) {
            // 处理内部命令，此处为永假
        } else {

        }
    }

    return 0;
}