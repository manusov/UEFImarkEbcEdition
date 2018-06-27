;--- EBC Instruction: CALL64A Immed64 --------------------------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL64A is used to jump directly to EBC code within a given application   ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= Operand 1 (target instruction addr.)                              ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL64A op1
{
;--- Byte 0 = Operation code ---
DB 0C3h
;--- Byte 1 = Mode select ---
DB 000h
;--- Bytes 2-9 = 64-bit immediate ---
DQ op1
}
