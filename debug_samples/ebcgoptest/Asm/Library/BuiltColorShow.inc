;--- Prepare color picture for benchmarks show --------------------------;
; This subroutine prepare transit buffer, without visual,                ;
; data can be visualized by subroutine EBC_GOP_BLT_Copy                  ;
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
EBC_Built_Color_Show:
		PUSH64		R1
		PUSH64		R2
;--- Start cycle for Y coordinate ---
EBC_CH_2:	PUSH64		R3			; Save X counter
		PUSH64		R4			; Save Y counter
		PUSH64		R5			; Save color value
		PUSH64		R7			; Save color delta Y
;--- Start cycle for X coordinate --- 
EBC_CH_1:	XOR64		R7,R7			; Clear R7=Color sum
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
		JMP8CC		EBC_CH_1		; Go cycle if X counter not zero
;--- End cycle for X coordinate --- 
		POP64		R7			; Restore color delta Y
		POP64		R5			; Restore color value
		POP64		R4			; Restore Y counter
		POP64		R3			; Restore X counter
		ADD64		R5,R7			; Add color delta Y
		MOVSNW		R4,R4,-1		; Decrement Y counter
		CMPI64WEQ	R4,0			; Check Y counter
		JMP8CC		EBC_CH_2		; Go cycle if Y counter not zero
;--- End cycle for Y coordinate, restore R1 and return --- 
		POP64		R2
		POP64		R1
		RET

