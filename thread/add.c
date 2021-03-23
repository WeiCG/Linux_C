#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void * thr_add(void *arg)
{
    char linebuf[LINESIZE];

    FILE *fp = fopen(FNAME, "r+");
    if (fp == NULL) {
        perror("fopen(): ");
        exit(1);
    }

    pthread_mutex_lock(&mut);
    fgets(linebuf, LINESIZE, fp);
    fseeko(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", (atoi(linebuf) + 1));
    pthread_mutex_unlock(&mut);

    fclose(fp);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t tid[THRNUM];
    for (size_t i = 0; i < THRNUM; i++)
    {
        int err = pthread_create(tid + i, NULL, thr_add, NULL);
        if (err != 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            return 1;
        }
    }

    for (size_t i = 0; i < THRNUM; i++)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&mut);

    return 0;
}