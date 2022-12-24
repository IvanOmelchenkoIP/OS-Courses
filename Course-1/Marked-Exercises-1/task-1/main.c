#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int num;

int main(int argc, char *argv[]) {

  // add code below this line
  num = 100;
  int child = fork();
  if (child > 0){
    printf("value in parent process: %d\n", num);
  } else if (child == 0) {
    printf("value in child process: %d\n", num);
  } else {
    printf("failed to create child\n");
    return 1;
  }
  // add code above this line

  return 0;
}
