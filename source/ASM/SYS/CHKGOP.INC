;--- Initialization routine handler: Detect Graphics Output Protocol -------;
; Use function Locate_Protocol for Graphics Output Protocol,                ;
; if detected, get and save info                                            ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;               Global variables:                                           ;
;                QWORD  GOP_Protocol , pointer to protocol interface        ;
;---------------------------------------------------------------------------;

Check_GOP_Video:
		MOVIQW		R2,_GuidGop
		CALL32		Locate_Protocol
		JMP8CS		CUC_2			; Go fatal error if absent
		MOVQW		@R1,0,_GOP_Protocol,R2
		JMP8		CUC_0			; Go OK if present
