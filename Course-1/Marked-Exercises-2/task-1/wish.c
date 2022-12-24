#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid

int batch_parse(char *argv[]);
int prompt_parse();

int main(int argc, char *argv[]) {
  // YOUR CODE HERE
  int return_code;

  if (argc > 1) return_code = batch_parse(argv);
  else return_code = prompt_parse();

  return return_code;
}

int batch_parse(char *argv[]) {
  char *filename = argv[1];
  FILE* ptr = fopen(filename, "r");
  if (ptr == NULL) {
    printError();
    printf("File not found or could not be opened\n");
    return 1;
  }

  size_t size = (BUFF_SIZE * sizeof(char));
  char *buffer = malloc(size);

  while (!feof(ptr)) {
    getline(&buffer, &size, ptr);
    printf("%s\n", buffer);
    free(buffer);
  }

  exit(0);
}

int prompt_parse() {
  size_t size = (BUFF_SIZE * sizeof(char));
  char *buffer = malloc(size);

  char *exit_str = "exit";
  while (1) {
    printf("wish> ");
    getline(&buffer, &size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    if (strcmp(buffer, exit_str) == 0) {
      printf("Exiting the bash...\n");
      free(buffer);
      return 0;
    }

    //int pid = execv();

    free(buffer);
  }
}
