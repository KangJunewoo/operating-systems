#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <queue>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>

using namespace std;
void enqueueScheduler(pid_t);
pid_t forker(int);


struct msg_buffer{
    long msg_type;
    char msg_text[100];
} msg;


int main(){
    pid_t parentId = getpid();
    pid_t pid = forker(10);
    
    key_t key;
    int msg_id;

    if(parentId == pid){ // 부모프로세스일 경우
        key_t key;
        int msg_id; 

        // key랑 msg_id를 각 
        key = ftok("progfile", 65); // ftok은 ipc키 생성함수
        msg_id = msgget(key, 0666 | IPC_CREAT); // 위의 ipc키를 통해 msg를 생성하는 것 같고..

        msgrcv(msg_id, &msg, sizeof(msg), 1, 0); // 큐에서 메세지를 받아서

        printf("Data Received : %s\n", msg.msg_text); // 뿌리고

        msgctl(msg_id, IPC_RMID, NULL); // 메세지큐 삭제
    } else{ // 자식프로세스일 경우
        key_t key;
            int msg_id;

            // 메세지 키 & 아이디 & 타입 설정
            key = ftok("progfile", 65);
            msg_id = msgget(key, 0666 | IPC_CREAT);
            msg.msg_type = 1;

            printf("Write Data : ");
            fgets(msg.msg_text, 100, stdin); // gets를 썼더니 경고가 떠서 fgets로 대체
            // gets(msg.msg_text);

            msgsnd(msg_id, &msg, sizeof(msg), 0); // 해당 메세지 전송

            printf("Data Sent : %s\n", msg.msg_text);
    }
    return 0;
}

// 재귀 방식의 프로세스 통신
pid_t forker(int processesLeft){
    pid_t pid;
    if(processesLeft > 0){
        pid = fork();
        if (pid == 0){
            printf("자식의 pid는 %d 부모의 pid는 %d\n", getpid(), getppid());
            
            // 근데 여기까지가 자식이 할 수 있는 부분임.
        }
        else{
            forker(processesLeft-1);
            wait(NULL);
        }
    }
    return getpid();
}
