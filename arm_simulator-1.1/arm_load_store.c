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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include "arm_instruction.h"
#include "arm_data_processing.h"

uint32_t addr_offset(arm_core p, uint32_t ins){
	uint8_t test,p_,u,w,rn,immedH,immedL;
	uint32_t imm_offset,val_rn,address;
	
	test=get_bits(ins,27,26);	
	p_=get_bit(ins,24);
	u=get_bit(ins,23);
	w=get_bit(ins,21);
	rn=get_bits(ins,19,16);
	imm_offset=get_bits(ins,11,0);	
	val_rn=arm_read_register(p, rn);
	immedH=get_bits(ins,11,8);	
	immedL=get_bits(ins,3,0);
		
	if(test==0){
		imm_offset=(immedH<<4)|immedL;
	}
	
	
	if(p_==1 && w==0){ // [<Rn>, #+/-<offset_12>]
		if(u==1){
			address=val_rn+imm_offset;
		}
		else{
			address=val_rn-imm_offset;
		}
		return address;
	}
	if(p_==1 && w==1){ // [<Rn>, #+/-<offset_12>]!
		if(u==1){
			address=val_rn+imm_offset;
		}
		else{
			address=val_rn-imm_offset;
		}
		if(test_cond(get_bits(ins,31,28),p)){
			arm_write_register(p,rn,address);
		}
		return address;
	}
	if(p_==0 && w==0){ // [<Rn>], #+/-<offset_12>
		address=val_rn;
		if(test_cond(get_bits(ins,31,28),p)){
			if(u==1){
				arm_write_register(p,rn,val_rn+imm_offset);
			}
			else{
				arm_write_register(p,rn,val_rn-imm_offset);
			}
		}
		return address;	
	}
	
	return UNDEFINED_INSTRUCTION;
}

uint32_t addr_register_shift(arm_core p, uint32_t ins){
	uint8_t rest,p_,u,w,rn,shift,rm,shift_imm,shift_val_imm;
	uint32_t val_rn,val_rm,address,index,cpsr;
	

	p_=get_bit(ins,24);
	u=get_bit(ins,23);
	w=get_bit(ins,21);

	rn=get_bits(ins,19,16);
	
	rest=get_bits(ins,11,4);
	shift=get_bits(ins,6,5);
	shift_imm=get_bits(ins,11,7);
	rm=get_bits(ins,3,0);
	
	val_rn=arm_read_register(p, rn);
	val_rm=arm_read_register(p, rm);
	shift_val_imm=get_bit(ins,4);
	cpsr = arm_read_cpsr(p);
	
	if(p_==1 && w==0){ // p463
		if(rest==0 || shift_val_imm==1){ // [<Rn>, +/-<Rm>]
			if(u==1){
				address=val_rn+val_rm;
			}
			else{
				address=val_rn-val_rm;
			}
			return address;
		}
		else{ // [<Rn>, +/-<Rm>, <shift> #<shift_imm>]
			switch (shift){
				case LSL:
					index=shift_lsl(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					break;
				case LSR:
					if(shift_imm==0){
						index=0;
					}
					else{
						index=shift_lsr(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				case ASR:
					if(shift_imm==0){
						if(get_bit(val_rm,31)==1)
							index = 0xFFFFFFFF;
						else
							index=0;
					}
					else{
						index=shift_asr(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				case ROR:
					if(shift_imm==0){ // RRX
						index=shift_rrx(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					else{ // ROR
						index=shift_ror(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				default:
					break;
			}
			if(u==1){
				address=val_rn+index;
			}
			else{
				address=val_rn-index;
			}
			return address;
		}
	}
	if(p_==1 && w==1){ 
		if(rest==0 || shift_val_imm==1){ // [<Rn>, +/-<Rm>]!
			if(u==1){
				address=val_rn+val_rm;
			}
			else{
				address=val_rn-val_rm;
			}
			if(test_cond(get_bits(ins,31,28),p)){
				arm_write_register(p,rn,address);
			}
			return address;
		}
		else{ // [<Rn>, +/-<Rm>, <shift> #<shift_imm>]!
			switch (shift){
				case LSL:
					index=shift_lsl(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					break;
				case LSR:
					if(shift_imm==0){
						index=0;
					}
					else{
						index=shift_lsr(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				case ASR:
					if(shift_imm==0){
						if(get_bit(val_rm,31)==1)
							index = 0xFFFFFFFF;
						else
							index=0;
					}
					else{
						index=shift_asr(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				case ROR:
					if(shift_imm==0){ // RRX
						index=shift_rrx(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					else{ // ROR
						index=shift_ror(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				default:
					break;
			}
			if(u==1){
				address=val_rn+index;
			}
			else{
				address=val_rn-index;
			}
			if(test_cond(get_bits(ins,31,28),p)){
				arm_write_register(p,rn,address);
			}
			return address;
		}
	}
	if(p_==0 && w==0){ 
		address=val_rn;
		if(rest==0 || shift_val_imm==1){ // [<Rn>], +/-<Rm>
			if(test_cond(get_bits(ins,31,28),p)){
				if(u==1){
					arm_write_register(p,rn,val_rn+val_rm);
				}
				else{
					arm_write_register(p,rn,val_rn-val_rm);
				}
			}
			return address;
		}
		else{ // [<Rn>], +/-<Rm>, <shift> #<shift_imm>
			switch (shift){
				case LSL:
					index=shift_lsl(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					break;
				case LSR:
					if(shift_imm==0){
						index=0;
					}
					else{
						index=shift_lsr(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				case ASR:
					if(shift_imm==0){
						if(get_bit(val_rm,31)==1)
							index = 0xFFFFFFFF;
						else
							index=0;
					}
					else{
						index=shift_asr(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				case ROR:
					if(shift_imm==0){ // RRX
						index=shift_rrx(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					else{ // ROR
						index=shift_ror(0,val_rm,shift_imm,shift_val_imm, &cpsr);
					}
					break;
				default:
					break;
			}
			if(u==1){
				arm_write_register(p,rn,val_rn+index);
			}
			else{
				arm_write_register(p,rn,val_rn-index);
			}
			return address;
		}
	}
	 return UNDEFINED_INSTRUCTION;
}

int arm_load_store(arm_core p, uint32_t ins) {
	uint8_t test,i,b;
	uint32_t address;
	test=get_bits(ins,27,26); // 01 pour str/ldr ( word & byte ) -- 00 pour half
	i=get_bit(ins,25);	
	b=get_bit(ins,22);
	
	if(test==1){//word & byte
		if(i==0){
			address=addr_offset(p,ins);
		}
		else{
			address=addr_register_shift(p,ins);
		}
		return address;
		
	}
	if(test==0){// half
		if(b==0){
			address=addr_offset(p,ins);
		}
		else{
			address=addr_register_shift(p,ins);
		}
		return address;
	}
	
	
	
    return UNDEFINED_INSTRUCTION;
}

int nb_registre_liste(ins){
  uint8_t i=0;
  int nb_registre = 0;
  while (i<16) {
    if ((get_bit(ins,i)==0))
      nb_registre++;
    i++;
  }
  return nb_registre;
}

int arm_load_store_multiple(arm_core p, uint32_t ins, int *end_address) {
  int address; //start_address
  uint8_t rn = get_bits(ins,19,16);
  int bit_W = get_bit(ins,21); //bit 21 de l'instruction
  uint32_t value;
  
  if (get_bit(ins,23)){//on est en incrementation
    if (get_bit(ins,24)==0){
      address = arm_read_register(p,rn);
      *end_address = address + (nb_registre_liste(ins)*4) - 4;
      if (bit_W){
	value = address + nb_registre_liste(ins)*4;
	arm_write_register(p,rn,value);
      }
    }
    else{
      address = arm_read_register(p,rn)+4;
      *end_address = address + (nb_registre_liste(ins)*4);
      if (bit_W){
	value = address + nb_registre_liste(ins)*4;
	arm_write_register(p,rn,value);
      }
    }
  }
  else{ //on est en decrementation
    if (get_bit(ins,24)==0){
      address = arm_read_register(p,rn) - nb_registre_liste(ins)*4 + 4 ;
      *end_address = arm_read_register(p,rn);
      if (bit_W){
	value = address -4;
	arm_write_register(p,rn,value);
      }
    }
    else{
      address = arm_read_register(p,rn) - nb_registre_liste(ins)*4;
      *end_address = arm_read_register(p,rn) - 4;
      if (bit_W){
	value = address;
	arm_write_register(p,rn,value);
      }
    }
  }
  return address;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
