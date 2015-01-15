.data
b:	.word 0xAA
	.word 0xBB
	.word 0xCC

a:	.byte 4 @.word 0x04030201
	.byte 3
	.byte 2
	.byte 1
	.word 0xFF
	.word 0xEE

c:	.word 0x12
	.word 0x34
	.word 0x56
error:	.word 0x404
.global main
.text
main: @un
	mov r0,#0x80000000
	movs r0,r0,asr #32
	beq fin
	mov r0,#5





	ldr r0,ptr_a
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
	ldmdb r0!, {r5-r7}
cmp:	cmp r7,r6
	bgt fin
	blt trois
	ldr r0,ptr_error
	ldr r0,[r0]
	bl fin
trois:	add r6,r6,r7,ror #24
	mov r1,#2000
	strb r6,[r1]
	ldr r7,[r1,#3]
	mov r6,r6,lsr #8
	bl cmp
fin:
	swi 0x123456

ptr_a:	.word a
ptr_error:	.word error
