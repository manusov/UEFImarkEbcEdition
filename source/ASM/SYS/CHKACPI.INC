;--- Check context handler: Detect ACPI presence, analyse MCFG -------------;
; First try find ACPI 2.0 , if not found try ACPI 1.0,                      ;
; this sequence logic inside subroutine Get_ACPI_Root                       ;
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
;               Global variables:                                           ;
;               _Last_PCI_Bus                                               ;
;                                                                           ;
;               THIS FUNCTIONALITY RESERVED YET:                            ;
;               Global variables: WORD  Support_Bitmap, bit 3 = ACPI        ;
;                                 QWORD ACPI_Pointer                        ;
;---------------------------------------------------------------------------;

; (!) Required optimization, use R4,R5 for compact opcodes with input R6,R7.
; or simple R5 instead R7 for useable R7=0
; Effect is 4 compares with 0 replace to 4 compares with R7,
; and remove XOR64 R7,R7
; (?) But use R6,R7 reserve space between input (R1-R3) and output (R6-R7)
; parameters for add new parameters.

Check_ACPI:

;**********
; BREAK 3
;**********


;--- Set default last PCI bus value ---
		MOVIBW		@R1,0,_Last_PCI_Bus,0FFh
;--- Detect ACPI ---
		CALL32		Get_ACPI_Root
		JMP8CS		CACP_1		; Go if error
;--- Setup signature, select branch 32/64 ---
		MOVIDD		R3,'MCFG'
		MOVBW		R2,@R1,0,_CPU_Native_Width
		CMPI32WEQ	R2,8
		JMP8CS		CACP_64

;--- Support UEFI IA32 ---
CACP_32:
		CMPI64WEQ	R6,0		; R6 = RSDT pointer (32)
		JMP8CS		CACP_0		; Go silent exit if no ACPI/RSDT found
		MOVQ		R2,R6		; R2 = Pointer to RSDT
		CALL32		Scan_RSDT
		JMP8CS		CACP_1		; Go if error
		CMPI64WEQ	R7,0
		JMP8CS		CACP_0		; Go silent exit if no MCFG found
		JMP8		CACP_2		; Go interpreting MCFG if no errors, R7 = Pointer

;--- Support UEFI x64 ---
CACP_64:	
		CMPI64WEQ	R7,0		; R7 = XSDT pointer (64)
		JMP8CS		CACP_32		; Go try ACPI/RSDT if no ACPI/XSDT found
		MOVQ		R2,R7		; R2 = Pointer to XSDT
		CALL32		Scan_XSDT
		JMP8CS		CACP_1		; Go if error
		CMPI64WEQ	R7,0
		JMP8CS		CACP_32		; Go try ACPI/RSDT if no ACPI/XSDT/MCFG found

;--- Set last PCI bus value from MCFG ---
CACP_2:
		MOVBW		R7,@R7,0,55
		MOVBW		@R1,0,_Last_PCI_Bus,R7

;--- Exit points ---
CACP_0:
		XOR64		R7,R7
		RET
CACP_1:
		MOVIQW		R7,0FFh
		RET
