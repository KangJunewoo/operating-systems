#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define __BIGMSGQUEUE_ENABLED



#define MSGSZ 128

typedef struct msgbuf {
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;

int
main (void)
{
    int msqid;
    key_t key = 1;

    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, IPC_CREAT|0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (fork() == 0) {
        strcpy(sbuf.mtext, "Did you get this?");
        sbuf.mtype = 1;

        if (msgsnd(msqid, &sbuf, MSGSZ, IPC_NOWAIT) < 0) {
            perror("msgsnd");
            exit(1);
        }
    }
    else {
        sleep(1);

        if (msgrcv(msqid, &rbuf, MSGSZ, 0, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Forked version: %s\n", rbuf.mtext);
        msgctl(msqid, IPC_RMID, NULL);
    }
}