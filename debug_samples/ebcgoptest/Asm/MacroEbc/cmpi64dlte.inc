;--- EBC Instruction: CMPI64DLTE {@}R1, {Index16}, Immed32 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DLTE is signed compare 64-bit values for LEAST OR EQUAL condition,  ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64DLTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0EEh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
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
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}

