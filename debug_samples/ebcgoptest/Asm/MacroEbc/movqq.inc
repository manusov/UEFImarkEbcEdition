;--- EBC Instruction: MOVQQ {@}R1 , {Index64} , {@}R2 , {Index64} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVQQ = Move qwords with 64-bit indexing.                                 ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVQQ op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 028h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
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
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<100h | op2<0 & op2>-100h
x9=1
end if
if op2>0FFh & op2<10000h | op2<-0FFh & op2>-10000h
x9=2
end if
if op2>0FFFFh & op2<1000000h | op2<-0FFFFh & op2>-1000000h
x9=3
end if
if op2>0FFFFFFh & op2<100000000h | op2<-0FFFFFFh & op2>-100000000h
x9=4
end if
if op2>0FFFFFFFFh & op2<10000000000h | op2<-0FFFFFFFFh & op2>-10000000000h
x9=5
end if
if op2>0FFFFFFFFFFh & op2<1000000000000h | op2<-0FFFFFFFFFFh & op2>-1000000000000h
x9=6
end if
if op2>0FFFFFFFFFFFFh & op2<100000000000000h | op2<-0FFFFFFFFFFFFh & op2>-100000000000000h
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*8
DQ x10 SHL 63 + x9 SHL 60 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<100h | op3<0 & op3>-100h
x14=1
end if
if op3>0FFh & op3<10000h | op3<-0FFh & op3>-10000h
x14=2
end if
if op3>0FFFFh & op3<1000000h | op3<-0FFFFh & op3>-1000000h
x14=3
end if
if op3>0FFFFFFh & op3<100000000h | op3<-0FFFFFFh & op3>-100000000h
x14=4
end if
if op3>0FFFFFFFFh & op3<10000000000h | op3<-0FFFFFFFFh & op3>-10000000000h
x14=5
end if
if op3>0FFFFFFFFFFh & op3<1000000000000h | op3<-0FFFFFFFFFFh & op3>-1000000000000h
x14=6
end if
if op3>0FFFFFFFFFFFFh & op3<100000000000000h | op3<-0FFFFFFFFFFFFh & op3>-100000000000000h
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*8
DQ x15 SHL 63 + x14 SHL 60 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<100h | op5<0 & op5>-100h
x14=1
end if
if op5>0FFh & op5<10000h | op5<-0FFh & op5>-10000h
x14=2
end if
if op5>0FFFFh & op5<1000000h | op5<-0FFFFh & op5>-1000000h
x14=3
end if
if op5>0FFFFFFh & op5<100000000h | op5<-0FFFFFFh & op5>-100000000h
x14=4
end if
if op5>0FFFFFFFFh & op5<10000000000h | op5<-0FFFFFFFFh & op5>-10000000000h
x14=5
end if
if op5>0FFFFFFFFFFh & op5<1000000000000h | op5<-0FFFFFFFFFFh & op5>-1000000000000h
x14=6
end if
if op5>0FFFFFFFFFFFFh & op5<100000000000000h | op5<-0FFFFFFFFFFFFh & op5>-100000000000000h
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*8
DQ x15 SHL 63 + x14 SHL 60 + x13 SHL x16 + x12
end if
end if

}

