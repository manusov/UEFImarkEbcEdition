;--- Initialization routine handler: Measure CPU clock by TSC --------------;
; For cross-platform support: this routine must be called CONDITIONALLY,    ;
; only if Legacy PC compatible platform detected.                           ;
; For emulating environment support: this routine must be called            ;
; CONDITIONALLY, only if Current Privelege Level (CPL) is 0 (supervisor).   ;
; Measure number of clocks per 1 second.                                    ;
; Measures clock source is TSC (Time Stamp Counter).                        ;
; Seconds source is RTC (Real Time Clock).                                  ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;                                                                           ;
;               Global variables:                                           ;
;               DWORD  Info_Bitmap, bit 7 = TSC clock data valid            ;
;               QWORD  TSC_Period  = TSC timer period, femtoseconds         ;
;---------------------------------------------------------------------------;

Measure_Clock:
;--- Select handler for detected native CPU type ---
		MOVBW		R2,@R1,0,_CPU_Native_Width
		MOVIQQ		R3,IA32_Measure_TSC	; Assign get-handler for IA32
		CMPI32WEQ	R2,4			; Native width is 4 bytes ?
		JMP8CS		MCLK_0			; Go if IA32
		MOVIQQ		R3,x64_Measure_TSC	; Assign get-handler for x64
		CMPI32WEQ	R2,8			; Native width is 8 bytes ?
		JMP8CC		CNC_3			; If not x64, go to R7=0 and RET
;--- Measure TSC period ---
MCLK_0:		PUSH64		R2			; Scratch pad for output parameter
		PUSHN		R0			; Pointer to output parameter
		CALL32EXA	R3			; Call Measure_TSC handler
		POPN		R2			; Remove R0 image
		POP64		R2			; Get output parameter
;--- Calculate results ---
		MOVIQQ		R3,1000000000000000	; femtoseconds per Second
		CMPI64WEQ	R2,0
		JMP8CS		CNC_3			; Go silent error if divisor=0
		DIVU64		R3,R2			; Fs-per-Sec / Clocks-per-Sec = Fs-per-Clock
;--- Exit point with update output ---
		MOVQW		@R1,0,_TSC_Period,R3
		MOVIQQ		R6,08000000000h		; Bit D39=1, this bit is Info_Bitmap.7
		CALL32		CUC_3			; Set bitmap, this also return R7=0
		RET

