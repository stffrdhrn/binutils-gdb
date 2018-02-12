/* support for bfd relocs */
#ifndef _ELF_SMH_H
#define _ELF_SMH_H

#include "elf/reloc-macros.h"

START_RELOC_NUMBERS (elf_smh_reloc_type)
  RELOC_NUMBER (R_SMH_NONE, 0)
  RELOC_NUMBER (R_SMH_DATA_DIR32, 1)
END_RELOC_NUMBERS (R_SMH_max)

#endif
