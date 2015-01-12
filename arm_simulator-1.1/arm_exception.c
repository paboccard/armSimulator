/*
  Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
  Copyright (C) 2011 Guillaume Huard
  Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
  termes de la Licence Publique Générale GNU publiée par la Free Software
  Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

  Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
  GARANTIE, ni explicite ni implicite, y compris les garanties de
  commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
  Licence Publique Générale GNU pour plus de détails.

  Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
  temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
  États-Unis.

  Contact: Guillaume.Huard@imag.fr
  ENSIMAG - Laboratoire LIG
  51 avenue Jean Kuntzmann
  38330 Montbonnot Saint-Martin
*/
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

#ifdef BIG_ENDIAN_SIMULATOR
#define CP15_reg1_EEbit 1
#else
#define CP15_reg1_EEbit 0
#endif

#define Exception_bit_9 (CP15_reg1_EEbit << 9)

void global(arm_core p,int exception, int mode){
    uint32_t old_cpsr; 
    old_cpsr = arm_read_cpsr(p);

    if (mode == IRQ || mode == FIQ)
	arm_write_register(p,14,arm_read_register(p,15)+4);
    else if (mode == ABT)
	arm_write_register(p,14,arm_read_register(p,15)+8);
    else
	arm_write_register(p,14,arm_read_register(p,15));
    
    arm_write_spsr(p,old_cpsr);

    
    old_cpsr &= 0xFFFFFFE0 | mode;

    old_cpsr &= ~(1<<5);
    if (exception == RESET || exception == FAST_INTERRUPT)
	    old_cpsr |= 1<<6;
    
    old_cpsr |= 1<<7;

    if (exception == UNDEFINED_INSTRUCTION || exception == SOFTWARE_INTERRUPT)
	old_cpsr |= 1<<8;

    arm_write_cpsr(p, old_cpsr | Exception_bit_9);
    arm_write_usr_register(p, 15, 4);
}

void reset(arm_core p){
    global(p,RESET,SVC);
}

void undefined_instruction(arm_core p){
    /* VERSION EN CAS DE BUG
    uint32_t old_cpsr; 
    old_cpsr = arm_read_cpsr(p);

    arm_write_register(p,14,arm_read_register(p,15));
    arm_write_spsr(p,old_cpsr);

    old_cpsr &= 0xFFFFFFE0 | UND;
    old_cpsr &= ~(1<<5);
    old_cpsr |= 1<<7;
    arm_write_cpsr(p, old_cpsr | Exception_bit_9);
    arm_write_usr_register(p, 15, 4);
    */
    global(p,UNDEFINED_INSTRUCTION,UND);
}

void software_interrupt(arm_core p){
   global(p,SOFTWARE_INTERRUPT,SVC);
}

void prefetch_abort(arm_core p){
    global(p,PREFETCH_ABORT,ABT);
}

void data_abort(arm_core p){
    global(p,DATA_ABORT,ABT);
}

void interrupt(arm_core p){
    global(p,INTERRUPT,IRQ);
}

void fast_interrupt(arm_core p){
    global(p,FAST_INTERRUPT,FIQ);
}



void arm_exception(arm_core p, unsigned char exception) {
    /* We only support RESET initially */
    /* Semantics of reset interrupt (ARM manual A2-18) */
    switch(exception){
    case(RESET):
	reset(p);
	break;
    case(UNDEFINED_INSTRUCTION):
	undefined_instruction(p);
	break;
    case(SOFTWARE_INTERRUPT):
	software_interrupt(p);
	break;
    case(PREFETCH_ABORT):
	prefetch_abort(p);
	break;
    case(DATA_ABORT):
	data_abort(p);
	break;
    case(INTERRUPT):
	interrupt(p);
	break;
    case(FAST_INTERRUPT):
	fast_interrupt(p);
	break;
    }
}

