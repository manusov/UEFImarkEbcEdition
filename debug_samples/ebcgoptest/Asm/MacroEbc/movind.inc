;--- EBC Instruction: MOVIND {@}R1 {Index16}, Index32 ----------------------;
;                                                                           ;
; This instruction moves an indexed value of form (+n,+c) to Operand 1.     ;
; The index value is converted from (+n,+c) format to a signed offset.      ;
; This form use 32-bit Operand 2 index data.                                ;
;                                                                           ;
; Operand 1 <= Operand 2 Index value                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIND op1, op2, op3, op4, op5
{

;--- Byte 0 = Operation code ---
DB 0B8h

;--- Byte 1 = First operand addressing and index presence ---
if op5 eq
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

;--- Conditional bytes 2-3 = 16-bit index for first operand addressing ---
if op5 eq
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
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if

;--- Conditional bytes 2-5 = 32-bit index for first operand addressing ---
;--- Conditional bytes 4-7 = 32-bit index for second operand loading ---
if op5 eq

if op2 < 0
x9=-op2
else
x9=op2
end if
if op3 < 0
x10=-op3
else
x10=op3
end if
if op2 = 0
x11=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x11=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x11=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x11=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x11=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x11=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x11=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x11=7
end if
end if
if op2<0
x12=1
else
x12=0
end if
x13=x11*4
DD x12 SHL 31 + x11 SHL 28 + x10 SHL x13 + x9

else

if op4 < 0
x9=-op4
else
x9=op4
end if
if op5 < 0
x10=-op5
else
x10=op5
end if
if op4 = 0
x11=0
else
if op4>0 & op4<16 | op4<0 & op4>-16
x11=1
end if
if op4>15 & op4<256 | op4<-15 & op4>-256
x11=2
end if
if op4>255 & op4<4096 | op4<-255 & op4>-4096
x11=3
end if
if op4>4095 & op4<65536 | op4<-4095 & op4>-65536
x11=4
end if
if op4>65535 & op4<1048576 | op4<-65535 & op4>-1048576
x11=5
end if
if op4>1048575 & op4<16777216 | op4<-1048575 & op4>-16777216
x11=6
end if
if op4>16777215 & op4<268435456 | op4<-16777216 & op4>-268435456
x11=7
end if
end if
if op4<0
x12=1
else
x12=0
end if
x13=x11*4
DD x12 SHL 31 + x11 SHL 28 + x10 SHL x13 + x9

end if

}

