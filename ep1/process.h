#ifndef PROCESS_H  
#define PROCESS_H

struct process {
	float arrival_time;
	char* name;
	float duration;
	float elapsed_time;
	float deadline;
	struct process* next;
};

void insert_new(float arrival_time, float duration, float deadline, char* name);

void insert_process(struct process *link);

void reverse();

struct process *pop();

bool is_empty();

void simulate_process(int duration);

void print_all();

#endif