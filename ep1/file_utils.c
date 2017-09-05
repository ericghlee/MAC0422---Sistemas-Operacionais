#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h> 
#include <assert.h>
#include "file_utils.h"
#include "process.h"

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
    insert_new(atoi(process_detail[0]), process_detail[1], atoi(process_detail[2]), atoi(process_detail[3]), 0);
  }

  reverse();
  fclose(fp);
}