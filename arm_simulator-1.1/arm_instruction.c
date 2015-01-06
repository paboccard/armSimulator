/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

static int arm_execute_instruction(arm_core p) {
  uint32_t *instr = NULL;
  int8_t opcode;
  int32_t rs = arm_read_register(p, 15);
    if (get_bit(rs,0)==0 && get_bit(rs,1)==0){
      //if ((memory_read_word(p->mem,1,p->register_storage[15]-8,instr))==0){
      if(arm_fetch(p,instr)==0){
	if ((0x3 & (*instr >> 26))){ //verifie � 0 les bit [27:26]
	    opcode = 0xF & (*instr >> 21);
	    switch(opcode){
	    case AND:
	      break;
	    case EOR:
	      break;
	    case SUB:
	      break;
	    case RSB:
	      break;
	    case ADD:
	      break;
	    case ADC:
	      break;
	    case SBC:
	      break;
	    case RSC:
	      break;
	    case TST:
	      break;
	    case TEQ:
	      break;
	    case CMP:
	      break;
	    case CMN:
	      break;
	    case ORR:
 	      break;
	    case MOV:
	      break;
	    case BIC:
	      break;
	    case MVN:
	      break;
	    default:
	      printf("Erreur d'instruction");
	      break;
	    }
	  }
      }
  }
  return PREFETCH_ABORT; //exception 
}

int arm_step(arm_core p) {
  int result;
  
  result = arm_execute_instruction(p);
  if (result)
    arm_exception(p, result);
      return result;
}
