#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#define MSGSIZE 128

typedef struct messageBuffer
{
  long mtype;
  char mtext[MSGSIZE];
} msgbuf;

int main()
{
  int msqid; // 메시지 큐 아이디와
  key_t key = 1; //

  msgbuf sbuf, rbuf;

  if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
  { // 메세지를 받는데 실패하면
    perror("msgget");
    exit(1);
  }

  for (int i = 0; i < 10; i++)
  {
    if (fork() == 0) // 프로세스를 생성 후, 자식 프로세스인 경우
    {
      strcpy(sbuf.mtext, "Did you get this?");
      sbuf.mtype = 1;

      if (msgsnd(msqid, &sbuf, MSGSIZE, IPC_NOWAIT) < 0)
      {
        perror("msgsnd");
        exit(1);
      }
      exit(0);
    }
    else
    {
      sleep(1);

      if (msgrcv(msqid, &rbuf, MSGSIZE, 0, 0) < 0)
      {
        perror("msgrcv");
        exit(1);
      }

      printf("Forked version: %s\n", rbuf.mtext);
      // msgctl(msqid, IPC_RMID, NULL);
    }
  }
}