;--- EBC Instruction: MOVIBQ {@}R1 {Index16}, Immed64 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 8 bits, Source width = 64 bits,         ;
; form not used, because destination width < source width.                  ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIBQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DQ op2
else
DQ op4
end if
}

