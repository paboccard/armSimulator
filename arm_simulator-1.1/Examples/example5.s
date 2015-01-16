.global main
.text

div :
	stmfd sp!, {r2,r3,r4,lr}
	
	mov r2, r1             /* r2 ← r0. We keep D in r2 */
	mov r1, r0             /* r1 ← r0. We keep N in r1 */
	
	mov r0, #0             /* r0 ← 0. Set Q = 0 initially */
	
	b .Lloop_check
.Lloop:
	add r0, r0, #1      /* r0 ← r0 + 1. Q = Q + 1 */
	sub r1, r1, r2      /* r1 ← r1 - r2 */
.Lloop_check:
	cmp r1, r2          /* compute r1 - r2 */
	bhs .Lloop            /* branch if r1 >= r2 (C=0 or Z=1) */
	
	/* r0 already contains Q */
	/* r1 already contains R */

	ldmfd sp!, {r2, r3, r4, pc}

main:

	mov r0, #10
	mov r1, #2
	mov r2, #4
	mov r3, #5
	mov r4, #6
	cmp r1, #0
bl div


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@ Division Euclidienne @@@@@@@@@@@@@@@@
	    @  r0 contains N and Ni
	    @  r1 contains D
	    @  r2 contains Q
	    @  r3 will contain Di

	mov r0, #10
	mov r1, #2
	
	mov r3, r1                   /* r3 ← r1 */
	cmp r3, r0, LSR #1           /* update cpsr with r3 - r0/2 */
.Lloop2:
	movls r3, r3, LSL #1       /* if r3 <= 2*r0 (C=0 or Z=1) then r3 ← r3*2 */
	cmp r3, r0, LSR #1         /* update cpsr with r3 - (r0/2) */
	bls .Lloop2                /* branch to .Lloop2 if r3 <= 2*r0 (C=0 or Z=1) */

	mov r2, #0                   /* r2 ← 0 */

.Lloop3:
	cmp r0, r3                 /* update cpsr with r0 - r3 */
	subhs r0, r0, r3           /* if r0 >= r3 (C=1) then r0 ← r0 - r3 */
	adc r2, r2, r2             /* r2 ← r2 + r2 + C.
	                                    Note that if r0 >= r3 then C=1, C=0 otherwise */

	mov r3, r3, LSR #1         /* r3 ← r3/2 */
	cmp r3, r1                 /* update cpsr with r3 - r1 */
	bhs .Lloop3                /* if r3 >= r1 branch to .Lloop3 */

	@FIN @ r3 = 5, r2 = 0


	
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
