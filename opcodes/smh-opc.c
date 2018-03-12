/* SMH opcodes function implementation.  */

#include "sysdep.h"
#include "opcode/smh.h"

const smh_opc_info_t smh_form1_opc_info[] =
{
  { 0x00, SMH_F1_A4,	"ldi.l" },
  { 0x01, SMH_F1_AB,	"mov" },
  { 0x02, SMH_F1_4,	"jsra" },
  { 0x03, SMH_F1_NARG,	"ret" },
  { 0x04, SMH_F1_NARG,	"nop" },
  { 0x05, SMH_F1_ABC,	"add.l" },
  { 0x06, SMH_F1_AB,	"push" },
  { 0x07, SMH_F1_AB,	"pop" },
  { 0x08, SMH_F1_ABi,	"ld.l" },
  { 0x09, SMH_F1_AiB,	"st.l" },
  { 0x0a, SMH_F1_A4,	"lda.l" },
  { 0x0b, SMH_F1_4A,	"sta.l" },
  { 0x0c, SMH_F1_ABi4,	"ldo.l" },
  { 0x0d, SMH_F1_AiB4,	"sto.l" },
  { 0x0e, SMH_F1_NARG,	"bad" },
  { -1, SMH_F1_NARG, NULL }
};

const smh_opc_info_t smh_form2_opc_info[] =
{
  { 0x00, SMH_F2_NARG, "bad" },
  { -1, SMH_F1_NARG, NULL }
};
