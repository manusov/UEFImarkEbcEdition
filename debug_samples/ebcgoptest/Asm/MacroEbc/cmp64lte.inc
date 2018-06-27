;--- EBC Instruction: CMP64LTE R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP64LTE is signed compare 64-bit values for LEAST OR EQUAL condition.    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP64LTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 046h
else
DB 0C6h
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
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
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
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}

