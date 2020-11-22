/**
 * OS Term Project 1 - Simple Scheduling
 * Author : 2016312924 강준우
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <queue>

#define TIMEQUANTUM 1

using namespace std;

typedef struct messageBuffer // ipc 메세지큐를 위한 버퍼
{
  long mtype;
  char mtext[100];
  pid_t pid;
} msgbuf;

typedef struct processControlBlock{ // pcb는 pid, 실행시간, 남은시간으로 구성.
  pid_t pid;
  int burst_time;
  int remaining_time;
} pcb;

queue<pcb> q; // pcb들이 들어갈 ready queue


int main()
{
  srand(time(NULL));
  int msqid; // 메시지 큐 아이디와
  key_t key = 1; // 계속 변화할 키값

  msgbuf sendbuf, receivebuf; // 전송버퍼와 수신버퍼 선언

  if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
  { // 메세지큐 생성 실패 시 오류 출력
    perror("msgget");
    exit(1);
  }

  for (int i = 0; i < 10; i++) // 반복문을 10번 돌면서
  {
    if (fork() == 0) // 프로세스를 생성 후, 자식 프로세스인 경우
    {
      strcpy(sendbuf.mtext, "sending.."); // 전송할 정보들 입력 후
      sendbuf.mtype = 1;
      sendbuf.pid = getpid();
      
      if (msgsnd(msqid, &sendbuf, sizeof(sendbuf), IPC_NOWAIT) < 0) // msgsnd 함수로 송신, 실패시 에러처리
      {
        perror("msgsnd");
        exit(1);
      }
      exit(0); // 자식프로세스 증식을 막기 위해 exit 처리
    }
    else // 부모 프로세스인 경우
    {
      sleep(1); // 1초 기다린 후

      if (msgrcv(msqid, &receivebuf, sizeof(receivebuf), 0, 0) < 0) // msgsnd 함수로 수신, 실패시 에러처리
      {
        perror("msgrcv");
        exit(1);
      }
      pcb p; // pcb 정보 세 개 입력
      p.pid = receivebuf.pid;
      p.burst_time = (rand()%10) + 1; // burst time은 1~10 중 하나로 랜덤으로 정함.
      p.remaining_time = p.burst_time;
      q.push(p); // run queue에 인큐
      printf("자식프로세스 %d의 PCB 생성 완료. 레디큐에 삽입\n", receivebuf.pid); // 성공메세지 출력
    }
  }
  
  int time_elapsed = 0;
  while(!q.empty()){ // 프로세스 처리부분
    pcb p = q.front(); // 레디큐 앞에서 pcb를 하나 꺼내
    printf("%d초 : %d 프로세스 처리중..  ", time_elapsed, p.pid); // 출력중 메세지를 띄우고
    
    time_elapsed += TIMEQUANTUM; // 타임퀀텀만큼 처리를 한 후
    p.remaining_time -= TIMEQUANTUM;
    sleep(1);

    if(p.remaining_time == 0){ // 만약 처리가 완료되었으면 완료메세지 출력
      printf("프로세스 처리 완료, 레디큐에서 빠져나갑니다.\n");
    } else{ // 처리가 완료되지 않았으면 메세지 출력 및 다시 enqueue
      printf("레디큐 뒤로 다시 들어갑니다. 남은 처리시간은 %d초입니다.\n", p.remaining_time);
      q.push(p);
    }
    q.pop(); // pop으로 다음 pcb를 처리할 준비
  }
  
  printf("모든 프로세스 처리가 완료되었습니다!\n"); // 모든 출력이 완료되었을 경우 메세지 출력 및
  msgctl(msqid, IPC_RMID, NULL); // 처리 후 메세지큐 삭제
  return 0;
}