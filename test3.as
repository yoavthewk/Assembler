; file ps.as                                   
MAIN:       add r3, LIST[r11]
            mov r3, r15
            not r7
            jmp         MAIN
            jsr   MAIN[r10]
            red   LOOP[r12]
LOOP:       prn #48
            macro m1
                inc r6            
                mov r3, W
            endm
            lea STR, r6
            m1
            sub r1, r4
            m1
            bne END
            m1
            cmp val1, #-6
            m1
            bne END[r15]
            m1
            dec K
            rts
.entry MAIN
            sub LOOP[r10] ,r14
 
END: stop
STR:        .string "abcd**************************"
LIST:       .data +6, -9
            .data -100
K:          .data 31
.extern val1
.entry LIST
 
.entry K
 
.extern W