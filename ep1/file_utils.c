#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h> 
#include <assert.h>
#include "file_utils.h"

FILE* out_file;

char** str_split(char* a_str, const char a_delim) {
  char** result = 0;
  size_t count = 0;
  char* tmp = a_str;
  char* last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;

  while(* tmp) {
    if (a_delim == *tmp) {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  count += last_comma < (a_str + strlen(a_str) - 1);
  count++;

  result = malloc(sizeof(char* ) * count);

  if (result) {
    size_t idx = 0;
    char* token = strtok(a_str, delim);

    while (token) {
      assert(idx < count); * (result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    assert(idx == count - 1); * (result + idx) = 0;
  }

  return result;
}

void read_file(char* file_name) {
  FILE* fp;
  char* line;
  size_t len = 0;
  ssize_t read;
  char** process_detail;

  fp = fopen(file_name, "r");

  while ((read = getline(&line, &len, fp)) != -1) {
    process_detail = str_split(line, ' ');
    insert_new(atof(process_detail[0]), atof(process_detail[1]), atof(process_detail[2]), strtok(process_detail[3], "\n"));
  }

  reverse();  
  fclose(fp);
}

void create_out_file(char* file_name) {
  out_file = fopen(file_name, "w");
  if (file_name == NULL)
  {
      printf("Erro ao criar o arquivo de saida.\n");
      exit(1);
  }

}

void insert_line(struct process* p, float end_time) {
  fprintf(out_file, "%s %.1f %.1f\n", p->name, end_time, end_time - p->arrival_time);
} 

void close_file() {
  fclose(out_file);
}