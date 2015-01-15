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

	adds r7, r2, #0			@ r7 = 1 Flags->0
	sbcs r8, r7, #0			@ r8 = 0  C->1 Z->1 
	sbcs r10, r1, r0		@ r10 = -6 N->1 C->0
	sbc r11, r0, #5			@ r11 = 9
	sbcs r11, r11, #8		@ Z = 1 C->1
	mov r7, #0xC0000000
	adds r9, r2, r0			@ C -> 0
	mov r8, #0x50000000
	sbcs r9, r7, r8			@ V -> 1 C->1
	adds r9, r2, r0			@ C -> 0
	mov r7, #0x40000000
	mov r8, #0x80000000
	sbcs r9, r7, r8			@ V->1 N->1 C->0
	
@@@@@@@@@@@@@@ test RSC @@@@@@@@@@@@@@@@@@@@@@@@@@@@

	adds r7, r2, #0			@ Flags -> 0
	movs r7, r3, RRX		@ r7 = 1 C -> 1
	rscs r8, r7, #1			@ r8 = 0  C -> 1 Z -> 1
	rscs r10, r0, r1		@ r10 = -6 N -> 1
	mov r7, #0xE0000000
	mov r8, #0x50000000
	rscs r9, r7, r8			@ V -> 1 C->1

@@@@@@@@@@@@@@ test TST @@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	mov r7, #0		@ r7 = 0
	TST r2, r7		@ r7 = 0   Z -> 1
	mov r7, #0xA0000000
	TST r7, #0xB0000000		@ r7 = #0xA0000000  N->1 C->1 

@@@@@@@@@@@@@@ test CMP @@@@@@@@@@@@@@@@@@@@@@@@@@@@

	cmp r1, r0			@ N->1
	mov r11, #10			@ r11 = 10
	cmp r11, #10			@ C->1 Z->1
	mov r7, #0xC0000000
	mov r8, #0x50000000
	cmp r7, r8				@ V -> 1   C -> 1
	mov r7, #0x40000000
	mov r8, #0x80000000
	cmp r7, r8				@ V -> 1 N->1
	
@@@@@@@@@@@@@@@@@@ Test MVN @@@@@@@@@@@@@@@@@@
	
 	MOV 	r1, #28			@ r1 = 1c
	MOV 	r2, #0x9		@ r2 = 1001
	ADDS	r8, r2, #0		@ Flags->0
	MVNS 	r3, #15			@ r3 = FFFFFFF0 -> N = 1
	MVNS	r5, #0xFFFFFFFF		@ r5 = 0 -> Z = 1
	MOV	R7, #9
	MOV 	R8, #5
	
@@@@@@@@@@@@@@@@@@ Test MRS @@@@@@@@@@@@@@@@@@

	MRS	r4, cpsr		@ r4 = CPSR
	MRS 	r6, spsr		@ r6 = SPSR

@@@@@@@@@@@@@@@@@@ Test LDR @@@@@@@@@@@@@@@@@@

	STR	R3, [R1]		@ [28] = FFFFFFF0
	LDR	R2, [R1]		@ R2 = FFFFFFF0

	STR	R7, [R2,#64]
	LDR	R2, [R2,#64]		@ R2 = 9
	
	STR	R8, [R7]
	LDR 	R2, [R7,R2]!

	STR	R8, [R1,R2,LSL #2]
	LDR	R9, [R1,R2,LSL #2]


	

@@@@@@@@@@@@@@@@@@ Test LDRM STRM @@@@@@@@@@@@@@@@@@	

	ldr r0,=a
	ldmia r0,{r6,r7,r8}
	ldmib r0,{r6,r7,r8}
	ldmda r0,{r6,r7,r8}
	ldmdb r0,{r6,r7,r8}
	mov r1,#1
	mov r2,#2
	mov r3,#3
	stmia r0,{r1-r3}
	ldmia r0,{r6,r7,r8}
	stmib r0,{r1-r3}
	ldmib r0,{r6,r7,r8}
	stmda r0,{r1-r3}
	ldmda r0,{r6,r7,r8}
	stmdb r0,{r1-r3}
	ldmdb r0,{r6,r7,r8}

	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ test sp @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
mov r4, #4
mov r5, #5
mov r6, #6
mov r7, #7
mov r8, #8
mov r9, #9

stmfd sp!, {r4, r5, r6, r7, r8, r9}                @ save variables to stack

	mov r4, #0
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	mov r9, #0
 
ldmfd   sp!, {r4, r5, r6, r7, r8, r9}       @ restore state from stack and leave subroutime


	swi 0x123456

.data
b:	.word 0xAA
	.word 0xBB @ 0x00 00 00 BB
	.word 0xCC

a:	.byte 4 @.word 0x04030201
	.byte 3
	.byte 2
	.byte 1
	.word 0xFF @ 0x000000FF
	.word 0xEE

c:	.word 0x12
	.word 0x34
	.word 0x56
ptr_a:	.word a
