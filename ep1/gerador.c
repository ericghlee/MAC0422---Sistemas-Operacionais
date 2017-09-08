#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {
  int input = 15;
  int i = 1;
  int curr = 0;

  srand(time(NULL));
  FILE* out_file;

  for (int k = 8; k < 16; k++) {
    char file_name[10] = "teste0.txt"; 
    file_name[5] = k + '0';
    out_file = fopen(file_name, "w");
    while(i <= input) { 
      if ((rand() % 10) + 1 < 3) {
        int duration = rand() % 5 + 1;
        int due_time = rand() % 5 + 1 + curr + duration;
        if ((rand() % 10) + 1 < 7) {
          fprintf(out_file, "%i %i %i p%i\n", curr, duration, due_time, i);
          i++;
        } else {
          int batch = (rand() % (input - i)) + 1;
          for (int j = 1; j <= batch; j++) {
            fprintf(out_file, "%i %i %i p%i\n", curr, j, due_time, i);
            i++;
          } 
        }

      }
      curr++;
    }
    fclose(out_file);
  }
}