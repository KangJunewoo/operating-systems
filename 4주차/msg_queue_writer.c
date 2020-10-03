/**
 * Code for OS Report 4
 * IPC 1 : Message Passing - 송신측 구현
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
    msg.msg_type = 1;

    printf("Write Data : ");
    fgets(msg.msg_text, 100, stdin);
    // gets(msg.msg_text);

    msgsnd(msg_id, &msg, sizeof(msg), 0);

    printf("Data Sent : %s\n", msg.msg_text);

    return 0;
}