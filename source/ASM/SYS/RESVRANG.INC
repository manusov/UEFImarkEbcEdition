;--- Restore routine handler: Video memory MTRR settings -------------------;
; This subroutine conditionally run by _Sys_Bitmap mechanism, only if       ;
; MTRR supported and re-programmed.                                         ;
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
;---------------------------------------------------------------------------;

Restore_Video_Range:
		MOVSNW		R7,R1,0,_MTRR_Area
		PUSHN		R7				; Parm#1 = Data area base address
		MOVINW		R7,1,0
		CMPI32WEQ	R7,4
		MOVIQW		R7,_IA32_Restore_MTRR		; This pointer for IA32 (native width=4)
		JMP8CS		RVR_0
		MOVIQW		R7,_x64_Restore_MTRR		; This pointer for x64 (native width=8)
RVR_0:		ADD64		R7,R1
		CALL32EXA	R7
		POPN		R7				; p#1
		XOR64		R7,R7				; Status=0
		RET

