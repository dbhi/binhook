#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "elf_hook.h"

#define LIBFUNC_PATH "./app" //position dependent code

char* hook(char* A, char* B, uint32_t length);

void set_hook(void * handle) {
  void *base = NULL;
  if (NULL == handle) {
    fprintf(stderr, "Failed to open \"%s\"! %s\n", LIBFUNC_PATH, dlerror());
    exit(1);
  }
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

  typedef int main_t(int, char**);
  main_t *client_main = (main_t*)dlsym(handle, "main");
  if (!client_main){
      fprintf(stderr, "%s\n", dlerror());
      exit(2);
  }

  client_main(1, (char*[]){"client", 0});

  printf("----\n");
  set_hook(handle);
  printf("----\n");

  client_main(1, (char*[]){"client", 0});

  dlclose(handle);

  return 0;
}
