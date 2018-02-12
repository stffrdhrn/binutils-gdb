/* Simulator for SMH dummy processor */

#include "sim-main.h"
#include "sim-options.h"
#include <sys/times.h>
#include <sys/param.h>
#include <netinet/in.h>	/* for byte ordering macros */
#include "bfd.h"
#include "gdb/callback.h"
#include "libiberty.h"
#include "gdb/remote-sim.h"

typedef int word;
typedef unsigned int uword;

static const char *reg_names[8] =
  { "$fp", "$sp", "$r0", "$r1", "$r2", "$r3", "$r4", "$r5" };

#define EXTRACT_WORD(addr) \
  ((sim_core_read_aligned_1 (cpu, cia, read_map, addr) << 24) \
   + (sim_core_read_aligned_1 (cpu, cia, read_map, addr+1) << 16) \
   + (sim_core_read_aligned_1 (cpu, cia, read_map, addr+2) << 8) \
   + (sim_core_read_aligned_1 (cpu, cia, read_map, addr+3)))

static INLINE void
wlat (sim_cpu *scpu, word pc, word x, word v)
{
  address_word cia = CPU_PC_GET (scpu);

  sim_core_write_aligned_4 (scpu, cia, write_map, x, v);
}

static INLINE int
rlat (sim_cpu *scpu, word pc, word x)
{
  address_word cia = CPU_PC_GET (scpu);

  return (sim_core_read_aligned_4 (scpu, cia, read_map, x));
}

static unsigned long
smh_extract_unsigned_integer (unsigned char * addr, int len)
{
  unsigned long retval;
  unsigned char * p;
  unsigned char * startaddr = (unsigned char *)addr;
  unsigned char * endaddr = startaddr + len;

  if (len > (int) sizeof (unsigned long))
    printf ("That operation is not available on integers of more than %ld bytes.",
	    sizeof (unsigned long));

  /* Start at the most significant end of the integer, and work towards
     the least significant.  */
  retval = 0;

  for (p = endaddr; p > startaddr;)
    retval = (retval << 8) | * -- p;

  return retval;
}

static void
smh_store_unsigned_integer (unsigned char * addr, int len, unsigned long val)
{
  unsigned char * p;
  unsigned char * startaddr = (unsigned char *)addr;
  unsigned char * endaddr = startaddr + len;

  for (p = startaddr; p < endaddr;)
    {
      * p ++ = val & 0xff;
      val >>= 8;
    }
}

static void
set_initial_gprs (SIM_CPU *cpu)
{
  int i;

  /* Set up machine just out of reset.  */
  cpu->regset.pc = 0;

  /* Clean out the register contents.  */
  for (i = 0; i < NUM_SMH_REGS; i++)
    cpu->regset.regs[i] = 0;
}

static void
free_state (SIM_DESC sd)
{
  if (STATE_MODULES (sd) != NULL)
    sim_module_uninstall (sd);
  sim_cpu_free_all (sd);
  sim_state_free (sd);
}

void
sim_engine_run (SIM_DESC sd,
		int next_cpu_nr, /* ignore  */
		int nr_cpus, /* ignore  */
		int siggnal) /* ignore  */
{
  word pc, opc;
  unsigned short inst;
  sim_cpu *cpu = STATE_CPU (sd, 0); /* FIXME */
  address_word cia = CPU_PC_GET (cpu);

  pc = cpu->regset.pc;

  /* Run instructions here. */

  do
    {
      opc = pc;

      /* Fetch the instruction at pc.  */
      inst = (sim_core_read_aligned_1 (cpu, cia, read_map, pc) << 8)
	+ sim_core_read_aligned_1 (cpu, cia, read_map, pc+1);

      /* Decode instruction.  */
      if (inst & (1 << 15))
	{
	  /* This is a Form 2 instruction.  */
	  /* We haven't implemented any yet, so just SIGILL for now.  */
	  TRACE_INSN (cpu, "SIGILL");
	  sim_engine_halt (sd, cpu, NULL, pc, sim_stopped, SIM_SIGILL);
	  break;
	}
      else
	{
	  /* This is a Form 1 instruction.  */
	  int opcode = inst >> 9;
	  switch (opcode)
	    {
	    case 0x00: /* ld.w (immediate) */
	      {
		int reg = (inst >> 6) & 0x7;

		unsigned int val = EXTRACT_WORD (pc + 2);
		cpu->regset.regs[reg] = val;
		pc += 4;

		TRACE_INSN (cpu, "# 0x%08x: %s = 0x%x", opc,
			    reg_names[reg], val);
	      }
	      break;
	    case 0x01: /* mov (register-to-register) */
	      {
		int dest  = (inst >> 6) & 0x7;
		int src = (inst >> 3) & 0x7;
		cpu->regset.regs[dest] = cpu->regset.regs[src];

		TRACE_INSN (cpu, "# 0x%08x: %s = %s (0x%x)",
			    opc,
			    reg_names[dest],
			    reg_names[src], cpu->regset.regs[src]);
	      }
	      break;
	    case 0x02: /* jsra */
	      {
		unsigned int fn = EXTRACT_WORD (pc + 2);
		unsigned int sp = cpu->regset.regs[1];

		/* Push return address and decrement stack.  */
		wlat (cpu, opc, sp, pc + 6);
		sp -= 4;

		/* Push frame pointer.  */
		wlat (cpu, opc, sp, cpu->regset.regs[0]);
		sp -= 4;

		cpu->regset.regs[1] = sp;
		pc = fn - 2;
		TRACE_INSN (cpu, "# 0x%08x: jsra 0x%x", opc, pc + 2);
	      }
	      break;
	    case 0x03: /* ret */
	      {
		unsigned int sp = cpu->regset.regs[1];

		/* Pop frame pointer.  */
		sp += 4;
		cpu->regset.regs[0] = rlat (cpu, opc, sp);

		/* Pop return address.  */
		sp += 4;
		pc = rlat (cpu, opc, sp) - 2;

		/* Update the sp register.  */
		cpu->regset.regs[1] = sp;

		TRACE_INSN (cpu, "# 0x%08x: ret (to 0x%x)", opc, pc + 2);
	      }
	      break;
	    case 0x04: /* nop */
		TRACE_INSN (cpu, "# 0x%08x: nop", opc);
		break;
	    default:
	      TRACE_INSN (cpu, "SIGILL1");
	      sim_engine_halt (sd, cpu, NULL, pc, sim_stopped, SIM_SIGILL);
	      break;
	    }
	}

      /* Hide away the things we've cached while executing.  */
      cpu->regset.insts++;		/* instructions done ... */
      pc += 2;
      cpu->regset.pc = pc;

      if (sim_events_tick (sd))
	sim_events_process (sd);

    } while (1);
}

static int
smh_reg_store (SIM_CPU *cpu, int rn, unsigned char *memory, int length)
{
  if (rn < NUM_SMH_REGS && rn >= 0)
    {
      if (length == 4)
	{
	  long ival;

	  /* misalignment safe */
	  ival = smh_extract_unsigned_integer (memory, 4);
	  cpu->regset.regs[rn] = ival;
	}

      return 4;
    }
  else
    return 0;
}

static int
smh_reg_fetch (SIM_CPU *cpu, int rn, unsigned char *memory, int length)
{
  if (rn < NUM_SMH_REGS && rn >= 0)
    {
      if (length == 4)
	{
	  long ival = cpu->regset.regs[rn];

	  /* misalignment-safe */
	  smh_store_unsigned_integer (memory, 4, ival);
	}

      return 4;
    }
  else
    return 0;
}

static sim_cia
smh_pc_get (sim_cpu *cpu)
{
  return cpu->regset.pc;
}

static void
smh_pc_set (sim_cpu *cpu, sim_cia pc)
{
  cpu->regset.pc = pc;
}

SIM_DESC
sim_open (SIM_OPEN_KIND kind, host_callback * cb, struct bfd * abfd,
	  char * const *argv)
{
  int i;
  SIM_DESC sd = sim_state_alloc (kind, cb);
  if (sim_cpu_alloc_all (sd, 1, /*cgen_cpu_max_extra_bytes ()*/0) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  if (sim_pre_argv_init (sd, argv[0]) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  /* The parser will print an error message for us, so we silently return.  */
  if (sim_parse_args (sd, argv) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  sim_do_command(sd," memory region 0x00000000,0x4000000") ; 
  sim_do_command(sd," memory region 0xE0000000,0x10000") ; 

  /* Check for/establish the a reference program image.  */
  if (sim_analyze_program (sd,
			   (STATE_PROG_ARGV (sd) != NULL
			    ? *STATE_PROG_ARGV (sd)
			    : NULL), abfd) != SIM_RC_OK)
    {
      free_state (sd);
      return 0;
    }

  /* Configure/verify the target byte order and other runtime
     configuration options.  */
  if (sim_config (sd) != SIM_RC_OK)
    {
      sim_module_uninstall (sd);
      return 0;
    }

  if (sim_post_argv_init (sd) != SIM_RC_OK)
    {
      /* Uninstall the modules to avoid memory leaks,
	 file descriptor leaks, etc.  */
      sim_module_uninstall (sd);
      return 0;
    }

  /* CPU specific initialization.  */
  for (i = 0; i < MAX_NR_PROCESSORS; ++i)
    {
      SIM_CPU *cpu = STATE_CPU (sd, i);

      CPU_REG_FETCH (cpu) = smh_reg_fetch;
      CPU_REG_STORE (cpu) = smh_reg_store;
      CPU_PC_FETCH (cpu) = smh_pc_get;
      CPU_PC_STORE (cpu) = smh_pc_set;

      set_initial_gprs (cpu);	/* Reset the GPR registers.  */
    }

  return sd;

}

SIM_RC
sim_create_inferior (SIM_DESC sd, struct bfd * prog_bfd, char * const *argv,
		     char * const *env)
{
  SIM_CPU *cpu = STATE_CPU (sd, 0);

  if (prog_bfd != NULL)
    cpu->regset.pc = bfd_get_start_address (prog_bfd);

  return SIM_RC_OK;
}



