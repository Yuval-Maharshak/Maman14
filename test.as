MAIN: 	mov S1.1 ,LENGTH
 	add r2,STR
LOOP: 	jmp END
	macro m
		fds
	endmacro
	macro 12m
		inc K
 		mov S1.2 ,r3
	endmacro
 	prn #-5
 	sub r1, r4
 	bne LOOP
 	h
END: 	hlt
STR: 	.string "abcdef"
LENGTH: .data 6,-9,15
K: 	.data 22
S1: 	.struct 8, "ab"