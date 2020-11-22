#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(){
    pid_t pid;
    for (int i = 0; i < 10; i++) {
        pid = fork();
        if(pid > 0){ // 부모 프로세스인 경우
            printf("부모 pid : %d 과연? %d\n", pid, getpid());
            wait(NULL);
            printf("자식 끝\n");
        } else if (pid == 0) { // 자식 프로세스인 경우
            printf("자식 pid : %d 과연? %d\n", pid, getpid());
            exit(0); // 여기서 종료를 하지 않으면 무한증식함.
        } else{ // 프로세스 생성에 실패한 경우
            cout << "프로세스 생성에 실패했습니다" << endl;
        }
    }

    return 0;
    // wait all child processes
    // int status;
    // for (i = 0; i < processes; ++i)
    //     wait(&status);
}

