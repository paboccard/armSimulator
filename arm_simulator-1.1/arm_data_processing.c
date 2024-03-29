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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"


// TO DO : faire les overflows
int32_t cpsr;

int shift_lsl(arm_core p, int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int8_t bit_S){ 

    if (shift_val_imm == 0){ //si on a une valeur immediate pour le shift
	if (bit_S)
	    cpsr = get_bit(val_rm,32-shift_imm) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
	return (int32_t)((uint32_t)val_rm << shift_imm);//val_rm << shift_imm;
    }
    else{ // si on a un registre pour le shift
	if (bit_S)
	    cpsr = (val_rs >= 32 ? ( val_rs == 32 ? get_bit(val_rm,0) : 0 ) : get_bit(val_rm,32-val_rs)) ? set_bit(cpsr, C) : clr_bit(cpsr, C);
	return val_rs < 32 ? (int32_t)((uint32_t)val_rm << val_rs) : 0;//val_rm << val_rs : 0;
    }  
}

int shift_lsr(arm_core p, int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int8_t bit_S){

    if (shift_val_imm == 0){ //si on a une valeur immediate pour le shift
	if (bit_S)
	    cpsr = get_bit(val_rm,shift_imm-1) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
	return (int32_t)((uint32_t)val_rm >> shift_imm); //val_rm >> shift_imm;
    }
    else{ // si on a un registre pour le shift
	if (bit_S)
	    cpsr = (val_rs >= 32 ? ( val_rs == 32 ? get_bit(val_rm,0) : 0 ) : get_bit(val_rm,val_rs-1)) ? set_bit(cpsr, C) : clr_bit(cpsr, C);
	return val_rs < 32 ? (int32_t)((uint32_t)val_rm >> val_rs) : 0;//val_rm >> val_rs : 0;
    }
}

int shift_asr(arm_core p, int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int8_t bit_S){

    if (shift_val_imm) {
    	if (!val_rs) {
	    if (bit_S)
		cpsr = get_bit(val_rm, 31) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
	    return get_bit(val_rm, 31) ? 0xFFFFFFFF : 0x00000000;
	}
	else {
	    if (bit_S)
		cpsr = get_bit(val_rm,val_rs-1) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
	    return asr(val_rm, val_rs);
	}
    }
    else {
	if (!shift_imm) {
	    if (bit_S)
		cpsr = get_bit(val_rm, 31) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
	    return get_bit(val_rm, 31) ? 0xFFFFFFFF : 0x00000000;
	}
	else {
	    if (bit_S)
		cpsr = get_bit(val_rm,shift_imm-1) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
	    return asr(val_rm, shift_imm);
	}
    }
	

}
  
int shift_ror(arm_core p, int8_t val_rs,int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int8_t bit_S){

    if (!shift_val_imm) {
        // p456
        if (!val_rm) {
            return val_rm;
        }
        else{
            if (!shift_imm) {
		if (bit_S){
		    if (get_bit(val_rm,31)) 
			cpsr = set_bit(cpsr,C);
		    else 
			cpsr = clr_bit(cpsr,C);
		}
                return val_rm;
            }
            else {
		if (bit_S){
		    if (get_bit(val_rm,(shift_imm - 1))) { // on cherche le bit qui sera le bit de poid fort apres le decalage
			cpsr = set_bit(cpsr,C);
                }
		    else 
			cpsr = clr_bit(cpsr,C);
		}
                return ror(val_rm, shift_imm);
            }
        }
    }
    else {	// c'est un registre
        if (!val_rm) {
	    if (bit_S)
		cpsr = clr_bit(cpsr,C);
            return val_rm;
        }
        else {
            
            if (!val_rs) {
		if (bit_S){
		    if (get_bit(val_rm,31)) {
			cpsr = set_bit(cpsr,C);
		    }
		    else {
			cpsr = clr_bit(cpsr,C);
		    }
		}
                return val_rm;
              
            }
            else {
		if (bit_S){
		    if (get_bit(val_rm,val_rs - 1)) {
			cpsr = set_bit(cpsr,C);
		    }
		    else
			cpsr = clr_bit(cpsr,C);
		}
                return ror(val_rm, val_rs);
            }
        }
    } 
}

int shift_rrx(arm_core p, int8_t val_rs, int32_t val_rm, int8_t shift_imm, int8_t shift_val_imm, int8_t bit_S){
    int carry = get_bit(cpsr,C);
        
    if (bit_S)
	cpsr = get_bit(val_rm, 0) ? set_bit(cpsr,C) : clr_bit(cpsr,C);
  
    return ((int32_t)((uint32_t)carry << 31)|(int32_t)((uint32_t)val_rm >> 1));

}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
    int8_t rs, rm, shift, shift_imm, shift_val_imm, bit_S;
    int shifter_operand = UNDEFINED_INSTRUCTION;
    int32_t val_rs, val_rm;    
    int rotation;

    cpsr = arm_read_cpsr(p);
    bit_S = get_bit(ins,20);
  
    if (get_bit(ins,25)==1){
	shifter_operand = get_bits(ins,7,0);
	rotation = get_bits(ins,11,8)*2;
	shifter_operand = ror(shifter_operand,rotation); // rotation droite
	if (bit_S){
	    if (get_bits(ins,11,8)){
		if (get_bit(shifter_operand,31)==1){
		    cpsr = set_bit(cpsr,C);
		}
		else {
		    cpsr = clr_bit(cpsr,C);
		}
	    }
	    arm_write_cpsr(p,cpsr);
	}
    }
    else{
	rs = get_bits(ins,11,8);       // numero de registre decrivant le shift
	rm = get_bits(ins,3,0);			// numero du registre source
	shift_imm = get_bits(ins,11,7); //valeur immediate pour le shift
	shift = get_bits(ins,6,5); //valeur du shift (lsr / lsl / aor...)
	shift_val_imm = get_bit(ins,4); //booleen faux si il y a une valeur immediate pour shift
	val_rs = arm_read_register(p,rs);	// valeur du shift si non val_imm
	val_rm = arm_read_register(p,rm);	// val du registre source (valeur a shifter)

	if (get_bits(ins,11,4)==0){ //shift_operand = Registre -> sans shift
	    shifter_operand = val_rm;
	}
	else if(shift == LSL){
	    shifter_operand = shift_lsl(p, val_rs, val_rm, shift_imm, shift_val_imm, bit_S);
	}
	else if(shift == LSR){
	    shifter_operand = shift_lsr(p, val_rs,val_rm, shift_imm, shift_val_imm, bit_S);
	}
	else if(shift == ASR){
	    shifter_operand = shift_asr(p, val_rs,val_rm, shift_imm, shift_val_imm, bit_S);
	}	
	else if((shift == RRX) && (shift_imm == 0)){
	    shifter_operand = shift_rrx(p, val_rs, val_rm, shift_imm, shift_val_imm, bit_S);
	}
	else if(shift == ROR){
	    shifter_operand = shift_ror(p, val_rs, val_rm, shift_imm, shift_val_imm, bit_S);
	}
	else
	    shifter_operand = UNDEFINED_INSTRUCTION;
    }
    arm_write_cpsr(p,cpsr);
    return shifter_operand;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}
