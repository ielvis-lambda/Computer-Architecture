#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char **argv)
{
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  if (argc != 2)
  {
    printf("Correct usage: ./files file_name.extension\n");
    return;
  }
  FILE *fp;
  char line[256];
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf("Error opening file.\n");
    return;
  }
  int address = 0;
  while (fgets(line, 256, fp) != NULL)
  {
    char *endptr;
    unsigned char val = strtoul(line, &endptr, 2);
    if (line == endptr)
    {
      //         printf("skipping: %s", line);
      continue;
    }
    cpu->ram[address++] = val;
  }
  fclose(fp);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

// MAR = Memory Address Register (the address)
// MDR = Memory Data Register (the data)
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

unsigned char cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  return cpu->ram[MAR] = MDR;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).

    // IR = Instruction Register (current instruction)
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction

    unsigned char operand0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operand1 = cpu_ram_read(cpu, cpu->PC + 2);

    // 4. switch() over it to decide on a course of action.

    switch (IR)
    {
    case LDI:
      // per spec LDI sets specified register to specified value
      cpu->registers[operand0] = operand1;
      cpu->PC += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operand0]);
      cpu->PC += 2;
      break;
    case HLT:
      running = 0;
      break;
    // Adds default case in case it gets a case it doesn't recognize
    default:
      // Print at a field width of 2 (2), add leading zeroes if necessary (0), use capital letters for hex values (X)
      printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->PC);
      exit(1);
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // Init Registers per spec
  for (int i = 0; i < 6; i++)
  {
    cpu->registers[i] = 0;
  }
  cpu->registers[7] = 0xF4;

  // Init PC
  cpu->PC = 0;

  // Init RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
