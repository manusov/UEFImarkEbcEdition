;===========================================================================;
;                                                                           ;
;              SUBROUTINES FOR PCI EXPRESS OPERATIONS SUPPORT.              ;
;                                                                           ;
;===========================================================================;

;---------- Detecting Selected Device PCI Express Capabilities -------------;
; Assume EFI PCI Root Bridge I/O Protocol detected if control here.         ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = PCI Bus/Dev/Fnc/Reg address in the 64-bit RBIO format       ;
;               Host Bridge or Video Adapter address                        ;
;                                                                           ;
; OUTPUT:  CF = Error flag: 0(CC) = No Errors, capability found             ;
;                           1(CS) = Error (bad status) or cap. not found    ;
;          R7 = UEFI Status, if error returned                              ;
;                                                                           ;
;          R7 = Data if no errors:                                          ;
;               Capabilities Register, bits [7-0], here actual bits [2-0],  ;
;               encoded maximum payload size:                               ;
;                000b = 128 bytes                                           ;
;                001b = 256 bytes                                           ;
;                010b = 512 bytes                                           ;
;                011b = 1024 bytes                                          ;
;                100b = 2048 bytes                                          ;
;                101b = 4096 bytes                                          ;
;                110b = Reserved                                            ;
;                111b = Reserved                                            ;
;                                                                           ;
;          R6 = PCI Express Control Register, located at CAP_BASE+8         ;
;               valid bits [15-0]                                           ;
;---------------------------------------------------------------------------;
Parse_PCIe_Capability:
;--- Save registers ---
		PUSH64		R2
;--- Read PCIe Device Control Register ---
		MOVSNW		R2,R2,8			; R2+8, select PCIe D.Ctrl Register
		CALL32		Read_PCI_Cfg_16		; Read PCIe D.Ctrl Register, word
		JMP8CS		Exit_PCIe_Cap		; Go if error
		MOVW		R6,R7			; R6 = Output parameter: control register
;--- Read PCI Express Device Capabilities Register, bits [7-0] ---
		MOVSNW		R2,R2,-4		; R2-4, select PCIe DCAP Register
		CALL32		Read_PCI_Cfg_8		; Read PCIe DCAP Register, byte
		JMP8CS		Exit_PCIe_Cap		; Go if error
		MOVB		R7,R7			; R7 = Output parameter: capabilities register
;--- Analysing results ---
		MOVIQW		R2,00000111b		; Valid bits [2-0]
		AND32		R2,R7			; Masking valid bits
		CMPI32WUGTE	R2,00000110b		; 110b=Wrong, 111b=Wrong
		JMP8CS		Error_PCIe_Cap		; Go if error: invalid parameter
;--- Restore registers and exit ---
Exit_PCIe_Cap:	POP64		R2
		RET
Error_PCIe_Cap:	MOVIQQ		R7,EFI_INVALID_PARAMETER
		CMPI64WUGTE	R7,1			; This for set CF=F(R7), error flag
		JMP8		Exit_PCIe_Cap

