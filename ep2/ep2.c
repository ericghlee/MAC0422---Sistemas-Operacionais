#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// Seu codigo deve possuir um vetor compartilhado “pista” que tem um tamanho igual a d
int length = 250;
int n_cyclists = 1;
int final_lap = 160;
int lap = 0;

// A simulacao do seu codigo deve simular a corrida em intervalos de 60ms ate as duas ultimas voltas.
int time_step = 60;

struct cyclist {
	int speed;
	int position;
	int lane;
};

bool has_blocking(struct cyclist *c) {

}

void move(struct cyclist *c) {

}

void start(struct cyclist *c) {
	while(c->lap < 160) {
		move(c);
	}
}

void setCyclistPosition(struct cyclist *c, int position) {

}

void prepareAndStart(struct cyclist *c) {
	c = (struct cyclist* ) malloc(sizeof(struct cyclist));	
	c->speed = 30;
	c->lap = 0;

	start(c);
};

// Seu simulador deve criar n threads “ciclista” iguais.
void createThreads(int n, struct cyclist *cyclists[]) {
	pthread_t cthreads[n];

	for (int i = 0; i < n; i++) {
		if (pthread_create( &cthreads[i], NULL, prepareAndStart(cyclists[i]), NULL) ) {
			printf("Erro ao criar a thread");
			abort();
		}
	}
}

void prepareLap(struct cyclist c) {
	int r = (rand() % 100) + 1;
		
	// Caso a volta anterior tenha sido feita a 30Km/h, o sorteio e feito com 70%
	// de chance de escolher 60Km/h e 30% de chance de escolher 30Km/h.
	if (c.speed == 30) {
		if (r < 70) {
			c.speed = 60;
		}
	}
	
	// Caso a volta anterior tenha sido feita a 60Km/h, o sorteio e feito com 50%
	// de chance de escolher 30Km/h e 50% de chance de escolher 60Km/h.
	else {
		if (r < 50) {
			c.speed = 30;
		}
	}
}

int main(int argc, char **argv) {
    int pista[length];
    struct cyclist *cyclists[n_cyclists]; 
    srand(time(NULL)); 

    return EXIT_SUCCESS;
}