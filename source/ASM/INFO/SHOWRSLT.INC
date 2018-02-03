;--- Show system information and benchmarks results ------------------------;
; INPUT:   None, but video subsystem and memory control variables used      ;
; OUTPUT:  None                                                             ;
;---------------------------------------------------------------------------;
Show_Results:

		MOVIWW		@R3,0FFFFh		; Sequence terminator 0FFFFh, must be word

;--- Built true color picture in the transit buffer ------------------------;

;*** OPTIMIZING REQUIRED: MAKE SUBROUTINE FOR THIS AND BENCHMARKS ***

		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVWW		R3,@R1,0,_Video_Xsize
		MOVWW		R4,@R1,0,_Video_Ysize
		MOVQ		R6,R3
		ADD64		R6,R4			; R6=Maximum pixel sum X+Y
		MOVIQD		R7,65535
		DIVU64		R7,R6			; R7=Additions per coordinate
		MOVIQW		R5,16			; R5=Shifts counter for R,G,B
		MOVQ		R6,R7			; R6=Blue16
		SHL64		R7,R5
		OR64		R6,R7			; R6=Green16+Blue16
		SHL64		R7,R5
		OR64		R6,R7			; R6=Red16+Green16+Blue16
		MOVQ		R7,R6			; R6=Color X addend, R7=Color Y addend
		XOR64		R5,R5			; R5=Base Color: Red16:Green16:Blue16
		CALL32		Built_Color_Show
;---------- Built program info strings in the transit buffer ---------------;
		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVWW		R3,@R1,0,_Video_Xsize
		MOVIQW		R4,100
		MUL64		R3,R4			; R3=Xsize*100
		ADD64		R3,R4			; R3=Xsize*100+100 (offsets x=100, y=100)
		ADD64		R2,R3
		XOR64		R3,R3			; R3 = Source, offset=0 relative R1
		MOVQ		R3,R1			; R3 = Absolute source pointer
;--- Start sequence cycle ---
SR_0:		MOVW		R4,@R3			; R4 = Addend X for built next string
		CMPI32DEQ	R4,0000FFFFh		; Compare with terminator word
		JMP8CS		SR_1			; Go if end of sequence
		PUSH64		R2
		MOVIQW		R5,2			; R5 = Shifts count for multiply by 4
		SHL64		R4,R5			; Multiply by 4 (4 bytes per pixel)
		ADD64		R2,R4			; Add physical addend X to transit buffer address
		MOVWW		R4,@R3,0,2		; R4 = Addend Y for built next string
		MOVWW		R5,@R1,0,_Video_Xdelta	; R5 = Delta Y for video mode, bytes per string
		MUL64		R4,R5			; Calculate physical addend Y
		ADD64		R2,R4			; Add physical addend Y to transit buffer address
		MOVDW		R4,@R3,0,4		; R4=True color: Reserved8.Red8.Green8.Blue8
		MOVSNW		R3,R3,8			; R3+8, skip addends words(2+2), color dword(4)
		CALL32		Draw_String		; Built string, R3+1 included for terminator byte
		POP64		R2
;--- Make sequence cycle, byte 000h=String terminator, word 0FFFFh=Sequence terminator ---
; Sequence terminator must be word 0FFFFh (not byte 0FFh),
; otherwise X addend = 255 make unexpected termination condition.
;---
		JMP8		SR_0
SR_1:
;---------- Copy from transit buffer to video memory, use GOP.BLT ----------;

;*** OPTIMIZING REQUIRED: MAKE SUBROUTINE FOR THIS AND BENCHMARKS ***

		XOR64		R2,R2			; R2 = Start X = 0
		XOR64		R3,R3			; R3 = Start Y = 0
		MOVWW		R4,@R1,0,_Video_Xsize	; R4 = End X = Screen Xsize
		MOVWW		R5,@R1,0,_Video_Ysize	; R5 = End Y = Screen Ysize
		MOVQW		R6,@R1,0,_Primary_Memory_Base	; R6 = Transit buffer pointer
		MOVWW		R7,@R1,0,_Video_Xdelta	; R7 = Delta Y, bytes per string
;---------- Drawings is visible after this operation -----------------------;
		CALL32		GOP_BLT_Copy

;---------- Return ---------------------------------------------------------;
		RET

