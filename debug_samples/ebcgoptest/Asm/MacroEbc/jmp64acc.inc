;--- EBC Instruction: JMP64A {@}R1, Immed64 --------------------------------;
;                                                                           ;
; The JMP64A instruction used to conditionally jump to a absolute           ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=0.                                                   ;
;                                                                           ;
; If Flags.C=0                                                              ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP64ACC op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 041h
else
DB 0C1h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 080h + x2 SHL 3 + x1
;--- Conditional bytes 2-9 = 64-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DQ op1
else
end if
end if
; Reserved for N+C index.
}

