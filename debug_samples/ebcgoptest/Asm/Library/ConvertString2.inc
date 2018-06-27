;--- Convert string from UNICODE16 to ASCII -----------------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to nul-terminated source string (flat)           ;
;          R3 = Pointer destination string (flat)                        ;
;          Source string is UNICODE (2 bytes per char)                   ;
;          Destination string is ASCII (1 byte per char)                 ;
;                                                                        ;
; OUTPUT:  R2 = Modified by read source string                           ;
;          R3 = Modified by write destination string                     ;
;------------------------------------------------------------------------;

EBC_Convert_String_2:
		PUSH64		R4
EBC_CS2_2:	MOVW		R4,@R2
		CMPI64WEQ	R4,0
		JMP8CS		EBC_CS2_0	; Go if end of string
		CMPI64WEQ	R4,0Ah
		JMP8CS		EBC_CS2_1	; Go if char=Line Feed
		CMPI64WEQ	R4,0Dh
		JMP8CS		EBC_CS2_1	; Go if char=Carriage Return
		CMPI64WLTE	R4,' '
		JMP8CS		EBC_CS2_1	; Go if legal char
		CMPI64WLTE	R4,'}'
		JMP8CS		EBC_CS2_1	; Go if legal char
		MOVIQW		R4,'.'
EBC_CS2_1:	MOVB		@R3,R4
		MOVSNW		R2,R2,2
		MOVSNW		R3,R3,1
		JMP8		EBC_CS2_2	; Cycle for all string
EBC_CS2_0:	MOVW		@R3,R4		; Write terminator=0
		POP64		R4
		RET

