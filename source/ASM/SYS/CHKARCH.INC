;--- Initialization routine handler: Detect CPU instruction set archit. ----;
; Try run daughter tasks for some architectures and check result,           ;
; yet detected IA32 and x64 architectures.                                  ;
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
;               _Both_Bitmaps                                               ;
;                                                                           ;
;---------------------------------------------------------------------------;

Check_CPU_Architecture:
;--- This operations for correct method if Oracle VirtualBox bug ---
; Oracle VirtualBox UEFI firmware lose control if native mismatch run, IA32 routine for x64.
; Note other firmware returns error status and this fact used for native machine detection.
; Error prevent method: skip IA32 code if Native Width = 8 bytes means x64 UEFI.
; For version v0.23
 

		; This field not ready at this step
		; MOVBW		R2,@R1,0,_CPU_Native_Width
		
		MOVINW		R2,1,0
		CMPI32WEQ	R2,8			; Native width is 8 bytes ?
		JMP8CS		CCA_3			; Go if x64 detected (N=8)

;--- Detect by run daughter tasks, IA32 ---
		MOVIQW		R2,Daughter_Task_IA32
		CALL32		Handler_CCA
		JMP8CC		CCA_2			; Go if run IA32 passed
;--- Detect by run daughter tasks, x64 ---
CCA_3:		MOVIQW		R2,Daughter_Task_x64
		CALL32		Handler_CCA
		JMP8CC		CCA_2			; Go if run x64 passed
;--- Detect by firmware vendor and revision, special for buggy(?) firmware ---
		CALL32		Firmware_Detect		; Return R7=Bitmap
		MOVIQW		R6,0001h		; R6=Mask
		AND32		R7,R6
		CMPI32WEQ	R7,0001h
		JMP8CC		CCA_1			; Go if D0=0 means non-x86 platform
;--- Mark system x86 compatible ---
CCA_2:		MOVIQW		R6,0002h		; D1=1, enable native ID
		CALL32		CUC_3
;--- Exit ---
CCA_1:		XOR64		R7,R7			; Status=OK, even if non-x86 CPU
		RET

;---------- Load and Execute daughter task subroutine ----------------------;
; Use task image in the memory (part of application), dynamically patched   ;
; for use single task body both for IA32 and x64.                           ;
; Return status = "no errors" means this architecture supported, can run    ;
; native code subroutines.                                                  ;
; Return status = "error" means load or execution error or this             ;
; architecture not supported, cannot run native code subroutine             ;
;                                                                           ;
; INPUT:  R1 = Global variables pool base address                           ;
;         R2 = CPU Architecture word:                                       ;
;                    014Ch = IA32, 8664h = x86-64                           ;
;                                                                           ;
; OUTPUT: CF flag: 0(CC) = No errors, application loaded and executed OK    ;
;                  1(CS) = Error loading or executing application,          ;
;                          include incompatible CPU architecture            ;
;         R7 = UEFI Status                                                  ;
;---------------------------------------------------------------------------;

Handler_CCA:
;--- Patch Architecture Type field ---
		MOVSNW		R3,R1,_Daughter_Task_Image+Daughter_Task_Patch
		MOVW		@R3,R2
;--- Prepare scratch pads for LoadImage() ---
		XOR64		R2,R2
		PUSH64		R2			; Image Handle
		MOVQ		R3,R0
;--- Prepare input stack frame for LoadImage() ---
		PUSHN		R3			; Parameter #6 = Pointer to Image Handle
		MOVIQW		R3,Daughter_Task_Size
		PUSHN		R3			; Parameter #5 = Source Size
		MOVSNW		R3,R1,_Daughter_Task_Image
		PUSHN		R3			; Parameter #4 = Source Base
		PUSHN		R2			; Parameter #3 = Device Path
		MOVQW		R3,@R1,0,_EFI_Handle
		PUSHN		R3			; Parameter #2 = Parent Handle
		PUSHN		R2			; Parameter #1 = Boot Policy
;--- Call function LoadImage() ---
		MOVNW		R3,@R1,0,_EFI_Table	; R3 = SysTable
		MOVNW		R3,@R3,9,24		; R3 = BootServices
		CALL32EXA	@R3,22,24		; Entry #22 = Load Image
;--- Remove stack frame after LoadImage() ---
		POPN		R2			; #1 Boot Policy
		POPN		R2			; #2 Parent Handle
		POPN		R2			; #3 Device Path
		POPN		R2			; #4 Source Base
		POPN		R2			; #5 Source Size
		POPN		R2			; #6 Pointer to New Image Handle
;--- Read scratch pads after LoadImage() ---
		POP64		R6			; Image Handle, R6 used later
;--- Check status ---
		MOVSNW		R7,R7			; Unify for 32 and 64
		CMPI64WEQ	R7,0
		JMP8CC		CCA_0
;--- Prepare scratch pads for StartImage() ---
		XOR64		R2,R2
		PUSH64		R2			; Exit data size
		MOVQ		R3,R0
		PUSH64		R2			; Exit data pointer
		MOVQ		R4,R0
;--- Prepare input stack frame for StartImage() ---
		PUSHN		R4			; Parameter #3 = Exit data pointer
		PUSHN		R3			; Parameter #2 = Exit data size
		PUSHN		R6			; Parameter #1 = Image Handle
;--- Call function StartImage() ---
		MOVNW		R3,@R1,0,_EFI_Table	; R3 = SysTable
		MOVNW		R3,@R3,9,24		; R3 = BootServices
		CALL32EXA	@R3,23,24		; Entry #23 = Start Image
;--- Remove stack frame after StartImage() ---
		POPN		R2			; #1 Image Handle
		POPN		R2			; #2 Exit data size
		POPN		R2			; #3 Exit data pointer
;--- Read scratch pads after StartImage() ---
		POP64		R2			; Exit data pointer
		POP64		R2			; Exit data size
;--- Exit with actual R7=Status and CF=Error ---
		MOVSNW		R7,R7			; Unify for 32 and 64
CCA_0:		CMPI64WUGTE	R7,1			; CF=1 if R7>0
		RET

