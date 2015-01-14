.global main
.text
main:

@@@@@@@@@@@@@@@@@@ Test MVN @@@@@@@@@@@@@@@@@@

	MOV 	r1, #15			@ r0 = 1111
	MOV 	r2, #0x9		@ r1 = 1001
	MVNS 	r3, #15			@ r3 = FFFFFFF0 -> N = 1
	MVNS	r5, #0xFFFFFFFF		@ r5 = 0 -> Z = 1

@@@@@@@@@@@@@@@@@@ Test MRS @@@@@@@@@@@@@@@@@@

	MRS	r4, cpsr		@ r4 = CPSR
	MRS 	r6, spsr		@ r6 = SPSR

	swi 	0x123456	
