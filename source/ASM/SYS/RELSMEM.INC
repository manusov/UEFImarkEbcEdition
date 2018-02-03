;--- Restore routine handler: Release memory, allocated by this applic. ----;
; Use EFI memory control API.                                               ;
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

Release_Memory:
		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVQW		R3,@R1,0,_Primary_Memory_Size
		MOVIQW		R7,12
		SHR64		R3,R7		; Bytes->Pages
		CALL32		EFI_Free_Pages
		RET
