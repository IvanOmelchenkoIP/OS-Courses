#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid

FILE *input_ptr = NULL;
char *paths[BUFF_SIZE] = { "/bin", NULL };
char *buffer = NULL;

void clean(void) {
  free(buffer);
  fclose(input_ptr);
}

char *trimNewline(char* buffer, int length);
void runParallelTasks(char *buffer);
int builtInCommand(char *args[], int args_num);

int main(int argc, char *argv[]) {
  // YOUR CODE HERE
  int mode = INTERACTIVE_MODE;
  input_ptr = stdin;
  size_t linecap = 0;

  if (argc > 1) {
    mode = BATCH_MODE;
    if ((input_ptr = fopen(argv[1], "r")) == NULL  || argc > 2) {
      printError();
      exit(EXIT_FAILURE);
    }
  }

  int length;
  while (1) {
    if (mode == INTERACTIVE_MODE) printf("wish> ");
    if ((length = getline(&buffer, &linecap, input_ptr)) <= 0) {
      if (feof(input_ptr) != 0) {
        clean();
        exit(EXIT_SUCCESS);
      }
      continue;
    }
    runParallelTasks(trimNewline(buffer, length));
  }
  return 0;
}

char *trimNewline(char* buffer, int length) {
  if (buffer[length - 1] == '\n') buffer[length - 1] = '\0';
  return buffer;
}

void runParallelTasks(char *buffer) {
  char *routine;
  int routines_num = 0;
  struct function_args args[BUFF_SIZE];

  while ((routine = strsep(&buffer, "&")) != NULL) {
    if (routine[0] != '\0') {
      args[routines_num++].command = strdup(routine);
      if (routines_num >= BUFF_SIZE) break;
    }
  }

  for (size_t i = 0; i < routines_num; i++){
    if (pthread_create(&args[i].thread, NULL, &parseInput, &args[i]) != 0) printError();
  }
  for (size_t i = 0; i < routines_num; i++) {
    if (pthread_join(args[i].thread, NULL) != 0) printError();
    if (args[i].command != NULL) free(args[i].command);
  }
}

void *parseInput(void *arg) {
  FILE *output_ptr = stdout;

  char *args[BUFF_SIZE];
  int args_num = 0;
  struct function_args *fun_args = (struct function_args *)arg;
  char *input = fun_args->command;

  char *command = strsep(&input, ">");
  if (command == NULL || *command == '\0') {
    printError();
    return NULL;
  }

  if (input != NULL) {
    regex_t reg;
    if (regcomp(&reg, "\\S\\s+\\S", REG_CFLAGS) != 0) {
      printError();
      regfree(&reg);
      return NULL;
    }
    if (regexec(&reg, input, 0, NULL, 0) == 0 || strstr(input, ">") != NULL) {
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

  command = trim(command);
  char **command_chunk = args;
  while((*command_chunk = strsep(&command, " \t")) != NULL) {
    if (args_num >= BUFF_SIZE) break;
    if (**command_chunk == '\0') continue;
    *command_chunk = trim(*command_chunk);
    command_chunk++;
    args_num++;
  }
  if (args_num > 0) executeCommands(args, args_num, output_ptr);
  return NULL;
}

char *trim(char *s) {
  while (isspace(*s)) s++;
  if (*s == '\0') return s;
  char *end = s + strlen(s) - 1;
  while (end > s && isspace(*end)) end--;
  end[1] = '\0';
  return s;
}

void executeCommands(char *args[], int args_num, FILE *out) {
  if (builtInCommand(args, args_num)) return;

  char command_path[BUFF_SIZE];
  if (!searchPath(command_path, args[0])) {
    printError();
    return;
  }

  int pid = fork();
  if (pid == 0) {
    redirect(out);
    if (execv(command_path, args) == -1) printError();
  } else if (pid > 0) {
    waitpid(pid, NULL, 0);
  } else {
    printError();
  }
}

int builtInCommand(char *args[], int args_num) {
  if (strcmp(args[0], "exit") == 0) {
    if (args_num == 1) {
      clean();
      exit(EXIT_SUCCESS);
    }
    printError();
    return 1;
  }
  if (strcmp(args[0], "cd") == 0) {
    if (args_num == 2) {
      if (chdir(trim(args[1])) != 0) printError();
    } else {
      printError();
    }
    return 1;
  }
  if (strcmp(args[0], "path") == 0) {
    char *temp[BUFF_SIZE] = { NULL };
    *paths = *temp;
    for (int i = 0; i < args_num - 1; i++) {
      paths[i] = strdup(args[i + 1]);
      paths[i + 1] = NULL;
    }
    return 1;
  }
  return 0;
}

int searchPath(char path[], char *firstArg) {
  int i = 0;
  while (paths[i] != NULL) {
    snprintf(path, BUFF_SIZE, "%s/%s", paths[i++], firstArg);
    if (access(path, X_OK) == 0) return 1;
  }
  return 0;
}

void redirect(FILE *out) {
  int outFileno;
  if ((outFileno = fileno(out)) == -1) {
    printError();
    return;
  }
  if (outFileno != STDOUT_FILENO) {
    if (dup2(outFileno, STDOUT_FILENO) == -1 || dup2(outFileno, STDERR_FILENO) == -1) {
      printError();
      return;
    }
    fclose(out);
  }
}
