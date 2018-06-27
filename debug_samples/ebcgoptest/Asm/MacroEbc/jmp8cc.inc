;--- EBC Instruction: JMP8CC Immed8 ----------------------------------------;
;                                                                           ;
; Conditionally jump to a relative offset and continue execution,           ;
; condition is Flags.C=0.                                                   ;
; The offset is a signed one-byte offset specified in the number of words.  ;
; The offset is relative to the start of the following instruction.         ;
;                                                                           ;
; If Flags.C=0                                                              ;
; IP <= IP + SizeOfThisInstruction + Operand 1 * 2 (target instr. addr.)    ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP8CC op1
{
;--- Byte 0 = Operation code ---
DB 082h
;--- Byte 1 = Scaled offset ---
x1=(op1-@f)/2
DB x1
@@:
}

