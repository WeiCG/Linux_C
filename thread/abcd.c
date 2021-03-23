#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <pthread.h>

#define THRNUM 4
static pthread_mutex_t mut[THRNUM];

static int next(int n)
{
    return ((n + 1) == THRNUM) ? 0 : (n + 1);
}

static void *thr_func(void *arg)
{
    int n = (int)arg;
    int ch = 'a' + n;
    while (true) {
        pthread_mutex_lock(mut + n);
        write(STDOUT_FILENO, &ch, 1);
        pthread_mutex_unlock(mut + next(n));
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t tid[THRNUM];
    for (size_t i = 0; i < THRNUM; i++)
    {
        pthread_mutex_init(mut + i, NULL);
        pthread_mutex_lock(mut + i);

        int err = pthread_create(tid + i, NULL, thr_func, (void *)i);
        if (err != 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            return 1;
        }
    }
    pthread_mutex_unlock(mut + 0);
    alarm(5);

    for (size_t i = 0; i < THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}