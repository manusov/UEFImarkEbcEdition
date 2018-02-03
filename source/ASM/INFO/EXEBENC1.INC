;--- Execute benchmarks: GOP.BLT.Fill video benchmarks ---------------------;
; Video benchmarks pass#1:                                                  ;
; Operation = Fill video memory with constant pattern.                      ;
; Method    = Protocol=GOP, Function=BLT, Operation=Fill.                   ;
; Visualized result is built monotonous rectangles.                         ;
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
;          Bit Info_Bitmap.24 = validity flag for benchmarks result         ;
;          Benchmarks results stored at Scratch_Pad multifunctional buffer, ;
;          6 subroutines use 128 bytes at R1 + [0A80h-0AFFh], this store:   ;
;          QWORD R1+0A80h = Time per 1 frame, femtoseconds (10^-15 sec)     ;
;          QWORD R1+0A88h = Time per 1 kilobyte, femtoseconds (10^-15 sec)  ;
;                                                                           ;
;          Transit variable (unified for all passes #1-6):                  ;
;          QWORD R1+0AE0h = CAP timer value at benchmark start, clocks      ;
;---------------------------------------------------------------------------;

Execute_Benchmarks_1:
;--- Save input R3 = Text strings destination ---
		PUSH64		R3

;---------- Get timer and draw without measurement for synchronization -----;

		CALL32		Read_CAP_Timer		; Empty read timer
		JMP8CS		EB1_1			; Go if CAP timer error

		XOR64		R2,R2			; R2 = Base X
		XOR64		R3,R3			; R3 = Base Y
		MOVWW		R4,@R1,0,_Video_Xsize	; R4 = Size X
		MOVWW		R5,@R1,0,_Video_Ysize	; R5 = Size Y
		XOR64		R6,R6			; R6 = Color, 0=black
		CALL32		GOP_BLT_Fill		; Return status at R7
		JMP8CS		EB1_1			; Go if error
		
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
		XOR64		R2,R2			; R2 = Base X
		XOR64		R3,R3			; R3 = Base Y
		MOVWW		R4,@R1,0,_Video_Xsize	; R4 = Size X
		MOVWW		R5,@R1,0,_Video_Ysize	; R5 = Size Y
		XOR64		R6,R6			; R6 = Color and counter
;--- Start cycle#1 for target routine ---
EB1_2:		CALL32		GOP_BLT_Fill		; Return status at R7
		JMP8CS		EB1_1			; Go if error
;--- Make cycle for BLUE, 256 cycles 00000000h-000000FFh ---
		MOVSNW		R6,R6,00000001h		; Add 1 to BLUE color field
		CMPI32WUGTE	R6,00000100h
		JMP8CC		EB1_2			; Go cycle if not overflow color
;--- Make cycle for GREEN, 256 cycles 00000000h-0000FF00h ---
		MOVIQQ		R7,00FFFF00h
		AND64		R6,R7
		MOVSNW		R6,R6,00000100h		; Add 1 to GREEN color field
		CMPI32DUGTE	R6,00010000h
		JMP8CC		EB1_2			; Go cycle if not overflow color
;--- Make cycle for RED, 256 cycles 00000000h-00FF0000h ---
		MOVIQQ		R7,00FF0000h
		AND64		R6,R7
		MOVSND		R6,R6,00010000h		; Add 1 to RED color field
		CMPI32DUGTE	R6,01000000h
		JMP8CC		EB1_2			; Go cycle if not overflow color
;--- Initializing cycle#2 ---
		XOR64		R6,R6
;--- Start cycle#2 for target routine ---
EB1_3:		CALL32		GOP_BLT_Fill		; Return status at R7
		JMP8CS		EB1_1			; Go if error
;--- Make cycle for WHITE, 256 cycles 00000000h-00FFFFFFh ---
		MOVSND		R6,R6,00010101h		; Add 1 to RED, GREEN, BLUE colors fields
		CMPI32DUGTE	R6,01000000h
		JMP8CC		EB1_3			; Go cycle if not overflow color
;--- Get stop time ---
		CALL32		Read_CAP_Timer		; Return: R5=Current CLK, R6=Fs per CLK
		JMP8CS		EB1_1			; Go if CAP timer error

;---------- End of critical timings, total 1024 frames done ----------------;

;--- Assign: Status entry address for this test pass, mark bit number  ---

		MOVSNW		R2,R1,0A80h		; R2 = Status entry address for this test
		MOVIQW		R3,56			; R3 = Bit number, 56=Info_Bitmap.24=Benchmark 1 valid flag

;--- Calculate target routine execution time in picoseconds ---
; This point used for some other subroutines
;---
EB1_4:		SUB64		R5,@R1,0,0AE0h		;  R5 = Time per 1024 frames, CAP timer clocks
		MUL64		R5,R6			;  R5 = Time per 1024 frames, femtoseconds
;--- Calculate time per 1 frame, femtoseconds ---
		PUSH64		R5
		MOVIQW		R6,1024
		DIVU64		R5,R6			; R5 = Time per 1 frame, femtoseconds
;--- Save time per 1 frame, femtoseconds ---
		MOVQ		@R2,R5
;--- Get Xsize, Ysize, BPP and calculate bytes per 1 frame ---
		MOVWW		R4,@R1,0,_Video_Xsize
		MOVWW		R6,@R1,0,_Video_Ysize
		MULU64		R4,R6
		MOVBW		R6,@R1,0,_Video_BPP
		MOVIBW		R7,8
		DIVU64		R6,R7			; Bits->Bytes
		MULU64		R4,R6			; R4=Bytes per 1 frame
;--- Calculate time per 1 kilobyte, femtoseconds ---
		POP64		R5			; R5=Time per 1024 frames, femtoseconds
		DIVU64		R5,R4			; R5=Time per 1024 bytes, femtoseconds
;--- Save time per 1 kilobyte, femtoseconds ---
		MOVQW		@R2,0,8,R5
;--- Mark results valid ---
		MOVIQW		R6,1			; Bit mask for shifts
		SHL64		R6,R3			; Positionong bit
		CALL32		CUC_3			; Logical OR bitmap with mask
;--- Exit points ---
EB1_1:		POP64		R3
EB1_0:		XOR64		R2,R2
		RET

