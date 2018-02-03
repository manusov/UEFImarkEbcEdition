;--- Execute benchmarks: GOP.BLT.Copy video benchmarks ---------------------;
; Video benchmarks pass#2:                                                  ;
; Operation = Copy from system memory to video memory.                      ;
; Method    = Protocol=GOP, Function=BLT, Operation=Copy.                   ;
; Visualized result is draw rectangle sprites with pictures.                ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  Status not used, because it saved at result variables            ;
;          R2 = Current Y position offset addend, here must be 0            ;
;          R3 = Updated destination string pointer, here must be saved      ;
;                                                                           ;
;          Result variables:                                                ;
;          Bit Info_Bitmap.25 = validity flag for benchmarks result         ;
;          Benchmarks results stored at Scratch_Pad multifunctional buffer, ;
;          6 subroutines use 128 bytes at R1 + [0A80h-0AFFh], this store:   ;
;          QWORD R1+0A90h = Time per 1 frame, femtoseconds (10^-15 sec)     ;
;          QWORD R1+0A98h = Time per 1 kilobyte, femtoseconds (10^-15 sec)  ;
;                                                                           ;
;          Transit variable (unified for all passes #1-6):                  ;
;          QWORD R1+0AE0h = CAP timer value at benchmark start, clocks      ;
;---------------------------------------------------------------------------;

Execute_Benchmarks_2:

;--- Save input R3 = Text strings destination ---
		PUSH64		R3

;---------- Get timer and draw without measurement for synchronization -----;

		CALL32		Read_CAP_Timer		; Empty read timer
		JMP8CS		EB1_1			; Go if CAP timer error

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
	;*	OR64		R6,R7			; R6=Green16+Blue16
		SHL64		R7,R5
		OR64		R6,R7			; R6=Red16+Green16+Blue16
		MOVQ		R7,R6			; R6=Color X addend, R7=Color Y addend
		XOR64		R5,R5			; R5=Base Color: Red16:Green16:Blue16
		CALL32		Built_Color_Show	; Built block in the transit buffer

		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVWW		R3,@R1,0,_Video_Xdelta	; R3 = Delta Y for video mode, bytes per string
		MOVIQW		R4,8			; Vertical offset = 8 pixels
		MULU64		R3,R4
		ADD64		R2,R3,8*4		; Horizontal offset = 8 pixels, required *4 BPP
		ADD64		R2,R4				; R2=Destination position
		MOVSNW		R3,R1,_String_Benchmarking	; R3=Source pointer
		MOVIQD		R4,00F0F000h			; R4=Color
		CALL32		Draw_String

		CALL32		Read_CAP_Timer		; Empty read timer
		JMP8CS		EB1_1			; Go if CAP timer error

;---------- Start of critical timings, total 1024 frames -------------------;

;--- Get and save start time ---
; For BLT timings precision, this step optimal location after set parameters,
; but this step corrupt parameters and must be located before set.
; Save-restore parameters don't used because it also take time.
;---
		CALL32		Read_CAP_Timer		; Return: R5=Current CLK, R6=Fs per CLK
		JMP8CS		EB1_1			; Go if CAP timer error
		MOVQW		@R1,0,0AE0h,R5
;--- Set parameters ---
		XOR32		R2,R2
;--- Start cycle for target routine ---
EB2_1:		PUSH64		R2
		XOR64		R2,R2			; R2 = Start X = 0
		XOR64		R3,R3			; R3 = Start Y = 0
		MOVWW		R4,@R1,0,_Video_Xsize	; R4 = End X = Screen Xsize
		MOVWW		R5,@R1,0,_Video_Ysize	; R5 = End Y = Screen Ysize
		MOVQW		R6,@R1,0,_Primary_Memory_Base	; R6 = Transit buffer pointer
		MOVWW		R7,@R1,0,_Video_Xdelta	; R7 = Delta Y, bytes per string
		CALL32		GOP_BLT_Copy
		POP64		R2
		JMP8CS		EB1_1			; Go if error
;--- Make cycle ---
		MOVSNW		R2,R2,1			; R2+1
		CMPI32WUGTE	R2,1024
		JMP8CC		EB2_1			; Go cycle for 1024 iterations
;--- Get stop time ---
		CALL32		Read_CAP_Timer		; Return: R5=Current CLK, R6=Fs per CLK
		JMP8CS		EB1_1			; Go if CAP timer error

;---------- End of critical timings, total 1024 frames done ----------------;

;--- Assign: Status entry address for this test pass, mark bit number  ---

		MOVSNW		R2,R1,0A90h		; R2 = Status entry address for this test
		MOVIQW		R3,57			; R3 = Bit number, 57=Info_Bitmap.24=Benchmark 2 valid flag

;--- Go to calculate, save results and exit ---
		JMP8		EB1_4

