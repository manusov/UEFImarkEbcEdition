;--- EBC Instruction: CALL32EXA {@}R1, {Immed32|Index32} -------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL32EX is used to jump to external code (either native or EBC), which   ;
; requires thunking.                                                        ;
; Yet macro supported for native only.                                      ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= Operand 1 (target instruction addr.)                              ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL32EXA op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 003h
else
DB 083h
end if
;--- Byte 1 = Operand addressing, mode select ---
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
DB 020h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1
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
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
}

