.data
a:	.byte 4
	.byte 3
	.byte 2
	.byte 1
	.word 0xFF
	.word 0xEE
.global main
.text
main:
	mov r2,#4
	ldr r0,ptr_a
loop:	ldrb r1,[r0],#1
	bl decr
	bne loop
	bl deux
   
decr:	subs r2,r2,#1
	mov pc,lr

deux:	
	ldr r1,[r0,#-4]!
	ldmia r0!, {r5,r6,r7}
	swi 0x123456
ptr_a:	.word a
