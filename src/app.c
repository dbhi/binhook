#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern char* func(char* A, char* B, uint32_t length);

int main() {
  printf("Hello world!\n");

  uint32_t length = 3;
  char *A = malloc(length*sizeof(int32_t));
  char *B = malloc(length*sizeof(int32_t));

  int i;
  for ( i=0; i<length; i++ ) {
    ((int32_t*)A)[i]= rand() % 100;
    ((int32_t*)B)[i] = rand() % 100;
  }

  func(A, B, length);

  printf("Bye!\n");

  return 0;
}
