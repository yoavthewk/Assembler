.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
END: stop
END: no
STR: .string "abcd"
LIST: .data 6, -9.5
K: .data 31
 lea STR, r6
 inc r6
mov r3, W
 sub r1, r4
 bne END
 cmp val1, #-6
 bne END[r15]
 dec K
.entry MAIN
sub LOOP[r10] ,r14
.data -100
.entry K
.extern val1 