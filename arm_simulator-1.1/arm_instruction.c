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



int arm_op_and(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rd, rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  if (get_bit(instr,25)==0) //test valeur immediate
    y = arm_read_register(p,rs);
  else
    y = rs;

  arm_write_register(p,rd,x&y);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
	  
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    // mettre  C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_eor(arm_core p, uint32_t instr, int32_t *cpsr){ 

  int8_t rn, rd, rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);
  y= arm_data_processing_shift(p,instr);
 
  arm_write_register(p,rd,x^y);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    // mettre  C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_sub(arm_core p, uint32_t instr, int32_t *cpsr){//a finir
  int8_t rn, rd, rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  y= arm_data_processing_shift(p,instr);

  arm_write_register(p,rd,x-y);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
      
    if ((x-y)>=0)
      *cpsr = ~set_bit(*cpsr,C); 
    else 
      *cpsr = clr_bit(*cpsr,C); 
    // mettre  C Flag en fonction de shifter_carry_out
    
    
    if ((x>0 && y<0 && (x-y)>0) || (x<0 && y>0 && (x-y)<0) )
      *cpsr = set_bit(*cpsr,V); 
    else 
      *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_rsb(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rd, rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  y= arm_data_processing_shift(p,instr);

  arm_write_register(p,rd,y-x);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
      
    if ((y-x)>=0)
      *cpsr = ~set_bit(*cpsr,C); 
    else 
      *cpsr = clr_bit(*cpsr,C); 
    // mettre  C Flag en fonction de shifter_carry_out
    
    
    if ((y>0 && x<0 && (y-x)>0) || (y<0 && x>0 && (y-x)<0) )
      *cpsr = set_bit(*cpsr,V); 
    else 
      *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_add(arm_core p, uint32_t instr, int32_t *cpsr){
   int8_t rn, rd, rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  y= arm_data_processing_shift(p,instr);

  arm_write_register(p,rd,x+y);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    
    long long int a= x+y;
    int32_t b=~0;
    if(a>b)
		*cpsr = set_bit(*cpsr,C);
	else
		*cpsr = clr_bit(*cpsr,C);
	if((x>0 && y>0 && (x+y)<0) || (x<0 && y<0 && (x+y)>0) )
		*cpsr = set_bit(*cpsr,V);
	else
		*cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_adc(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rd, rs;
  int x, y, dest;

  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  if (get_bit(instr,25)==0) //test valeur immediate
    y = arm_read_register(p,rs);
  else
    y = rs;
    
  arm_write_register(p, rd,x + y + get_bits(*cpsr,30) );
  
  if(get_bit(instr,20) && rd ==15){
	if(arm_current_mode_has_spsr(p)){
	  *cpsr = amd_read_spsr(p);
	}
	else
	  return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
      
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    
    long long int test;  
    test = x + y + get_bits(*cpsr,30);
    
    int32_t b=~0;
    if(test > b)
      *cpsr = set_bit(*cpsr,C);
    else
      *cpsr = clr_bit(*cpsr,C);
    //Fonction OverflowFrom avec 3 param�tres
  }
  return 0;
}

int arm_op_sbc(arm_core p, uint32_t instr, int32_t *cpsr){
	
  return 0;
}

int arm_op_rsc(arm_core p, uint32_t instr, int32_t *cpsr){
  return 0;
}

int arm_op_tst(arm_core p, uint32_t instr, int32_t *cpsr){
  return 0;
}

int arm_op_teq(arm_core p, uint32_t instr, int32_t *cpsr){

int8_t rn;
  int32_t n;
  int64_t alu_out;

  rn = get_bits(instr,19,16);
  n = arm_read_register(p,rn);
  alu_out = n^arm_data_processing_shift(p,instr);

  if (get_bit(alu_out,31)==1)
    *cpsr = set_bit(*cpsr,N);
  else
    *cpsr = clr_bit(*cpsr,N);
  if (alu_out==0)
    *cpsr = set_bit(*cpsr,Z); 
  else 
    *cpsr = clr_bit(*cpsr,Z);
  
  *cpsr = clr_bit(*cpsr,V);
  
  return 0;
}
// CMP2 p552 mais surement qu'il faut faire les trois versions
int arm_op_cmp(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rm;
  int32_t n, m, res;
  int64_t alu_out;
 
  rn = get_bits(instr,2,0);
  rm = get_bits(instr,5,3);
  n = arm_read_register(p,rn);
  m = (arm_read_register(p,rm));
  alu_out = n-m;
  res = get_bits(alu_out,31,0);

  if (get_bit(alu_out,31)==1)
    *cpsr = set_bit(*cpsr,N);
  else
    *cpsr = clr_bit(*cpsr,N);
  if (alu_out==0)
    *cpsr = set_bit(*cpsr,Z); 
  else 
    *cpsr = clr_bit(*cpsr,Z);
  if (get_bit(alu_out,32)==1)
    *cpsr = set_bit(*cpsr,C);
  else
    *cpsr = clr_bit(*cpsr,C);
  if (((n<0) && (m>0) && (res>0)) || ((n<0) && (m<0) && (res<0)))
    *cpsr = set_bit(*cpsr,V);
  else
    *cpsr = clr_bit(*cpsr,V);
  
  return 0;
}

int arm_op_cmn(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rm;
  int32_t n, m, res;
  int64_t alu_out;
 
  rn = get_bits(instr,2,0);
  rm = get_bits(instr,5,3);
  n = arm_read_register(p,rn);
  m = (arm_read_register(p,rm));
  alu_out = n+m;
  res = get_bits(alu_out,31,0);

  if (get_bit(alu_out,31)==1)
    *cpsr = set_bit(*cpsr,N);
  else
    *cpsr = clr_bit(*cpsr,N);
  if (alu_out==0)
    *cpsr = set_bit(*cpsr,Z); 
  else 
    *cpsr = clr_bit(*cpsr,Z);
  if (get_bit(alu_out,32)==1)
    *cpsr = set_bit(*cpsr,C);
  else
    *cpsr = clr_bit(*cpsr,C);
  if (((n>0) && (m>0) && (res<0)) || ((n<0) && (m<0) && (res>0)))
    *cpsr = set_bit(*cpsr,V);
  else
    *cpsr = clr_bit(*cpsr,V);

  return 0;
}

// p234
int arm_op_orr(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rd;
  int32_t rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  y = shifter_operand(rs, *cpsr,instr);

  arm_write_register(p,rd,x|y);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    // mettre  C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_mov(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rd, rs;
  int x, dest;
 
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);

  if (get_bit(instr,25)==0) //test valeur immediate
    x = arm_read_register(p,rs);
  else
    x = rs;

  arm_write_register(p,rd,x);
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    // TODO: mettre C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_bic(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rn, rd, rs;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);
  x = arm_read_register(p,rn);

  if (get_bit(instr,25)==0) //test valeur immediate
    y = arm_read_register(p,rs);
  else
    y = rs;

  arm_write_register(p,rd,x&~y); // Rd = Rn AND NOT shifter_operand
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p);
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    //TODO: mettre  C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_mvn(arm_core p, uint32_t instr, int32_t *cpsr){
  int8_t rd, rs;
  int x, dest;
 
  rd = get_bits(instr,15,12);
  rs = get_bits(instr,11,0);

  if (get_bit(instr,25)==0) //test valeur immediate
    x = arm_read_register(p,rs);
  else
    x = rs;

  arm_write_register(p,rd,~x); // Rd = NOT shifter_operand
  if ((get_bit(instr,20)) && (rd==15)){
    if (arm_current_mode_has_spsr(p)){
      *cpsr = arm_read_spsr(p); 
    }
    else
      return DATA_ABORT;
  }
  else if (get_bit(instr,20)==1){
    dest = arm_read_register(p,rd);
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
    // TODO: mettre C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

static int arm_execute_instruction(arm_core p) {
  uint32_t *instr = NULL;
  int8_t opcode;
  int z,n,c,v;
  int32_t cpsr;
  int32_t rs = arm_read_register(p, 15);
  if (get_bit(rs,0)==0 && get_bit(rs,1)==0){
    //if ((memory_read_word(p->mem,1,p->register_storage[15]-8,instr))==0){
    if(arm_fetch(p,instr)==0){
      if ((0x3 & (*instr >> 26))){ //verifie � 0 les bit [27:26]
	int8_t cond = get_bits(*instr, 31, 28);
	cpsr = arm_read_cpsr(p);
	n = get_bit(cpsr,N); 
	z = get_bit(cpsr,Z); 
	c = get_bit(cpsr,C); 
	v = get_bit(cpsr,V);
	
	switch(cond){
	case EQ:
	  if (z==0)
	    return 0;   
	case NE :
	  if (z!=0)
	    return 0; 
	case CS :
	  if (c==0)
	    return 0; 
	case CC :
	  if (c!=0)
	    return 0; 
	case MI:
	  if (n==0)  
	    return 0; 
	case PL:
	  if (n!=0)
	    return 0; 
	case VS:
	  if (v==0) 
	    return 0;
	case VC:
	  if (v!=0)
	    return 0; 
	case HI:
	  if (c==0 || z!=0)
	    return 0; 
	case LS:
	  if (c!=0 && z==0)
	    return 0; 
	case GE:
	  if (n!=v)
	    return 0; 
	case LT:
	  if (n==v)
	    return 0; 
	case GT:
	  if (z!=0 || n!=v)
	    return 0; 
	case LE:
	  if (z==0 && n==v)
	    return 0; 
	case AL:
	  break;
	case UNPREDICTABLE:
	  return PREFETCH_ABORT; //exception
	} 

	opcode = get_bits(*instr,24,21);

	switch(opcode){
	case AND:
	  return arm_op_and(p,*instr,&cpsr);
	  break;
	case EOR:
	  return arm_op_eor(p,*instr,&cpsr);
	  break;
	case SUB:
	  return arm_op_sub(p,*instr,&cpsr);
	  break;
	case RSB:
	  return arm_op_rsb(p,*instr,&cpsr);
	  break;
	case ADD:
	  return arm_op_add(p,*instr,&cpsr);
	  break;
	case ADC:
	  return arm_op_adc(p,*instr,&cpsr);
	  break;
	case SBC:
	  return arm_op_sbc(p,*instr,&cpsr);
	  break;
	case RSC:
	  return arm_op_rsc(p,*instr,&cpsr);
	  break;
	case TST:
	  return arm_op_tst(p,*instr,&cpsr);
	  break;
	case TEQ:
	  return arm_op_teq(p,*instr,&cpsr);
	  break;
	case CMP:
	  return arm_op_cmp(p,*instr,&cpsr);
	  break;
	case CMN:
	  return arm_op_cmn(p,*instr,&cpsr);
	  break;
	case ORR:
	  return arm_op_orr(p,*instr,&cpsr);
	  break;
	case MOV:
	  return arm_op_mov(p,*instr,&cpsr);
	  break;
	case BIC:
	  return arm_op_bic(p,*instr,&cpsr);
	  break;
	case MVN:
	  return arm_op_mvn(p,*instr,&cpsr);
	  break;
	default:
	  return UNDEFINED_INSTRUCTION;
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
