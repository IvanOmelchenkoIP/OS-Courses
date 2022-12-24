#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  // insert your code below this line
  int child = fork();
  if (child > 0) {
    sleep(1);
    printf("Goodbye from the parent\n");
  } else if (child == 0) {
    printf("Hello from the child\n");
  } else {
    printf("Failed to create child\n");
    return 1;
  }
  // insert your code above this line

  return 0;
}


