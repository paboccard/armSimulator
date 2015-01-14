.global main
.text
main:

	mov r0, #15			@ r0 = 1111
	mov r1, #0x9			@ r1 = 1001
	
	mov r2, #1
	mov r3, #3

@@@@@@@@@@@@ test CMN @@@@@@@@@@@@@@@@@

	cmn r1, #0			@ C = 0
	cmn r1, #-9			@ r9 = 0    Z = 1
	add r10, r1, #-9
	mov r9, #0
	cmn r9, #-1			@ r9 = 0    N = 1   C = 1
	mov r9, #0x80000000    
	cmn r9, r9			@ V -> 1   C -> 1   Z -> 1
	mov r9, #0x40000000   
	cmn r9, r9			@ V -> 1   C -> 1
 


	swi 0x123456
