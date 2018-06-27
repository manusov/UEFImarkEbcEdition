;--- Draw char in the graphics mode -------------------------------------;
; This subroutine for fixed font size 8x16 pixels.                       ;
; This subroutine for "telemetry mode", save original background color.  ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address (not used here)       ;
;          R2 = Pointer to all transit buffer position for draw          ;
;          R3 = Pointer to font table                                    ;
;          R4 = Front color, true color32: Reserved8.Red8.Green8.Blue8   ;
;          R5 = Char ASCII code                                          ;
;          R6 = Delta Y                                                  ;
;                                                                        ;
; OUTPUT:  None                                                          ;
;          Registers saved                                               ;
;------------------------------------------------------------------------;
EBC_Draw_Char:
;--- Save registers ---
		PUSH64		R1
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
;--- Calculate pointer to font element ---
		MOVIQW		R7,4			; Shift left 4 bits means *16
		SHL64		R5,R7			; *16 because 16 bytes per char
		ADD64		R3,R5			; R5=Pointer to font char element
;--- Prepare cycle for 16 lines ---
		XOR32		R7,R7
;--- Start cycle for 16 lines, prepare cycle for 8 pixels horizontal line ---
EBC_DC_3:	MOVQ		R5,@R3			; R5=One line of font
		MOVIDW		R1,80h			; R1=Mask
		PUSH64		R7
;--- Start cycle for 8 pixels horizontal line ---
EBC_DC_1:	MOVD		R7,R1			; Prepare mask for font bits
		AND32		R7,R5			; Check font bit
		CMP32EQ		R7,R1
		JMP8CC		EBC_DC_2		; Skip if font bit=0
		MOVD		@R2,R4			; Write 32-bit true color pixel
EBC_DC_2:	MOVSNW		R2,R2,4			; R2+4, one pixel right
		MOVIQW		R7,1
		SHR32		R1,R7			; Positioning mask to next bit
		CMPI32WEQ	R1,0			; Mask=0 means all bits done
		JMP8CC		EBC_DC_1		; Cycle if not done
;--- End cycle for 8 pixels horizontal line ---
		POP64		R7
		ADD64		R2,R6
		MOVSNW		R3,R3,1			; R3+1, modify font pointer
		MOVSNW		R7,R7,1			; R7+1, modify font bytes counter
		CMPI32WEQ	R7,16
		JMP8CC		EBC_DC_3		; Cycle for font 16 bytes
;--- Restore registers and return ---
		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		POP64		R1
		RET

