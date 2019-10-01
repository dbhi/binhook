#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "elf_hook.h"

#define LIBFUNC_PATH "./app" //position dependent code

void hook();

void set_hook(void * handle) {
  void *base = NULL;
  if(get_module_base_address(LIBFUNC_PATH, handle, &base)) {
    fprintf(stderr, "Failed to get module base addresses\n");
    exit(1);
  }
  printf("base: %p\n", base);
  void *orig = elf_hook(LIBFUNC_PATH, base, "func", hook);
  if (NULL == orig) {
    fprintf(stderr, "Redirection failed!\n");
    //exit(1);
  }
}

int main() {
  void *handle = dlopen(LIBFUNC_PATH, RTLD_LAZY);
  if (!handle){
      fprintf(stderr, "%s\n", dlerror());
      exit(1);
  }

  typedef int func_t();

  func_t *ffunc = (func_t*)dlsym(handle, "func");
  if (!ffunc){
      fprintf(stderr, "%s\n", dlerror());
      exit(2);
  }

  ffunc();

  printf("----\n");
  set_hook(handle);
  printf("----\n");

  ffunc();

  dlclose(handle);

  return 0;
}
