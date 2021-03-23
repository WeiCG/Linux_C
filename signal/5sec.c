#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    int64_t count = 0;
    time_t end = time(NULL) + 5;

    while (time(NULL) <= end)
        count++;

    printf("%lld\n", count);

    return 0;
}