// simplevm.h
//

#define MAX_IMAGE     ( 128 * 1024 )

#define HALT_OP     0
#define SET_OP      1
#define PUSH_OP     2
#define POP_OP      3
#define EQ_OP       4
#define GT_OP       5
#define JUMP_OP     6
#define JNZ_OP      7
#define JZ_OP       8
#define ADD_OP      9
#define MULT_OP    10
#define MOD_OP     11
#define AND_OP     12
#define OR_OP      13
#define NOT_OP     14
#define RMEM_OP    15
#define WMEM_OP    16
#define CALL_OP    17
#define RET_OP     18
#define OUT_OP     19
#define IN_OP      20
#define NO_OP      21
#define MAX_INSTR  22

#define MAX_STACK    4096

typedef struct stack
{
   unsigned short system_stack[ MAX_STACK ];
   unsigned short head;
} stack;

typedef struct vm
{
   stack opstack;
   unsigned short image[ MAX_IMAGE ];
   unsigned int   imagePC;
   unsigned int   imageSize;
} vm;

typedef void ( *disasm_func ) ( void );
typedef void ( *execute_func ) ( void );

typedef struct instruction
{
   disasm_func disassemble;
   execute_func execute;
} instruction;

