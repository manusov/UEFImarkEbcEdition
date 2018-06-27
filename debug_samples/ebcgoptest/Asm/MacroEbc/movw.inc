;--- EBC Instruction: MOVW {@}R1, {@}R2 ------------------------------------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVW = Move words without indexing.                                       ;
;                                                                           ;
; Operand 1 bits [15-00] <= Operand 2 bits [15-00]                          ;
; Operand 1 bits [63-16] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVW op1, op2
{
;--- Byte 0 = Operation code ---
DB 01Eh
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
}

