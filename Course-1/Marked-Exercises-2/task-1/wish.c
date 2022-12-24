#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid

FILE *input_ptr = stdin;

char *buffer = NULL;
size_t buffer_size = (BUFF_SIZE * sizeof(char));
void initBuffer() {
  char *buffer = malloc(buffer_size);
  if (buffer == NULL) {
    printError();
    exit(1);
  }
  return buffer;
}

int path_ind = 1;
char **paths = NULL;
void initPaths() {
  paths = malloc(BUFF_SIZE * sizeof(char *));
  if (paths == NULL) {
    printError();
    exit(1);
  }
  for (int i = 0; i < BUFF_SIZE; i++) paths[i] = (char *)malloc(sizeof(char *));
  paths[0] ="/bin";
}

char *trim_spaces(char *buffer);
void parallelRoutines(char* buffer);
int parseCommands(char *args[], int args_num);
int builtInCommand(char *args[], int args_num);

int main(int argc, char *argv[]) {
  // YOUR CODE HERE
  int flag_interractive = 1;

  initPaths();
  initBuffer();

  if (argc > 1) {
    char *filename = argv[1];
    input_ptr = fopen(filename, "r");
    if (input_ptr == NULL || input_ptr == stdin || argv > 2) {
      printError();
      exit(0);
    }
  }

  if (flag_interractive) printf("wish> ");
  while(1) {
    int length = getfile(&buffer, &buffer_size, input_ptr);
    if (length == 0) continue;
    if (feof(input_ptr)) {
      clean();
      return (0);
    }
    if (parseInput(trim_spaces(buffer)) != NULL) {
      clean();
      printError();
      exit(1);
    }
  }
  return (0);
}

char *trim_spaces(char *buffer) {
  buffer[strcspn(buffer, "\r\n")] = 0;
  return strdup(buffer);
};

void parallelRoutines(char* buffer) {
  int routine_num = 0;
  char *routine;
  struct function_args args[BUFF_SIZE];

  while (routine = strsep(&buffer, "&")) != NULL && routine_num <= BUFF_NUM) {
    if (strlen(routine)) args[routine_num++].command = routine;

    for (int i = 0; i < routine_num; i++) {
      if (pthread_create(&args[i].thread, NULL, &parseInput, &args[i] != 0) {
        printError();
        exit(1);
      }
    }

    for (size_t i = 0; i < commands_num; i++) {
      if (pthread_join(args[i].thread, NULL) != 0) {
        printError();
        exit(1);
      }
      free(args[i].command);
    }
  }
}

void *parseInput(void *arg) {
  char *input = strdup(arg);
  char *chunk;
  char **args = malloc(BUFF_SIZE * sizeof(char *));
  if (args == NULL) return (void *)malloc(1);
  for (int i = 0; i < BUFF_SIZE; i++) args[i] = malloc(sizeof(char *));
  int counter = 0;
  while((chunk = strsep(&input, " ")) != NULL) {
    char c = chunk[0];
    if (isspace(c) == 0) continue;
    args[counter++] = chunk;
  }
  int status = parseCommands(args, counter);
  free(args);
  if (status) return (void *)malloc(1);
  return NULL;
};

int parseCommands(char *args[], int args_num) {
  char **command_args = malloc(args_num * sizeof(char *));
  if (command_args == NULL) return 1;
  for (int i = 0; i < args_num; i++) command_args[i] = malloc(sizeof(char *));
  int command_args_num = 0;
  for (int i = 0; i < args_num; i++) {
    FILE *ptr = NULL;
    char *arg = args[i];

    if (ptr != NULL && strcmp(arg, ">") != 0) {
      free(command_args);
      return 1;
    }

    if (strcmp(arg, ">") == 0) {
      if (i + 1 >= args_num) {
        free(command_args);
        return 1;
      }
      char *filename = args[i + 1];
      if (strcmp(filename, ">") == 0 || ptr != NULL) {
        free(command_args);
        return 1;
      }
      ptr = fopen(filename, "w");
      i++;
      continue;
    }

    if (strcmp(arg, "&&") == 0 || i == (args_num - 1)) {
      executeCommands(command_args, args_num, ptr);
      command_args_num = 0;
      ptr = NULL;
      free(command_args);
      continue;
    }

    command_args[command_args_num++] = arg;
  }
  return 0;
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
    clean();
    free(args);
    printError();
    exit(1);
  }

  int pid = fork();
  if (pid == 0) {
    if (execv(args[0], args)) {
      clean();
      free(args);
      printError();
      exit(1);
    }
  } else if (pid > 0) {
    wait(NULL);
  } else {
    clean();
    free(args);
    printError();
    exit(1);
  }
}

void redirect(FILE *out) {
  dup2(fileno(out), STDOUT_FILENO);
  fclose(out);
}

int builtInCommand(char *args[], int args_num) {
  if (strcmp(args[0], "exit") == 0) {
    clean();
    free(args);
    exit(0);
  }

  if (strcmp(args[0], "cd") == 0) {
    if (args_num > 1) {
      if (chdir(args[1])) {
        clean();
        free(args);
        printError();
        exit(1);
      }
    } else {
      clean();
      free(args);
      printError();
      exit(1);
    }
    return 1;
  }

  if (strcmp(args[0], "path") == 0) {
    for (int i = 1; i < args_num; i++) {
      paths[path_ind++] = args[i];
    }
    return 1;
  }

  return 0;
}

void clean() {
  free(buffer);
  free(paths);
  fclose(input_ptr);
}
