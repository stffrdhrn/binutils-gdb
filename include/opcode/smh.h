/* SMH opcodes types.  */

#define SMH_F1_NARG	0x100
#define SMH_F1_A	0x101
#define SMH_F1_AB	0x102
#define SMH_F1_ABC	0x103
#define SMH_F1_A4	0x104
#define SMH_F1_4	0x105
#define SMH_F1_AiB	0x106
#define SMH_F1_ABi	0x107
#define SMH_F1_4A	0x108

#define SMH_F2_NARG	0x200
#define SMH_F2_12V	0x201

typedef struct smh_opc_info_t
{
  short opcode;
  unsigned itype;
  const char *name;
} smh_opc_info_t;

extern const smh_opc_info_t smh_form1_opc_info[];
extern const smh_opc_info_t smh_form2_opc_info[];
