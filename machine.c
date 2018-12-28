#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "simplevm.h"

vm VM;

// Stack operations

void push( unsigned short value )
{
   VM.opstack.system_stack[ VM.opstack.head++ ] = value;
}

unsigned short pop( void )
{
   return VM.opstack.system_stack[ --VM.opstack.head ];
}

// Helpers

unsigned short deref( unsigned short value )
{
   if ( value < 32768 )
   {
      return value;
   }

   return VM.image[ value ];
}

// Instructions

void execute_halt( void )
{
   printf( "Execution halted.\n" );
   exit( 0 );

   return;
}

void disasm_halt( void )
{
   printf( "[%04x]  HALT\n", VM.imagePC );

   VM.imagePC += 1;

   return;
}

void execute_set( void )
{
   int reg;
   int value;

   reg = VM.image[ VM.imagePC+1 ];
   value = deref( VM.image[ VM.imagePC+2 ] );

   VM.image[ reg ] = value;

   VM.imagePC += 3;

   return;
}

void disasm_set( void )
{
   printf( "[%04x]  SET  %4x %4x\n", 
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ] );

   VM.imagePC += 3;

   return;
}

void execute_push( void )
{
   int value;

   value = deref( VM.image[ VM.imagePC+1 ] );

   push( value );

   VM.imagePC += 2;

   return;
}

void disasm_push( void )
{
   printf( "[%04x]  PUSH %4x\n",
            VM.imagePC, VM.image[ VM.image[ VM.imagePC+1 ] ] );

   VM.imagePC += 2;

   return;
}

void execute_pop( void )
{
   int reg;

   reg = VM.image[ VM.imagePC+1 ];

   VM.image[ reg ] = pop( );

   VM.imagePC += 2;

   return;
}

void disasm_pop( void )
{
   printf( "[%04x]  POP  %04x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ] );

   VM.imagePC += 2;

   return;
}

void disasm_eq( void )
{
   printf( "[%04x]  EQ   %04x %04x %04x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );
   VM.imagePC += 4;

   return;
}

void execute_eq( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 == reg3 );

   VM.imagePC += 4;

   return;
}

void disasm_gt( void )
{
   printf( "[%04x]  GT   %04x %04x %04x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );
   VM.imagePC += 4;

   return;
}

void execute_gt( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 > reg3 );

   VM.imagePC += 4;

   return;
}

void execute_jump( void )
{
   unsigned short value;

   value = deref( VM.image[ VM.imagePC+1 ] );

   VM.imagePC = value;

   return;
}

void disasm_jump( void )
{
   printf( "[%04x]  JUMP %4x\n", VM.imagePC, VM.image[ VM.imagePC+1 ] );

   VM.imagePC += 2;
   return;
}

void execute_jnz( void )
{
   unsigned short reg, value;

   reg = deref( VM.image[ VM.imagePC+1 ] );
   value = deref( VM.image[ VM.imagePC+2 ] );

   if ( reg != 0 )
   {
      VM.imagePC = value;
   }
   else
   {
      VM.imagePC += 3;
   }

   return;
}

void disasm_jnz( void )
{
   printf( "[%04x]  JNZ  %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ] );

   VM.imagePC += 3;

   return;
}

void execute_jz( void )
{
   unsigned short reg, value;

   reg = deref( VM.image[ VM.imagePC+1 ] );
   value = deref( VM.image[ VM.imagePC+2 ] );

   if ( reg == 0 )
   {
      VM.imagePC = value;
   }
   else
   {
      VM.imagePC += 3;
   }

   return;
}

void disasm_jz( void )
{
   printf( "[%04x]  JZ   %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ] );

   VM.imagePC += 3;

   return;
}

void execute_add( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 + reg3 ) & 0x7fff;

   VM.imagePC += 4;

   return;
}

void disasm_add( void )
{
   printf( "[%04x]  ADD  %4x %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );

   VM.imagePC += 4;

   return;
}

void execute_mult( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 * reg3 ) & 0x7fff;

   VM.imagePC += 4;

   return;
}

void disasm_mult( void )
{
   printf( "[%04x]  MULT %4x %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );

   VM.imagePC += 4;

   return;
}

void execute_mod( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 % reg3 );

   VM.imagePC += 4;

   return;
}

void disasm_mod( void )
{
   printf( "[%04x]  MOD  %4x %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );

   VM.imagePC += 4;

   return;
}

void execute_and( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 & reg3 );

   VM.imagePC += 4;

   return;
}

void disasm_and( void )
{
   printf( "[%04x]  AND  %4x %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );

   VM.imagePC += 4;

   return;
}

void execute_or( void )
{
   unsigned int reg1, reg2, reg3;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );
   reg3 = deref( VM.image[ VM.imagePC+3 ] );

   VM.image[ reg1 ] = ( reg2 | reg3 );

   VM.imagePC += 4;

   return;
}

void disasm_or( void )
{
   printf( "[%04x]  OR   %4x %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ],
            VM.image[ VM.imagePC+3 ] );

   VM.imagePC += 4;

   return;
}

void execute_not( void )
{
   unsigned int reg1, reg2;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );

   VM.image[ reg1 ] = ( ~reg2 & 0x7fff );

   VM.imagePC += 3;

   return;
}

void disasm_not( void )
{
   printf( "[%04x]  NOT  %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ] );

   VM.imagePC += 3;

   return;
}

void execute_rmem( void )
{
   unsigned short reg1, reg2;

   reg1 = VM.image[ VM.imagePC+1 ];
   reg2 = deref( VM.image[ VM.imagePC+2 ] );

   VM.image[ reg1 ] = VM.image[ reg2 ];

   VM.imagePC += 3;

   return;
}

void disasm_rmem( void )
{
   printf( "[%04x]  RMEM %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ] );

   VM.imagePC += 3;

   return;
}

void execute_wmem( void )
{
   unsigned short addr, value;

   addr  = deref( VM.image[ VM.imagePC+1 ] );
   value = deref( VM.image[ VM.imagePC+2 ] );

   VM.image[ addr ] = value;

   VM.imagePC += 3;

   return;
}

void disasm_wmem( void )
{
   printf( "[%04x]  WMEM %4x %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ], VM.image[ VM.imagePC+2 ] );

   VM.imagePC += 3;

   return;
}

void execute_call( void )
{
   unsigned short reg1;

   reg1 = deref( VM.image[ VM.imagePC+1 ] );

   push( (VM.imagePC+2 ) );

   VM.imagePC = reg1;

   return;
}

void disasm_call( void )
{
   printf( "[%04x]  CALL %4x\n",
            VM.imagePC, VM.image[ VM.imagePC+1 ] );

   VM.imagePC += 2;

   return;
}

void execute_ret( void )
{
   VM.imagePC = pop( );

   return;
}

void disasm_ret( void )
{
   printf( "[%04x]  RET\n", VM.imagePC );

   VM.imagePC += 1;

   return;
}

void execute_out( void )
{
   printf( "%c", deref( VM.image[ VM.imagePC+1 ] ) );

   VM.imagePC += 2;

   return;
}

void disasm_out( void )
{
   printf( "[%04x]  OUT  %4x  ", VM.imagePC, VM.image[ VM.imagePC+1 ] );

   if ( isalnum( VM.image[ VM.imagePC+1 ] ) )
   {
      printf( "(%c)\n", VM.image[ VM.imagePC+1 ] );
   }
   else
   {
      printf( "(.)\n" );
   }

   VM.imagePC += 2;

   return;
}

void execute_in( void )
{
   unsigned reg1;

   reg1 = VM.image[ VM.imagePC+1 ] ;

   VM.image[ reg1 ] = getchar( );

   VM.imagePC += 2;

   return;
}

void disasm_in( void )
{
   printf( "[%04x]   IN   %4x\n", VM.imagePC, VM.image[ VM.imagePC+1 ] );

   VM.imagePC += 2;

   return;

}

void disasm_nop( void )
{
   printf( "[%04x]  NOP\n", VM.imagePC );

   VM.imagePC += 1;

   return;
}

void execute_nop( void )
{
   VM.imagePC += 1;

   return;
}

void disasm_data( void )
{
   printf( "[%04x]  DATA  %4x\n", VM.imagePC, VM.image[ VM.imagePC ] );

   VM.imagePC += 1;

   return;

}

void execute_data( void )
{
   printf( "Illegal Opcode detected (%04x)\n", VM.image[ VM.imagePC ] );
   exit( -1 );
}


instruction instructions[ MAX_INSTR ] = {
   [ HALT_OP ] = { disasm_halt, execute_halt },
   [ SET_OP  ] = { disasm_set, execute_set },
   [ PUSH_OP ] = { disasm_push, execute_push },
   [ POP_OP  ] = { disasm_pop, execute_pop },
   [ EQ_OP   ] = { disasm_eq, execute_eq },
   [ GT_OP   ] = { disasm_gt, execute_gt },
   [ JUMP_OP ] = { disasm_jump, execute_jump },
   [ JNZ_OP  ] = { disasm_jnz, execute_jnz },
   [ JZ_OP   ] = { disasm_jz, execute_jz },
   [ ADD_OP  ] = { disasm_add, execute_add },
   [ MULT_OP ] = { disasm_mult, execute_mult },
   [ MOD_OP  ] = { disasm_mod, execute_mod },
   [ AND_OP  ] = { disasm_and, execute_and },
   [ OR_OP   ] = { disasm_or, execute_or },
   [ NOT_OP  ] = { disasm_not, execute_not },
   [ RMEM_OP ] = { disasm_rmem, execute_rmem },
   [ WMEM_OP ] = { disasm_wmem, execute_wmem },
   [ CALL_OP ] = { disasm_call, execute_call },
   [ RET_OP  ] = { disasm_ret, execute_ret },
   [ OUT_OP  ] = { disasm_out, execute_out },
   [ IN_OP   ] = { disasm_in, execute_in },
   [ NO_OP   ] = { disasm_nop, execute_nop },

};

void loadVM( char* filename )
{
   FILE *fin;
   VM.imagePC = 0;
   unsigned int index = 0;

   printf( "loadVM: Loading from %s\n", filename );

   fin = fopen( filename, "rb" );
   if ( fin == NULL )
   {
      fprintf( stderr, "Could not open %s\n", filename );
      exit( -1 );
   }

   while ( !feof( fin ) )
   {
      ( void ) fread( ( void * ) &VM.image[ index++ ], 2, 1, fin );
   }

   fclose( fin );

   VM.imageSize = index;

   printf( "loadVM: Loaded %d words\n\n", index );

   return;
}

int initVM( int argc, char *argv[] )
{
   int opt;
   char *file;
   int disasm = 0;

   while ( ( opt = getopt( argc, argv, "df:" ) ) != -1 )
   {
      switch( opt )
      {
         case 'd':
            disasm = 1;
            break;
         case 'f':
            loadVM( optarg );
            break;
         default:
            fprintf( stderr, "Usage: %s [-d] -f <filename>\n", argv[ 0 ] );
            exit( -1 );
            break;
      }
   }

   VM.opstack.head = 0;

   return disasm;
}

void executeVM( int disasm )
{
   unsigned short instruction;

   while ( VM.imagePC < VM.imageSize )
   {
      instruction = VM.image[ VM.imagePC ];

      if ( instruction >= MAX_INSTR )
      {
         disasm_data( );
      }
      else
      {
         if ( disasm )
         {
            ( instructions[ instruction ].disassemble )( );
         }
         else
         {
            ( instructions[ instruction ].execute )( );
         }
      }

   }

   return;
}


int main( int argc, char *argv[] )
{
   int disasm = 0;

   disasm = initVM( argc, argv );

   executeVM( disasm );

   return 0;
}


