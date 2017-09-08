#include <stdio.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include "process.h"
#include "file_utils.h"

struct process* waiting = NULL;
float time_count = 0;
float quantum = 1;
bool active_print = false;
float tempo_estourado = 0;
int procesos_estourados = 0;

void print_line(struct process* p) {
  fprintf(stderr, "%.1f %.1f %.1f %s\n", p->arrival_time, p->deadline, p->duration, p->name);
}

bool equals_float(float a, float b) {
  return fabs(a-b) < 0.001;
}

bool has_waiting() {
  return waiting != NULL;
}

struct process* get_next() {
  struct process* temp_link = waiting;

  waiting = waiting->next;

  return temp_link;
}

void run_process(struct process* link, float time) {
  // simulate_process(time);
  link->elapsed_time = link->elapsed_time + time;
  time_count += time; 
  // printf("%.1f: [%s] %.1f->%.1f\n", time_count, link->name, time_count, time_count+time);

  if (equals_float(link->duration - link->elapsed_time, 0) && time_count > link->deadline) {
    tempo_estourado += time_count - link->deadline;
    procesos_estourados++;
  }

} 

void insert_sjf_waiting(struct process *link) {
  struct process* current;
  struct process* previous;
  current = waiting;

  if (current == NULL) {
    // caso onde não ha nenhum na espera
    link->next = current;
    waiting = link;
  } else {
    // compara qual o menor
    if (link->duration < current->duration) {
      link->next = current;
      waiting = link; 
    } else {
      while (link->duration > current->duration || equals_float(link->duration, current->duration)) {
        if (current->next == NULL)
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

struct process* get_next_sjf() {
  struct process* next = NULL;
  struct process* aux;

  if (!is_empty()) {
    next = pop();
    // pega os processos que já podem ser iniciados e põe na lista de espera 
    if (equals_float(next->arrival_time, time_count)) {
      while (equals_float(next->arrival_time, time_count) && !is_empty()) {
        insert_sjf_waiting(next);
        if (active_print)
          print_line(next);
        next = pop();
      }

      // o ultimo processo
      if (is_empty() && equals_float(next->arrival_time, time_count)) {
        insert_sjf_waiting(next);
          if (active_print)
            print_line(next);
      } else {
        insert_process(next);      
      }
      next = get_next();

      return next;      
    } else {
      insert_process(next);
      next = NULL;
    }
  }  
  
  if (has_waiting()) {
    aux = get_next();
    if (next != NULL) {
      if (aux->duration < next->duration) {
        insert_sjf_waiting(next);
        next = aux; 
      } else {
        insert_sjf_waiting(aux);
      }
    } else {
      next = aux;
    }
  }

  return next;
}

// shortest job first
void sjf() {
  struct process* todo = NULL;
  struct process* doing = NULL;

  while (!is_empty() || has_waiting()) {
    todo = get_next_sjf();

    if (doing != NULL) {
      if (todo != NULL) {  
        // caso o proximo processo seja mais curto
        if (doing->duration < todo->duration) {
          insert_sjf_waiting(todo);
        } else {
          // mudanca de contexto
          insert_sjf_waiting(doing);
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
        // printf("[%s] %.1f->%.1f \n", doing->name, doing->arrival_time, time_count);
        insert_line(doing, time_count);
        doing = NULL;
        free(doing);
      }
    } else {
      time_count++;
    }
  }

  if (doing != NULL) {
    run_process(doing, doing->duration - doing->elapsed_time);
    insert_line(doing, time_count);
    // printf("[%s] %.1f->%.1f \n", doing->name, doing->arrival_time, time_count);
    doing = NULL;
    free(doing);
  }
}

// insere um novo processo como ultimo da fila
void insert_rr_waiting(struct process *link) {
  struct process* aux;

  if (waiting == NULL) {
    link->next = waiting;
    waiting = link;
  } else {
    aux = waiting;
    while(aux->next != NULL) {
      aux = aux->next;
    }
    aux->next = link;
    link->next = NULL;
  }
}

struct process* get_next_batch() {
  struct process* next = NULL;

  if (!is_empty()) {
    next = pop();
    if (next->arrival_time <= time_count) {
      while (equals_float(next->arrival_time, time_count) && !is_empty()) {
        insert_rr_waiting(next);
        if (active_print)
          print_line(next);
        next = pop();
      }

      // ultimo processo
      if (is_empty() && equals_float(next->arrival_time, time_count)) {
        insert_rr_waiting(next);
      } else {
        insert_process(next);      
      }
      next = get_next();

      return next;      
    } else {
      insert_process(next);
      next = NULL;
    }
  }  

  if (has_waiting()) {
    return get_next();
  }
  
  return next;
}

void verify_rr(float relative_time) {
  struct process* next = NULL;

  if (!is_empty()) {
    next = pop();
    // pega os processos que já podem ser iniciados e põe na lista de espera 
    if (equals_float(next->arrival_time, relative_time)) {
      while (equals_float(next->arrival_time, relative_time) && !is_empty()) {
        insert_rr_waiting(next);
        if (active_print)
          print_line(next);
        next = pop();
      }

      // ultimo processo
      if (is_empty() && equals_float(next->arrival_time, relative_time)) {
        insert_rr_waiting(next);
      } else {
        insert_process(next);      
      }
    } else {
      insert_process(next);
      next = NULL;
    }
  }  
}

// round robin
void rr() {
  struct process* todo = NULL;
  float counter;

  // enquanto houver processos novos
  while (!is_empty() || has_waiting()) {
    todo = get_next_batch();
    counter = time_count;

    if(todo != NULL) {
      run_process(todo, quantum);

      while(!equals_float(time_count, counter)) {
        verify_rr(counter);
        counter += 0.1;
      }

      if (equals_float(todo->duration - todo->elapsed_time, 0)) {
        // printf("[%s] %.1f->%.1f \n", todo->name, todo->arrival_time, time_count);
        insert_line(todo, time_count);
        todo = NULL;
        free(todo);
      } else {
        insert_rr_waiting(todo);  
      }
    } else {
      time_count += 0.1;
    }
  }
}

void insert_ps_waiting(struct process *link) {
  struct process* current;
  struct process* previous;
  current = waiting;

  if (current == NULL) {
    // caso onde não ha nenhum na espera
    link->next = current;
    waiting = link;
  } else {
    // compara qual tem mais prioridade
    // link->duration < current->duration
    if (link->deadline-time_count < current->deadline-time_count) {
      link->next = current;
      waiting = link; 
    } else {
      while (link->deadline-time_count > current->deadline-time_count || equals_float(link->deadline-time_count, current->deadline-time_count)) {
        if (current->next == NULL)
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

struct process* get_next_ps() {
  struct process* next = NULL;
  struct process* aux;

  if (!is_empty()) {
    next = pop();
    // pega os processos que já podem ser iniciados e põe na lista de espera 
    if (equals_float(next->arrival_time, time_count)) {
      while (equals_float(next->arrival_time, time_count) && !is_empty()) {
        insert_ps_waiting(next);
        if (active_print)
          print_line(next);

        next = pop();
      }

      // o ultimo processo
      if (is_empty() && equals_float(next->arrival_time, time_count)) {
        insert_ps_waiting(next);
      } else {
        insert_process(next);      
      }
      next = get_next();

      return next;      
    } else {
      insert_process(next);
      next = NULL;
    }
  }  
  
  if (has_waiting()) {
    aux = get_next();
    if (next != NULL) {
      if (aux->deadline-time_count < next->deadline-time_count) {
        insert_sjf_waiting(next);
        next = aux; 
      } else {
        insert_sjf_waiting(aux);
      }
    } else {
      next = aux;
    }
  }

  return next;
}

// priority scheduling
void ps() {
  struct process* todo = NULL;
  struct process* doing = NULL;

  while (!is_empty() || has_waiting()) {
    todo = get_next_ps();

    if (doing != NULL) {
      if (todo != NULL) {  
        // caso o proximo processo seja mais curto
        if (doing->deadline-time_count < todo->deadline-time_count) {
          insert_ps_waiting(todo);
        } else {
          // mudanca de contexto
          insert_ps_waiting(doing);
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
        // printf("[%s] %.1f->%.1f \n", doing->name, doing->arrival_time, time_count);
        insert_line(doing, time_count);
        doing = NULL;
        free(doing);
      }
    } else {
      time_count++;
    }
  }

  if (doing != NULL) {
    run_process(doing, doing->duration - doing->elapsed_time);
    // printf("[%s] %.1f->%.1f \n", doing->name, doing->arrival_time, time_count);
    insert_line(doing, time_count);
    doing = NULL;
    free(doing);
  }
}

int main(int argc, char** argv) {
  if(argv[4] != NULL && strcmp(argv[1], "d")) {
    active_print = true;
  } 

  read_file(argv[2]);

  create_out_file(argv[3]);

  if (strcmp(argv[1], "1") == 0) {
    sjf(); // shortest job first
  } else if(strcmp(argv[1], "2") == 0) {
    rr(); // round robin
  } else if(strcmp(argv[1], "3") == 0) {
    ps(); // priority scheduling
  } 


  close_file();
  printf("tempo_estourado: %.2f\n", tempo_estourado);
  printf("procesos_estourados: %i\n", procesos_estourados);
  return EXIT_SUCCESS;
}
