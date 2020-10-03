/**
 * Code for OS Report 4
 * IPC 1 : Message Passing - 수신측 구현
 * Ref : https://www.geeksforgeeks.org/ipc-using-message-queues/
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer{
    long msg_type;
    char msg_text[100];
} msg; // 메

int main(){
    // key와 메세지id 선언해서
    key_t key;
    int msg_id; 

    // key랑 msg_id를 각 
    key = ftok("progfile", 65); // ftok은 ipc키 생성함수
    msg_id = msgget(key, 0666 | IPC_CREAT); // 위의 ipc키를 통해 msg를 생성하는 것 같고..

    msgrcv(msg_id, &msg, sizeof(msg), 1, 0); // 큐에서 메세지를 받아서

    printf("Data Received : %s\n", msg.msg_text); // 뿌리고

    msgctl(msg_id, IPC_RMID, NULL); // 메세지큐 삭제

    return 0;
}