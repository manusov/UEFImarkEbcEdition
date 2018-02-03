;--- Initialization routine handler:  Detect video memory BAR --------------;
; Find PCI PnP prefetchable BAR (Base Address Register) for                 ;
; video memory access.                                                      ;
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

Check_Video_BAR:
;--- Pre-blank variables, use fact: R7=0 ---
		MOVQW		@R1,0,_PBAR_Base,R7
		MOVQW		@R1,0,_PBAR_Size,R7
		MOVBW		@R1,0,_PBAR_Reg,R7
;--- Detect video adapter BARs, get adapter bus/device/function ---
		MOVQW		R2,@R1,0,_Adapter_Cfg
		MOVSNW		R2,R2,10h		; First BAR = 0010h
CVB_1:		CALL32		Read_PCI_Cfg_32
		JMP8CS		CVB_0			; Go if error
		MOVB		R6,R7
		MOVIQW		R3,00001011b
		AND32		R7,R3
		CMPI32WEQ	R7,00001000b		; Prefetchable, 32/64-bit
		JMP8CS		CVB_4			; Go when first prefetchable BAR found
		MOVIQW		R3,00000111b
		AND32		R6,R3
		CMPI32WEQ	R6,00000100b
		JMP8CC		CVB_2			; Go if not 64-bit memory BAR
		MOVSNW		R2,R2,4 		; R2+4, skip extension of 64-bit BAR
CVB_2:		MOVSNW		R2,R2,4 		; R2+4, modify pointer for next BAR
		MOVB		R3,R2
		CMPI32WULTE	R3,0024h		; Last BAR = 0024h
		JMP8CS		CVB_1			; Cycle for 6 BARs
;--- Exit point 1 ---
CVB_0:		RET
;--- BAR found, detect parameters ---
CVB_4:		MOVBW		@R1,0,_PBAR_Reg,R2	; Used by extreme, AL=BAR Reg.
		CALL32		PCI_Detect_Memory_BAR
		JMP8CS		CVB_0			; Go if error
;--- Store parameters ---
		MOVQW		@R1,0,_PBAR_Base,R6	; R6 = Base address, 64 bit
		MOVQW		@R1,0,_PBAR_Size,R7	; R7 = Size, 64 bit
;--- Exit point 2 ---
		XOR64		R7,R7
		RET

