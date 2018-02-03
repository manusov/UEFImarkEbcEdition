;--- Wait user press any key for start messages view -----------------------;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;               Global variables:                                           ;
;                Sys_Bitmap, Start_Pause                                    ;
;---------------------------------------------------------------------------;

Setup_Wait:
;--- Wait for key press ---
		CALL32		Input_Wait_Key		; Wait for key
;--- Analysing input ---
		MOVIQW		R3,16
		SHR32		R2,R3			; Move scan code to bits R2.[7-0]
		MOVIQW		R3,00100000b
		OR32		R2,R3			; Set lower case, D5=1
		CMPI32WEQ	R2,06Eh			; Compare with ASCII code "n"
		JMP8CC		SWAIT_0			; Go skip if not "n"
;--- Update variables if press "N"=No wait ---
		MOVDW		R3,@R1,0,_Sys_Bitmap
		MOVIDW		R4,000004000h		; D14=1, request for save configuration
		OR32		R3,R4
		MOVIDD		R4,07FFFFFFFh		; D31=0, disable wait (redundant, step already done)
		AND32		R3,R4
		MOVDW		@R1,0,_Sys_Bitmap,R3
		MOVIBW		@R1,0,_Start_Pause,0	; This redundant, step already done
;--- Exit ---
SWAIT_0:	RET