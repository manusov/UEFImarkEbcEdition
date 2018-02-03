;--- Initialization routine handler: Set video mode by GOP -----------------;
; Use function GOP.SetMode, if OK, get and save info.                       ;
; Video memory range base and size for given GOP video mode available after ;
; set this mode, this three actions possible only after set mode:           ;
; 1) Get video memory range base and size from GOP info table               ;
; 2) Video memory size detection by hardware scan                           ;
; 3) Video memory range attribute setup (MTRR if x86)                       ;
; This three operations executed here.                                      ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;               Global variables:                                           ;
;                Program_Video_Mode = Target video mode                     ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;               Global variables:                                           ;
;                Video_Xsize, Video_Ysize, Video_Xdelta, Video_BPP          ;
;---------------------------------------------------------------------------;

Setup_Video_Mode:

;--- Assign video mode number ---
		MOVDW		R3,@R1,0,_Program_Video_Mode
;--- Set video mode by GOP.SetMode function ---
		MOVQW		R2,@R1,0,_GOP_Protocol
		PUSHN		R3			; Parm#2=Mode number
		PUSHN		R2			; Parm#1=Protocol pointer
		CALL32EXA	@R2,1,0			; GopTable.ModeSet
		POPN		R2
		POPN		R3
;--- Check status after set video mode ---
		MOVSNW		R7,R7			; Make unify for 32 and 64
		CMPI64WEQ	R7,0
		JMP8CC		SVM_0			; Go if error, R7=UEFI status
;--- Mark set video mode operation executed ---
		MOVIDD		R6,00008000h		; Bit Sys_Bitmap.15 = video mode switched
		CALL32		CUC_3
;--- Get and save video mode number and parameters, required for drawings ---
		MOVNW		R6,@R2,3,0		; R6=GOP info table pointer, N=3, C=0
		MOVDW		R3,@R6,0,4		; R3=Current (program) video mode
		MOVDW		@R1,0,_Program_Video_Mode,R3
		MOVNW		R5,@R6,0,8		; R5=Pointer to mode info block, N=0, C=8
		MOVNW		R4,@R6,1,8		; R4=Size of mode info block, N=1, C=8
		; here reserved point, need check R4=24h or above
		MOVNW		R2,@R5,0,4		; R2=Horizontal resolution, N=0, C=4
		MOVNW		R3,@R5,0,8		; R3=Vertical resolution, N=0, C=8
		; here reserved point, need check R2, R3 with Xlimit, Ylimit
		MOVWW		@R1,0,_Video_Xsize,R2	; Horizontal resolution, pixels
		MOVWW		@R1,0,_Video_Ysize,R3	; Vertical resolution, pixels
		MOVIQW		R3,2			; Shift left by 2 means *4
		SHL64		R2,R3			; Multiply by 4 bytes per pixel
		MOVWW		@R1,0,_Video_Xdelta,R2	; Bytes per horizontal line
		MOVIBW		@R1,0,_Video_BPP,32	; Bits per pixel = 32 for all GOP modes

;--- Additional parameters: check video memory base address and size valid ---
; Here valid:
; R5 = Pointer to mode info block
;---
		MOVBW		R2,@R5,00,12		; R2 = EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.PixelFormat
		CMPI32WUGTE	R2,3			; 3 = BltOnly, >3 is invalid
		JMP8CS		SVM_3			; Go if GOP video memory not valid

;--- Additional parameters: get video memory base and size from GOP info table ---
; Here valid:
; R5 = Pointer to GOP mode info table
; R6 = Pointer to GOP info table
;---
		MOVQW		R2,@R6,02,08		; R2=Video memory base address, physical 64-bit
		MOVNW		R3,@R6,02,16		; R3=Video memory size, natural width (32 or 64 bits)

;--- This operations for correct value if Oracle VirtualBox report mode-aware video memory size bug ---
; For version v0.23

		MOVIQQ		R6,07FFFFFFFh		; R6 = MASK
		CMP64ULTE	R3,R6
		JMP8CS		SVM_4			; Go skip if Oracle VirtualBox bug not detected
		AND64		R3,R6			; R3 AND MASK
		MOVIQW		R6,1			; R6 = ADDEND
		ADD64		R3,R6			; R3 + 1
SVM_4:

;--- Store parameters ---
		MOVQW		@R1,0,_Video_Memory_Base_GOP,R2
		MOVQW		@R1,0,_Video_Memory_Size_GOP,R3

;--- Mark video memory base, size by GOP valid ---
		MOVIQQ		R6,040000000000h	; Bit Sys_Bitmap.10 = GOP video memory valid
		CALL32		CUC_3
SVM_3:
;--- Setup MTRR if x86 and MTRR supported ---
		MOVQW		R7,@R1,0,_Sys_Bitmap
		MOVIQW		R6,0800h		; D11=MTRR
		AND32		R7,R6
		CMPI32WEQ	R7,0
		JMP8CS		SVM_2
		CALL32		Setup_Video_Range
SVM_2:

;--- Detect video memory size by hardware scan ---
; Use native subroutine
; Optimize required, re-use R7.
;---
		MOVQW		R7,@R1,0,_Sys_Bitmap
		MOVIQW		R6,0020h		; D5=PCI
		AND32		R7,R6
		CMPI32WEQ	R7,0
		JMP8CS		SVM_0

;--- Check support ---
		MOVBW		R7,@R1,0,_PBAR_Reg
		CMPI32WEQ	R7,0
		JMP8CS		SVM_0			; Go if VRAM Base/Size by PCI PnP not valid, R7=0 also for status
;--- Built input parameters frame ---
		MOVQW		R7,@R1,0,_PBAR_Size
		PUSHN		R7				; Parm#2 = Size
		MOVQW		R7,@R1,0,_PBAR_Base
		PUSHN		R7				; Parm#1 = Base
;--- Select IA32/x64 native handler ---
		MOVINW		R7,1,0
		CMPI32WEQ	R7,4
		MOVIQW		R7,_IA32_Sizing_VRAM		; This pointer for IA32 (native width=4)
		JMP8CS		SVM_1
		MOVIQW		R7,_x64_Sizing_VRAM		; This pointer for x64 (native width=8)
;--- Call native handler ---
SVM_1:		ADD64		R7,R1
		CALL32EXA	R7
;--- Remove input parameters frame, but output R7 must be saved ---
		POPN		R6				; p#1
		POPN		R6				; p#2
;--- Update result variable, yet used low dword only ---
		EXTNDD64	R7,R7
		MOVQW		@R1,0,_Video_Memory_Size_HW,R7
;--- Mark video memory base, size by hardware scan valid ---
		MOVIQQ		R6,080000000000h	; Bit Sys_Bitmap.11 = Hardware video memory valid
		CALL32		CUC_3
;--- Return with R7=Status ---
SVM_0:		RET
