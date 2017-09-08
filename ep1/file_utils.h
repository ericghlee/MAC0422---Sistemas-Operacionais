#ifndef FILE_UTILS_H  
#define FILE_UTILS_H

#include "process.h"

char** str_split(char* a_str, const char a_delim);

void read_file(char* file_name);

void create_out_file(char* file_name);

void insert_line(struct process* p, float end_time);

void close_file();

#endif