#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char* func(char* A, char* B, uint32_t length) {
  printf("replacement: %p %p %d\n", A, B, length);

  char *O = malloc(length*sizeof(int32_t));;
  int i;
  for ( i=0; i<length; i++ ) {
    ((int32_t*)O)[i] = ((int32_t*)A)[i] * ((int32_t*)B)[i] / 4;
    printf("replacement: %d * %d /4 = %d\n", ((int32_t*)A)[i], ((int32_t*)B)[i], ((int32_t*)O)[i]);
  }

  return O;
}
