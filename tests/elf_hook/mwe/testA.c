#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "elf_hook.h"

#define LIBFUNC_PATH "./libfunc.so" //position dependent code

void wrapfunc();
void hook();

int set_hook(void * handle) {
  void *base = NULL;
  if (NULL == handle) {
    fprintf(stderr, "Failed to open \"%s\"! %s\n", LIBFUNC_PATH, dlerror()); exit(1);
  }
  if(get_module_base_address(LIBFUNC_PATH, handle, &base)) {
    fprintf(stderr, "Failed to get module base addresses\n"); exit(1);
  }
  printf("base: %p\n", base);
  void *orig = elf_hook(LIBFUNC_PATH, base, "func", hook);
  if (NULL == orig) {
    fprintf(stderr, "Redirection failed!\n"); exit(1);
  }
}

int main() {
  wrapfunc();

  void *handle = dlopen(LIBFUNC_PATH, RTLD_LAZY);
  printf("----\n");
  set_hook(handle);
  printf("----\n");

  wrapfunc();

  dlclose(handle);
  return 0;
}