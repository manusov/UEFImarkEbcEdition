;--- EBC Instruction: MULU32 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Perform a unsigned multiply of two operands and store the result back to  ;
; Operand 1. For 32-bit.
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 * Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MULU32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Fh
else
DB 08Fh
end if
;--- Byte 1 = Two operands addressing ---
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
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}

