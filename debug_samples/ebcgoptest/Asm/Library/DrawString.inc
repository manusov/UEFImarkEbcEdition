;--- Draw string of chars in the graphics mode --------------------------;
; This subroutine for fixed font size 8x16 pixels.                       ;
; This subroutine for "telemetry mode", save original background color.  ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address (not used here)       ;
;          R2 = Pointer to all transit buffer position for draw          ;
;          R3 = Pointer to string                                        ;
;          R4 = Front color, true color32: Reserved8.Red8.Green8.Blue8   ;
;                                                                        ;
; OUTPUT:  None                                                          ;
;------------------------------------------------------------------------;
EBC_Draw_String:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
;--- Prepare parameters ---
		MOVQ		R7,R3
		MOVIQQ		R3,Font_8x16		; R3=Pointer to font table
		MOVWW		R6,@R1,0,_Video_Xdelta	; R6 = Delta Y, bytes per string
		MOVIQW		R5,32			; 32 bytes added by char write
		SUB64		R6,R5			; R6 = Full Addend - 32
;--- Start cycle for string write ---
EBC_DS_0:	MOVB		R5,@R7			; R5=ASCII Char
		MOVSNW		R7,R7,1			; R7+1, modify string pointer
		CMPI32WULTE	R5,0Fh
		JMP8CS		EBC_DS_1		; End string if Char<0Fh
		CALL32		EBC_Draw_Char		; Draw this char
		MOVSNW		R2,R2,32		; R2+32, 8 pixels * 4 bytes per pixel
		JMP8		EBC_DS_0		; Cycle for string write
;--- Restore registers and return ---
EBC_DS_1:	POP64		R7
		POP64		R6
		POP64		R5
		POP64		R3
		POP64		R2
		RET