/* smh elf support */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"

#define TARGET_BIG_SYM	smh_elf32_vec
#define TARGET_BIG_NAME	"elf32-smh"
#define ELF_ARCH	bfd_arch_smh
#define ELF_MACHINE_CODE	EM_SMH
#define ELF_MAXPAGESIZE		1
#define bfd_elf32_bfd_reloc_type_lookup bfd_default_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup _bfd_norelocs_bfd_reloc_name_lookup
#define elf_info_to_howto	_bfd_elf_no_info_to_howto

#include "elf32-target.h"
