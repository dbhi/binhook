#include <stdio.h>

extern void func();

int main() {
  printf("Hello world!\n");
  func();
  printf("Bye!\n");
  return 0;
}
