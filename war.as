; This is a program with error

; 1 - Warning in war.am - label A marked as entry but wasn't defined
.entry A
; 2 - Warning in war.am line 6 - .entry is already declared for label A
.entry A
; 3 - Warning in war.am line 9 - .extern is already declared for label B
.extern B
.extern B

