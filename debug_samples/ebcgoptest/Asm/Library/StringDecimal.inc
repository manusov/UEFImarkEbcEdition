;--- Output 32-bit unsigned decimal number as ASCII text string ---------;
;                                                                        ;
; INPUT:   R2 = Pointer to destination string                            ;
;          R3 = Number, valid bits 0-31                                  ;
;          R4 = Template size, chars, 0 means no template                ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

EBC_String_Decimal32:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
;--- Prepare start parameters ---
		EXTNDD64	R3,R3
		MOVIQQ		R5,1000000000
;--- Generate current digit ---
EBC_SD_0:	PUSH64		R3
		DIVU64		R3,R5
		MOVIQW		R6,0Fh
		AND64		R3,R6
		CMPI64WGTE	R4,10
		JMP8CS		EBC_SD_1
		CMPI64WEQ	R5,1
		JMP8CS		EBC_SD_1
		CMPI64WEQ	R3,0
		JMP8CS		EBC_SD_2
;--- Write ASCII char ---
EBC_SD_1:	MOVSNW		R3,R3,30h
		MOVB		@R2,R3
		MOVSNW		R2,R2,1		; R2+1
		MOVIQW		R4,11
EBC_SD_2:	POP64		R3
;--- Generate modulus for next digit ---
		MODU64		R3,R5
;--- Generate divisor for next digit ---
		MOVIQW		R6,10
		DIVU64		R5,R6
;--- Cycle for entire number ---
		MOVSNW		R4,R4,1		; R4+1
		CMPI64WEQ	R5,0
		JMP8CC		EBC_SD_0
;--- Restore registers and exit ---
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		RET

