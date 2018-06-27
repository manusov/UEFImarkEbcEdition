;--- EBC Instruction: RET --------------------------------------------------;
;                                                                           ;
; Fetches the return address from stack, sets the IP to the value, adjust   ;
; the stack pointer register R0 and continues execution at the return       ;
; address. If the RET is a final return from the EBC driver (application)   ;
; then execution control returns to the caller, which may be EBC or native  ;
; code.                                                                     ;
;                                                                           ;
; IP <= [R0]                                                                ;
; R0 <= R0+16                                                               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO RET
{
DW 00004h
}
