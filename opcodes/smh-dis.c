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
  const smh_opc_info_t *opcode;
  unsigned short iword;

  stream = info->stream;
  fpr = info->fprintf_func;

  if ((status = info->read_memory_func (addr, (unsigned char*) &iword, 2, info)))
    goto fail;

  if ((iword & (1<<15)) == 0)
    {
      opcode = &smh_form1_opc_info[iword >> 9];
      switch (opcode->itype)
	{
	case SMH_F1_NARG:
	  fpr (stream, "%s", opcode->name);
	  break;
	default:
	  abort();
	}
    }
  else
    {
      opcode = &smh_form2_opc_info[(iword >> 12) & 7];
      switch (opcode->itype)
	{
	case SMH_F2_NARG:
	  fpr (stream, "%s", opcode->name);
	  break;
	default:
	  abort();
	}
    }
  return 2;

  fail:
    info->memory_error_func (status, addr, info);
    return -1;
}
