.entry LIST
.extern W

MAIN: add r3, LIST
LOOP: prn #48
        macro m1
        inc r6
        mov r3, W
        endm
        lea STR, r6
        m1
        sub r1, r4
        bne END
        hello there I'm so fucking happy


