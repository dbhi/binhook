#include <stdint.h>
#include <stdio.h>

char* func(char* A, char* B, uint32_t length);

char* wrapfunc(char* A, char* B, uint32_t length) {
  printf("begin wrap\n");
  char* O = func(A, B, length);
  printf("end wrap\n");
  return O;
}
