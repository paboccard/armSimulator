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
 
 
/****************** traitement de donnees ******************/
int arm_op_and(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
  
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x&y);
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	  
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	// mettre  C Flag en fonction de shifter_carry_out
	cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_eor(arm_core p, uint32_t instr){ 
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);
    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

 
    arm_write_register(p,rd,x^y);
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	// mettre  C Flag en fonction de shifter_carry_out
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_sub(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x-y);
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
      

	//NOT BorrowFrom(Rn - shifter_operand)
	if ((dest)>=0 && !get_bit(cpsr,C))
	    cpsr = set_bit(cpsr,C); 
	 
	// mettre  C Flag en fonction de shifter_carry_out
    
    
	if ((x>=0 && y<0 && dest <0 ) || (x<=0 && y>0 && dest>=0) )
	    cpsr = set_bit(cpsr,V); 
	else 
	    cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_rsb(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,y-x);
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	//NOT BorrowFrom(shifter_operand - Rn)
	if ((y-x)>=0 && !get_bit(cpsr,C))
	    cpsr = set_bit(cpsr,C); 
    
    
	if ((y>0 && x<0 && (y-x)<0) || (y<0 && x>0 && (y-x)>0) )
	    cpsr = set_bit(cpsr,V); 
	else 
	    cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_add(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x+y);
    if ((get_bit(instr,20)) && (rd==15)){
		if (arm_current_mode_has_spsr(p)){
			cpsr = arm_read_spsr(p);
		}
		else
			return DATA_ABORT;
    }

    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
    
	long long int a= x+y;
	uint32_t b=~0;
	if(a>b && !get_bit(cpsr,C))
	    cpsr = set_bit(cpsr,C);

	if((x>0 && y>0 && (x+y)<0) || (x<0 && y<0 && (x+y)>=0) )
	    cpsr = set_bit(cpsr,V);
	else
	    cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
    
}

int arm_op_adc(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest, carry;

    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);
	carry = get_bit(arm_read_cpsr(p),C);
    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);
    
    arm_write_register(p, rd,x + y + carry );
  
    if(get_bit(instr,20) && rd ==15){
		if(arm_current_mode_has_spsr(p)){
			cpsr = arm_read_spsr(p);
		}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
		dest = arm_read_register(p,rd);
		if (get_bit(dest,31)==1)
			cpsr = set_bit(cpsr,N);
		else
			cpsr = clr_bit(cpsr,N);
		  
		if (dest==0)
			cpsr = set_bit(cpsr,Z); 
		else 
			cpsr = clr_bit(cpsr,Z);
		
		long long int test; 
		test = x + y + carry;
		
		uint32_t b=~0;
		if(test > b && !get_bit(cpsr,C))
			cpsr = set_bit(cpsr,C);
		  
		//Fonction OverflowFrom avec 3 paramètres
		if((x>=0 && y>0 && x+y<0) || (x<=0 && y<0 && x+y>0))
			cpsr = set_bit(cpsr,V);
		else if((x+y>0 && carry>0 && x+y+carry < 0) || (x+y<0 && carry<0 && x+y+carry > 0))

			cpsr = set_bit(cpsr,V);
		else
			cpsr = clr_bit(cpsr,V);
		
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_sbc(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest, carry;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);
	carry = get_bit(arm_read_cpsr(p),C);
	carry = carry==0 ? 1 : 0 ;
    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x-y-carry);
    if ((get_bit(instr,20)) && (rd==15)){
		if (arm_current_mode_has_spsr(p)){
			cpsr = arm_read_spsr(p);
		}
		else
			return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
		dest = arm_read_register(p,rd);
		//Flag N
		if (get_bit(dest,31)==1)
			cpsr = set_bit(cpsr,N);
		else
			cpsr = clr_bit(cpsr,N);
	   
		//Flag Z
		if (dest==0)
			cpsr = set_bit(cpsr,Z); 
		else 
			cpsr = clr_bit(cpsr,Z);
		  
		//Flag C
		//NOT BorrowFrom(Rn - shifter_operand)
		  
		int flagC=get_bit(cpsr,C)==0?1:0;
		int res=x-y;
	
		if ((res-flagC)>=0 && !get_bit(cpsr,C))
		   cpsr = set_bit(cpsr,C); 
		
		//Flag V
		if ((x>=0 && y<0 && (x-y)<0) || (x<=0 && y>0 && (x-y)>=0) )
			cpsr = set_bit(cpsr,V);
		else 

		

			if ((res>0 && flagC<0 && (res-flagC)>0) || (res<0 && flagC>0 && (res-flagC)<0) ){
			cpsr = clr_bit(cpsr,V); 
			}
			else{
			cpsr = set_bit(cpsr,V);
			}
      
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_rsc(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x-y);
    if ((get_bit(instr,20)) && (rd==15)){
		if (arm_current_mode_has_spsr(p)){
			cpsr = arm_read_spsr(p);
		}
		else
			return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	//Flag N
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
   
	//Flag Z
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
      
	//Flag C
	//NOT BorrowFrom(Rn - shifter_operand)
      
	int flagC=get_bit(cpsr,C)==0?1:0;
	int res=y-x;
	
	if ((res-flagC)<0 && !get_bit(cpsr,C))
	   cpsr = set_bit(cpsr,C); 
    
	//Flag V
	if ((y>0 && x<0 && (y-x)>0) || (y<0 && x>0 && (y-x)<0) )
	    cpsr = set_bit(cpsr,V);
	else {
		
	    if ((res>0 && flagC<0 && (res-flagC)>0) || (res<0 && flagC>0 && (res-flagC)<0) ){
		cpsr = set_bit(cpsr,V); 
	    }
	    else{
		cpsr = clr_bit(cpsr,V);
	    }
	}
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_tst(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn;
    int64_t alu_out;
    int32_t x, y;
  
    rn = get_bits(instr,19,16);
  
    x = arm_read_register(p,rn);
    y = arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);
    
    alu_out = x & y;
  
    //Flag N
    if (get_bit(alu_out,31)==1)
	cpsr = set_bit(cpsr,N);
    else
	cpsr = clr_bit(cpsr,N);
      
    //Flag Z
    if (alu_out==0)
	cpsr = set_bit(cpsr,Z); 
    else 
	cpsr = clr_bit(cpsr,Z);
    
    //Flag C
    //Fait dans data processing
  
    //Flag V
    cpsr = clr_bit(cpsr,V);
    
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_teq(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn;
    int32_t n;
    int64_t alu_out;

    rn = get_bits(instr,19,16);
    n = arm_read_register(p,rn);
    alu_out = n^arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    if (get_bit(alu_out,31)==1)
	cpsr = set_bit(cpsr,N);
    else
	cpsr = clr_bit(cpsr,N);
    if (alu_out==0)
	cpsr = set_bit(cpsr,Z); 
    else 
	cpsr = clr_bit(cpsr,Z);
  
  
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_cmp(arm_core p, uint32_t instr){
    uint32_t cpsr;
    int8_t rn;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);
    dest = x-y;
    if (get_bit(dest,31)==1)
	cpsr = set_bit(cpsr,N);
    else
	cpsr = clr_bit(cpsr,N);
      
    if (dest==0)
	cpsr = set_bit(cpsr,Z); 
    else 
	cpsr = clr_bit(cpsr,Z);
      
    //NOT BorrowFrom(Rn - shifter_operand)
    if ((x-y)>=0 || get_bit(cpsr, C) )
	cpsr = clr_bit(cpsr,C); 
	else 
	cpsr = set_bit(cpsr,C); 
    
    
    if ((x>=0 && y<0 && (x-y)<0) || (x<=0 && y>0 && (x-y)>=0))
	cpsr = set_bit(cpsr,V); 
    else 
	cpsr = clr_bit(cpsr,V);
    arm_write_cpsr(p,cpsr);
    return 0;

}

int arm_op_cmn(arm_core p, uint32_t instr){
    uint32_t cpsr;
    int8_t rn;
    int32_t x, y, dest;
 
    rn = get_bits(instr,19,16);
    x = arm_read_register(p,rn);

    y= arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);
    dest = x-y;
    
    //  ici
	if (get_bit(dest,31)==1)
		cpsr = set_bit(cpsr,N);
    else
		cpsr = clr_bit(cpsr,N);
    if (dest==0)
		cpsr = set_bit(cpsr,Z); 
    else 
		cpsr = clr_bit(cpsr,Z);
    
	long long int a= x+y;
	uint32_t b=~0;
	if(a>b && !get_bit(cpsr,C))
	    cpsr = set_bit(cpsr,C);

	if((x>0 && y>0 && (x+y)<0) || (x<0 && y<0 && (x+y)>=0) )
	    cpsr = set_bit(cpsr,V);
	else
	    cpsr = clr_bit(cpsr,V);
    
    arm_write_cpsr(p,cpsr);
    return 0;

}

// p234
int arm_op_orr(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y = arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x|y);
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	// C Flag en fonction de shifter_carry_out
	cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_mov(arm_core p, uint32_t instr){
    uint8_t rd;
    int x, dest;
	uint32_t cpsr;
	
    rd = get_bits(instr,15,12);
    x = arm_data_processing_shift(p,instr);
    
    cpsr = arm_read_cpsr(p);
	
    arm_write_register(p,rd,x);

    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1){
	    cpsr = set_bit(cpsr,N);
	    }
	else{
	    cpsr = clr_bit(cpsr,N);
	    }
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	// C Flag en fonction de shifter_carry_out
	//cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_bic(arm_core p, uint32_t instr){
    uint32_t cpsr;
    uint8_t rn, rd;
    int x, y, dest;
 
    rn = get_bits(instr,19,16);
    rd = get_bits(instr,15,12);
    x = arm_read_register(p,rn);

    y = arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,x&~y); // Rd = Rn AND NOT shifter_operand
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p);
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	//C Flag en fonction de shifter_carry_out
	cpsr = clr_bit(cpsr,V);
    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

int arm_op_mvn(arm_core p, uint32_t instr){
	uint32_t cpsr;
    uint8_t rd;
    int x, dest;
 
    rd = get_bits(instr,15,12);

    x = arm_data_processing_shift(p,instr);

    cpsr = arm_read_cpsr(p);

    arm_write_register(p,rd,~x); // Rd = NOT shifter_operand
    if ((get_bit(instr,20)) && (rd==15)){
	if (arm_current_mode_has_spsr(p)){
	    cpsr = arm_read_spsr(p); 
	}
	else
	    return DATA_ABORT;
    }
    else if (get_bit(instr,20)==1){
	printf("S = 1\n");
	dest = arm_read_register(p,rd);
	if (get_bit(dest,31)==1)
	    cpsr = set_bit(cpsr,N);
	else
	    cpsr = clr_bit(cpsr,N);
	if (dest==0)
	    cpsr = set_bit(cpsr,Z); 
	else 
	    cpsr = clr_bit(cpsr,Z);
	// C Flag en fonction de shifter_carry_out
	//cpsr = clr_bit(cpsr,V);

    }
    arm_write_cpsr(p,cpsr);
    return 0;
}

/****************** LOAD / STORE ******************/
int arm_op_ldr(arm_core p, uint32_t instr){
    /*Attention aux acces non-aligne*/

    uint8_t rd;
    int cpsr;
    uint32_t y, val_rd;
 
    rd = get_bits(instr,15,12);

    y = arm_load_store(p,instr); //y = adresse de la valeur a load
  
    if ((arm_read_word(p, y, &val_rd)==0)) {
	if (rd == 15){
	    val_rd = val_rd & 0xFFFFFFFE;
	    cpsr = arm_read_cpsr(p);
	    cpsr = get_bit(instr,0) ? set_bit(cpsr,5) : clr_bit(cpsr,5); // T bit = y[0] 
	}
	arm_write_register(p,rd,val_rd);
    }
    else
	return DATA_ABORT;

    return 0;
}

int arm_op_str(arm_core p, uint32_t instr){
    uint8_t rd;
    uint32_t y, val_rd;
 
    rd = get_bits(instr,15,12);
    val_rd = arm_read_register(p,rd);

    y = arm_load_store(p,instr); //y = adresse de la valeur a load
  
    if ((arm_write_word(p, y, val_rd)!=0)) {
    
	return DATA_ABORT;
    }
    return 0;
}

int arm_op_ldrb(arm_core p, uint32_t instr){

    uint8_t rd, val_rd;
    uint32_t y;

    rd = get_bits(instr,15,12);
  
    y = arm_load_store(p,instr); //y = adresse de la valeur a load
 
    if ((arm_read_byte(p, y, &val_rd)==-1))
	return DATA_ABORT;    
    arm_write_register(p, rd, val_rd);
    
    return 0;
}

int arm_op_strb(arm_core p, uint32_t instr){
    uint8_t rd;
    uint32_t y, val_rd;
 
    rd = get_bits(instr,15,12);
    val_rd = arm_read_register(p,rd);

    y = arm_load_store(p,instr); //y = adresse de la valeur a load
  
    if ((arm_write_byte(p, y, val_rd)!=0)) {
    
	return DATA_ABORT;
    }
    return 0;
}

int arm_op_ldrh(arm_core p, uint32_t instr){
    uint8_t rd;
    uint16_t val_rd;
    uint32_t y;

    rd = get_bits(instr,15,12);
  
    y = arm_load_store(p,instr); //y = adresse de la valeur a load
 
    if ((arm_read_half(p, y, &val_rd)==-1))
	return DATA_ABORT;    
    if (rd == 15)
	return UNPREDICTABLE;
    arm_write_register(p, rd, val_rd);

    return 0;
}


int arm_op_strh(arm_core p, uint32_t instr){
    uint8_t rd;
    uint32_t y, val_rd;
  
    rd = get_bits(instr,15,12);
    val_rd = arm_read_register(p,rd);
  
    y = arm_load_store(p,instr); //y = adresse de la valeur a load
  
    if ((arm_write_half(p, y, val_rd)!=0)) {
    
	return DATA_ABORT;
    }
    return 0;
}
 
int arm_op_ldm1(arm_core p, uint32_t instr){
    int i;
    uint8_t ri, cpsr;
    uint16_t register_list;
    uint32_t value;
    int address, end_address;

    address = arm_load_store_multiple(p,instr,&end_address);
    //  address = start_address(p,instr);
    register_list = get_bits(instr,15,0);
    for (i=0; i<15; i++){
	if (get_bit(register_list,i)){
	    if((arm_read_word(p,address,&value))==0){
		ri = i;
		arm_write_register(p,ri,value);
		address += 4;
	    }
	}
    }
    if (get_bit(register_list,15)){
	if((arm_read_word(p,address,&value))==0){
	    ri = 15;
	    value &= 0xFFFFFFFE;
	    arm_write_register(p,ri,value);
	    cpsr = arm_read_cpsr(p);
	    cpsr = get_bit(instr,0) ? set_bit(cpsr,5) : clr_bit(cpsr,5); // T bit = y[0] 
	}
	address+=4;
    }
    return 0;
    if ((end_address==address-4))
	return 0;
    else
	return UNPREDICTABLE;
}

int arm_op_stm1(arm_core p, uint32_t instr){
    int address, end_address, i;
    uint8_t ri;
    uint16_t register_list;
    //processor_id = ExecutingProcessor()
    address = arm_load_store_multiple(p,instr,&end_address);
    register_list = get_bits(instr,15,0);
    for (i = 0; i>= 15;i++){
	if (get_bit(register_list,i)){
	    ri = i;
	    if (arm_write_word(p,address,arm_read_register(p,ri))){ //Memory[address,4] = Ri
		address = address + 4;
	    }
	    else
		return UNPREDICTABLE;
	}
    }
    if ((end_address==address-4))
	return 0;
    else
	return UNPREDICTABLE;  
    
}


/****************** MRS ******************/
int arm_op_mrs(arm_core p, uint32_t ins){
    uint8_t rd = get_bits(ins,15,12);
    if (get_bit(ins,22))
	arm_write_register(p,rd,arm_read_spsr(p));
    else
	arm_write_register(p,rd,arm_read_cpsr(p));
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
	break;
    case NE :
	if (z!=0)
	    return 0;
	break;
    case CS :
	if (c==0)
	    return 0;
	break;
    case CC :
	if (c!=0)
	    return 0;
	break;
    case MI:
	if (n==0)  
	    return 0;
	break; 
    case PL:
	if (n!=0)
	    return 0;
	break; 
    case VS:
	if (v==0) 
	    return 0;
       	break;
    case VC:
	if (v!=0)
	    return 0;
	break; 
    case HI:
	if (c==0 || z!=0)
	    return 0;
	break; 
    case LS:
	if (c!=0 && z==0)
	    return 0;
	break; 
    case GE:
	if (n!=v)
	    return 0;
	break; 
    case LT:
	if (n==v)
	    return 0;
	break; 
    case GT:
	if (z!=0 || n!=v)
	    return 0;
	break; 
    case LE:
	if (z==0 && n==v)
	    return 0;
	break; 
    case AL:
	return 1;
	break;
    default:
	return UNPREDICTABLE; //exception
    }
    return 1;
}  

static int arm_execute_instruction(arm_core p) {
    uint32_t instr;
    uint8_t opcode;
    int test, res;
    uint32_t cpsr;
    int32_t rs = arm_read_register(p, 15);

    cpsr = arm_read_cpsr(p);
  
    if (get_bit(rs,0)==0 && get_bit(rs,1)==0){
	//if ((memory_read_word(p->mem,1,p->register_storage[15]-8,instr))==0){
    

	if(arm_fetch(p,&instr)==0){
	    arm_coprocessor_others_swi(p,instr); // Fini le programme quand l'instruction est swi 0x123456
	    
	    uint8_t cond = get_bits(instr,31, 28);
	    test = test_cond(cond,p);
	    
	    if (test==0 || test ==  PREFETCH_ABORT)  return test;	

	    if (get_bits(instr,27,26)==0){ //verifie à 0 les bit [27:26]

		
		if (get_bit(instr,25)==1 || (get_bits(instr,7,4)!=11)){ // test pour différencier les instruction avec MSR, STRH, LDRH
	  
		    if ((get_bits(instr,27,23)==2) && get_bits(instr,21,20)==0)
			res = arm_op_mrs(p,instr);
		    else{

			opcode = get_bits(instr,24,21);
	  
			switch(opcode){
			case AND:
			    res = arm_op_and(p,instr);
			    return res;
			    break;
			case EOR:
			    res = arm_op_eor(p,instr);
			    return res;
			    break;
			case SUB:
			    res = arm_op_sub(p,instr);
			    
			    return res;
			    break;
			case RSB:
			    res = arm_op_rsb(p,instr);
			    
			    return res;
			    break;
			case ADD:
			    res = arm_op_add(p,instr);
			    
			    return res;
			    break;
			case ADC:
			    res = arm_op_adc(p,instr);
			    
			    return res;
			    break;
			case SBC:
			    res = arm_op_sbc(p,instr);
			    
			    return res;
			    break;
			case RSC:
			    res = arm_op_rsc(p,instr);
			    
			    return res;
			    break;
			case TST:
			    res = arm_op_tst(p,instr);
			    
			    return res;
			    break;
			case TEQ:
			    res = arm_op_teq(p,instr);
			    
			    return res;
			    break;
			case CMP:
			    res = arm_op_cmp(p,instr);
			    
			    return res;
			    break;	    
			case CMN:
			    res = arm_op_cmn(p,instr);
			    
			    return res;
			    break;
			case ORR:
			    res = arm_op_orr(p,instr);
			    
			    return res;
			    break;
			case MOV:
			    res = arm_op_mov(p,instr);
			    return res;
			    break;
			case BIC:
			    res = arm_op_bic(p,instr);
			    
			    return res;
			    break;
			case MVN:
			    res = arm_op_mvn(p,instr);
			    
			    return res;
			    break;
			default:
			    return UNDEFINED_INSTRUCTION;
			    break;
			}
		    }
		}
		else{ // cas pour STRH et LDRH
		    if (get_bit(instr,20)==1) {
			res = arm_op_ldrh(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		    else{
			res = arm_op_strh(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		}
	    }
	    else if (get_bits(instr,27,26)==1){ 		//verifie à 01 les bit [27:26]
		uint8_t cond = get_bits(instr, 31, 28);
		test = test_cond(cond,p);
		if (test==0 || test ==  PREFETCH_ABORT)  return test;
	
		if (get_bit(instr,22)==0){
		    if (get_bit(instr,20)==1){		//test pour savoir si c'est un load
			res = arm_op_ldr(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		    else {
			res = arm_op_str(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		}
		else {
		    if (get_bit(instr,20)==1) {		//test pour savoir si c'est un load
			res = arm_op_ldrb(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		    else {
			res = arm_op_strb(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		} 
	    }
 	    else if (get_bits(instr,27,26)==2) { 		//verifie à 10 les bit [27:26]
		if (get_bit(instr,25)==0){
		    if (get_bit(instr,20)==1) {//test pour savoir si c'est un load
			res = arm_op_ldm1(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		    else {
			res = arm_op_stm1(p,instr);
			arm_write_cpsr(p,cpsr);
			return res;
		    }
		}
		else {
		    res = arm_branch(p,instr);
		    arm_write_cpsr(p,cpsr);
		    return res;
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
 
