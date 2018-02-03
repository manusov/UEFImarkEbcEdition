;--- Initialization routine handler: Detect PCI bus support ----------------;
; Detect PCI bus presence by detect EFI PCI Root Bridge I/O Protocol.       ;
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

Check_PCI_Bus:
;--- Detect PCI Root Bridge I/O Protocol ---
		MOVIQW		R2,_GuidPciRoot
		CALL32		Locate_Protocol
;--- Check status ---
		MOVSNW		R7,R7			; Unify for 32 and 64
		CMPI64WEQ	R7,0
		JMP8CC		CPB_0			; Skip update if error detected
;--- Set variables if status OK ---
		MOVQW		@R1,0,_PCI_RB_IO_Protocol,R2
		MOVIQW		R6,00F0h		; Bits D[7-4]=1, for PCI
		CALL32		CUC_3
;--- Return ---
CPB_0:		XOR64		R7,R7			; Status=OK even if PCI not detected
		RET
