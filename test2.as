mov #-17,           val
cmp r3   , r4
macro Y2
    sub r5, AMIT
    rts
endm
val: .data 4,6,-19,78
LOOP: add r4,YUV
sub YUV[r14] ,r2
lea val , r6
Y2
.entry AMIT
clr MAIN
not r8
inc r15
dec val
MAIN: jmp YUV
COOOOOOOOOOOOOL: bne val[r11]
jsr         COOOOOOOOOOOOOL
red r9
.entry val
YUV: .string "yuval"
prn GGG
rts
                AMIT:  add #67       ,r11
stop
.extern GGG
