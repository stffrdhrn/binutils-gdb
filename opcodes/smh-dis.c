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

#define OP_A(i) ((i >> 6) & 0x7)
#define OP_B(i) ((i >> 3) & 0x7)
#define OP_C(i) ((i >> 0) & 0x7)

static const char *reg_names[8] =
  { "$fp", "$sp", "$r0", "$r1", "$r2", "$r3", "$r4", "$r5" };

int
print_insn_smh (bfd_vma addr, struct disassemble_info *info)
{
  int length = 2;
  int status;
  const smh_opc_info_t *opcode;
  bfd_byte buffer[4];
  unsigned short iword;

  stream = info->stream;
  fpr = info->fprintf_func;

  if ((status = info->read_memory_func (addr, buffer, 2, info)))
    goto fail;
  iword = bfd_getb16 (buffer);

  if ((iword & (1<<15)) == 0)
    {
      opcode = &smh_form1_opc_info[iword >> 9];
      switch (opcode->itype)
	{
	case SMH_F1_NARG:
	  fpr (stream, "%s", opcode->name);
	  break;
	case SMH_F1_AB:
	  fpr (stream, "%s\t%s, %s", opcode->name,
	       reg_names[OP_A (iword)],
	       reg_names[OP_B (iword)]);
	  break;
	case SMH_F1_ABC:
	  fpr (stream, "%s\t%s, %s, %s", opcode->name,
	       reg_names[OP_A (iword)],
	       reg_names[OP_B (iword)],
	       reg_names[OP_C (iword)]);
	  break;
	case SMH_F1_A4:
	case SMH_F1_4:
	case SMH_F1_4A:
	  {
	    unsigned imm;
	    if ((status = info->read_memory_func (addr+2, buffer, 4, info)))
	      goto fail;
	    imm = bfd_getb32 (buffer);

	    if (opcode->itype == SMH_F1_4)
	      fpr (stream, "%s\t0x%x", opcode->name, imm);
	    else if (opcode->itype == SMH_F1_A4)
	      fpr (stream, "%s\t%s, 0x%x", opcode->name,
	 	   reg_names[OP_A(iword)], imm);
	    else if (opcode->itype == SMH_F1_4A)
	      fpr (stream, "%s\t0x%x, %s", opcode->name,
		   imm, reg_names[OP_A(iword)]);
	    else
	      abort ();

	    length = 6;
	  }
	  break;
	case SMH_F1_AiB:
	  fpr (stream, "%s\t(%s), %s", opcode->name,
	       reg_names[OP_A (iword)],
	       reg_names[OP_B (iword)]);
	  break;
	case SMH_F1_ABi:
	  fpr (stream, "%s\t%s, (%s)", opcode->name,
	       reg_names[OP_A (iword)],
	       reg_names[OP_B (iword)]);
	  break;
	case SMH_F1_AiB4:
	  {
	    unsigned int imm;

	    if ((status = info->read_memory_func (addr+2, buffer, 4, info)))
	      goto fail;
	    imm = bfd_getb32 (buffer);

	    fpr (stream, "%s\t0x%x(%s), %s", opcode->name,
		 imm,
	  	 reg_names[OP_A (iword)],
		 reg_names[OP_B (iword)]);
	    length = 6;
	  }
	  break;
	case SMH_F1_ABi4:
	  {
	    unsigned int imm;

	    if ((status = info->read_memory_func (addr+2, buffer, 4, info)))
	      goto fail;
	    imm = bfd_getb32 (buffer);

	    fpr (stream, "%s\t%s, 0x%x(%s)", opcode->name,
		 reg_names[OP_A (iword)],
		 imm,
		 reg_names[OP_B (iword)]);
	    length = 6;
	  }
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
  return length;

  fail:
    info->memory_error_func (status, addr, info);
    return -1;
}
