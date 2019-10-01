#ifdef PLUGINS_NEW

#include <stdio.h>
#include <assert.h>
#include <locale.h>
#include <plugins.h>

#define orig_name "{{ .Orig }}"
#define func_name {{ .Func }}
#define func_argd {{ .Argd }}
#define func_handler {{ .Hand }}

extern void func_name(func_argd);

int func_handler(mambo_context *ctx) {
  uint32_t *wrp = (uint32_t *)ctx->code.write_p;
#ifdef __arm__
  int inst_set = mambo_get_inst_type(ctx);
  if (inst_set == ARM_INST) {
    uint32_t *write_p = wrp;
    arm_push_regs((1 << es) | (1 << lr));
    arm_copy_to_reg_32bit(&write_p, es, (uintptr_t)func_name);
    arm_blx(&write_p, es);
    write_p++;
    arm_pop_regs((1 << es) | (1 << lr));
    arm_inline_hash_lookup(ctx->thread_data, (uint32_t**)&write_p, ctx->code.fragment_id, lr);
    wrp = write_p;
  } else if (inst_set == THUMB_INST) {
    uint16_t *write_p = (uint16_t *)wrp;
    thumb_push_regs(&write_p, (1 << es) | (1 << lr));
    copy_to_reg_32bit(&write_p, es, (uintptr_t)func_name);
    thumb_blx16(&write_p, es);
    write_p++;
    thumb_pop_regs(&write_p, (1 << es) | (1 << lr));
    thumb_inline_hash_lookup(ctx->thread_data, &write_p, ctx->code.fragment_id, lr);
    wrp = (uint32_t*)write_p;
  }

#elif __aarch64__
  uint32_t *write_p = wrp;
  a64_push_pair_reg(es, lr)
  a64_copy_to_reg_64bits(&write_p, es, (uintptr_t)func_name);
  a64_BLR(&write_p, es);
  write_p++;
  a64_pop_pair_reg(es, lr)
  a64_inline_hash_lookup(ctx->thread_data, ctx->code.fragment_id, &write_p,
                         ctx->code.read_address, lr, false, true);
  wrp = write_p;
#endif
  ctx->code.write_p = (void *)wrp;
  return 0;
}

void mambo_register_function_replace(mambo_context *ctx, char *func, void *replace) {
  // TODO `void *replace` is not used ATM. It'd be interesing to pass `*replace` (`func_name`) to `func_handler` here, instead of using defines.
  mambo_register_function_cb(ctx, func, func_handler, NULL);
}

__attribute__((constructor)) void ghdl_plugin() {
  mambo_context *ctx = mambo_register_plugin();
  assert(ctx != NULL);
  mambo_register_function_replace(ctx, orig_name, func_name);
  setlocale(LC_NUMERIC, "");
}

#endif
