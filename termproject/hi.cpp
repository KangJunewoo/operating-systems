#include <iostream>
#include <stdio.h> 
#include <unistd.h>
#include <queue>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

using namespace std;

typedef struct process{
    pid_t pid;
    float arrival_time; // 도착시간
} p;

typedef struct messageBuffer{
    long mtype; // 베세지 타입
    char mtext[30]; // 베세지
} msgbuf;

queue<string> q;
clock_t startTime, endTime;
pid_t pid;

int main()
{ 
    startTime = clock(); // 시작할 떄 시간 측정
    int messageQueueId;
    key_t key = 1;

    msgbuf sendBuffer, receiveBuffer;

    messageQueueId = msgget(key, IPC_CREAT|0666);

    for(int i=0;i<10;i++) // 10번 반복문을 돌며
    { 
        pid = fork(); //프로세스 생성
        if(pid == 0) // 자식프로세스면
        {   
            strcpy(sendBuffer.mtext, "sample");
            sendBuffer.mtype = 1;
            msgsnd(messageQueueId, &receiveBuffer, sizeof(receiveBuffer), 0);
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); // 간단하게 정보를 출력하고

        } else{
            sleep(1);
            msgrcv(messageQueueId, &receiveBuffer, sizeof(receiveBuffer), 0, 0);
            printf("Forked version: %s\n", receiveBuffer.mtext);
            // wait(NULL); // 자식프로세스 종료될때까지 기다림.
        }
        
        

        
    } 

    

    endTime = clock();
    printf("실행시간 : %f", (double)(endTime-startTime));
    return 0;
} 