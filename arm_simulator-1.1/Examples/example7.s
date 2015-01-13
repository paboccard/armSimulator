.global main
.text
main:

	mov r0, #15			@ r0 = 1111
	mov r1, #0x9			@ r1 = 1001
	
	mov r2, #1
	mov r3, #3

@@@@@@@@@@@@ test CMN @@@@@@@@@@@@@@@@@



	swi 0x123456
