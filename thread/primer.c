#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define LEFT   30000000
#define RIGHT  30000200
#define THRNUM (RIGHT - LEFT + 1)

static void * thr_prime(void *arg)
{
    int i = *((int *)arg);
    bool mark = true;
    for (int j = 2; j < i / 2; j++)
        if (i % j == 0) {
            mark = true;
            break;
        }

    if (mark) printf("%d is a primer\n", i);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t tid[THRNUM];
    for (int i = LEFT; i <= RIGHT; i++) {
        int err = pthread_create(tid + (i - LEFT), NULL, thr_prime, &i);
        if (err != 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            return 1;
        }
    }

    for (int i = LEFT; i <= RIGHT; i++) {
        pthread_join(tid[i - LEFT], NULL);
    }

    return 0;
}