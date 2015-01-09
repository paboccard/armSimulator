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



int arm_op_and(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  x = arm_read_register(p,rn);

  y= arm_data_processing_shift(p,instr);

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

  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
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

int arm_op_sub(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
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
      
    //NOT BorrowFrom(Rn - shifter_operand)
    if ((x-y)>=0)
      *cpsr = clr_bit(*cpsr,C); 
    else 
      *cpsr = set_bit(*cpsr,C); 
    // mettre  C Flag en fonction de shifter_carry_out
    
    
    if ((x>0 && y<0 && (x-y)>0) || (x<0 && y>0 && (x-y)<0) )
      *cpsr = set_bit(*cpsr,V); 
    else 
      *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_rsb(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
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
    //NOT BorrowFrom(shifter_operand - Rn)
    if ((y-x)>=0)
      *cpsr = clr_bit(*cpsr,C); 
    else 
      *cpsr = set_bit(*cpsr,C); 
    // mettre  C Flag en fonction de shifter_carry_out
    
    
    if ((y>0 && x<0 && (y-x)>0) || (y<0 && x>0 && (y-x)<0) )
      *cpsr = set_bit(*cpsr,V); 
    else 
      *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_add(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
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
  uint8_t rn, rd;
  int x, y, dest;

  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  x = arm_read_register(p,rn);

  y= arm_data_processing_shift(p,instr);
    
  arm_write_register(p, rd,x + y + get_bit(*cpsr,30) );
  
  if(get_bit(instr,20) && rd ==15){
    if(arm_current_mode_has_spsr(p)){
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
    
    long long int test;
    int c = get_bit(*cpsr,C);  
    test = x + y + c;
    
    int32_t b=~0;
    if(test > b)
      *cpsr = set_bit(*cpsr,C);
    else
      *cpsr = clr_bit(*cpsr,C);
      
    //Fonction OverflowFrom avec 3 paramètres
    if((x>0 && y>0 && x+y<0) || (x<0 && y<0 && x+y>0))
      *cpsr = set_bit(*cpsr,V);
    else if((x+y>0 && c>0 && x+y+c < 0) || (x+y<0 && c<0 && x+y+c > 0))

      *cpsr = set_bit(*cpsr,V);
    else
      *cpsr = clr_bit(*cpsr,V);
    
  }
  return 0;
}

int arm_op_sbc(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
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
    //Flag N
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
   
    //Flag Z
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
      
    //Flag C
    //NOT BorrowFrom(Rn - shifter_operand)
      
    int flagC=get_bit(*cpsr,C)==0?1:0;
    int res=x-y;
	
    if ((res-flagC)>=0)
      *cpsr = clr_bit(*cpsr,C); 
    else 
      *cpsr = set_bit(*cpsr,C); 
    
    //Flag V
    if ((x>0 && y<0 && (x-y)>0) || (x<0 && y>0 && (x-y)<0) )
      *cpsr = set_bit(*cpsr,V);
    else 
		

      if ((res>0 && flagC<0 && (res-flagC)>0) || (res<0 && flagC>0 && (res-flagC)<0) ){
	*cpsr = set_bit(*cpsr,V); 
      }
      else{
	*cpsr = clr_bit(*cpsr,V);
      }
      
  }
  return 0;
}

int arm_op_rsc(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
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
    //Flag N
    if (get_bit(dest,31)==1)
      *cpsr = set_bit(*cpsr,N);
    else
      *cpsr = clr_bit(*cpsr,N);
   
    //Flag Z
    if (dest==0)
      *cpsr = set_bit(*cpsr,Z); 
    else 
      *cpsr = clr_bit(*cpsr,Z);
      
    //Flag C
    //NOT BorrowFrom(Rn - shifter_operand)
      
    int flagC=get_bit(*cpsr,C)==0?1:0;
    int res=y-x;
	
    if ((res-flagC)>=0)
      *cpsr = clr_bit(*cpsr,C); 
    else 
      *cpsr = set_bit(*cpsr,C); 
    
    //Flag V
    if ((y>0 && x<0 && (y-x)>0) || (y<0 && x>0 && (y-x)<0) )
      *cpsr = set_bit(*cpsr,V);
    else {
		
      if ((res>0 && flagC<0 && (res-flagC)>0) || (res<0 && flagC>0 && (res-flagC)<0) ){
	*cpsr = set_bit(*cpsr,V); 
      }
      else{
	*cpsr = clr_bit(*cpsr,V);
      }
    }
  }
  return 0;
}

int arm_op_tst(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn;
  int64_t alu_out;
  int32_t x, y;
  
  rn = get_bits(instr,19,16);
  
  x = arm_read_register(p,rn);
  y = arm_data_processing_shift(p,instr);
    
  alu_out = x & y;
  
  //Flag N
  if (get_bit(alu_out,31)==1)
    *cpsr = set_bit(*cpsr,N);
  else
    *cpsr = clr_bit(*cpsr,N);
      
  //Flag Z
  if (alu_out==0)
    *cpsr = set_bit(*cpsr,Z); 
  else 
    *cpsr = clr_bit(*cpsr,Z);
    
  //Flag C
  //Fait dans data processing
  
  //Flag V
  *cpsr = clr_bit(*cpsr,V);
    
  return 0;
}

int arm_op_teq(arm_core p, uint32_t instr, int32_t *cpsr){

  uint8_t rn;
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

int arm_op_cmp(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn;
  int32_t n,m, res;
  int64_t alu_out;
 
  rn = get_bits(instr,19,16);
  n = arm_read_register(p,rn);
  m = arm_data_processing_shift(p,instr);
  alu_out = n-m;
  res = get_bits(alu_out,31,1); //MODIF modifie 0->1

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
  uint8_t rn;
  int32_t n, m, res;
  int64_t alu_out;
 
  rn = get_bits(instr,19,16);
  n = arm_read_register(p,rn);
  m = arm_data_processing_shift(p,instr);
  alu_out = n+m;
  res = get_bits(alu_out,31,1); //MODIF modifie 0->1

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
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  x = arm_read_register(p,rn);

  y = arm_data_processing_shift(p,instr);

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
  uint8_t rd;
  int x, dest;

  rd = get_bits(instr,15,12);
  x = arm_data_processing_shift(p,instr);

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
    // C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_bic(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rn, rd;
  int x, y, dest;
 
  rn = get_bits(instr,19,16);
  rd = get_bits(instr,15,12);
  x = arm_read_register(p,rn);

  y = arm_data_processing_shift(p,instr);

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
    //C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

int arm_op_mvn(arm_core p, uint32_t instr, int32_t *cpsr){
  uint8_t rd;
  int x, dest;
 
  rd = get_bits(instr,15,12);

  x = arm_data_processing_shift(p,instr);

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
    // C Flag en fonction de shifter_carry_out
    *cpsr = clr_bit(*cpsr,V);
  }
  return 0;
}

/****************** LOAD / STORE ******************/
int arm_op_ldr(arm_core p, uint32_t instr){
  uint8_t rd;
  int val_rd;
  rd = get_bits(instr,15,12);
  val_rd = arm_read_register(p,rd);

  if (CP15_reg1_Ubit == 0)//A Modifier
    data = Memory[address,4] Rotate_Right (8 * address[1:0]); //A Modifier
  else
    /* CP15_reg_Ubit == 1 */
    data = Memory[address,4]; //A Modifier
  if (rd ==15){
    /*A Modifier*/
    PC = data AND 0xFFFFFFFE// 
      T Bit = data[0]//
    else
      PC = data AND 0xFFFFFFFC//
      else
	Rd = data//
  return 0;
}

int arm_op_str(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_ldrb(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_strb(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_ldrh(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_strh(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_ldm1(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_stm1(arm_core p, uint32_t instr){
  return 0;
}

int arm_op_bl(arm_core p, uint32_t instr){
  return 0;
}

int test_cond(uint8_t cond, arm_core p){
  int z,n,c,v;
  int32_t cpsr;
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
    return 1;
    break;
  case UNPREDICTABLE:
    return PREFETCH_ABORT; //exception
  }
  return PREFETCH_ABORT;
}  

static int arm_execute_instruction(arm_core p) {
  uint32_t instr;
  uint8_t opcode;
  int test;
  int32_t cpsr;
  int32_t rs = arm_read_register(p, 15);

  if (get_bit(rs,0)==0 && get_bit(rs,1)==0){
    //if ((memory_read_word(p->mem,1,p->register_storage[15]-8,instr))==0){
    
    if(arm_fetch(p,&instr)==0){

      if (get_bits(instr,27,26)==0){ //verifie à 0 les bit [27:26]

	if (!(get_bit(instr,4) && get_bit(instr,7))){ // test pour différencier les instruction avec MSR, STRH, LDRH

	  uint8_t cond = get_bits(instr,31, 28);
	  test = test_cond(cond,p);

	  if (test==0 || test ==  PREFETCH_ABORT)  return test;
	  
	  opcode = get_bits(instr,24,21);
	  
	  switch(opcode){
	  case AND:
	    return arm_op_and(p,instr,&cpsr);
	    break;
	  case EOR:
	    return arm_op_eor(p,instr,&cpsr);
	    break;
	  case SUB:
	    return arm_op_sub(p,instr,&cpsr);
	    break;
	  case RSB:
	    return arm_op_rsb(p,instr,&cpsr);
	    break;
	  case ADD:
	    return arm_op_add(p,instr,&cpsr);
	    break;
	  case ADC:
	    return arm_op_adc(p,instr,&cpsr);
	    break;
	  case SBC:
	    return arm_op_sbc(p,instr,&cpsr);
	    break;
	  case RSC:
	    return arm_op_rsc(p,instr,&cpsr);
	    break;
	  case TST:
	    return arm_op_tst(p,instr,&cpsr);
	    break;
	  case TEQ:
	    return arm_op_teq(p,instr,&cpsr);
	    break;
	  case CMP:
	    return arm_op_cmp(p,instr,&cpsr);
	    break;
	  case CMN:
	    return arm_op_cmn(p,instr,&cpsr);
	    break;
	  case ORR:
	    return arm_op_orr(p,instr,&cpsr);
	    break;
	  case MOV:
	    return arm_op_mov(p,instr,&cpsr);
	    break;
	  case BIC:
	    return arm_op_bic(p,instr,&cpsr);
	    break;
	  case MVN:
	    return arm_op_mvn(p,instr,&cpsr);
	    break;
	  default:
	    return UNDEFINED_INSTRUCTION;
	    break;
	  }
	}
	else // cas pour STRH et LDRH
	  if (get_bit(instr,20)==1)
	    return arm_op_ldrh(p,instr);
	  else
	    return arm_op_strh(p,instr);
      }
      else if (get_bits(instr,27,26)==1){ //verifie à 01 les bit [27:26]
	uint8_t cond = get_bits(instr, 31, 28);
	test = test_cond(cond,p);
	if (test==0 || test ==  PREFETCH_ABORT)  return test;
	
	if (get_bit(instr,22)==0){
	  if (get_bit(instr,20)==1)//test pour savoir si c'est un load
	    return arm_op_ldr(p,instr);
	  else
	    return arm_op_str(p,instr);
	}
	else{
	  if (get_bit(instr,20)==1)//test pour savoir si c'est un load
	    return arm_op_ldrb(p,instr);
	  else
	    return arm_op_strb(p,instr);
	} 
      }
      else if (get_bits(instr,27,26)==2){ //verifie à 10 les bit [27:26]
	if (get_bit(instr,25)==0){
	  if (get_bit(instr,20)==1)//test pour savoir si c'est un load
	    return arm_op_ldm1(p,instr);
	  else
	    return arm_op_stm1(p,instr);
	}
	else
	  return arm_op_bl(p,instr);
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
 
