;--- Output text string -------------------------------------------------;
; Use conversion ASCII->UNICODE16 and Simple Text Output Protocol,       ;
; string must be in ASCII.                                               ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, use buffer on it     ;
;          R2 = Pointer to string                                        ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;

EBC_String_Write:
		PUSH64		R2
		PUSH64		R3
		MOVQ		R3,R1			; R3=Destination
		CALL32		EBC_Convert_String_1	; Convert ASCII-UNICODE16
		MOVQ		R2,R1
		CALL32		EBC_Output_String	; Write UNICODE16
		POP64		R3
		POP64		R2
		RET

