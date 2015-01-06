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
	if ((0x3 & (*instr >> 26))){ //verifie à 0 les bit [27:26]
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
