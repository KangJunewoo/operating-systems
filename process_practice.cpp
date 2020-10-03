/**
 * Code for OS Report 3-1
 * Ref : https://kim-hoya.tistory.com/7
*/
#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  int x = 0;

  pid = fork(); // 부모 프로세스엔 자식 pid가, 자식 프로세스엔 0이 반환됨.

  if (pid > 0)
  { // 부모 프로세스
    x = 1;
    printf("부모 pid : %ld, x : %d, 자식 pid : %d\n", (long)getpid(), x, pid);
  }
  else if (pid == 0)
  { // 자식 프로세스
    x = 2;
    printf("자식 pid : %ld, x : %d\n", (long)getpid(), x);
  }
  else
  { // fork실패
    printf("fork 실패\n");
    return -1;
  }

  return 0;
}
