;===========================================================================;
;                                                                           ;
;            LIBRARY FOR PICTURES GENERATING BY EBC INSTRUCTIONS.           ;
;                                                                           ;
;===========================================================================;

;--- Prepare color picture for benchmarks show --------------------------;
; This subroutine prepare transit buffer, without visual,                ;
; data can be visualized by subroutine GOP_BLT_Copy                      ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address (not used here)       ;
;          R2 = Pointer to all screen transit buffer                     ;
;          R3 = Screen X size (pixels)                                   ;
;          R4 = Screen Y size (pixels)                                   ;
;          R5 = True color value for base point (X=0, Y=0),              ;
;               units=1/256, use 16 bits per primary color               ;
;               D[63-48] = Not used                                      ;
;               D[47-32] = Red color                                     ;
;               D[31-16] = Green color                                   ;
;               D[15-00] = Blue color                                    ;
;          R6 = True color addend for delta X, same format 16:16:16      ;
;          R7 = True color addend for delta Y, same format 16:16:16      ;
;               Addends must be limited, for addition without carry,     ;
;               otherwise colors corrupted by carry.                     ;
;                                                                        ;
; OUTPUT:  None                                                          ;
;          R1 preserved                                                  ;
;------------------------------------------------------------------------;
Built_Color_Show:
		PUSH64		R1
		PUSH64		R2
;--- Start cycle for Y coordinate ---
BCH_2:		PUSH64		R3			; Save X counter
		PUSH64		R4			; Save Y counter
		PUSH64		R5			; Save color value
		PUSH64		R7			; Save color delta Y
;--- Start cycle for X coordinate --- 
BCH_1:		XOR64		R7,R7			; Clear R7=Color sum
		MOVIQD		R4,0000FF00h		; Mask for Blue[15-8]
		AND64		R4,R5			; Clear other bits
		MOVIQW		R1,8			; Shifts count
		SHR64		R4,R1			; Shift to BLT Blue position
		OR64		R7,R4			; Add Blue color to color sum
		MOVIQQ		R4,00000000FF000000h	; Mask for Green[15-8]
		AND64		R4,R5			; Clear other bits
		MOVIQW		R1,16			; Shifts count
		SHR64		R4,R1			; Shift to BLT Green position
		OR64		R7,R4			; Add Green color to color sum
		MOVIQQ		R4,0000FF0000000000h	; Mask for Red[15-8]
		AND64		R4,R5			; Clear other bits
		MOVIQW		R1,24			; Shifts count
		SHR64		R4,R1			; Shift to BLT Red position
		OR64		R7,R4			; Add Red color to color sum
;--- Store pixel ---
		MOVD		@R2,R7			; Store current pixel
;--- Make X cycle ---
		ADD64		R5,R6			; Add color delta X
		MOVSNW		R2,R2,4			; Advance pixels pointer, 4 bytes per pixel
		MOVSNW		R3,R3,-1		; Decrement X counter
		CMPI64WEQ	R3,0			; Check X counter
		JMP8CC		BCH_1			; Go cycle if X counter not zero
;--- End cycle for X coordinate --- 
		POP64		R7			; Restore color delta Y
		POP64		R5			; Restore color value
		POP64		R4			; Restore Y counter
		POP64		R3			; Restore X counter
		ADD64		R5,R7			; Add color delta Y
		MOVSNW		R4,R4,-1		; Decrement Y counter
		CMPI64WEQ	R4,0			; Check Y counter
		JMP8CC		BCH_2			; Go cycle if Y counter not zero
;--- End cycle for Y coordinate, restore R1 and return --- 
		POP64		R2
		POP64		R1
		RET

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
Draw_Char:
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
DC_3:		MOVQ		R5,@R3			; R5=One line of font
		MOVIDW		R1,80h			; R1=Mask
		PUSH64		R7
;--- Start cycle for 8 pixels horizontal line ---
DC_1:		MOVD		R7,R1			; Prepare mask for font bits
		AND32		R7,R5			; Check font bit
		CMP32EQ		R7,R1
		JMP8CC		DC_2			; Skip if font bit=0
		MOVD		@R2,R4			; Write 32-bit true color pixel
DC_2:		MOVSNW		R2,R2,4			; R2+4, one pixel right
		MOVIQW		R7,1
		SHR32		R1,R7			; Positioning mask to next bit
		CMPI32WEQ	R1,0			; Mask=0 means all bits done
		JMP8CC		DC_1			; Cycle if not done
;--- End cycle for 8 pixels horizontal line ---
		POP64		R7
		ADD64		R2,R6
		MOVSNW		R3,R3,1			; R3+1, modify font pointer
		MOVSNW		R7,R7,1			; R7+1, modify font bytes counter
		CMPI32WEQ	R7,16
		JMP8CC		DC_3			; Cycle for font 16 bytes
;--- Restore registers and return ---
		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		POP64		R1
		RET

;--- Draw string of chars in the graphics mode --------------------------;
; This subroutine for fixed font size 8x16 pixels.                       ;
; This subroutine for "telemetry mode", save original background color.  ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Pointer to all transit buffer position for draw          ;
;          R3 = Pointer to string, absolute 64-bit                       ;
;          R4 = Front color, true color32: Reserved8.Red8.Green8.Blue8   ;
;                                                                        ;
; OUTPUT:  R3 = Pointer to string, modified by string write              ;
;------------------------------------------------------------------------;
Draw_String:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
;--- Prepare parameters ---
		MOVQ		R7,R3			; R7=Pointer to string
		MOVSNW		R3,R1,_Font_8x16	; R3=Pointer to font table
		MOVWW		R6,@R1,0,_Video_Xdelta	; R6 = Delta Y, bytes per string
		MOVIQW		R5,32			; 32 bytes added by char write
		SUB64		R6,R5			; R6 = Full Addend - 32
;--- Start cycle for string write ---
DS_0:		MOVB		R5,@R7			; R5=ASCII Char
		MOVSNW		R7,R7,1			; R7+1, modify string pointer
		CMPI32WULTE	R5,0Fh
		JMP8CS		DS_1			; End string if Char<0Fh
		CALL32		Draw_Char		; Draw this char
		MOVSNW		R2,R2,32		; R2+32, 8 pixels * 4 bytes per pixel
		JMP8		DS_0			; Cycle for string write
;--- Set output parameter, restore registers and return ---
DS_1:		MOVQ		R3,R7
		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R2
		RET
