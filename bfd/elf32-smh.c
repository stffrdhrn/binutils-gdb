/* smh elf support */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/smh.h"

static bfd_reloc_status_type
smh_elf_reloc (bfd *abfd,
	       arelent *reloc_entry,
	       asymbol *symbol_in,
	       void *data,
	       asection *input_section,
	       bfd *output_bfd,
	       char **error_message ATTRIBUTE_UNUSED)
{
  unsigned long insn;
  bfd_vma sym_value;
  enum elf_smh_reloc_type r_type;
  bfd_vma addr = reloc_entry->address;
  bfd_byte *hit_data = addr + (bfd_byte *) data;

  r_type = (enum elf_smh_reloc_type) reloc_entry->howto->type;

  if (output_bfd != NULL)
    {
      reloc_entry->address += input_section->output_offset;
      return bfd_reloc_ok;
    }

  if (symbol_in != NULL
      && bfd_is_und_section (symbol_in->section))
    return bfd_reloc_undefined;

  if (bfd_is_com_section (symbol_in->section))
    sym_value = 0;
  else
    sym_value = (symbol_in->value +
		 symbol_in->section->output_section->vma +
		 symbol_in->section->output_offset);

  switch (r_type)
    {
    case R_SMH_DATA_DIR32:
      insn = sym_value + reloc_entry->addend;
      bfd_put_32 (abfd, (bfd_vma) insn, hit_data);
      break;

    default:
      abort ();
      break;
    }
  return bfd_reloc_ok;
}

static reloc_howto_type smh_elf_howto_table[] =
{
  HOWTO (R_SMH_NONE,
	 0,
	 0,
	 0,
	 FALSE,
	 0,
	 complain_overflow_dont,
	 smh_elf_reloc,
	 "R_SMH_NONE",
	 FALSE,
	 0,
	 0,
	 FALSE),
  HOWTO (R_SMH_DATA_DIR32,
	 0,
	 2,		/* 0 - byte, 1 - short, 2 - long) */
	 32,
	 FALSE,
	 0,
	 complain_overflow_bitfield,
	 smh_elf_reloc,
	 "R_SMH_DIR32",
	 TRUE,		/* partial inplace */
	 0xffffffff,
	 0xffffffff,
	 FALSE),	/* pcrel offset */
};

struct elf_reloc_map
{
  bfd_reloc_code_real_type bfd_reloc_val;
  unsigned char elf_reloc_val;
};

static const struct elf_reloc_map smh_reloc_map[] =
{
  { BFD_RELOC_NONE,	R_SMH_NONE },
  { BFD_RELOC_32,	R_SMH_DATA_DIR32 },
};

static reloc_howto_type *
smh_elf_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			   bfd_reloc_code_real_type code)
{
  unsigned int i;

  for (i = 0; i < sizeof (smh_reloc_map) / sizeof (struct elf_reloc_map); i++)
    if (smh_reloc_map[i].bfd_reloc_val == code)
      return &smh_elf_howto_table[(int) smh_reloc_map[i].elf_reloc_val];

  return NULL;
}

static reloc_howto_type *
smh_elf_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			   const char *r_name)
{
  unsigned int i;

  for (i = 0;
       i < sizeof (smh_elf_howto_table) / sizeof (smh_elf_howto_table[0]); i++)
    if (smh_elf_howto_table[i].name != NULL
        && strcasecmp (smh_elf_howto_table[i].name, r_name) == 0)
      return &smh_elf_howto_table[i];

  return NULL;
}

static void
smh_elf_info_to_howto (bfd *abfd ATTRIBUTE_UNUSED,
		       arelent *cache_ptr,
		       Elf_Internal_Rela *dst)
{
  unsigned int r;

  r = ELF32_R_TYPE (dst->r_info);

  BFD_ASSERT (r < (unsigned int) R_SMH_max);

  cache_ptr->howto = &smh_elf_howto_table[r];
}

#define TARGET_BIG_SYM	smh_elf32_vec
#define TARGET_BIG_NAME	"elf32-smh"
#define ELF_ARCH	bfd_arch_smh
#define ELF_MACHINE_CODE	EM_SMH
#define ELF_MAXPAGESIZE		1
#define bfd_elf32_bfd_reloc_type_lookup smh_elf_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup smh_elf_reloc_name_lookup
#define elf_info_to_howto		smh_elf_info_to_howto

#include "elf32-target.h"
