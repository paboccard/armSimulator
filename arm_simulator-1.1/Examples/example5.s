.global main
.text
main:

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

	swi 	0x123456	
