// 使用条件变量的版本
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <pthread.h>

#define THRNUM 4
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int num = 0;

static int next(int n) {
    return ((n + 1) == THRNUM) ? 0 : (n + 1);
}

static void *thr_func(void *arg)
{
    int n = (int)arg;
    int ch = 'a' + n;
    while (true) {
        pthread_mutex_lock(&mut);
        while (num != n) {
            pthread_cond_wait(&cond, &mut);
        }
        write(STDOUT_FILENO, &ch, 1);
        num = next(num);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t tid[THRNUM];
    for (size_t i = 0; i < THRNUM; i++)
    {
        int err = pthread_create(tid + i, NULL, thr_func, (void *)i);
        if (err != 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            return 1;
        }
    }
    alarm(1);

    for (size_t i = 0; i < THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    return 0;
}