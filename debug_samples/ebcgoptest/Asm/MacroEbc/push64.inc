;--- EBC Instruction: PUSH64 {@}R1, {Index16|Immed16} ----------------------;
;                                                                           ;
; Adjust the stack pointer R0 and store a 64-bit value on the stack.        ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the Operand 1     ;
; register contents such that Operand 1 = R1+Immed16.                       ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the Operand 1    ;
; value pushed is fetched from [R1+Index16].                                ;
;                                                                           ;
; R0   <= R0 - 8                                                            ;
; [R0] <= Operand 1                                                         ;
;                                                                           ;
; op1 = Operand to push                                                     ;
; op2 = Natural offset or addend to pushed value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO PUSH64 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 06Bh
else
DB 0EBh
end if
;--- Byte 1 = Operand addressing ---
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
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}

