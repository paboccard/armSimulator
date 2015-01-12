.global main
.text
main:

	mov r0, #15
	mov r1, #-9

	add r8, r1, #9

@	and r4, r0, r1
@	and r5, r0, #3

@	eor r7, r0, r1
@	eor r8, r0, #12
	
@	sub r10, r0, r1
@	sub r11, r0, #5

@	rsb r4, r1, r0
@	rsb r5, r1, #10

@	add r7, r0, r1
@	add r8, r0, #3
@	add r9, r1, #-9
@test avec des valeurs dépassant les 32 bits


@	adc r10, r0, r1
@	adc r11, r0, #3

@    swi 0x123456
	
@	sbc r4, r0, r1
@	sbc r5, r1, #5

@	rsc r7, r1, r0
@	rsc r8, r1, #10

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
