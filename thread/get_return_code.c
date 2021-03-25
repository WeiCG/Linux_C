#include <stdio.h>
#include <string.h>
#include <pthread.h>

static void *thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return ((void *)1);
}

static void *thr_fn2(void *arg)
{
    printf("thread 2 returning\n");
    pthread_exit((void *)2);
}

int main(int argc, char const *argv[])
{
    int err;
    pthread_t tid1, tid2;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(err));
        return -1;
    }

    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(err));
        return -1;
    }

    void *ret;
    err = pthread_join(tid1, &ret);
    if (err != 0) {
        fprintf(stderr, "pthread_join(): %s", strerror(err));
        return -1;
    }
    printf("thread 1 exit code %ld\n", (long)ret);

    err = pthread_join(tid2, &ret);
    if (err != 0) {
        fprintf(stderr, "pthread_join(): %s", strerror(err));
        return -1;
    }
    printf("thread 2 exit code %ld\n", (long)ret);

    return 0;
}