/* SMH dissassemble instructions.  */

#include "sysdep.h"
#include <stdio.h>
#define STATIC_TABLE
#define DEFINE_TABLE

#include "opcode/smh.h"
#include "dis-asm.h"

extern const smh_opc_info_t smh_opc_info[128];

static fprintf_ftype fpr;
static void *stream;

int
print_insn_smh (bfd_vma addr, struct disassemble_info *info)
{
  int status;
  unsigned char opcode;

  stream = info->stream;
  fpr = info->fprintf_func;

  if ((status = info->read_memory_func (addr, &opcode, 1, info)))
    goto fail;

  fpr (stream, "%s", smh_opc_info[opcode].name);

  return 1;

  fail:
    info->memory_error_func (status, addr, info);
    return -1;
}
