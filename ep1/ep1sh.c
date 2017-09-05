#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ls.h"
#include "date.h"

void ep1sh_loop();
char *ep1sh_read_line();
char **ep1sh_split_line(char *line);
int ep1sh_execute(char **args);

int main(int argc, char **argv)
{
    ep1sh_loop();

    return EXIT_SUCCESS;
}

void ep1sh_loop()
{
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = ep1sh_read_line();
    args = ep1sh_split_line(line);
    status = ep1sh_execute(args);

    free(line);
    free(args);
  } while (status);
}

char *ep1sh_read_line()
{
  char *line = NULL;
  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);

  return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **ep1sh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "ep1sh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "ep1sh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int ep1sh_execute(char **args)
{
    int i;    

    if (args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "ls") == 0) {
        if (args[1] != NULL && strcmp(args[1], "-l") == 0) {
            list_files();  
        } else {
            print_list_files_usage();
        }

    } 

    else if (strcmp(args[0], "date") == 0) {
        print_date();
    } 

    else if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    return 1;
}