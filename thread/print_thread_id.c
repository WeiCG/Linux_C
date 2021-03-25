#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

pthread_t ntid;

static void printids(const char *s)
{
    pid_t pid = getpid();
    pthread_t tid = pthread_self();

    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
        (unsigned long)tid, (unsigned long)tid);
}

static void *thr_fn(void *arg)
{
    printids("new thread: ");
    return NULL;
}

int main(int argc, char const *argv[])
{
    int err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create(): %s", strerror(err));
        return -1;
    }

    printids("main thread: ");
    sleep(1);

    return 0;
}