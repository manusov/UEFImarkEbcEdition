;--- EBC Instruction: CALL64EXA {@}R1, {Immed32|Index32} -------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL64EXA is used to jump to external code (either native or EBC), which  ;
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

MACRO CALL64EXA op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 043h
else
DB 0C3h
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
;--- Conditional bytes 2-9 = 64-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DQ op1
else
end if
end if

; Reserved place for Constant+Native addressing

}

