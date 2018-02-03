;===========================================================================;
;                                                                           ;
; LIBRARY FOR ACCESS PCI CONFIG. SPACE BY EFI PCI ROOT BRIDGE I/O PROTOCOL. ;
;                                                                           ;
;===========================================================================;

;--- Read byte/word/dword/qword register from PCI configuration space ------;
; Use EFI PCI Root Bridge I/O Protocol.                                     ;
; Caller must care about alignment and width restrictions, some platform    ;
; not support 64-bit operations include return data=0 and status=OK.        ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Address bits:                                               ;
;                D[07-00] = PCI Register address, ignored if ext. reg. >0   ;
;                D[15-08] = PCI Function address, range 0-7                 ;
;                D[23-16] = PCI Device address, range 0-1Fh                 ;
;                D[31-24] = PCI Bus address, range 0-FFh                    ;
;                D[63-32] = PCI Extended register address                   ;
;                           if this field zero, field D[07-00] used         ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, if CF=1(CS) , error or warning code,           ;
;               convert to 64-bit unified status in this subroutine         ;
;               Data after Read, if CF=0(CC) means no errors/warnings       ;
;---------------------------------------------------------------------------;

Read_PCI_Cfg_8:					; Entry for Read 8-bit
		PUSH64		R3
		XOR64		R3,R3		; R3 = 0 = EfiPciWidthUint8
		JMP8		RPC_0
Read_PCI_Cfg_16:				; Entry for Read 16-bit
		PUSH64		R3
		MOVIQW		R3,1		; R3 = 1 = EfiPciWidthUint16
		JMP8		RPC_0
Read_PCI_Cfg_32:				; Entry for Read 32-bit
		PUSH64		R3
		MOVIQW		R3,2		; R3 = 2 = EfiPciWidthUint32
		JMP8		RPC_0
Read_PCI_Cfg_64:				; Entry for Read 64-bit
		PUSH64		R3
		MOVIQW		R3,3		; R3 = 3 = EfiPciWidthUint64
;--- Adjust for select required function ---
RPC_0:		PUSH64		R4
RPC_1:		MOVINW		R4,07,00	; Entry #07 = Access.PciCfg.Read, N=7, C=0
RPC_3:		XOR64		R7,R7
RPC_2:		PUSH64		R5
		PUSH64		R6
;--- Built scratch pad for output parameter: read data ---
		PUSH64		R7		; Set scratch pad data = 0 or Input
		MOVQ		R5,R0		; R5 = Address of scratch pad
;--- Built input parameters stack frame ---
		PUSHN		R5		; Parm #5 = Buffer address
		MOVIQW		R5,1		; Count = 1
		PUSHN		R5		; Parm #4 = Count
		PUSH64		R2		; Parm #3 = PCI Cfg. space addr.
		PUSHN		R3		; Parm #2 = Width
		MOVQW		R2,@R1,0,_PCI_RB_IO_Protocol
		PUSHN		R2		; Parm #1 = Protocol pointer
;--- Call function ---
		ADD64		R2,R4
		CALL32EXA	@R2		; Entry #07 = Access.PciCfg.Read
;--- Unify and analyse status ---
		MOVSNW		R7,R7		; Unify for 32 and 64
		CMPI64WUGTE	R7,1		; CF=1(CS) if R7>0
;--- Remove input frame parameters ---
		POPN		R2		; #1
		POPN		R2		; #2
		POP64		R2		; #3, restore R2 = Cfg. addr.
		POPN		R3		; #4
		POPN		R3		; #5
;--- Remove parameters from stack frame, conditionally update output data ---
		POP64		R3		; R3 = Read data
		JMP8CS		RPC_9		; Skip load data to R7 if status=bad
		MOVQ		R7,R3		; Load data to R7 if status=ok
RPC_9:		POP64		R6
		POP64		R5
		POP64		R4		; R4 = Selector
		POP64		R3
;--- Return, here actual R7=Data or Status, CF=Error flag ---
		RET


;--- Write byte/word/dword/qword register in the PCI configuration space ---;
; Use EFI PCI Root Bridge I/O Protocol.                                     ;
; Caller must care about alignment and width restrictions, some platform    ;
; not support 64-bit operations include return data=0 and status=OK.        ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Address bits:                                               ;
;                D[07-00] = PCI Register address, ignored if ext. reg. >0   ;
;                D[15-08] = PCI Function address, range 0-7                 ;
;                D[23-16] = PCI Device address, range 0-1Fh                 ;
;                D[31-24] = PCI Bus address, range 0-FFh                    ;
;                D[63-32] = PCI Extended register address                   ;
;                           if this field zero, field D[07-00] used         ;
;          R7 = Data for Write                                              ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, include errors and warnings codes              ;
;               R7 convert to 64-bit unified status in this subroutine      ;
;---------------------------------------------------------------------------;

Write_PCI_Cfg_8:				; Entry for Write 8-bit
		PUSH64		R3
		XOR64		R3,R3		; R3 = 0 = EfiPciWidthUint8
		JMP8		WPC_0
Write_PCI_Cfg_16:				; Entry for Write 16-bit
		PUSH64		R3
		MOVIQW		R3,1		; R3 = 1 = EfiPciWidthUint16
		JMP8		WPC_0
Write_PCI_Cfg_32:				; Entry for Write 32-bit
		PUSH64		R3
		MOVIQW		R3,2		; R3 = 2 = EfiPciWidthUint32
		JMP8		WPC_0
Write_PCI_Cfg_64:				; Entry for Write 64-bit
		PUSH64		R3
		MOVIQW		R3,3		; R3 = 3 = EfiPciWidthUint64
;--- Adjust for select required function ---
WPC_0:		PUSH64		R4
		MOVINW		R4,08,00	; Entry #08 = Access.PciCfg.Write, N=8, C=0
;--- Go continue to common branch ---
		JMP8		RPC_2


;--- Read byte/word/dword/qword register from PCI MMIO memory space --------;
; Use EFI PCI Root Bridge I/O Protocol.                                     ;
; Caller must care about alignment and width restrictions, some platform    ;
; not support 64-bit operations include return data=0 and status=OK.        ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Address                                                     ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, if CF=1(CS) , error or warning code,           ;
;               convert to 64-bit unified status in this subroutine         ;
;               Data after Read, if CF=0(CC) means no errors/warnings       ;
;---------------------------------------------------------------------------;

Read_PCI_MMIO_8:				; Entry for Write 8-bit
		PUSH64		R3
		XOR64		R3,R3		; R3 = 0 = EfiPciWidthUint8
		JMP8		RPM_0
Read_PCI_MMIO_16:				; Entry for Write 16-bit
		PUSH64		R3
		MOVIQW		R3,1		; R3 = 1 = EfiPciWidthUint16
		JMP8		RPM_0
Read_PCI_MMIO_32:				; Entry for Write 32-bit
		PUSH64		R3
		MOVIQW		R3,2		; R3 = 2 = EfiPciWidthUint32
		JMP8		RPM_0
Read_PCI_MMIO_64:				; Entry for Write 64-bit
		PUSH64		R3
		MOVIQW		R3,3		; R3 = 3 = EfiPciWidthUint64
;--- Adjust for select required function ---
RPM_0:		PUSH64		R4
		MOVINW		R4,03,00	; Entry #03 = Access.PciMmio.Read, N=3, C=0
;--- Go continue to common branch ---
		JMP8		RPC_3


;--- Write byte/word/dword/qword register in the PCI MMIO memory space -----;
; Use EFI PCI Root Bridge I/O Protocol.                                     ;
; Caller must care about alignment and width restrictions, some platform    ;
; not support 64-bit operations include return data=0 and status=OK.        ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Address                                                     ;
;          R7 = Data for Write                                              ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, include errors and warnings codes              ;
;               R7 convert to 64-bit unified status in this subroutine      ;
;---------------------------------------------------------------------------;

Write_PCI_MMIO_8:				; Entry for Write 8-bit
		PUSH64		R3
		XOR64		R3,R3		; R3 = 0 = EfiPciWidthUint8
		JMP8		WPM_0
Write_PCI_MMIO_16:				; Entry for Write 16-bit
		PUSH64		R3
		MOVIQW		R3,1		; R3 = 1 = EfiPciWidthUint16
		JMP8		WPM_0
Write_PCI_MMIO_32:				; Entry for Write 32-bit
		PUSH64		R3
		MOVIQW		R3,2		; R3 = 2 = EfiPciWidthUint32
		JMP8		WPM_0
Write_PCI_MMIO_64:				; Entry for Write 64-bit
		PUSH64		R3
		MOVIQW		R3,3		; R3 = 3 = EfiPciWidthUint64
;--- Adjust for select required function ---
WPM_0:		PUSH64		R4
		MOVINW		R4,04,00	; Entry #04 = Access.PciMmio.Write, N=4, C=0
;--- Go continue to common branch ---
		JMP8		RPC_2


;--- Read byte/word/dword/qword register from PCI I/O space ----------------;
; Use EFI PCI Root Bridge I/O Protocol.                                     ;
; Caller must care about alignment and width restrictions, some platform    ;
; not support 64-bit operations include return data=0 and status=OK.        ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Address                                                     ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, if CF=1(CS) , error or warning code,           ;
;               convert to 64-bit unified status in this subroutine         ;
;               Data after Read, if CF=0(CC) means no errors/warnings       ;
;---------------------------------------------------------------------------;

Read_PCI_IO_8:					; Entry for Write 8-bit
		PUSH64		R3
		XOR64		R3,R3		; R3 = 0 = EfiPciWidthUint8
		JMP8		RPIO_0
Read_PCI_IO_16:					; Entry for Write 16-bit
		PUSH64		R3
		MOVIQW		R3,1		; R3 = 1 = EfiPciWidthUint16
		JMP8		RPIO_0
Read_PCI_IO_32:					; Entry for Write 32-bit
		PUSH64		R3
		MOVIQW		R3,2		; R3 = 2 = EfiPciWidthUint32
		JMP8		RPIO_0
Read_PCI_IO_64:					; Entry for Write 64-bit
		PUSH64		R3
		MOVIQW		R3,3		; R3 = 3 = EfiPciWidthUint64
;--- Adjust for select required function ---
RPIO_0:		PUSH64		R4
		MOVINW		R4,05,00	; Entry #05 = Access.PciIo.Read, N=5, C=0
;--- Go continue to common branch ---
		JMP8		RPC_3


;--- Write byte/word/dword/qword register in the PCI I/O space -------------;
; Use EFI PCI Root Bridge I/O Protocol.                                     ;
; Caller must care about alignment and width restrictions, some platform    ;
; not support 64-bit operations include return data=0 and status=OK.        ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Address                                                     ;
;          R7 = Data for Write                                              ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, include errors and warnings codes              ;
;               R7 convert to 64-bit unified status in this subroutine      ;
;---------------------------------------------------------------------------;

Write_PCI_IO_8:					; Entry for Write 8-bit
		PUSH64		R3
		XOR64		R3,R3		; R3 = 0 = EfiPciWidthUint8
		JMP8		WPIO_0
Write_PCI_IO_16:				; Entry for Write 16-bit
		PUSH64		R3
		MOVIQW		R3,1		; R3 = 1 = EfiPciWidthUint16
		JMP8		WPIO_0
Write_PCI_IO_32:				; Entry for Write 32-bit
		PUSH64		R3
		MOVIQW		R3,2		; R3 = 2 = EfiPciWidthUint32
		JMP8		WPIO_0
Write_PCI_IO_64:				; Entry for Write 64-bit
		PUSH64		R3
		MOVIQW		R3,3		; R3 = 3 = EfiPciWidthUint64
;--- Adjust for select required function ---
WPIO_0:		PUSH64		R4
		MOVINW		R4,06,00	; Entry #06 = Access.PciIo.Write, N=6, C=0
;--- Go continue to common branch ---
		JMP8		RPC_2


;--- Scan PCI Bus for devices matched BaseClass/SubClass/PI ----------------;
; INPUT:   R1 = Global variables pool base address                          ;
;          R3 = Pointer to destination buffer                               ;
;          R4 = Destination buffer size limit, exit when R3=R4              ;
;          R5 = CMP-mask for BaseClass/SubClass/PI                          ;
;          R6 = AND-mask for BaseClass/SubClass/PI                          ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = UEFI Status, include errors and warnings codes              ;
;               R7 convert to 64-bit unified status in this subroutine      ;
;          R3 = Number of entries in the output buffer (before error)       ;
;          Memory at [input R3] contain 64-bit entries, each entry:         ;
;                D[07-00] = PCI Register address = 0, not used here         ;
;                D[15-08] = PCI Function address, range 0-7                 ;
;                D[23-16] = PCI Device address, range 0-1Fh                 ;
;                D[31-24] = PCI Bus address, range 0-FFh                    ;
;                D[63-32] = PCI Extended reg. address = 0, not used here    ;
;---------------------------------------------------------------------------;

PCI_Scan_Class:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
;--- Initializing scan cycle parameters ---
		XOR64		R2,R2			; R2 = Start Bus/Dev/Fnc/Reg = 0
;--- Start scan cycle, check device exist ---
Scan_PCI:	CALL32		Read_PCI_Cfg_32		; Read DeviceID:VendorID
		JMP8CS		Exit_Scan_PCI		; Exit scan if error
		CMPI32WEQ	R7,0FFFFh
		JMP8CS		Next_Dev		; Skip if device absent
;--- Read Header type ---
Scan_Fnc:	MOVIQW		R4,000Eh		; Reg 0Eh = Header Type
		OR64		R2,R4			; Add register address
		CALL32		Read_PCI_Cfg_8		; Read Header Type
		JMP8CS		Exit_Scan_PCI		; Exit scan if error
;--- Check single/multi function ---
		MOVIQW		R4,0080h
		AND32		R4,R7
		CMPI32WEQ	R4,0
		JMP8CC		Multi_Func		; Go if bit HeaderType.7 = 1 (multi function)
		MOVIWW		R4,0FF00h		; Mask for function number, really used 3 low bits [8-11]
		AND32		R4,R2
		CMPI32WEQ	R4,0
		JMP8CC		Next_Dev		; Skip function 1-7 if Single-Function
;--- Scan functions of multi-function ---
Multi_Func:	MOVIQQ		R4,0FFFFFF00h
		AND64		R2,R4			; Extended Register=0, Register=0
		CALL32		Read_PCI_Cfg_32		; Read DeviceID:VendorID
		JMP8CS		Exit_Scan_PCI		; Exit scan if error
		CMPI32WEQ	R7,0FFFFh
		JMP8CS		Next_Fnc		; Skip if function absent
;--- Detect entry ---
		MOVIQW		R4,0008h		; Reg 08h = BaseClass/SubClass/ProgInf/Rev
		OR64		R2,R4			; Add register address
		CALL32		Read_PCI_Cfg_32		; Read BaseClass/SubClass/ProgInf/Rev
		JMP8CS		Exit_Scan_PCI		; Exit scan if error
		SHR32		R7,R4			; R7 = BaseClass/SubClass/ProgInf
		AND32		R7,R6			; AND-mask, select valid bits
		CMP32EQ		R7,R5			; CMP-mask, compare valid bits
		JMP8CC		Next_Fnc		; Go skip if mismatch
;--- Restore R2 and save entry: Bus/Device/Function, at 64-bit format ---
		MOVIQQ		R4,0FFFFFF00h
		AND64		R2,R4			; Extended Register=0, Register=0
		MOVQ		@R3,R2			; Save entry to output buffer
		MOVSNW		R3,R3,8			; R3+8, modify pointer
;--- Check output buffer size limit ---
		CMP64UGTE	R3,@R0			; Top of stack contain input R4 value
		JMP8CS		Bad_Scan_PCI		; Go if buffer overflow
;--- Continue scan cycle, next function ---
Next_Fnc:	

	; Sign-extended masking bug fixed at v0.22.
	; Word extended to FFFFFFFFFFFFFF00h, but requird 000000000000FF00h
	; Can also MOVIQW with 00F00h mask

	;!BUG	MOVIQW		R4,0FF00h		; Mask for function number, really used 3 low bits [8-11]
		MOVIQD		R4,0FF00h		; Mask for function number, really used 3 low bits [8-11]

		AND32		R4,R2
		CMPI32WUGTE	R4,00700h
		MOVIQW		R4,00100h
		ADD64		R2,R4			; Function Number + 1
		JMP8CC		Scan_Fnc		; Go cycle for next function if previous function<7
;--- Cycle for next device ---
Next_Dev:	MOVIQD		R4,000010000h
		ADD64		R2,R4			; Device Number + 1
		MOVIQQ		R4,0FF1F0000h
		AND64		R2,R4			; Extended Register=0, Function=0, Register=0
		MOVIQD		R4,0001F0000h
		AND32		R4,R2			; Select device number only
		CMPI32WEQ	R4,0
		JMP8CC		Scan_PCI		; Go cycle if device number not overflow
		MOVIQD		R4,001000000h
		ADD64		R2,R4			; Bus Number + 1
		MOVIQQ		R4,0FF000000h
		AND64		R2,R4			; Select bus number only

	;--- changes for v0.24 ---
	;
	;	CMPI32WEQ	R2,0
	;	JMP8CC		Scan_PCI		; Go cycle if bus number not overflow

;**********
; BREAK 3
;**********

		MOVBW		R7,@R1,0,_Last_PCI_Bus
		MOVIQW		R4,24
		SHL64		R7,R4
		CMP64ULTE	R2,R7
		JMP8CS		Scan_PCI


	;---


;--- End of scan cycle ---
Exit_Scan_PCI:	SUB64		R3,@R0,0,8		; Subtract input R3
		MOVIQW		R4,3			; Need right shift=3 for number of Bytes->Qwords
		SHR64		R3,R4			; R3 = Number of QWORDs
;--- Restore registers and exit ---
Ret_Scan_PCI:	POP64		R4
		POP64		R2			; Instead R3, because R3 = Output
		POP64		R2
		RET
;--- Error if buffer overflow ---
Bad_Scan_PCI:	MOVIQQ		R7,EFI_BUFFER_TOO_SMALL
		CMPI64WUGTE	R7,1			; This for set CF=1(CS), error flag
		JMP8		Ret_Scan_PCI


;---------- Scan for required PCI Standard Capability Structure ------------;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = PCI Bus/Device/Function address in the 64-bit RBIO format   ;
;          R3 = Structure type byte, 8-bit for Standard Capabilities        ;
;                                                                           ;
; OUTPUT:  CF = Error flag: 0(CC) = No Errors, structure found              ;
;                           1(CS) = Error or structure not found            ;
;          R7 = UEFI Status, if CF=1(CS) , error or warning code,           ;
;               convert to 64-bit unified status in this subroutine         ;
;               Contain EFI_NOT_FOUND if no fails but structure not found   ;
;          R2 = Structure address in the 64-bit RBIO format:                ;
;                                                                           ;
;                D[07-00] = PCI Register address                            ;
;                D[15-08] = PCI Function address, range 0-7                 ;
;                D[23-16] = PCI Device address, range 0-1Fh                 ;
;                D[31-24] = PCI Bus address, range 0-FFh                    ;
;                D[63-32] = PCI Extended reg. address = 0, not used here    ;
;---------------------------------------------------------------------------;

PCI_Scan_Capability:
;--- Save registers ---
		PUSH64		R5
		PUSH64		R6
;--- Assign PCI capabilities pointer ---
		MOVIQW		R7,0034h		; R7 = Register address, capabilities pointer
		OR64		R2,R7			; R2 = Bus/Device/Function/Register at RBIO format
		CALL32		Read_PCI_Cfg_8		; Read PCI capabilities pointer
		JMP8CS		Exit_PCI_Cap		; Go if PCI access error
;--- Start cycle for scan capabilities chain, read register ---
Re_Scan_Cap:	MOVIQW		R6,00FFh		; Mask for zero register check
		AND32		R6,R7			; Masking capabilities pointer
		CMPI32WEQ	R6,0			; Check capabilities pointer = 0
		JMP8CS		No_PCI_Cap		; Go if end of chain detected
		MOVIQW		R6,0003h		; Mask for alignment check
		AND32		R6,R7
		CMPI32WEQ	R6,0
		JMP8CC		Bad_PCI_Cap		; Go if alignment error (non zero after and)
		MOVIDD		R6,0FFFFFF00h
		AND64		R2,R6			; Clear previous register address
		NOT32		R6,R6			; Patched v0.21  R6 / R6,R6
		AND64		R7,R6			; Select register address only
		OR64		R2,R7			; Set new register address
		CALL32		Read_PCI_Cfg_16		; Read PCI capability structure
		JMP8CS		Exit_PCI_Cap		; Go if PCI access error
;--- Extract R6=Type, R7=Next pointer ---
		MOVB		R6,R7			; R6 = Structure type byte
		MOVIQW		R5,8			; R5 = 8, shifts count
		SHR32		R7,R5			; Shift R7 for extract pointer byte
		MOVB		R7,R7			; R7 = Next structure pointer byte
;--- Compare type ---
		CMP32EQ		R6,R3			; R6 = Read type, R3 = Required type
		JMP8CC		Re_Scan_Cap		; Go cycle for next structure if mismatch
;--- Handling compare match: structure found ---
Found_PCI_Cap:	XOR64		R7,R7
;--- Set CF=0(CC) if R7=0, CF=1(CS) if R7>0 ---
Status_PCI_Cap:	CMPI64WUGTE	R7,1			; This for set CF=F(R7), error flag
;--- Restore registers and exit ---
Exit_PCI_Cap:	POP64		R6
		POP64		R5
		RET
;--- Handling wrong values errors ---
Bad_PCI_Cap:	MOVIQQ		R7,EFI_DEVICE_ERROR
		JMP8		Status_PCI_Cap
;--- Handling structure not found ---
No_PCI_Cap:	MOVIQQ		R7,EFI_NOT_FOUND
		JMP8		Status_PCI_Cap


;---------- Scan for required PCI Extended Capability Structure ------------;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = PCI Bus/Device/Function address in the 64-bit RBIO format   ;
;          R3 = Structure type word, 16-bit for Extended Capabilities       ;
;                                                                           ;
; OUTPUT:  CF = Error flag: 0(CC) = No Errors, structure found              ;
;                           1(CS) = Error or structure not found            ;
;          R7 = UEFI Status, if CF=1(CS) , error or warning code,           ;
;               convert to 64-bit unified status in this subroutine         ;
;               Contain EFI_NOT_FOUND if no fails but structure not found   ;
;          R2 = Structure address in the 64-bit RBIO format:                ;
;                                                                           ;
;                D[07-00] = PCI Register address                            ;
;                D[15-08] = PCI Function address, range 0-7                 ;
;                D[23-16] = PCI Device address, range 0-1Fh                 ;
;                D[31-24] = PCI Bus address, range 0-FFh                    ;
;                D[63-32] = PCI Extended reg. address = 0, not used here    ;
;---------------------------------------------------------------------------;

PCI_Scan_Extended_Capability:
;--- Save registers ---
		PUSH64		R5
		PUSH64		R6
;--- Assign PCI capabilities pointer ---
		MOVIQQ		R7,010000000000h	; Register address, extended
;--- Start cycle for scan capabilities chain, read register ---
Re_Scan_E_Cap:	MOVIQQ		R6,0FFF00000000h	; Mask for zero register check
		AND64		R6,R7
		CMPI64WEQ	R6,0		
		JMP8CS		No_PCI_Cap		; Go if end of chain detected
		MOVIQQ		R6,000300000000h	; Mask for alignment check
		AND64		R6,R7
		CMPI32WEQ	R6,0
		JMP8CC		Bad_PCI_Cap		; Go if alignment error
		MOVIQD		R6,0FFFFFF00h
		AND64		R2,R6			; Clear previous register address
		NOT64		R6
		AND64		R7,R6			; Select register address only
		OR64		R2,R7			; Set new register address
		CALL32		Read_PCI_Cfg_16		; Read PCI capability structure
		JMP8CS		Exit_PCI_Cap		; Go if PCI access error
;--- Extract R6=Type, R7=Next pointer ---
		MOVW		R6,R7			; R6 = Structure type word
		MOVIQW		R5,16
		SHR32		R7,R5
		MOVW		R7,R7			; R7 = Next structure pointer word
;--- Compare type ---
		CMP32EQ		R6,R3			; R6 = Read type, R3 = Required type
		JMP8CC		Re_Scan_E_Cap		; Go cycle for next structure if mismatch


;---------- Detecting Selected Video Adapter Activity ----------------------;
; INPUT:  R1 = Global variables pool base address                           ;
;         R2 = PCI Bus/Device/Function address in the 64-bit RBIO format    ;
; OUTPUT: CF = Status Flag, 0(CC)=Adapter active,                           ;
;                           1(CS)=Adapter passive or error.                 ;
;         R7 = UEFI Status                                                  ;
;---------------------------------------------------------------------------;
; Method: Try to disable selected VGA adapter by clear I/O Enable bit.      ;
;         If VGA ports disabled by I/O Enable=0, this VGA is active         ;
;         If VGA ports not disabled by I/O Enable=0, this adapter passive,  ;
;         Bus:Device:Function contain I/O enable bit is address of other    ;
;         (passive) adapter.                                                ;
;                                                                           ;
; BUG: Require add disable/enable hardware interrupts, use CPU Arch. Prot.  ;
;---------------------------------------------------------------------------;

Check_Active_PCI_Video:
;--- Save registers ---
		PUSH64		R5
		PUSH64		R6
;--- Check VGA adapter access ---
		CALL32		Local_03C4_03C5
;--- Check status and data value ---
		JMP8CS		Status_PCI_Cap		; Go if Read PCI I/O operation error
		MOVIQW		R6,00FFh
		AND32		R7,R6
		CMP32EQ		R7,R6
		JMP8CS		No_PCI_Cap		; Go if adapter not found
;--- Read PCI Command Register ---
		MOVIQW		R5,4
		ADD64		R2,R5
		CALL32		Read_PCI_Cfg_16		; Read PCI Command Register
		SUB64		R2,R5
		JMP8CS		Status_PCI_Cap		; Go if Read PCI I/O operation error
;--- Invert I/O Slave bit ---
		MOVIQW		R6,1
		XOR64		R7,R6
		AND64		R6,R7
;--- Check I/O Slave bit state for this adapter ---
		CMPI32WEQ	R6,1
		JMP8CS		No_PCI_Cap		; Go if this adapter disabled		
;--- Write PCI Command Register ---
		ADD64		R2,R5
		CALL32		Write_PCI_Cfg_16	; Write PCI Command Register
		SUB64		R2,R5
		JMP8CS		Status_PCI_Cap		; Go if Write PCI I/O operation error
;--- Check VGA adapter access, here no check status ---
		CALL32		Local_03C4_03C5
;--- Restore PCI Command Register, here no check status ---
		PUSH64		R2
		PUSH64		R7
		ADD64		R2,R5
		CALL32		Read_PCI_Cfg_16		; Read PCI Command Register
		MOVIQW		R6,1
		XOR64		R7,R6			; Invert IO Slave bit
		CALL32		Write_PCI_Cfg_16	; Write PCI Command Register
		POP64		R7
		POP64		R2
;--- Check data value, go to restore registers and exit --- ---
		MOVIQW		R6,00FFh
		AND32		R7,R6
		CMP32EQ		R7,R6
		JMP8CC		No_PCI_Cap		; Go if adapter not found
		JMP8		Found_PCI_Cap		; This branch for adaptor found
;--- Local subroutine ---
Local_03C4_03C5:
		PUSH64		R2
;--- Start access VGA I/O ports, write byte 01h to port 03C4h (index) ---
		MOVIQW		R2,03C4h		; Select port 03C4h (index)
		MOVIQW		R7,1			; Index value = 1
		CALL32		Write_PCI_IO_8		; Write index
		JMP8CS		Result_03C5		; Go if Read PCI I/O operation error
;--- Read port 03C5h (data) ---
		MOVSNW		R2,R2,1			; R2+1, select port 03C5h (data)
		CALL32		Read_PCI_IO_8		; Read data
;--- Exit ---
Result_03C5:	POP64		R2
		RET

		
;---------- Sizing PCI Address BAR -----------------------------------------;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = PCI Bus/Device/Function address in the 64-bit RBIO format   ;
;          R3 = PCI Register address                                        ;
;          R4 = Bitmask for AND with PCI Command register                   ;
;                                                                           ;
; OUTPUT:  CF = Error flag: 0(CC)=No Errors, 1(CS)=Bad PCI status           ;
;          R7 = UEFI Status                                                 ;
;          R6 = Bitmap after PCI BAR Read Back                              ;
;                                                                           ;
; BUG: Require add disable/enable hardware interrupts, use CPU Arch. Prot.  ;
;---------------------------------------------------------------------------;

PCI_BAR_Sizing:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R5
;--- Read and save PCI Command Register ---
		MOVSNW		R2,R2,4			; R2+4, set register address
		CALL32		Read_PCI_Cfg_16		; Read PCI Command Register
		JMP8CS		Exit_Sizing		; Go if PCI access error
		MOVQ		R5,R7			; R5 = Storage for PCI Command
;--- Modify and write PCI Command Register ---
		AND32		R7,R4			; Clear Memory or IO access enable
		CALL32		Write_PCI_Cfg_16	; Write PCI Command Register
		JMP8CS		Exit_Sizing		; Go if PCI access error
;--- Read and save BAR(i) ---
		MOVSNW		R2,R2,-4		; R2-4, restore zero address
		ADD64		R2,R3			; Set target PCI BAR address
		CALL32		Read_PCI_Cfg_32		; Read target PCI BAR address
		JMP8CS		Exit_Sizing_1		; Go if PCI access error
		PUSH64		R7
;--- Write pattern and read back, here no status check ---
		MOVIQD		R7,0FFFFFFFFh
		CALL32		Write_PCI_Cfg_32	; Write PCI BAR
		CALL32		Read_PCI_Cfg_32		; Read back PCI BAR
		MOVQ		R6,R7			; R6 = Storage for Read Back
;--- Restore BAR(i), set output=R6 ---
		POP64		R7
		CALL32		Write_PCI_Cfg_32	; Write PCI BAR
;--- Restore PCI Command Register ---
Exit_Sizing_1:	SUB64		R2,R3			; Restore zero address
		MOVSNW		R2,R2,4			; R2+4 = Address
		MOVQ		R7,R5			; R7 = Data
		CALL32		Write_PCI_Cfg_16	; Write PCI Command Register
;--- Restore registers and exit ---
Exit_Sizing:	POP64		R5
		POP64		R2
		RET


;---------- Check validity of PCI BAR Read Back Bitmap ---------------------;
; INPUT:   R7 = Bitmap, valid bits D[00-31]                                 ;
; OUTPUT:  CF = Error flag: 0(CC)=No Errors, 1(CS)=Bad Bitmap               ;
;---------------------------------------------------------------------------;

PCI_BAR_Check_Integrity:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
;--- Initializing parameters ---
		XOR64		R3,R3			; R3 = Cycle counter, 0-32
		MOVIQW		R4,1			; R4 = Shift counter, 1 bit
;--- Cycle for verify "1" portion ---
Chk_Bar_1:	MOVIDD		R2,80000000h		; R2 = Mask
		AND64		R2,R7			; Analyse R7 bit 31
		SHL32		R7,R4
		CMPI32WEQ	R2,0
		JMP8CS		Chk_Bar_0
		MOVSNW		R3,R3,1			; R3+1, counter
		CMPI32WUGTE	R3,32
		JMP8CC		Chk_Bar_1
;--- Cycle for verify "0" portion ---
Chk_Bar_0:	MOVIDD		R2,80000000h		; Transfer here after first "0" detect
		AND64		R2,R7
		SHL32		R7,R4
		CMPI32WEQ	R2,0
		JMP8CC		Chk_Bar_Bad
		MOVSNW		R3,R3,1			; R3+1, counter
		CMPI32WUGTE	R3,32
		JMP8CC		Chk_Bar_0
;--- Exit points ---
Chk_Bar_Ok:	CMPI32WUGTE	R3,0FFh			; This for CF=0(CC)
		JMP8		Chk_Bar_Exit
Chk_Bar_Bad:	CMPI32WULTE	R3,0FFh			; This for CF=1(CS)
Chk_Bar_Exit:	POP64		R4
		POP64		R3
		POP64		R2
		RET


;--- Extract address from Memory BAR and verify BAR validity ---------------;
; INPUT:   R2 = PCI Bus/Dev/Fnc/Reg address in the 64-bit RBIO format       ;
;               legal registers for BARs: 10h,14h,18h,1Ch,20h,24h           ;
;               not-header BARs also possible for device-specific           ;
;                                                                           ;
; OUTPUT:  CF = Error flag: 0(CC)=No Errors, 1(CS)=Error                    ;
;          R7 = UEFI status, if error                                       ;
;          R7 = MMIO Range size, if no errors                               ;
;          R6 = Base MMIO Address, control bits masked                      ;
;               if MMIO above 4GB not sup., R6 (address bits 63-32)         ;
;                must check caller, not this subroutine                     ;
;                (x86 can sup. memory above 4GB, by PAE or x86-64)          ;
;---------------------------------------------------------------------------;

PCI_Detect_Memory_BAR:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
;--- Check Base Address Register (BAR(i)) ---
		CALL32		Read_PCI_Cfg_32
		JMP8CS		PDMB_11
		MOVQ		R6,R7
		MOVIQW		R4,0FF00h		; Mask for clear bits D0-D7
		AND64		R6,R4			; R6 = Base address
		MOVDW		@R0,00,08,R6		; R6 [31-00] image
		MOVIQW		R5,11110111b		; Prefetchable bit don't care
		AND64		R5,R7
		CMPI32WEQ	R5,0
		JMP8CS		PDMB_9			; Good 00000000b or 00000100b
		CMPI32WEQ	R5,00000100b
		JMP8CC		PDMB_5			; Bad control or alignment ?
;--- Section for 64-bit BAR only (check BAR(i+1))---
		MOVB		R3,R2			; R3 = Register
		AND64		R2,R4			; R2 = Bus/Dev/Fnc
		CMPI32WUGTE	R3,0024h		; Cannot extend to 64-bit if last BAR
		JMP8CS		PDMB_5			; Exit if width not extensible
		MOVSNW		R3,R3,4			; R3+4 for access BAR(i+1) address
		MOVIQW		R4,11111101b		; Clear D1 = Memory Slave Bit
		CALL32		PCI_BAR_Sizing		; Sizing test for BAR extension 64
		JMP8CS		PDMB_1			; Exit if error
;--- Update output parameter: range size, high dword ---
		MOVQ		R5,R6
		NOT32		R5,R5			; R5 = Size bits [63-32]
		MOVDW		@R0,00,04,R5		; R7 [63-32] image
;--- Analyse read-back ---
		CMPI32WEQ	R6,0
		JMP8CS		PDMB_10			; Good if 00000000h
		MOVSNW		R6,R6,1			; R6+1
		CMPI32WEQ	R6,0
		JMP8CC		PDMB_5			; Error if not FFFFFFFFh
;--- Read high address bits ---
PDMB_10:	PUSH64		R2
		ADD64		R2,R3
		CALL32		Read_PCI_Cfg_32
		POP64		R2
		JMP8CS		PDMB_11
		MOVDW		@R0,00,12,R7		; R6 [63-32] image
		MOVSNW		R3,R3,-4		; R3-4, select PCI BAR(i) address
		JMP8		PDMB_8			; Go to analyse low BAR
;--- Section for 32-bit BAR only ---
PDMB_9:		MOVIDD		R5,0FFF00000h		; Mask for check 1MB
		AND32		R5,R7
		CMPI32WEQ	R5,0
		JMP8CS		PDMB_5			; Error if below 1MB
;--- BAR width-specific sections done ---
		MOVB		R3,R2
		AND64		R2,R4
;--- Verify BAR(i) ---
PDMB_8:		MOVIQW		R4,11111101b		; Clear D1 = Memory Slave Bit
		CALL32		PCI_BAR_Sizing		; Sizing test for BAR
;--- Update output parameter: range size, low dword ---
		MOVQ		R5,R6
		MOVIQW		R4,0FFF0h
		AND32		R5,R4
		NOT32		R5,R5			; Invert R5
		MOVSNW		R5,R5,1			; R5+1
		MOVQ		@R0,R5			; R7 [31-00] image
;--- Analyse read-back ---
		MOVIQW		R5,11110011b		; Good 00000000b or 00000100b, prefetchable bit don't care
		AND32		R5,R6
		CMPI32WEQ	R5,0
		JMP8CC		PDMB_5			; Bad control or alignment ?
		MOVIDD		R7,0FFFFFF00h
		AND32		R7,R6
		CALL32		PCI_BAR_Check_Integrity
		JMP8CS		PDMB_5			; Exit if 0/1 integrity error
;--- Restore registers and exit ---
PDMB_11:	POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		RET
;--- Additional exit points ---
PDMB_1:		; Reserved for status update, R7 image in the stack
PDMB_12:	MOVIQQ		R7,EFI_DEVICE_ERROR
		CMPI64WUGTE	R7,1			; This for CF=1	
		JMP8		PDMB_11
PDMB_5:		; Reserved for status update, R7 image in the stack
		JMP8		PDMB_12

