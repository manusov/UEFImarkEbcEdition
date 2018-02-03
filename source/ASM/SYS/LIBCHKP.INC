;===========================================================================;
;                                                                           ;
;               LIBRARY FOR SEND CHECKPOINTS TO DEBUG PORTS.                ;
;                                                                           ;
;===========================================================================;


; Optimization required, single R7 instead R4, R5, R7
; Optimization required, possible AND32 with memory location without load to R6
; *** UNDER CONSTRUCTION ***

Send_Checkpoint_R4:					; Entry point for R4 = Debug Code
		PUSH64		R7
		MOVQ		R7,R4
		JMP8		Send_Checkpoint

Send_Checkpoint_R5:					; Entry point for R5 = Debug Code
		PUSH64		R7
		MOVQ		R7,R5
		JMP8		Send_Checkpoint

Send_Checkpoint_R7:					; Entry point for R7 = Debug Code
		PUSH64		R7

Send_Checkpoint:
		PUSH64		R6
		PUSH64		R5
		PUSH64		R4
		PUSH64		R3
		PUSH64		R2
		PUSH64		R1

		MOVDW		R6,@R1,0,_Sys_Bitmap	; R1 = Global variables pool base address
		MOVIQW		R5,0001h
		AND32		R5,R6
		CMPI32WEQ	R5,0
		JMP8CS		Checkpoints_Disabled

		MOVSNW		R6,R1,_IA32_Checkpoint	; R6 = Absolute address of native subroutine
		PUSHN		R7			; R7 = Input parameter, debug code
		CALL32EXA	R6			; Call native subroutine, output byte from R7 to Port 80h
		POPN		R7

Checkpoints_Disabled:
		POP64		R1
		POP64		R2
		POP64		R3
		POP64		R4
		POP64		R5
		POP64		R6
		POP64		R7
		RET

