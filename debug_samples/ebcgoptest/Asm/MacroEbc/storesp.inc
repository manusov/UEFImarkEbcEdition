;--- EBC Instruction: STORESP R1 , [IP|Flags] ------------------------------;
;                                                                           ;
; This instruction transfers the contents of a dedicated register to a      ;
; general-purpose register. The dedicated register is specified by the      ;
; the index: 0=Flags, 1=IP, 2-7=Reserved.                                   ;
;                                                                           ;
; Operand 1 <= Special Reg                                                  ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO STORESP op1, op2
{
;--- Byte 0 = Operation code ---
DB 02Ah
;--- Byte 1 = Two operands addressing ---
if op2 eq Flags
x1=0
else
x1=1
end if
if op1 in < R0 >
x2=0
end if
if op1 in < R1 >
x2=1
end if
if op1 in < R2 >
x2=2
end if
if op1 in < R3 >
x2=3
end if
if op1 in < R4 >
x2=4
end if
if op1 in < R5 > 
x2=5
end if
if op1 in < R6 >
x2=6
end if
if op1 in < R7 >
x2=7
end if
DB x1 SHL 4 + x2
}

