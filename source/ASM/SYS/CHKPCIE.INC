;--- Initialization routine handler: Detect PCI Express video adapter ------;
; Detect PCI Express support for video adapter and parent bridge,           ;
; use PCI Express capabilities structures scan.                             ;
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
;               _Host_Cfg , _Adapter_Cfg , _PCIe_H_Reg , _PCIe_A_Reg        ;
;               _PCIe_H_Caps , _PCIe_H_Ctrl                                 ;
;               _PCIe_A_Caps , _PCIe_A_Ctrl                                 ;
;                                                                           ;
;---------------------------------------------------------------------------;

Check_PCIe:
;--- Initialize counter for repeat twice, pass #2 = find separate 3D ---
		XOR64		R6,R6			; R6 = 0
;--- Blank variables, here start cycle for re-scan 3D controller ---
CPE_1:		MOVIBW		@R1,0,_PCIe_H_Reg,0	; Blank PCIe Host register address, 0 means device not found
		MOVIBW		@R1,0,_PCIe_A_Reg,0	; Blank PCIe Adapter register address, 0 means device not found
;--- Scan PCI bus for PCI-PCIe bridges ---
		PUSH64		R6			; Save R6, Fix v0.21
		MOVQ		R3,R1			; R3 = Pointer to destination buffer
		MOVSNW		R4,R3,Scratch_Pad_Size	; R4 = Limit for destination buffer
		MOVIDD		R5,0060400h		; R5 = Data(cmp), R6 = Mask(and), scan for PCI-PCI bridges
		MOVIDD		R6,0FFFFFEh		; PCI-PCI Bridges: 060400, 060401
		CALL32		PCI_Scan_Class		; Scan for this class, return R7=Status, R3=Count, buffer at input R3
		POP64		R6			; Restore R6, Fix v0.21
		CMPI32WEQ	R3,0			; Check for entries count = 0
		JMP8CS		CPE_0			; Go if no entries (no PCI-PCI bridges)
;--- Detect bridge, parent bus provider ---
; Here R4=List of B/D/F-entries, R3=Number of entries
;---
		MOVQ		R4,R1			; R4 = Pointer to source buffer, it destination for previous code
CPE_2:		MOVQ		R2,@R4			; Cycle for bridges, load R2 = Address
		MOVSNW		R4,R4,8			; R4 + 8, modify pointer for next qword
		MOVSNW		R2,R2,18h		; R2 + 18h, register address
		CALL32		Read_PCI_Cfg_32		; R7 = Data: [SecLat][SubOrdBus][SecBus][PrimBus]
		JMP8CS		CPE_0			; Go if PCI access error, means R7 = Status
;--- Analyse bus number match ---
		MOVSNW		R2,R2,-18h		; R2 - 18h, register address=0
		PUSH64		R2			; Save R2
		MOVD		R2,R7			; R2 = Data: [SecLat][SubOrdBus][SecBus][PrimBus]
		MOVIQW		R5,8			; R5 = 8, shifts count for bitfield in R2
		SHR32		R2,R5			; Shift secondary bus number to bits [7-0]
		MOVB		R2,R2			; R2 = Secondary bus number for bridge
		MOVBW		R5,@R1,0,_Adapter_Cfg+3	; R5 = Target bus number for video adapter
		CMP32UGTE	R5,R2			; Check Target bus < Secondary bus
		POP64		R2			; Restore R2
		JMP8CC		CPE_3			; Go if Target bus < Secondary bus
		PUSH64		R2			; Save R2
		MOVIQW		R2,16			; R2 = 8, shifts count for bitfield in R7
		SHR32		R7,R2			; Shift subordinate bus number to bits [7-0]
		MOVB		R7,R7			; R7 = Subordinate bus number for bridge
		CMP32ULTE	R5,R7			; Check Target bus <= Subordinate bus
		POP64		R2			; Restore R2
		JMP8CS		CPE_4			; Go if Target bus <= Subordinate bus (match)
;--- Cycle for detect parent bridge ---
CPE_3:		MOVSNW		R3,R3,-1		; R3-1, entries counter
		CMPI32WEQ	R3,0			; Check counter done
		JMP8CC		CPE_2			; Cycle for entries = host bridges
;--- Additionally find separate 3D-controller ---
		PUSH64		R6			; Save R6, Fix v0.21
		MOVQ		R3,R1			; R3 = Pointer to destination buffer
		MOVSNW		R4,R3,Scratch_Pad_Size	; R4 = Limit for destination buffer
		MOVIDD		R5,0030200h		; R5 = Data(cmp), R6 = Mask(and)
		MOVIDD		R6,0FFFFFFh		; Class = 3D Controller
		CALL32		PCI_Scan_Class		; Scan for this class, return R7=Status, R3=Count, buffer at input R3
		POP64		R6			; Restore R6, Fix v0.21
		CMPI32WEQ	R3,0			; R3 = Number of matched devices
		JMP8CS		CPE_0			; Go if no entries (no 3D controllers) ; old comment (no PCI-PCI bridges)

;--- Patch at v0.21 ---
	;*BUG*	MOVQ		R7,@R4			; Load 3D Controller address
		MOVQ		R7,@R1			; Load 3D Controller address
;----------------------

		MOVQW		@R1,0,_Adapter_Cfg,R7	; Set video adapter configuration address
		PUSH64		R6			; Save R6
		MOVIQD		R6,01000000h		; D24=1, 3D-controller mode		
		CALL32		CUC_3			; Set bit in bitmap
		POP64		R6			; Restore R6
		MOVSNW		R6,R6,1			; R6+1, counter for second pass
		CMPI32WUGTE	R6,2			; Check for second pass
		JMP8CC		CPE_1			; Repeat as cycle if first pass
;--- Exit if bridge not found, no errors ---
CPE_0:		XOR64		R7,R7			; R7 = UEFI status = 0
CPE_5:		RET					; Go here if exit with error, R7 = UEFI status
;--- Parent bridge found, here valid R2 = PCI Cfg. address ---
CPE_4:
;--- PCIe Host ---
		MOVIQW		R3,10h			; R3 = PCIe Capability Code
		CALL32		PCI_Scan_Capability
		JMP8CS		CPE_0			; Go if error, silent handling
		MOVBW		@R1,0,_PCIe_H_Reg,R2
		CALL32		Parse_PCIe_Capability
		JMP8CS		CPE_0			; Go if error, silent handling
		MOVBW		@R1,0,_PCIe_H_Caps,R7	; PCIe maximum payload
		MOVWW		@R1,0,_PCIe_H_Ctrl,R6	; PCIe control register
;--- Update address ---

	;* BUG FIX v0.25
	;*	MOVD		R7,0FFFFFF00h		; Register D[7-0]=0 and extended register D[63-32]=0
		MOVIQQ		R7,0FFFFFF00h
	;* 

		AND64		R2,R7			; Clear address bits: Extended Register=0, Register=0
		MOVQW		@R1,0,_Host_Cfg,R2
;--- PCIe Adapter ---
		MOVQW		R2,@R1,0,_Adapter_Cfg
		MOVIQW		R3,10h			; R3 = PCIe Capability Code
		CALL32		PCI_Scan_Capability	; Return: R7 = Status, R2 = Address as RBIO
		JMP8CS		CPE_0			; Go if error, silent handling
		MOVBW		@R1,0,_PCIe_A_Reg,R2
		CALL32		Parse_PCIe_Capability	; Return: R7 = Data/Status, R6 = Register value
		JMP8CS		CPE_0			; Go if error, silent handling
		MOVBW		@R1,0,_PCIe_A_Caps,R7	; Save R7 = PCIe maximum payload (adapter)
		MOVWW		@R1,0,_PCIe_A_Ctrl,R6	; Save R6 = PCIe control register (adapter)
;--- Check PCI Express re-programming possibility ---
		MOVBW		R2,@R1,0,_PCIe_H_Caps	; 
		MOVIQW		R5,0007h		; R5 = Mask for D[2-0] = Packet length supported
		AND32		R2,R5			; R2 = Host Cap. D[2-0]
		AND32		R7,R5			; R7 = Adapter Cap. D[2-0]
		CMP32ULTE	R2,R7			; Check R2 <= R7
		JMP8CS		CPE_6			; Go if R2 not above
		MOVB		R2,R7			; R2 = TLP Capability, min (Host, Adapter)
CPE_6:		MOVBW		R3,@R1,0,_PCIe_H_Ctrl	; 
		MOVIQW		R4,5			; R4 = Shifts count for D[7-5] -> D[2-0]
		SHR32		R3,R4			; R2 = Host Ctrl. D[7-5]
		SHR32		R6,R4			; R6 = Adapter Ctrl. D[7-5]
		CMP32EQ		R3,R7			; Compare settings: R3 = Current TLP, R7 = Adapter maximum
		JMP8CC		CPE_0			; Go if settings mismatch error, silent handling
		CMP32ULTE	R2,R3			; R2 = Host max. supported TLP , R3 = Current TLP
		JMP8CC		CPE_0			; Go if silent error (sup.<set.) or no reprogram (sup=set)
;--- Mark bitmap if PCI Express re-programming possible ---
		MOVIQW		R6,0400h		; Bit D10=1, for PCIe reprogramming
		CALL32		CUC_3			; Set bit in the bitmap
;--- Exit ---
		JMP8CS		CPE_0			; Return

