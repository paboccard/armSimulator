.global main
.text
main:


	mov r0, #15			@ r0 = 1111
	mov r1, #0x9			@ r1 = 1001
	
	mov r2, #1
	mov r3, #3
	and r4, r2, r3			@r4 = 1
	ands r5, r4, #2			@r5 = 0 maj Z 

	eor r7, r0, r1			@r7 = 6
	eor r8, r0, #12			@ 12 1100, r8 = 0011
	eors r8, r8, r8         	@ r8 = 0  maj Z

@@@@@@@@@@@@ test sub @@@@@@@@@@@@@@@@@
	
	subs r10, r1, r0		@ r10 = -6 N -> 1
	sub r11, r0, #5			@ r11 = 10
	subs r11, r11, #10		@ Z = 1  c =1
	mov r7, #0xC0000000
	mov r8, #0x50000000
	subs r9, r7, r8			@ V -> 1   C -> 1
	mov r7, #0x40000000
	mov r8, #0x80000000
	subs r9, r7, r8			@ V -> 1 N ->1 

	
	
@@@@@@@@@@@@@ test add @@@@@@@@@@@@@@@@
	add r7, r0, r1			@ r7 = 24
	add r8, r0, #3			@ r8 = 18
	adds r9, r1, #-9		@ r9 = 0    Z = 1  C = 1
	adds r9, r9, #-1		@ r9 = 0    N = 1
	mov r9, #0x80000000    
	adds r9, r9, r9			@ V -> 1   C -> 1 Z -> 1
	mov r9, #0x40000000   
	adds r9, r9, r9			@ V -> 1  N -> 1 



@@@@@@@@@@@@@@ shifter test qvec add @@@@@@@@@@@

@@ asr 
	add r7, r2, r0, asr #1     @ r7 = 8
	adds r7, r0, #0				@ flags O
	mov r7, r0, asr #1  		@ r7 = 7
	movs r7, r7, asr #32			@ r7 = 0 Z -> 1
	mov r7, #0x80000000
	movs r7, r7, asr #32			@ r7 = 0xFFFFFFFF  C -> 1 N -> 1
	add r8, r2, r0, asr r2	  @ r8 = 8     
	mov r7, #0x8f000000
	movs r7, r7, asr #1         @ r7 = 0xc7800000     N -> 1 

@@ lsr 
	mov r7, #8
	mov r7, r7, lsr #2        @ r7 = 2
	add r7, r2, r0, lsr #1     @ r7 = 8
	add r8, r2, r0, lsr r2	  @ r8 = 8    
	mov r7, #0x80000000
	mov r7, r7, lsr #1         @ r7 = 0x40000000
	mov r7, r2, lsr #1			@ Z -> 1 C -> 1     

@@ lsl
	mov r7, r2, lsl #1         @ r7 = 2
	mov r8, r2, lsl r2	  @ r8 = 2
	mov r7, #0xfffffffe	  @ r7 = -2
	adds r8, r2, #0		@ Maj des flags -> 0
	movs r7, r7, lsl #1	  @ r7 = 0xfffffffc  C -> 1

@@ ror
	adds r7, r2, #0		@ Maj des flags -> 0
	movs r7, r2, ror r2	  @ r7 = 0x80000000 C->1 N->1
	movs r7, r2, ror #2	  @ r7 = 0x40000000 flags a 0
	
@@ rrx
	adds r9, r2, #0
	movs r9, r9, RRX 		@ r9 = 0 c -> 1 Z->1
	movs r9, r9, RRX		@ r9 = 0x80000000 c -> 0 N->1
	
	

@@@@@@@@@@@@@@ test AND @@@@@@@@@@@@@@@@@@@@@@@@@

	mov r7, r2, RRX		@ r7 = 0
	ANDS r7, r2, r7		@ r7 = 0   Z -> 1
	mov r7, #0xA0000000
	ANDS r7, r7, #0xB0000000		@ r7 = #0xA0000000  N -> 1 C->1
	
@@@@@@@@@@@@@@ test EOR @@@@@@@@@@@@@@@@@@@@@@@@@@

	mov r7, #13
	mov r8, #6
	EOR r7, r8, #13		@ r7 = 11
	EORS r7, r7, r7		@ r7 = 0  Z -> 0
	EORS r7, r7, #0x80000000		@ r7 = 0x80000000 N->1 C->1 (Rotation pour un nbr imm > 4 bits)
	

@@@@@@@@@@@@@@ test RSB @@@@@@@@@@@@@@@@@@@@@@@@@@

	rsbs r10, r0, r1		@ r10 = -6 N -> 1
	rsb r11, r0, #5			@ r11 = -10  #0xFFFFFFF6
	rsbs r11, r11, r11		@ Z = 1  c =1
	mov r7, #0xC0000000
	mov r8, #0x50000000
	rsbs r9, r8, r7			@ V -> 1   C -> 1
	mov r7, #0x40000000
	mov r8, #0x80000000
	rsbs r9, r8, r7			@ V -> 1 N ->1 

@@@@@@@@@@@@@@ test ADC @@@@@@@@@@@@@@@@@@@@@@@@@@@

	adds r7, r2, #0			@ Maj des flags
	movs r7, r2, RRX 		@ r7 = 0 C->1 Z->1
	adcs r8, r7, #5			@ r8 = 6 Flags->0
	adc r7, r8, r7			@ r7 = 6
	mov r9, #0x80000000    
	adcs r9, r9, r9			@ V -> 1   C -> 1 Z -> 1
	mov r9, #0x40000000   
	adds r8, r2, r0			@ C -> 0
	adcs r9, r9, r9			@ V -> 1  N -> 1
	
@@@@@@@@@@@@@@ test SBC @@@@@@@@@@@@@@@@@@@@@@@@@@@

	subs r7, r0, r0 		@ r7 = 1 C -> 1
	add r7, r7, #1			@ r7 = 1
	sbcs r8, r7, #1			@ r8 = -1  C -> 0
	sbcs r10, r1, r0		@ r10 = -6 N -> 1
	sbc r11, r0, #5			@ r11 = 10
	sbcs r11, r11, #10		@ Z = 1  c =1
	mov r7, #0xC0000000
	adds r9, r2, r0			@ C -> 0
	mov r8, #0x50000000
	sbcs r9, r7, r8			@ V -> 1   C -> 1 Z -> 1
	adds r9, r2, r0			@ C -> 0
	mov r7, #0x40000000
	mov r8, #0x80000000
	sbcs r9, r7, r8			@ V -> 1 N ->1
	
@@@@@@@@@@@@@@ test RSC @@@@@@@@@@@@@@@@@@@@@@@@@@@@

	adds r7, r3, r3, RRX		@ r7 = 4 C -> 1
	rscs r8, r7, #1			@ r8 = 0  C -> 0 Z -> 1
	rscs r10, r0, r1		@ r10 = -6 N -> 1
	mov r7, #0xC0000000
	mov r8, #0x50000000
	rscs r9, r8, r7			@ V -> 1   C -> 1 Z -> 1

@@@@@@@@@@@@@@ test TST @@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	mov r7, r2, RRX		@ r7 = 0
	TST r2, r7		@ r7 = 0   Z -> 1
	mov r7, #0xA0000000
	TST r7, #0xB0000000		@ r7 = #0xA0000000  N -> 1

@@@@@@@@@@@@@@ test CMP @@@@@@@@@@@@@@@@@@@@@@@@@@@@

	cmp r1, r0				@ C -> 1
	mov r11, #10			@ r11 = 10
	cmp r11, #10			@ c =0
	mov r7, #0xC0000000
	mov r8, #0x50000000
	cmp r7, r8				@ V -> 1   C -> 0
	mov r7, #0x40000000
	mov r8, #0x80000000
	cmp r7, r8				@ V -> 1 
	


	
@test avec des valeurs dépassant les 32 bits


	adc r10, r0, r1 		
	adc r11, r0, #3

	swi 0x123456
	
	sbc r4, r0, r1
	sbc r5, r1, #5

	rsc r7, r1, r0
	rsc r8, r1, #10

@@@@@@@@@ fonctions avec Modifications des Flags uniquement @@@@@@@@

	tst r0, r1
	tst r0, #5

	teq r0, #1000
	teq r0, r1
	teq r0, #5

	cmp r0, r1
	cmp r0, #5

	cmn r0, r1
	cmn r0, #5

@@@@@@@@@ 		 @@@@@@@@@

@	orr r4, r1, r0
@	orr r5, r1, #5

@	mov r0, #9
@	mov r1, #9

@	bic r7, r0, r1		@ r1 = 9, !r1 = 6, r0 & !r1 = 0
@	bic r8, r0, #5		@ !#5 = 10, r0 & !#5 = 8

@	mvn r10, r0		@ r0 = 9, !r0 = 6
@	mvn r11, #5		@ !#5 = 10 


 swi 0x123456

@    ldr r0, =limite
@    ldrb r1, [r0]
@    add r0, r0, #3
@    ldrb r2, [r0]
@.data
@limite:
@    .word 0x12345678
