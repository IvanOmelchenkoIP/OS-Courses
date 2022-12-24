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
  FILE* ptr = fopen(filename, "r+");
  if (ptr == NULL) {
    printError();
    printf("File not found or could not be opened\n");
    return 1;
  }

  size_t size = (BUFF_SIZE * sizeof(char));
  char *buffer = malloc(size);

  while (!feof(ptr)) {
    getline(&buffer, &size, ptr);
    if (strlen(buffer) == 0) continue;
    free(buffer);
    //printf("%ld\n", strlen(buffer));
  }
  fclose(ptr);
  return 0;
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
    free(buffer);

    //char *args[] = parseInput(buffer);

    if (access("/bin/ls", X_OK) != 0) {
      printf("1\n");
      printError();
      return 1;
    }
    int pid = fork();
    if (pid == 0) {
      char *args[2];
      args[0] = "/bin/ls";
      args[1] = "-la";
      int status = execv(args[0], args);
      printf("%d\n", status);
      /*if (status) {
        printError();
        return 1;
      } */
    }else if (pid > 0) {
      wait(NULL);
    } else {
      printError();
      return 1;
    }

  }
}


void *parseInput(void *arg) {
  char *input = strdup(arg);
  char *chunk;
  char *args[strlen(input)];
  int counter = 0;
  while((chunk = strsep(&input, " ")) != NULL) {
    char c = chunk[0];
    if (isspace(c) != 0) continue;
    printf("%s\n", chunk);
    counter++;
    args[counter] = chunk;
  }
  return *args;
};

void executeCommands(char *args[], int args_num, FILE *out) {
  int pid = fork();
  printf("%d", pid);
}
