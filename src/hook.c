#include <stdio.h>
#include <stdint.h>

extern char* func(char* A, char* B, uint32_t length);

char* hook(char* A, char* B, uint32_t length) {
  printf("pre-hook!\n");
  char* O = func(A, B, length);
  printf("post-hook!\n");
  return O;
}
