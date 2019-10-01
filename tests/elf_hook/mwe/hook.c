#include <stdio.h>

extern void func();

void hook() {
  printf("pre-hook!\n");
  func();
  printf("post-hook!\n");
  return;
}
