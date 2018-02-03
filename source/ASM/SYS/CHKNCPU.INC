;--- Initialization routine handler: Check native processor features -------;
; For x86 processors (IA32, x64) use EFLAGS.21 bit (ID-bit),                ;
; CPUID instr. funct. and read CPL from code seg. register, bits CS.1-0).   ;
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
;               Read data at last 256 bytes of Scratch_Pad:                 ;
;               0000h-000Fh = CPUID 00000000h result (EAX,EBX,ECX,EDX)      ;
;               0010h-001Fh = CPUID 00000001h result                        ;
;               0020h-002Fh = CPUID 80000000h result                        ;
;               0030h-003Fh = CPUID 80000001h result                        ;
;               0040h-004Fh = CPUID 80000002h result                        ;
;               0050h-005Fh = CPUID 80000003h result                        ;
;               0060h-006Fh = CPUID 80000004h result                        ;
;               0070h-007Fh = CPUID 00000007h result                        ;
;                                                                           ;
;               Global variables:
;
;
;---------------------------------------------------------------------------;

Check_Native_CPU:
;--- Check system is x86 (IA32 or x64) ---

		; UNDER CONSTRUCTION

;--- Select handler for detected native CPU type ---
		MOVBW		R2,@R1,0,_CPU_Native_Width
		MOVIQW		R3,_IA32_Check_CPUID	; Assign check-handler for IA32
		MOVIQW		R4,_IA32_Get_CPUID	; Assign get-handler for IA32
		MOVIQW		R5,_IA32_Get_CPL	; Assign cpl-handler for IA32
		MOVIQQ		R6,0400000000h		; Info_Bitmap.2 = IA32 EFI
		CMPI32WEQ	R2,4			; Native width is 4 bytes ?
		JMP8CS		CNC_1			; Go if IA32 detected (N=4)
		MOVIQW		R3,_x64_Check_CPUID	; Assign check-handler for x64
		MOVIQW		R4,_x64_Get_CPUID	; Assign get-handler for x64
		MOVIQW		R5,_x64_Get_CPL		; Assign cpl-handler for x64
		MOVIQQ		R6,0800000000h		; Info_Bitmap.3 = x64 UEFI
		CMPI32WEQ	R2,8			; Native width is 8 bytes ?
		JMP8CS		CNC_1			; Go if x64 detected (N=8)
CNC_0:		RET					; Return without handling if unknown width
;--- Start common branch for IA32 and x64, mark detected mode ---
CNC_1:		CALL32		CUC_3			; Mark IA32 or x64 mode, R6=bitmap
;--- Check CPUID instruction supported, check CPU vendor string availability ---
		MOVQ		R7,R3			; Assign handler
		CALL32		CNC_10			; Call native handler R7 = Check CPUID
		CMPI32WEQ	R7,1			; R7=0 if CPUID not supported, R7=1 if sup.
		JMP8CC		CNC_0			; Go if CPUID not supported
;--- Mark CPU vendor string availability ---
		MOVIQQ		R6,1000000000h		; Info_Bitmap.4 = CPU vendor string
		CALL32		CUC_3
;--- Get Standard CPUID maximum number, prepare for check TSC availability ---
		MOVSNW		R2,R1,Scratch_Pad_Size-256	; R2=Pointer
		XOR64		R6,R6				; R6=Function=0 (CPUID#0)
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
;--- Check maximum supported standard CPUID number ---
		CMPI32WUGTE	R7,1			; R7 = Output EAX after CPUID#0
		JMP8CC		CNC_2			; Go skip TSC if no CPU features list
;--- Re-positioning data pointer ---
		MOVSNW		R2,R2,16		; Re-positioning data pointer
;--- Mark CPU features list available ---
		MOVIQQ		R6,4000000000h		; Info_Bitmap.6 = CPU features string
		CALL32		CUC_3
;--- Get CPU standard features list ---
		MOVIQW		R6,1			; R6=Function=1 (CPUID#1)
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
		MOVDW		R6,@R2,0,12		; R6 = Output EDX after CPUID#1, features list
;--- Additionally check current privelege level (CPL), CPL>0 means run under emulator ---
; Note set OK to use TSC and MTRR, only if CPL=0 (supervisor mode)
;---
		MOVQ		R7,R5			; Assign handler
		CALL32		CNC_10			; Call native handler R7 = Get CPL
		CMPI32WEQ	R7,0
		JMP8CC		CNC_2			; Go skip set flag if CPL>0
;--- Transfer bits: TSC 4/2 , MTRR 12/8 ---
		MOVQ		R7,R6
		MOVIQW		R5,0010h
		AND64		R6,R5			; R6.4=TSC
		MOVIQW		R5,2
		SHR64		R6,R5			; R6.2=TSC
		MOVIQW		R5,1000h
		AND64		R7,R5			; R7.12=MTRR
		MOVIQW		R5,4
		SHR64		R7,R5			; R7.8=MTRR
		OR64		R6,R7
;--- Mark TSC and MTRR availability ---
		CALL32		CUC_3			; Mark TSC usage enable
;--- Re-positioning data pointer ---
CNC_2:		MOVSNW		R2,R2,16		; Re-positioning data pointer
;--- Get Extended CPUID maximum number, check CPU extended features availability ---
		MOVIDD		R6,80000000h		; R6=Function=80000000h (CPUID#80000000h)
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
;--- Re-positioning data pointer ---
		MOVSNW		R2,R2,16		; Re-positioning data pointer
;--- Get CPU extended features list --- 
		MOVIDD		R6,80000001h		; R6=Function=80000001h (CPUID#80000001h)
		CMP32UGTE	R7,R6			; R7 = Output EAX after CPUID#0
		JMP8CC		CNC_3			; Go if get extended features function not supported
		PUSH64		R7
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
;--- Mark CPU features list available ---
		MOVIQQ		R6,4000000000h		; Info_Bitmap.6 = CPU features string
		CALL32		CUC_3
		POP64		R7
;--- Check CPU name string availability ---
		MOVIDD		R6,80000004h		; R6 = Maximum function for get string
		CMP32UGTE	R7,R6			; R7 = Output EAX after CPUID#80000000h
		JMP8CC		CNC_3			; Go if name string functions not supported
;--- Set function number, re-positioning data pointer as part of cycle ---
		PUSH64		R7			; Save maximum CPUID available
		MOVQ		R5,R6			; Function limit
		MOVSNW		R6,R6,-2		; First function # 80000002h
CNC_4:		MOVSNW		R2,R2,16		; Re-positioning data pointer
;--- Get CPU name string ---
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
		MOVSNW		R6,R6,1			; Function number+1		
		CMP32ULTE	R6,R5			; Compare current function = R5 with limit = R6
		JMP8CS		CNC_4			; Cycle for functions 80000002/3/4
;--- Mark CPU name string availability ---
		MOVIQQ		R6,2000000000h		; Info_Bitmap.5 = CPU name string
		CALL32		CUC_3
		POP64		R7			; Restore maximum CPUID available
;--- Re-positioning data pointer ---
		MOVSNW		R2,R2,16		; Re-positioning data pointer
;--- Get CPU physical address width ---
		MOVIDD		R6,80000008h		; R6 = Function for get address width
		CMP32UGTE	R7,R6			; R7 = Output EAX after CPUID#80000000h
		JMP8CC		CNC_3			; Go if get address width function not supported
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
;--- Save CPU physical address width ---
		MOVB		R7,@R2
		MOVBW		@R1,0,_CPU_Phys_Addr_Width,R7
;--- Mark CPU physical address width available ---
		MOVIQQ		R6,20000000000h		; Info_Bitmap.9 = CPU physical address width
		CALL32		CUC_3


;--- Get CPU structured features, added at v0.25 ---
CNC_3:		XOR64		R6,R6			; R6 = Function=0 (CPUID#0), output buffer pointer at R2
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
		CMPI32WUGTE	R7,7			; R7 = Output EAX after CPUID#0, check function 7 available
		XOR64		R7,R7			; R7 = 0 for clear bitmap if CPUID#7 not supported
		MOVQ		@R2,R7			; Pre-blank bitmap (low dword), required 0 if not supported
		MOVQW		@R2,0,8,R7		; Pre-blank bitmap (high dword), required 0 if not supported
		JMP8CC		CNC_20			; Go skip AVX512 if no CPU structured features list
		MOVIQW		R6,7			; R6 = Function=7 (CPUID#7), output buffer pointer at R2 
		MOVQ		R7,R4			; Assign handler
		CALL32		CNC_11			; Call native handler R7 = Get CPUID
;--- End of added at v0.25 ---


;--- Go to clear status (R7=0) and return ---
; CNC_3:
CNC_20:
		XOR64		R7,R7			; R7=0, means no errors
		RET

;--- Local subroutine: gate for native handler without input parametes ---
; Input:   R1 = Global_Variables_Pool base address
;          R7 = Signed offset for subroutines call, relative R1
; Output:  R7 = Return parameter from native handler
;---
CNC_10:
		PUSH64		R4			; Save for caller
		PUSH64		R5			; Save for caller
		PUSH64		R6			; Save for caller
		ADD64		R7,R1			; Add for generating absolute address
		CALL32EXA	R7			; Call native subroutine
		POP64		R6
		POP64		R5
		POP64		R4
		RET

;--- Local subroutine: gate for native handler with 2 input parametes ---
; Input:   R1 = Global_Variables_Pool base address
;          R2 = Parameter # 2 for native handler, buffer pointer (16 bytes)
;          R6 = Parameter # 1 for native handler, CPUID function number
;          R7 = Signed offset for subroutines call, relative R1
; Output:  R7 = Return parameter from native handler
;---
CNC_11:
		PUSH64		R4			; Save for caller
		PUSH64		R5			; Save for caller
		PUSHN		R2			; Send parameter #2
		PUSHN		R6			; Send parameter #1
		ADD64		R7,R1			; Add for generating absolute address
		CALL32EXA	R7			; Call native subroutine

		;--- bug fix at v0.25, R7 output bits[63-32] not clean, occured at IA32 mode ---
		MOVIQQ		R6,0FFFFFFFFh
		AND64		R7,R6
		;---

		POPN		R6
		POPN		R2
		POP64		R5
		POP64		R4
		RET

