;--- Initialization routine handler: Detect CPU MTRR Support ---------------;
; MTRR (Memory Type and Range Registers) presence detected by CPUID instr.  ;
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
;               Global variables:                                           ;
;
;
;---------------------------------------------------------------------------;

Check_Video_Range:
;--- Prepare pointer to MTRR variables block, built frame ---
		MOVSNW		R7,R1,0,_MTRR_Area
		PUSHN		R7
;--- Call native subroutine ---
		MOVINW		R7,1,0
		CMPI32WEQ	R7,4
		MOVIQW		R7,_IA32_Check_MTRR	; This pointer for IA32 (native width=4)
		JMP8CS		CVR_1
		MOVIQW		R7,_x64_Check_MTRR	; This pointer for x64 (native width=8)
CVR_1:		ADD64		R7,R1
		CALL32EXA	R7
;--- Remove frame ---
		POPN		R7
;--- Analyse status ---
		MOVD		R7,@R7			; Get variable MTRR_Used
		CMPI32WUGTE	R7,2			; Special values: 0=None, 1=No vacant
		JMP8CC		CVR_2			; Go if spec. value, MTRR not available
;--- Enable MTRR reprogramming if available ---
		MOVIQW		R6,0800h		; D11=1, Enable setup MTRR
		CALL32		CUC_3
;--- Exit with status=ok, independent of absent/present MTRR ---
CVR_2:		XOR64		R7,R7
		RET
