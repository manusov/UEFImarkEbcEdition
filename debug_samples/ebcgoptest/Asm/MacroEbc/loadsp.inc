;--- EBC Instruction: LOADSP [special reg] , R2 ----------------------------;
;                                                                           ;
; This instruction loads a VM dedicated register with the contents of a VM  ;
; general-purpose register R0-R7. The dedicated register is specified by    ;
; the index: 0=Flags, 1=IP, 2-7=Reserved.                                   ;
;                                                                           ;
; Special Reg <= Operand 2                                                  ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO LOADSP op1, op2
{
;--- Byte 0 = Operation code ---
DB 029h
;--- Byte 1 = Two operands addressing ---
if op1 eq Flags
x1=0
else
x1=1
end if
if op2 in < R0 >
x2=0
end if
if op2 in < R1 >
x2=1
end if
if op2 in < R2 >
x2=2
end if
if op2 in < R3 >
x2=3
end if
if op2 in < R4 >
x2=4
end if
if op2 in < R5 > 
x2=5
end if
if op2 in < R6 >
x2=6
end if
if op2 in < R7 >
x2=7
end if
DB x2 SHL 4 + x1
}

