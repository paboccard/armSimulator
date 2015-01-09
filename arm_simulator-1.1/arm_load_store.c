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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

int arm_load_store(arm_core p, uint32_t ins) {// A finir
	/*uint8_t test,i,p,u,b,w,l,rn,rd,shift,rm;
	uint32_t imm_offset;
	
	test=get_bits(ins,27,26); // 01 pour str/ldr ( word & byte ) -- 00 pour half
	i=get_bit(ins,25);
	p=get_bit(ins,24);
	u=get_bit(ins,23);
	b==get_bit(ins,22);
	w=get_bit(ins,21);
	l=get_bit(ins,20);
	rn=get_bits(ins,19,16);
	rd=get_bits(ins,15,12);
	shift=get_bits(ins,11,4);
	rm=get_bits(ins,3,0);
	imm_offset=get_bits(ins,11,0);
	if(test){//word & byte
	}
	else{// half
	}
	
	
	*/
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
