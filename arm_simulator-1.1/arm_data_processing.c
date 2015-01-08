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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

int shift_lsl(int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int *cpsr){ 
  
  if (shift_val_imm == 0){ //si on a une valeur immediate pour le shift
    *cpsr = get_bit(val_rm,32-shift_imm) ? set_bit(*cpsr,C) : clr_bit(*cpsr,C);
    return val_rm << shift_imm;
  }
  else{ // si on a un registre pour le shift
    *cpsr = val_rs >= 32 ? ( val_rs == 32 ? get_bit(val_rm,0) : 0 ) : get_bit(val_rm,32-val_rs);
    return val_rs < 32 ? val_rm << val_rs : 0;
  }  
}

int shift_lsr(int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int *cpsr){
  if (shift_val_imm == 0){ //si on a une valeur immediate pour le shift
    *cpsr = get_bit(val_rm,shift_imm-1) ? set_bit(*cpsr,C) : clr_bit(*cpsr,C);
    return val_rm >> shift_imm;
  }
  else{ // si on a un registre pour le shift
    *cpsr = val_rs >= 32 ? ( val_rs == 32 ? get_bit(val_rm,0) : 0 ) : get_bit(val_rm,val_rs-1);
    return val_rs < 32 ? val_rm >> val_rs : 0;
  }
}

int shift_asr(int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int *cpsr){
  if (!shift_val_imm) {
    if (!get_bits(val_rs,7,0)) {
      *cpsr = clr_bit(*cpsr,C);
      return val_rm;
    }
    else if (get_bits(val_rs,7,0) < 32) {
      *cpsr = get_bit(val_rm,(get_bits(val_rs,7,0)-1)) ? set_bit(*cpsr,C) : clr_bit(*cpsr,C);
      return asr(val_rm, val_rs);
    }
    else {
      if (!get_bit(val_rm,31)){
        *cpsr = clr_bit(*cpsr,C);
        return 0;
      }
      else {
        *cpsr = set_bit(*cpsr,C);
        return ~0;
      }
    }
  }
  else {
    if (!get_bits(shift_imm,7,0)) {
      *cpsr = clr_bit(*cpsr,C);
      return val_rm;
    }
    else if (get_bits(shift_imm,7,0) < 32) {
      *cpsr = get_bit(val_rm,(get_bits(shift_imm,7,0)-1)) ? set_bit(*cpsr,C) : clr_bit(*cpsr,C);
      return asr(val_rm, shift_imm);
    }
    else {
      if (!get_bit(val_rm,31)){
        *cpsr = clr_bit(*cpsr,C);
        return 0;
      }
      else {
        *cpsr = set_bit(*cpsr,C);
        return ~0;
      }
    }
  }

}
  
int shift_ror(int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int *cpsr){
 
  if (shift_val_imm) {
    // p456
    if (!(get_bits(val_rs,7,0))) {
      *cpsr = clr_bit(*cpsr,C);
      return val_rm;
    }
    else if (!(get_bits(val_rs,4,0))) {
      if (get_bit(val_rm,31)) {
        *cpsr = set_bit(*cpsr,C);
      }
      else 
        *cpsr = clr_bit(*cpsr,C);
      return val_rm;
      
    }
    else {
      if (get_bit(val_rm,(get_bits(val_rs,4,0) - 1))) {
        *cpsr = set_bit(*cpsr,C);
      }
      else 
        *cpsr = clr_bit(*cpsr,C);

      return ror(val_rm, val_rs);
    }
  }
  else {
    if (!(get_bits(shift_imm,7,0))) {
      *cpsr = clr_bit(*cpsr,C);
      return val_rm;
    }
    else if (!(get_bits(shift_imm,4,0))) {
      if (get_bit(val_rm,31)) {
        *cpsr = set_bit(*cpsr,C);
      }
      else {
        *cpsr = clr_bit(*cpsr,C);
      }
      return val_rm;
      
    }
    else {
      if (get_bit(val_rm,(get_bits(shift_imm,4,0) - 1))) {
        *cpsr = set_bit(*cpsr,C);
      }
      else 
        *cpsr = clr_bit(*cpsr,C);
      
      return ror(val_rm, shift_imm);
    }
  } 
}

int shift_rrx(int8_t val_rs, int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int *cpsr){
  
  *cpsr = get_bit(val_rm, 0) ? set_bit(*cpsr,C) : clr_bit(*cpsr,C);
  return ((get_bit(*cpsr,29)<<31) | (val_rm>>1));

}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
  int8_t rs, rm, shift, shift_imm, shift_val_imm;
  int shifter_operand = UNDEFINED_INSTRUCTION;
  int32_t val_rs, val_rm;
  int32_t cpsr;

  cpsr = arm_read_cpsr(p);
  
  if (get_bit(ins,25)==1){
    shifter_operand = get_bits(ins,7,0); // TODO: Traiter la rotation droite
    if (get_bits(ins,11,8)==0)
      cpsr = clr_bit(cpsr,C); // shifter_carry_out = C Flags
    else
      if (get_bit(shifter_operand,31)==1)
	cpsr = set_bit(cpsr,C);
      else
	cpsr = clr_bit(cpsr,C);
  }
  else{
    rs = get_bits(ins,11,8);
    rm = get_bits(ins,3,0);
    shift_imm = get_bits(ins,11,7); //valeur immediate pour le shift
    shift = get_bits(ins,6,5); //valeur du shift
    shift_val_imm = get_bit(ins,4); //booleen pour si il y a une valeur immediate pour shift
    val_rs = arm_read_register(p,rs);
    val_rm = arm_read_register(p,rm);

    if (get_bits(ins,11,4)==0){ //shift_operand = Registre -> sans shift
      shifter_operand = val_rm;
      cpsr = clr_bit(cpsr,C); // shifter_carry_out = C Flags
    }
    else if(shift == LSL){
      shifter_operand = shift_lsl(val_rs,val_rm, shift_imm, shift_val_imm, &cpsr);
    }
    else if(shift == LSR){
      shifter_operand = shift_lsr(val_rs,val_rm,shift_imm,shift_val_imm, &cpsr);
    }
    else if(shift == ASR){
      shifter_operand = shift_asr(val_rs,val_rm,shift_imm,shift_val_imm, &cpsr);
    }
    else if(shift == ROR){
      shifter_operand = shift_ror(val_rs,val_rm, shift_imm,shift_val_imm, &cpsr);
    }
    else if((shift == RRX) && (shift_val_imm == 0)){
      shifter_operand = shift_rrx(val_rs,val_rm, shift_imm,shift_val_imm, &cpsr);
    }
    else
      shifter_operand = UNDEFINED_INSTRUCTION;
  }
  return shifter_operand;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
  return UNDEFINED_INSTRUCTION;
}
