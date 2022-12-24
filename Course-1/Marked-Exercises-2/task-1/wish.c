#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid

#define SYMBOL_AND "&&"
#define SYMBOL_REDIRECT ">"

#define COMMAND_EXIT "exit"
#define COMMAND_CD "cd"
#define COMMAND_PATH "path"

int path_ind = 1;
char *paths[BUFF_SIZE] = {"/bin"};

typedef struct {
  char *args[BUFF_SIZE];
  int args_num;
} ParseResponse;

int parseCommands(char *args[], int args_num);
int builtInCommand(char *args[], int args_num);

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

      int status = *((int *)parseCommands(trim(buffer)));
      if (status) {
        printError();
        exit(1);
      }
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

      int status = *((int *)parseCommands(trim(buffer)));
      if (status) {
        printError();
        exit(1);
      }
  }
  return (0);
}

char *trim(char *buffer) {
  buffer[strcspn(buffer, "\r\n")] = 0;
  return buffer;
};

void *parseInput(void *arg) {
  char *input = strdup(arg);
  char *chunk;
  char *args[BUFF_SIZE];
  int counter = 0;
  while((chunk = strsep(&input, " ")) != NULL) {
    char c = chunk[0];
    if (isspace(c) != 0) continue;
    counter++;
    args[counter] = chunk;
  }
  return &parseCommands(args, counter);
};

int parseCommands(char *args[], int args_num) {
  char *command_args[sizeof(char *) * counter];
  command_args[0] = command_args[0];
  int command_args_num = 0
  for (int i = 0; i < counter; i++) {
    FILE *ptr = NULL;
    char *arg = args[i];

    if (ptr != NULL && strcmp(arg, SYMBOL_REDIRECT) != 0) {
      printError();
      exit(1);
    }

    if (strcmp(arg, SYMBOL_REDIRECT) == 0) {
      if (i + 1 >= counter) {
        printError();
        exit(1);
      }
      char *filename = args[i + 1];
      if (strcmp(filename, SYMBOL_REDIRECT) == 0 || ptr != NULL) {
        printError();
        exit(1);
      }
      ptr = fopen(filename, "w");
      i++;
      continue;
    }

    if (strcmp(arg SYMBOL_AND) == 0 || i == (counter - 1)) {
      executeCommands(command_args, args_num, ptr);
      command_args_num = 0;
      free(command_args);
      ptr = NULL;
      continue;
    }

    command_args[command_args_num++] = arg;
  }
}


void executeCommands(char *args[], int args_num, FILE *out) {
  if (out != NULL) redirect(out);
  if (builtInCommand(args, args_num)) return;

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

int builtInCommand(char *args[], int args_num) {
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


