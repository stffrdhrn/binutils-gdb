## Binutils with SMH Dummy Patches

[![Build Status](https://travis-ci.org/stffrdhrn/binutils-gdb.svg?branch=smh-port)](https://travis-ci.org/stffrdhrn/binutils-gdb)

This is the port of the dummy architecture smh.  This is done following the
21 patches series for ggx which became moxie.

 - See: http://atgreen.github.io/ggx/
 - See: http://stffrdhrn.github.io/software/embedded/openrisc/2018/02/03/openrisc_gcc_rewrite.html

Here I used the latest binutils so the patches are quite a bit different in
places.  I will try to keep notes of the differences below.

### P1 differences

 - Using name `SMH` instead of ggx.

### P3 differences

 - Ensure to place `#include "sysdep.h"` before `#include &lt;stdio.h&gt;` in
   `opcodes/smh-dis.c`.

Order from the ggx patches is not allowed anylonger:

```
../ggx/ggx-03-src.patch-+#include <stdio.h>
../ggx/ggx-03-src.patch:+#include "sysdep.h"
```

This causes the error:

```
In file included from ../../binutils-gdb/opcodes/sysdep.h:34:0,
                 from ../../binutils-gdb/opcodes/smh-dis.c:4:
./config.h:7:4: error: #error config.h must be #included before system headers
 #  error config.h must be #included before system headers
    ^~~~~
```

### P7 differences

 - Used Arrays with `NULL` terminator instead of fixed size.

### P8 differences

 - make `md_apply_fix` args used
 - remove unused `shift` var from `md_apply_fix`

### P10 differences

 - Mostly a rewrite of sim to use the common infra
