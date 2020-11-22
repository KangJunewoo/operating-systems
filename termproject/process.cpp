#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <queue>

int main(){
    pid_t pid;

    pid = fork();

    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        return 1;
    } else if (pid == 0){ // 0이면 자식
        execlp("/bin/ls", "ls", NULL);
    } else{ // 0보다 크면 부모
        wait(NULL);
        printf("Child Complete");
    }

    return 0;
}