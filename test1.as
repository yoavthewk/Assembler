.entry LIST
.extern W

MAIN: 	add r3, LIST
LOOP: 	prn #48
	lea STR, r6
	inc r6
	mov r3, W
	sub r1, r4
	bne END
	cmp val1, #-6
	bne END[r15]
	dec K
.entry MAIN
	sub LOOP[r10], r14
END:    stop
STR:   .string "abcd"
LIST:  .data 6, -9
       .data -100
       .data 58743, 32423, 4234234, 3423, 324234, 432432, 1,2,3,4,5,6,7,8,99

.entry K
K: .data 31
.extern val1
