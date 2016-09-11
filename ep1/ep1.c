#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h> 
#include <assert.h> 
#include "process.h"

struct process *waiting = NULL;
int time_count = 0;

bool has_waiting() {
  return waiting != NULL;
}

int waiting_length() {
  int length = 0;
  struct process * current;

  for (current = waiting; current != NULL; current = current -> next) {
    length++;
  }

  return length;
}

void print_waiting() {
  struct process *current = waiting;
  printf("waiting: ");
  while (current != NULL) {
    printf("%s ", current->name);  
    current = current->next;
  }
  printf("\n");
}

void insert_waiting(struct process *link) {
  struct process *current;
  struct process *previous;
  current = waiting;

  if (current == NULL) {
    link->next = current;
    waiting = link;
  } else {
    if (link->duration - link->elapsed_time < current->duration - current->elapsed_time) {
      link->next = current;
      waiting = link; 
    } else {
      // printf("caso1 \n");
      while (link->duration - link->elapsed_time >= current->duration - current->elapsed_time) {
        // printf("caso2 \n");
        if(current->next == NULL)
          break;
        
        previous = current;
        current = current->next;
      }

      if (current->next != NULL) {
        // printf("caso3 \n");
        previous->next = link;
        link->next = current;
      } else {
        // printf("caso4 \n");
        current->next = link;
        link->next = NULL;
      }   
    }

  }
}


struct process* get_shortest_remaining() {
  struct process *temp_link = waiting;

  waiting = waiting->next;

  return temp_link;
}

char **str_split(char * a_str, const char a_delim) {
  char * * result = 0;
  size_t count = 0;
  char * tmp = a_str;
  char * last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;

  while ( * tmp) {
    if (a_delim == * tmp) {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  count += last_comma < (a_str + strlen(a_str) - 1);
  count++;

  result = malloc(sizeof(char * ) * count);

  if (result) {
    size_t idx = 0;
    char * token = strtok(a_str, delim);

    while (token) {
      assert(idx < count); * (result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    assert(idx == count - 1); * (result + idx) = 0;
  }

  return result;
}

void read_file(char * file_name) {
  FILE * fp;
  char * line;
  size_t len = 0;
  ssize_t read;
  char * * process_detail;

  fp = fopen(file_name, "r");

  while ((read = getline( & line, & len, fp)) != -1) {
    process_detail = str_split(line, ' ');
    /*        inicio                   nome               duracao                  deadline*/
    /*printf("[%s] arrival_time: %d \n", process_detail[1], atoi(process_detail[0]));*/
    insert_new(atoi(process_detail[0]), process_detail[1], atoi(process_detail[2]), atoi(process_detail[3]), 0);
  }
  reverse();
  fclose(fp);
}


void run_process(struct process *link, int time) {
  printf("%d: [%s] %d->%d \n", time_count, link->name, time_count, time_count+time);
  // simulate_process(time);
  link->elapsed_time = link->elapsed_time + time;
  time_count += time;
} 

void fcfs() {
  struct process * link;

  while (!is_empty()) {
    link = pop();

    if (link -> arrival_time <= time_count) {
      run_process(link, link->duration);

      /*printf("%s %d %d \n", link->name, time_count, link->elapsed_time);*/
      free(link);
    } else {
      insert_process(link);
      time_count++;
    }
  }
}

struct process *get_next_shortest() {
  struct process *next = NULL;
  struct process *aux;

  if (!is_empty()) {
    next = pop();
    if (next->arrival_time == time_count) {
      while (next->arrival_time == time_count && !is_empty()) {
        insert_waiting(next);
        next = pop();
      }

      if (is_empty() && next->arrival_time == time_count) {
        insert_waiting(next);
      } else {
        insert_process(next);      
      }
      next = get_shortest_remaining();

      return next;      
    } else {
      insert_process(next);
      next = NULL;
    }
  }  
  
  if (has_waiting()) {
    aux = get_shortest_remaining();
    if (next != NULL) {
      /*printf("%s %d %d \n", next->name, next->duration, next->elapsed_time);
      printf("%s %d %d \n", aux->name, aux->duration, aux->elapsed_time);*/
      if (aux->duration - aux->elapsed_time < next->duration - next->elapsed_time) {
        insert_waiting(next);
        next = aux; 
      } else {
        insert_waiting(aux);
      }
    } else {
      next = aux;
    }
  }
  return next;
}

void srtn() {
  struct process *todo = NULL;
  struct process *doing = NULL;

  while (!is_empty() || has_waiting()) {
    todo = get_next_shortest();

    if (doing != NULL) {
      if (todo != NULL) {  
        // printf("todo: %s %d %d \n", todo->name, todo->duration, todo->elapsed_time);
        // printf("doing: %s %d %d \n", doing->name, doing->duration, doing->elapsed_time);
        if (doing->duration - doing->elapsed_time < todo->duration - todo->elapsed_time) {
          insert_waiting(todo);
        } else {
          insert_waiting(doing);
          doing = todo;
        }
      }
    } else { 
      if (todo != NULL) {  
        doing = todo;
      }
    }

    if (doing != NULL) {
      run_process(doing, 1);
      if (doing->elapsed_time >= doing->duration) {
        doing = NULL;
        free(doing);
      }
    } else {
      time_count++;
    }
  }
}

void test() {
  struct process *link;
  while(!is_empty()) {
    link = pop();
    printf("[%s] ", link->name);
    free(link);
  }
  printf("\n");
}

int main(int argc, char * * argv) {
  read_file(argv[2]);

  if (strcmp(argv[1], "1") == 0) {
    fcfs();
  } else if (strcmp(argv[1], "2") == 0) {
    srtn();
  }

  return EXIT_SUCCESS;
}