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
    parallelRoutines(trim_spaces(buffer));
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
        clean();
        printError();
        exit(1);
      }
    }

    for (size_t i = 0; i < commands_num; i++) {
      if (pthread_join(args[i].thread, NULL) != 0) {
        clean();
        printError();
        exit(1);
      }
      free(args[i].command);
    }
  }
}

void *parseInput(void *arg) {
  FILE *output_ptr = stdout;

  char *args[BUFF_SIZE];
  int args_num = 0;
  struct function_args *fun_args = (struct function_args *)arg;
  char *input = fun_args->command;

  char *command = strsep(&input, ">");
  if (command == NULL || strlen(command) == 0) {
    printError();
    return;
  }

  if (input != NULL) {
    regex_t reg;
    if (regcomp(&reg, "\\S\\s+\\S", REG_CFLAGS) != 0 || strstr(input, ">") != NULL) {
      printError();
      regfree(&reg);
      return NULL;
    }
    regfree(&reg);

    if ((output_ptr = fopen(trim(input), "w")) == NULL) {
      printError();
      return NULL;
    }
  }

  input = trim(input);
  char *chunk;
  while((chunk = strsep(&input, " ")) != NULL) {
    char c = chunk[0];
    if (isspace(c) == 0) continue;
    args[counter++] = chunk;
  }
  if (args_num > 0) executeCommands(args, args_num, output_ptr);
  return NULL;
};

char *trim(char *str) {
  while(isspace(*str)) str++;

  if(strlen(str) == 0) return str;

  char *end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';

  return str;
}

void executeCommands(char *args[], int args_num, FILE *out) {
  if (out != NULL) redirect(out);
  if (builtInCommand(args, args_num)) return;

  char command_path[];
  if (!searchPath(command_path, args[0])) {
    printError();
    return;
  }

  int pid = fork();
  if (pid == 0) {
    if (execv(args[0], args)) printError();
  } else if (pid > 0) {
    waitpid(pid, NULL, 0);
  } else {
    printError();
  }
}

void redirect(FILE *out) {
  int filenoOut;
  if (fileno(out) == -1) {
    printError();
    return;
  }

  if (dup2(fileno(filenoOut), STDOUT_FILENO) != 0 || dup2(fileno(filenoOut), STDERR_FILENO) != 0){
    printError();
    return;
  }
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
      if (chdir(args[1]) != 0)printError();
    } else {
      printError();
    }
    return 1;
  }

  if (strcmp(args[0], "path") == 0) {
    for (int i = 1; i < args_num; i++) {
      if (path_ind < BUFF_SIZE) paths[path_ind++] = args[i];
    }
    return 1;
  }

  return 0;
}

int searchPath(char path[], char *firstArg) {
  for (int i = 0; i <= path_ind; i++) {
    char *command = snprintf(paths, BUFF_SIZE, "%s/%s", paths[i], firstArg);
    if (access(command, X_OK == 0)) return 1;
  }
  return 0;
}

void clean() {
  free(buffer);
  free(paths);
  fclose(input_ptr);
}
