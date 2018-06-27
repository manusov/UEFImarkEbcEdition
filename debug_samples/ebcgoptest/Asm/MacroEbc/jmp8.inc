;--- EBC Instruction: JMP8 Immed8 ------------------------------------------;
;                                                                           ;
; Unconditionally jump to a relative offset and continue execution.         ;
; The offset is a signed one-byte offset specified in the number of words.  ;
; The offset is relative to the start of the following instruction.         ;
;                                                                           ;
; IP <= IP + SizeOfThisInstruction + Operand 1 * 2 (target instr. addr.)    ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP8 op1
{
;--- Byte 0 = Operation code ---
DB 002h
;--- Byte 1 = Scaled offset ---
x1=(op1-@f)/2
DB x1
@@:
}

