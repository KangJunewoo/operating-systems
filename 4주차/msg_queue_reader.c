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
} msg;

int main(){
    key_t key;
    int msg_id;

    key = ftok("progfile", 65);

    msg_id = msgget(key, 0666 | IPC_CREAT);

    msgrcv(msg_id, &msg, sizeof(msg), 1, 0);

    printf("Data Received : %s\n", msg.msg_text);

    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}