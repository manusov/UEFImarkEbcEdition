;--- Restore routine handler: Restore original video mode by GOP -----------;
; Use function GOP.SetMode, if OK, get and save info.                       ;
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

Restore_Video_Mode:


;--- Corrections at v0.20 ---

	;(!)	MOVIQW		R3,0				; R3=Mode number
		MOVDW		R3,@R1,0,_Firmware_Video_Mode	; Get variable used when save video mode	

;----------------------------

		MOVQW		R2,@R1,0,_GOP_Protocol

; old here PUSH64/POP64

		PUSHN		R3		; Added at v0.30, shift stack before EBC-to-native call, under debug

		PUSHN		R3		; Parm#2=Mode number
		PUSHN		R2		; Parm#1=Protocol pointer
		CALL32EXA	@R2,1,0		; GopTable.ModeSet
		POPN		R2
		POPN		R3

		POPN		R3		; Added at v0.30, shift stack before EBC-to-native call, under debug

		CALL32		Output_Clear
		RET
