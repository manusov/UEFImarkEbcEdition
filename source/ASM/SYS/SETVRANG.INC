;--- Initialization routine handler: Video memory MTRR settings ------------;
; This subroutine conditionally run by _Sys_Bitmap mechanism, only if MTRR  ;
; supported and re-programmed.                                              ;
; Registers values, changed by this procedure, already saved                ;
; by Check_MTRR procedure.                                                  ;
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
;---------------------------------------------------------------------------;

Setup_Video_Range:
;--- Select base,size parameters source: prefetchable BAR or GOP ---
;--- Get prefetchable BAR info ---
		MOVQW		R7,@R1,0,_PBAR_Base		; R7 = Region base, qword
		MOVQW		R6,@R1,0,_PBAR_Size		; R6 = Region size, qword
;--- Check PCI PnP parameters valid ---
		MOVBW		R5,@R1,0,_PBAR_Reg
		CMPI32WEQ	R5,0
		JMP8CS		SVR_0				; Go to use GOP parameters if no PCI PnP
		MOVIQQ		R5,0FFFFFFFF00000000h
		PUSH64		R5
		AND64		R5,R7
		CMPI64WEQ	R5,0
		POP64		R5
		JMP8CC		SVR_0				; Go if base > 4GB (replace this parameters)
		AND64		R5,R7
		CMPI64WEQ	R5,0
		JMP8CS		SVR_1				; Go if size < 4GB (use this parameters)
;--- Get GOP BAR info ---
SVR_0:		MOVQW		R7,@R1,0,_Video_Memory_Base_GOP	; R7 = Region base, qword
		MOVQW		R6,@R1,0,_Video_Memory_Size_GOP	; R6 = Region size, qword
;--- Built input parameters frame ---
SVR_1:		PUSH64		R6				; Parm#4 = Region Size, qword
		PUSH64		R7				; Parm#3 = Region Base, qword
		MOVBW		R7,@R1,0,_CPU_Phys_Addr_Width
		PUSHN		R7				; Parm#2 = Physical address width, dword
		MOVDW		R7,@R1,0,_MTRR_Used
		PUSHN		R7				; Parm#1 = MSR address for access MTRR base:size pair, dword
;--- Call native subroutine ---
		MOVINW		R7,1,0
		CMPI32WEQ	R7,4
		MOVIQW		R7,_IA32_Setup_MTRR		; This pointer for IA32 (native width=4)
		JMP8CS		SVR_2
		MOVIQW		R7,_x64_Setup_MTRR		; This pointer for x64 (native width=8)
SVR_2:		ADD64		R7,R1
		CALL32EXA	R7
;--- Remove input parameters frame ---
		POPN		R7				; p#1
		POPN		R7				; p#2
		POP64		R7				; p#3
		POP64		R7				; p#4
;--- Set Restore Requirement Flag, bit D11=MTRR ---

		; This planned unlocked at v0.22 (or locked because duplicate operation)

		;MOVIQW		R6,0800h
		;CALL32		CUC_3
;--- Clear status and exit ---
		XOR64		R7,R7
		RET

