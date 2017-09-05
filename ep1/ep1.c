#include <stdio.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>
#include "process.h"
#include "file_utils.h"

struct process* waiting = NULL;
float time_count = 0;

bool has_waiting() {
  return waiting != NULL;
}

void insert_waiting(struct process *link) {
  struct process* current;
  struct process* previous;
  current = waiting;

  if (current == NULL) {
    // caso onde não ha nenhum na espera
    link->next = current;
    waiting = link;
  } else {
    if (link->duration < current->duration) {
      link->next = current;
      waiting = link; 
    } else {
      while (link->duration >= current->duration) {
        if(current->next == NULL)
          break;
        
        previous = current;
        current = current->next;
      }

      if (current->next != NULL) {
        previous->next = link;
        link->next = current;
      } else {
        current->next = link;
        link->next = NULL;
      }   
    }

  }
}

struct process* get_shortest_job() {
  struct process* temp_link = waiting;

  waiting = waiting->next;

  return temp_link;
}

void run_process(struct process* link, int time) {
  printf("%.1f: [%s] %.1f->%.1f \n", time_count, link->name, time_count, time_count+time);
  // simulate_process(time);
  link->elapsed_time = link->elapsed_time + time;
  time_count += time;
} 

// void fcfs() {
//   struct process* link;

//   while (!is_empty()) {
//     link = pop();

//     if (link -> arrival_time <= time_count) {
//       run_process(link, link->duration);

//       /*printf("%s %d %d \n", link->name, time_count, link->elapsed_time);*/
//       free(link);
//     } else {
//       insert_process(link);
//       time_count++;
//     }
//   }
// }

struct process* get_next_shortest() {
  struct process* next = NULL;
  struct process* aux;

  if (!is_empty()) {
    next = pop();
    // pega os processos que já podem ser iniciados e põe na lista de espera 
    if (next->arrival_time == time_count) {
      while (next->arrival_time == time_count && !is_empty()) {
        insert_waiting(next);
        next = pop();
      }

      // o ultimo processo
      if (is_empty() && next->arrival_time == time_count) {
        insert_waiting(next);
      } else {
        insert_process(next);      
      }
      next = get_shortest_job();

      return next;      
    } else {
      insert_process(next);
      next = NULL;
    }
  }  
  
  if (has_waiting()) {
    aux = get_shortest_job();
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

void sjf() {
  struct process* todo = NULL;
  struct process* doing = NULL;

  while (!is_empty() || has_waiting()) {
    todo = get_next_shortest();

    if (doing != NULL) {
      if (todo != NULL) {  
        // caso o proximo processo seja mais curto
        if (doing->duration < todo->duration) {
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
  struct process* link;
  while(!is_empty()) {
    link = pop();
    printf("[%s] ", link->name);
    free(link);
  }
  printf("\n");
}

int main(int argc, char** argv) {
  read_file(argv[2]);

  if (strcmp(argv[1], "1") == 0) {
    sjf();
  }

  // if (strcmp(argv[1], "1") == 0) {
    // fcfs();
  // } else if (strcmp(argv[1], "2") == 0) {
  // }

  return EXIT_SUCCESS;
}