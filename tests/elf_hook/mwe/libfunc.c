#include <stdio.h>

void func() {
  printf("func!\n");
}

void wrapfunc() {
  printf("begin wrap\n");
  func();
  printf("end wrap\n");
}