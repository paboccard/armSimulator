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
