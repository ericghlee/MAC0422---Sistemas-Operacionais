#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <time.h>
#include "process.h"

struct process * default_head = NULL;
struct process * current = NULL;

void insert_new(int arrival_time, char* name, int duration, int deadline, int elapsed_time) {
  struct process* link = (struct process* ) malloc(sizeof(struct process));

  link -> arrival_time = arrival_time;
  link -> name = name;
  link -> duration = duration;
  link -> deadline = deadline;
  link -> elapsed_time = elapsed_time;
  link -> next = default_head;

  default_head = link;
}

void insert_process(struct process *link) {
  link->next = default_head;
  default_head = link;
}

void reverse() {
  struct process *prev = NULL;
  struct process *current = default_head;
  struct process *next;

  while (current != NULL) {
    next = current -> next;
    current -> next = prev;
    prev = current;
    current = next;
  }

  default_head = prev;
}

struct process *pop() {
  struct process * temp_link = default_head;

  default_head = default_head -> next;

  return temp_link;
}

bool is_empty() {
  return default_head == NULL;
}

void simulate_process(int duration) {
  clock_t begin = clock();
  clock_t end;
  double time_spent;
  int i, num = 1, primes = 0;

  while (true) {
    i = 2;
    while (i <= num) {
      if (num % i == 0)
        break;
      i++;
    }
    if (i == num)
      primes++;

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    if (time_spent >= duration) {
      break;
    }
  }
}

void print_all() {
  struct process *current;
  current = default_head;
  printf("fila: ");
  while (current != NULL) {
    printf("%s ", current->name);  
    current = current->next;
  }
  printf("\n");
}