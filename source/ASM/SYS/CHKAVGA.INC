;--- Scan PCI configuration space (RBIO), detect active VGA/XGA ------------;
; Detect active video adapter by EFI PCI Root Bridge I/O Protocol.          ;
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
;                QWORD  Act_Video_Cfg = Active VGA bus/device/function      ;
;                QWORD, Adapter_Cfg   = This mismatch if 3D controller only ;
;                format same as Cfg.Address for subroutines at LIBPCI.INC   ;
;---------------------------------------------------------------------------;

Check_Active_Video:
;--- Scan PCI bus ---
		MOVQ		R3,R1			; R3 = Pointer to destination buffer
		MOVSNW		R4,R3,Scratch_Pad_Size	; R4 = Limit for destination buffer
		MOVIDD		R5,0030000h		; R5 = Data(cmp), R6 = Mask(and)
		MOVIDD		R6,0FFFE00h		; VGA = 0300xxh, XGA = 0301xxh
		CALL32		PCI_Scan_Class
		CMPI32WEQ	R3,0
		JMP8CS		CAV_0			; Go if no entries
;--- Detect Active VGA ---
; Here R4=List of B/D/F-entries, R3=Number of entries
;---
		MOVQ		R4,R1			; R4 = Load pointer
CAV_1:		MOVQ		R2,@R4			; Cycle for adapters, load R2 = Address
		MOVSNW		R4,R4,8			; R4 + 8, modify pointer for next qword
		MOVQW		@R1,0,_Adapter_Cfg,R2	; This for PCIe device
		MOVQW		@R1,0,_Act_Video_Cfg,R2	; This required if owervrite

;--- Corrections at v0.20 ---

		CMPI32WEQ	R3,1			; Prevent scan if single Video Adapter
		JMP8CS		CAV_2			; Just exit (OK) if number of adapters = 1 

;----------------------------

		CALL32		Check_Active_PCI_Video
		JMP8CC		CAV_0			; Go if active adapter found OK
		MOVSNW		R3,R3,-1		; R3-1, entries counter
		CMPI32WEQ	R3,0
		JMP8CC		CAV_1			; Go cycle if counter not done
;--- Exit points ---
CAV_0:		RET
CAV_2:		XOR64		R7,R7
		RET
