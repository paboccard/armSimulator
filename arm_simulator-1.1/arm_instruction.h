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
#ifndef __ARM_INSTRUCTION_H__
#define __ARM_INSTRUCTION_H__
#include "arm_core.h"

int arm_step(arm_core p);

int arm_op_and(arm_core p, uint32_t instr);

int arm_op_eor(arm_core p, uint32_t instr);

int arm_op_sub(arm_core p, uint32_t instr);

int arm_op_rsb(arm_core p, uint32_t instr);

int arm_op_add(arm_core p, uint32_t instr);

int arm_op_adc(arm_core p, uint32_t instr);

int arm_op_sbc(arm_core p, uint32_t instr);

int arm_op_rsc(arm_core p, uint32_t instr);

int arm_op_tst(arm_core p, uint32_t instr);

int arm_op_teq(arm_core p, uint32_t instr);

int arm_op_cmp(arm_core p, uint32_t instr);

int arm_op_cmn(arm_core p, uint32_t instr);

int arm_op_orr(arm_core p, uint32_t instr);

int arm_op_mov(arm_core p, uint32_t instr);

int arm_op_bic(arm_core p, uint32_t instr);

int arm_op_mvn(arm_core p, uint32_t instr);

int test_cond(uint8_t cond, arm_core p);

#endif
