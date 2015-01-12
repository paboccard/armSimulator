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
