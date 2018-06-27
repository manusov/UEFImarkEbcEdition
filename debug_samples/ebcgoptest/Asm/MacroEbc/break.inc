;--- EBC Instruction: BREAK break_code -------------------------------------;
;                                                                           ;
; BREAK 0 = Exception when program likely executed from cleared memory      ;
; BREAK 1 = Get virtual machine version in the R7                           ;
; BREAK 2 = Reserved                                                        ;
; BREAK 3 = Debug breakpoint                                                ;
; BREAK 4 = Reserved for system call                                        ;
; BREAK 5 = Create thunk                                                    ;
; BREAK 6 = Set compiler version                                            ;
; BREAK 7-255 = Reserved                                                    ;
;                                                                           ;
; op1 = Break code                                                          ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO BREAK op1
{
;--- Byte 0 = Operation code ---
DB 000h
;--- Byte 1 = Break code ---
DB op1
}


