#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void cleanup_func(void *arg)
{
    puts(arg);
}

static void* func(void* arg)
{
    puts("Thread is working!");

    pthread_cleanup_push(cleanup_func, "cleanup:1");
    pthread_cleanup_push(cleanup_func, "cleanup:2");
    pthread_cleanup_push(cleanup_func, "cleanup:3");

    puts("push over!");

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

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