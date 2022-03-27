; This is a program with errors
 
; 1 - ERROR in err1.am line 4 - label can't be a register
r3: add r3,r15 
; 2 - ERROR in err1.am line 6 - label can't be a command name
add: add r3,r15 
; 3 - ERROR in err1.am line 8 - character "#" is not allowed in label
LAB#EL1: move r5,r6
; 4 - ERROR in err1.am line 10 - after a label must be a space
LABEL1:move r5,r6
; 5 - ERROR in err1.am line 12 - Illegal command "move"
move r1
; 6 - ERROR in err1.am line 14 - 2 operands are needed in command "mov"
mov r1
; 7 - ERROR in err1.am line 16 - extra char "," after second operand
mov r1,r1,
; 8 - ERROR in err1.am line 18 - Can't evaluate opernd(s) in this line
mov r1,,r1
; 9 - ERROR in err1.am line 20 - Expecting a number
mov r1,#r
; 10 - ERROR in err1.am line 22 - immidiate "10000000" exceed 16 bits
mov r1,#10000000
; 11 - ERROR in err1.am line 24 - wrong dest add mode for mov command
mov r1, #2
; 12 - ERROR in err1.am line 26 - wrong source add mode for lea command
lea r3,r3
; 13 - ERROR in err1.am line 28 - 1 operands is needed in command "clr"
clr r1,r2
; 14 - ERROR in err1.am line 31 - label A can't be .extern and .entry together
.entry A
.extern A
; 15 - ERROR in err1.am line 33 - Register must be between 10 to 15
mov r3,A[r1]
; 16 - ERROR in err1.am line 35 - Expecting a number
mov r3,A[rrrr]
; 17 - ERROR in err1.am line 37 - Add Mode Index must have ] after the reg
mov r3,A[r12A]
