/* sim main defs */
#include "sim-basics.h"
#include "sim-base.h"

#define NUM_SMH_REGS 8
struct smh_regset
{
  int		  regs[NUM_SMH_REGS];	/* primary registers */
  int		  pc;			/* the program counter */
  int		  exception;
  unsigned long   msize;
  unsigned char * memory;
  unsigned long   insts;                /* instruction counter */
};

struct _sim_cpu
{
  struct smh_regset regset;

  sim_cpu_base base;
};

struct sim_state
{
  sim_cpu *cpu[MAX_NR_PROCESSORS];

  sim_state_base base;
};
