#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid

#define COMMAND_EXIT "exit"
#define COMMAND_CD "cd"
#define COMMAND_PATH "path"

int path_ind = 1;
char *paths[BUFF_SIZE] = {"/bin"};

typedef struct {
  char *args[BUFF_SIZE];
  int args_num;
} ParseResponse;

int built_in_command(char *args[], int args_num);

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
      trim(buffer)
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
      if (strlen(buffer == 0)) continue;
      buffer = trim(buffer);

      char *args[BUFF_SIZE];
      int counter = 0;
      while((chunk = strsep(&input, " ")) != NULL) {
        char c = chunk[0];
        if (isspace(c) != 0) continue;
        printf("%s\n", chunk);
        counter++;
        args[counter] = chunk;
      }

      char *command_args[sizeof(char *) * counter];
      command_args[0] = command_args[0];
      int args_num = 0
      for (int i = 0; i < counter; i++) {
        FILE *ptr = NULL;
        command_args[args_num++] = args[i];

        if (strcmp(command_args[i], "&&") == 0) {
          executeCommands(command_args, args, NULL);

        }
      }
      executeCommands(args, counter, NULL);
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
};*/

void executeCommands(char *args[], int args_num, FILE *out) {
  if (out != NULL) redirect(out);
  if (built_in_command(args, args_num)) return;

  int flagAccess = 0;
  for (int i = 0; i <= path_ind; i++) {
    char *command = strcat(paths[i], args[0]);
    if (access(command, X_OK == 0)) {
      args[0] = command;
      flagAccess = 1;
      break;
    }
  }
  if (!flagAccess) {
    printError();
    exit(1);
  }

  int pid = fork();
  if (pid == 0) {
    if (execv(args[0], args)) {
      printError();
      exit(1);
    }
  } else if (pid > 0) {
    wait(NULL);
  } else {
    printError();
    exit(1);
  }
}

void redirect(FILE *out) {
  dup2(fileno(out), FILENO_STDOUT);
  fclose(out);
}

int built_in_command(char *args[], int args_num) {
  if (strcmp(args[0], COMMAND_EXIT) == 0) {
    exit(0);
  }

  if (strcmp(args[0], COMMAND_CD) == 0) {
    if (args_num > 1) {
      if (chdir(args[1])) {
        printError();
        exit(1);
      }
    } else {
      printError();
      exit(1);
    }
    return 1;
  }

  if (strcmp(args[0], COMMAND_PATH) == 0) {
    for (int i = 1; i < args_num; i++) {
      paths[path_ind++] = args[i];
    }
    return 1;
  }

  return 0;
}


