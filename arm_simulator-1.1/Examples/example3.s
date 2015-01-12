.global main
.text
main:

	mov r0, #15			@ r0 = 1111
	mov r1, #9			@ r1 = 1001
	
	mov r2, #1
	mov r3, #3
	and r4, r2, r3			@r4 = 1
	ands r5, r4, #2			@r5 = 0 maj Z 

	eor r7, r0, r1			@r7 = 6
	eor r8, r0, #12			@ 12 1100, r8 = 0011
	eors r8, r8, r8         @ r8 = 0  maj Z

@@@@@@@@@@@@ test sub @@@@@@@@@@@@@@@@@
	
	subs r10, r1, r0		@ r10 = -6 maj N
	sub r11, r0, #5			@ r11 = 10
	subs r11, r11, #10		@ Z = 1
	mov r7, #0xF7000000
	mov r8, #0x9F000000
	subs r9, r7, r8			@ V -> 1   C -> 1


	

	rsb r4, r1, r0			@ r4 = 6
	rsb r5, r1, #10			@ r5 = 1
	
	
@@@@@@@@@@@@@ test add @@@@@@@@@@@@@@@@
	add r7, r0, r1			@ r7 = 24
	add r8, r0, #3			@ r8 = 18
	adds r9, r1, #-9		@ r9 = 0    Z = 1
	adds r9, r9, #-1		@ r9 = 0    N = 1
	adds r9, r2, #0x7FFFFFFF    @ maj V = 1
	mov r3, #0xC0000000
	adds r9, r3, r3			@ C = V = 1



@@@@@@@@@@@@@@ shifter test qvec add @@@@@@@@@@@

@@ asr 
	add r7, r2, r0 ASR #1     @ r7 = 8
	add r8, r2, r0 ASR r2	  @ r8 = 8     c -> 0
	mov r7, #8F000000
	mov r7, r7 ASR #1         @ r7 = -947912704     C -> 1

@@ lsr 
	add r7, r2, r0 LSR #1     @ r7 = 8
	add r8, r2, r0 LSR r2	  @ r8 = 8     c -> 0
	mov r7, #80000000
	mov r7, r7 ASR #1         @ r7 = 1073741824      C -> 

@@ lsl
	mov r7, r2 LSL #1         @ r7 = 2
	mov r8, r2 LSL r2  		  @ r8 = 2



	
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
