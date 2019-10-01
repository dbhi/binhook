#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "elf_hook.h"

#define LIBFUNC_PATH "./libfunc.so" //position dependent code

void hook();

int set_hook(void * handle) {
  void *base = NULL;
  if(get_module_base_address(LIBFUNC_PATH, handle, &base)) {
    fprintf(stderr, "Failed to get module base addresses\n");
    exit(1);
  }
  printf("base: %p\n", base);
  void *orig = elf_hook(LIBFUNC_PATH, base, "func", hook);
  if (NULL == orig) {
    fprintf(stderr, "Redirection failed!\n");
    exit(1);
  }
}

int main() {
  void *handle = dlopen(LIBFUNC_PATH, RTLD_LAZY);
  if (!handle){
      fprintf(stderr, "%s\n", dlerror());
      exit(1);
  }

  typedef int func_t();

  func_t *wrapfunc = (func_t*)dlsym(handle, "wrapfunc");
  if (!wrapfunc){
      fprintf(stderr, "%s\n", dlerror());
      exit(2);
  }

  wrapfunc();

  printf("----\n");
  set_hook(handle);
  printf("----\n");

  wrapfunc();

  dlclose(handle);

  return 0;
}