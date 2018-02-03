;--- Initialization routine handler: Check UEFI firmware configuration -----;
; Parameters get from EFI System Table, width extract from instruction.     ;
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
;                DWORD  EFI_Revision       , major16:minor16                ;
;                DWORD  Firmware_Revision  , major16:minor16                ;
;                QWORD  Firmware_Vendor    , pointer to UNICODE16 string    ;
;                BYTE   CPU_Native_Width   , width, units=bytes             ;
;---------------------------------------------------------------------------;

Check_UEFI_Configuration:
;--- Get pointer to EFI System Table ---
		MOVQW		R2,@R1,0,_EFI_Table
;--- Get, verify, save parameter: EFI Spefification Revision ---
		MOVDW		R3,@R2,0,8
		CMP32LTE	R3,R7			; Here R7=0
		JMP8CS		CUC_1			; Go if error, revision=0 or bit D31=1
		MOVDW		@R1,0,_EFI_Revision,R3
;--- Get, verify, save parameter: EFI Firmware Revision ---
		MOVDW		R3,@R2,1,24
		CMP32LTE	R3,R7			; Here R7=0
		JMP8CS		CUC_1			; Go if error, revision=0 or bit D31=1
		MOVDW		@R1,0,_Firmware_Revision,R3
;--- Get, verify, save parameter: Pointer to EFI Firmware Vendor String ---
; Note string is UNICODE16, nul-terminated
		MOVNW		R3,@R2,0,24
		CMP32EQ		R3,R7			; Here R7=0
		JMP8CS		CUC_1			; Go if error, pointer=0
		MOVQW		@R1,0,_Firmware_Vendor,R3
;--- Get and save parameter: EBC interpreter version ---
		BREAK		1
		MOVDW		@R1,0,_EBC_VM_Version,R7
;--- Get and save parameter: CSM presence ---
		MOVIQW		R2,_GuidLegacyBios
		CALL32		Locate_Protocol
		JMP8CS		CUC_4			; Go if not found
		MOVQW		@R1,0,_Legacy_BIOS_Protocol,R2
;--- Set bitmap for mark CSM presence bit ---
		MOVIQQ		R6,0200000000h		; Bit D33=1, bit Info_Bitmap.1
		CALL32		CUC_3
;--- Get and save parameter: CPU native width by UEFI, units=bytes ---
CUC_4:		MOVINW		R3,1,0
		MOVBW		@R1,0,_CPU_Native_Width,R3
;--- Detect native machine type ---
		;
		; Moved to step 01h
		;
;--- This point used by some other subroutines ---
CUC_3:		MOVQW		R7,@R1,0,_Both_Bitmaps
		OR64		R7,R6
		MOVQW		@R1,0,_Both_Bitmaps,R7
;--- Return points, here R7=0, but also used by some other routines ---
CUC_0:		XOR64		R7,R7			; R7=0, means no errors
		RET
CUC_1:		MOVIQW		R7,00FFh		; R7=00FFh, means undetalized error
CUC_2:		RET

