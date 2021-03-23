#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <syslog.h>

#define FNAME "/tmp/out"

static int daemonize(void)
{
    pid_t pid = fork();
    if (pid < 0) return -1;

    if (pid > 0)  exit(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) return -1;

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2) close(fd);

    setsid();
    chdir("/");
    umask(0);

    return 0;
}

int main(int argc, char const *argv[])
{
    openlog("mydaemon", LOG_PID, LOG_DAEMON);

    if (daemonize() < 0) {
        syslog(LOG_ERR, "daemonize() failed!");
        return 1;
    } else {
        syslog(LOG_INFO, "daemonize() successded!");
    }

    FILE* fp = fopen(FNAME, "w");
    if (fp == NULL) {
        syslog(LOG_ERR, "fopen(): %s", strerror(errno));
        return 1;
    }

    syslog(LOG_ERR, "%s was opened.", FNAME);
    syslog(LOG_ERR, "%s was opened.", FNAME);
    syslog(LOG_ERR, "%s was opened.", FNAME);
    syslog(LOG_ERR, "%s was opened.", FNAME);
    for (int i = 0; true; ++i)
    {
        fprintf(fp, "%d\n", i);
        fflush(fp);
        syslog(LOG_DEBUG, "%d is printed.", i);
        sleep(1);
    }

    fclose(fp);
    closelog();

    return 0;
}