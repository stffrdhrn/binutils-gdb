/* Macros for smh gas.  */

#define TC_SMH 1
#define TARGET_BYTES_BIG_ENDIAN 1
#define WORKING_DOT_WORD
#define TARGET_FORMAT "elf32-smh"
#define TARGET_ARCH bfd_arch_smh

#define md_undefined_symbol(NAME)           0

#define md_estimate_size_before_relax(A, B) (as_fatal (_("estimate size\n")),0)
#define md_convert_frag(B, S, F)            as_fatal (_("convert_frag\n"))

#define md_pcrel_from(FIX) \
  ((FIX)->fx_where + (FIX)->fx_frag->fr_address - 1)

#define md_section_align(SEGMENT, SIZE)     (SIZE)
