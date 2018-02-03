;--- Initialization routine handler: Detect UEFI shell ---------------------;
; Use function Locate_Protocol for EFI Shell Protocol,                      ;
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
;                QWORD  EFI_Shell_Protocol , pointer to protocol interface  ;
;
;               *** UNDER CONSTRUCTION ***
;
;---------------------------------------------------------------------------;

Check_Shell_Configuration:
;--- Detect protocol ---
		MOVIQW		R2,_GuidShell
		CALL32		Locate_Protocol
		JMP8CS		CSC_0			; Skip if not found
		MOVQW		@R1,0,_Shell_Protocol,R2
;--- Get info ---

		;
		; UNDER CONSTRUCTION
		;

;--- Return with no errors, because this protocol absence is non-fatal ---
CSC_0:		JMP8		CUC_0