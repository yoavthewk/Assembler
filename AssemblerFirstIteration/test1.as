.entry LIST
.extern W

add r3, LIST
prn #48
        macro m1
        inc r6
        mov r3, W
        endm
        lea STR, r6
        m1
        sub r1, r4
        bne END
        hello there I'm so fucking happy