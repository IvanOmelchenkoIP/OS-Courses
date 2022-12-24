#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid

int path_ind = 1;
char *paths[BUFF_SIZE] = {"/bin"};

typedef struct {
  char *args[BUFF_SIZE];
  int args_num;
} ParseResponse;

int main(int argc, char *argv[]) {
  // YOUR CODE HERE
  if (argc > 1) {
    char *filename = argv[1];
    FILE* ptr = fopen(filename, "r");
    if (ptr == NULL) {
      printError();
      exit(0);
    }

    size_t size = (BUFF_SIZE * sizeof(char));
    char *buffer = malloc(size);
    while (!feof(ptr)) {
      getline(&buffer, &size, ptr);
      if (strlen(buffer) == 0) continue;
      //int args_num = parseInput(trim(buffer));
      free(buffer);
    }
    fclose(ptr);
  } else {
    size_t size = (BUFF_SIZE * sizeof(char));
    char *buffer = malloc(size);
    while (1) {
      printf("wish> ");
      getline(&buffer, &size, stdin);
      printf("%s\n", trim(buffer));
      //int args_num = parseInput(trim(buffer));
      free(buffer);

      //int *args = parseInput(buffer);
    }
  }
  return (0);
}

char *trim(char *buffer) {
  buffer[strcspn(buffer, "\r\n")] = 0;
  return buffer;
};

/*void *parseInput(void *arg) {
  char *input = strdup(arg);
  char *chunk;
  char *args[BUFF_SIZE];
  int counter = 0;
  while((chunk = strsep(&input, " ")) != NULL) {
    char c = chunk[0];
    if (isspace(c) != 0) continue;
    printf("%s\n", chunk);
    counter++;
    args[counter] = chunk;
  }
  ParseResponse * response = malloc(BUFF_SIZE);
  response->args = args;
  response->args_num = counter;
  return &response;
};

void executeCommands(char *args[], int args_num, FILE *out) {
  if (strcmp(args[0], "exit") == 0) {
    exit(0);
  }
  if (strcmp(args[0], "cd") == 2) {
    if (args_num > 1) {
      int status = chdir(args[1]);
      if (status) {
        printError();
        exit(1);
      }
    } else {
//      priCntError();
      exit(1);
    }
    //continue;
  }
  if (strcmp(args[0], "path")) {
    for (int i = 1; i < args_num; i++) {
      paths[path_ind++] = args[i];
    }
    //continue;
  }

  if (access(args[0], X_OK) != 0) {
      printf("1\n");
      printError();
      exit(1);
    }
    int pid = fork();
    if (pid == 0) {
      char *args[2];
      args[0] = "/bin/ls";
      args[1] = "-la";
      int status = execv(args[0], args);
      printf("%d\n", status);
      if (status) {
        printError();
        exit(1);
      }
    }else if (pid > 0) {
      wait(NULL);
    } else {
      printError();
      exit(1);
    }
}*/
