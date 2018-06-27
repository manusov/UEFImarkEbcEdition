;--- Convert string from ASCII to UNICODE16 -----------------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to nul-terminated source string (flat)           ;
;          R3 = Pointer destination string (flat)                        ;
;          Source string is ASCII (1 byte per char)                      ;
;          Destination string is UNICODE (2 bytes per char)              ;
;                                                                        ;
; OUTPUT:  R2 = Modified by read source string                           ;
;          R3 = Modified by write destination string                     ;
;------------------------------------------------------------------------;

EBC_Convert_String_1:
		PUSH64		R4
EBC_CS1_2:	MOVB		R4,@R2
		CMPI64WEQ	R4,0
		JMP8CS		EBC_CS1_0	; Go if end of string
		CMPI64WEQ	R4,0Ah
		JMP8CS		EBC_CS1_1	; Go if char=Line Feed
		CMPI64WEQ	R4,0Dh
		JMP8CS		EBC_CS1_1	; Go if char=Carriage Return
		CMPI64WLTE	R4,' '
		JMP8CS		EBC_CS1_1	; Go if legal char
		CMPI64WLTE	R4,'}'
		JMP8CS		EBC_CS1_1	; Go if legal char
		MOVIQW		R4,'.'
EBC_CS1_1:	MOVW		@R3,R4
		MOVSNW		R2,R2,1
		MOVSNW		R3,R3,2
		JMP8		EBC_CS1_2	; Cycle for all string
EBC_CS1_0:	MOVW		@R3,R4		; Write terminator=0
		POP64		R4
		RET
