#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <pthread.h>

#define LEFT   30000000
#define RIGHT  30000200
#define THRNUM 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static void *thr_prime(void *arg)
{
    while (true) {
        pthread_mutex_lock(&mut_num);
        while (num == 0) {
            pthread_cond_wait(&cond_num, &mut_num);
        }
        if (num == -1) {
            pthread_mutex_unlock(&mut_num);
            break;
        }

        int i = num;
        num = 0;
        pthread_mutex_unlock(&mut_num);
        pthread_cond_broadcast(&cond_num);

        bool mark = true;
        for (size_t j = 2; j < (i / 2); j++) {
            if (i % j == 0) {
                mark = false;
                break;
            }
        }

        if (mark) printf("[%d]: %d is a primer\n", (int)arg, i);
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t tid[THRNUM];
    for (size_t i = 0; i < THRNUM; i++) {
        int err = pthread_create(tid + i, NULL, thr_prime, (void *)i);
        if (err != 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            return 1;
        }
    }

    for (size_t i = LEFT; i < RIGHT; i++) {
        pthread_mutex_lock(&mut_num);
        while (num != 0) {
            pthread_cond_wait(&cond_num, &mut_num);
        }
        num = i;
        pthread_mutex_unlock(&mut_num);
        pthread_cond_signal(&cond_num);
    }

    pthread_mutex_lock(&mut_num);
    while (num != 0) {
        pthread_mutex_unlock(&mut_num);
        sched_yield();
        pthread_mutex_lock(&mut_num);
    }
    num = -1;
    pthread_cond_broadcast(&cond_num);
    pthread_mutex_unlock(&mut_num);

    for (size_t i = 0; i < THRNUM; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_cond_destroy(&cond_num);
    pthread_mutex_destroy(&mut_num);
    return 0;
}