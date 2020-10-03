/**
 * Code for OS Report 4
 * IPC 2 - Signaling
 * 컨트롤씨를 눌러도 죽지 않는 프로그램을 구현했습니다.
 * Ref : https://www.youtube.com/watch?v=83M5-NPDeWs&ab_channel=JacobSorber
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int num){
    printf("merong I'm not dying haha\n");
}

int main(){
    signal(SIGINT, handler);
    while(1){
        printf("Wasting your cycles. %d\n", getpid());
        sleep(1);
    }
}