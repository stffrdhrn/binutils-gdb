/* SMH cpu file.  */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

const bfd_arch_info_type bfd_smh_arch =
  {
    32, /* word bits */
    32, /* address bits */
    8,  /* byte bits */
    bfd_arch_smh,
    bfd_mach_smh,
    "smh",
    "smh",
    2, /* alignment power */
    TRUE,
    bfd_default_compatible,
    bfd_default_scan,
    bfd_arch_default_fill,
    0,
  };
