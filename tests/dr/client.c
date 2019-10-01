#include "dr_api.h"
#include "drmgr.h"
#include "drwrap.h"
#include <stdint.h>

#define orig_name "func"
#define func_name hook
#define func_argd char* A, char* B, uint32_t length
#define func_args A, B, length

extern void func_name (func_argd);
void *dbhi_ctx;

void dbhi_func (func_argd) {
  func_name (func_args);
  drwrap_replace_native_fini(dbhi_ctx);
}

static void module_load_event(void *drcontext, const module_data_t *mod, bool loaded) {
  dbhi_ctx = drcontext;
  app_pc orig_pc = (app_pc)dr_get_proc_address(mod->handle, orig_name);
  if (orig_pc != NULL) {
    drwrap_replace_native ( orig_pc , (app_pc)dbhi_func , false , 0 , NULL , false );
  }
}

static void event_exit(void) {
  drwrap_exit();
  drmgr_exit();
}

DR_EXPORT void dr_client_main(client_id_t id, int argc, const char *argv[]) {
  dr_set_client_name("DynamoRIO Sample Client 'wrap'", "http://dynamorio.org/issues");
  dr_log(NULL, DR_LOG_ALL, 1, "Client 'wrap' initializing\n");
  drmgr_init();
  drwrap_init();
  dr_register_exit_event(event_exit);
  drmgr_register_module_load_event(module_load_event);
}
