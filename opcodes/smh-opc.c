/* SMH opcodes function implementation.  */

#include "sysdep.h"
#include "opcode/smh.h"

const smh_opc_info_t smh_form1_opc_info[] =
{
  { 0x00, SMH_F1_A4,	"ldi.l" },
  { 0x01, SMH_F1_AB,	"mov" },
  { 0x02, SMH_F1_4,	"jsra" },
  { 0x03, SMH_F1_NARG,	"ret" },
  { 0x04, SMH_F1_NARG,	"bad" },
  { -1, SMH_F1_NARG, NULL }
};

const smh_opc_info_t smh_form2_opc_info[] =
{
  { 0x00, SMH_F2_NARG, "bad" },
  { -1, SMH_F1_NARG, NULL }
};
