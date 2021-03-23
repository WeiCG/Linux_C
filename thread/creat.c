#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void *func(void *argv)
{
    puts("Thread is working!");
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    puts("Begin!");

    pthread_t tid;
    int err = pthread_create(&tid, NULL, func, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create(): %s\n", strerror(err));
        return 1;
    }

    pthread_join(tid, NULL);
    puts("End!");
    return 0;
}