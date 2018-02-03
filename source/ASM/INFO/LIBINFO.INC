;===========================================================================;
;                                                                           ;
;   DISPATCHER AND LIBRARY FOR BUILT BLOCK OF SYSTEM INFORMATION STRINGS.   ;
;                                                                           ;
;===========================================================================;

;--- Dispatcher for get system information sequence execution -----------;
; Handlers can corrupt registers R2-R7, dispatcher must save-restore it  ;
; if required for sequence execution, but not saved for caller.          ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, use buffer on it     ;
;          R2 = Pointer to sequence control table, offset relative R1    ;
;                                                                        ;
; OUTPUT:  None, R2-R7 corrupted                                         ;
;------------------------------------------------------------------------;

Dispatcher_SysInfo:
;--- Prepare parameters ---
		ADD64		R2,R1		; R2 = Pointer to sequence control table
		MOVQ		R3,R1		; R3 = Pointer to buffer for built strings
		XOR64		R4,R4		; R4 = Current Y-position offset
;--- Work cycle ---
DSI_0:		MOVIQW		R6,1		; Assist for handlers, R6=1
		XOR64		R7,R7		; Assist for handlers, R7=0
		MOVB		R5,@R2		; R5 = Control byte
		CMP32EQ 	R5,R7		; 0 means end of sequence, other=debug code
		JMP8CS		DSI_1		; Go if end of sequence
		CALL32		Send_Checkpoint_R5	; Conditional send checkpoint to debug port
		EXTNDW64	R5,@R2,0,1	; R5 = Offset for subroutine call
		ADD64		R5,R1		; Add R1=Base64 and R5=SignedOffset16
;--- Target call ---
		PUSH64		R2		; R2 must be saved, it is sequence pointer
		PUSH64		R4		; R4 must be saved, it is Y position
		CALL32A 	R5		; Call handler, built text string or benchmark
		POP64		R4
		ADD64		R4,R2		; Add Y addend
		POP64		R2
;--- Make cycle ---
		MOVSNW		R2,R2,3 	; R2+3, address next element of sequence
		JMP8		DSI_0		; Go cycle for next element of sequence
;--- Return ---
DSI_1:		RET

;--- Built entry for info strings ---------------------------------------;
; Each builted entry format:                                             ;
; WORD   X-addend    , pixels , spec. value 0FFFFh = end of sequence     ;
; WORD   Y-addend    , pixels                                            ;
; DWORD  True color  , Reserved8.Red8.Green8.Blue8                       ;
; BYTEs  Text string , length is variable                                ;
; BYTE   00h         , End of string                                     ;
; Sequence terminator must be word 0FFFFh (not byte 0FFh),               ;
; because X addend can be 255.                                           ;
;                                                                        ;
; INPUT:   R2 = Source pointer, signed offset relative R1                ;
;          R3 = Destination pointer for built entries, absolute address  ;
;          R4 = Y-addend, at bits [15-00], other bits = 0                ;
;          R5 = Header (total 8 bytes):                                  ;
;                QWORD  D[63-32] = True color                            ;
;                DWORD  D[31-16] = 0000h, blanked for Y-addend           ;
;                DWORD  D[15-00] = X-addend or special value 0FFFFh=End  ;
;                                                                        ;
; OUTPUT:  R2 = Source pointer modified by entry write                   ; 
;          R3 = Destination pointer modified by entry write              ;
;------------------------------------------------------------------------;

Built_Info_Entry:
		PUSH64		R4
		PUSH64		R6
		MOVIQW		R6,16
		SHL64		R4,R6		; Positioning Y addend
		ADD64		R4,R5		; R4 = Y addend + Other fields
		ADD64		R2,R1		; Convert R2 to absolute pointer
		MOVQ		@R3,R4		; Store header
		MOVSNW		R3,R3,8 	; R3+8, skip header
		CALL32		Copy_String	; Store ASCII text string, include terminator 00h
		MOVSNW		R3,R3,1 	; R3+1, skip terminator byte
		POP64		R6
		POP64		R4
		RET

;--- Get CPU Architectural Protocol timer period and current value ------;
; INPUT   : R1 = Global variables pool base address                      ;
;                                                                        ;
; OUTPUT  : Condition Flag (CF) = Status:                                ;
;            0(CC) = No errors, 1(CS) = Error detected                   ;
;           R7 = UEFI Status, unified for 32 and 64                      ;
;           R6 = CAP timer period, picoseconds                           ;
;           R5 = CAP timer value, clocks                                 ;
;           R2-R4 can be corrupted, because save-restore operations      ;
;                 minimized for better timings.                          ;
;------------------------------------------------------------------------;

Read_CAP_Timer:
;--- Assign CAP protocol pointer ---
		MOVQW		R2,@R1,0,_CAP_Protocol	; Save CAP interface pointer
;--- Assign timer index=0 ---
		XOR64		R6,R6
;--- Prepare scratch pads in the stack ---
		XOR64		R7,R7		; R7=0 for blank storages
		PUSH64		R7		; Storage for Timer Value
		PUSH64		R7		; Storage for Timer Period
;--- Built function frame frame ---
		MOVQ		R7,R0		; R7=Address of Storage for Timer Period
		PUSHN		R7		; Parm#4=Pointer to Timer Period
		MOVSNW		R7,R7,8 	; R7=Address of Storage for Timer Value
		PUSHN		R7		; Parm#3=Pointer to Timer Value
		PUSHN		R6		; Parm#2=Timer index
		PUSHN		R2		; Parm#1=Protocol pointer
;--- Call function ---
		CALL32EXA	@R2,6,0 	; CapTable.GetTimerValue
;--- Remove function stack frame ---
		POPN		R2		; Parm#1, restore
		POPN		R6		; Parm#2, restore
		POPN		R3		; Parm#3, remove only
		POPN		R3		; Parm#4, remove only
;--- Read and remove output parameters ---
		POP64		R6		; R6=Timer Period
		POP64		R5		; R5=Timer Value
;--- Check status ---
		MOVSNW		R7,R7		; Make unify for 32 and 64
		CMPI64WUGTE	R7,1		; CF=1 if R7>0
;--- Return ---
		RET


;--- Built PCIe device info string --------------------------------------;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Pointer to source text strings sequence                  ;
;          R3 = Pointer to destination string                            ;
;          R4 = PCI Bus/Dev/Fnc/Reg address in the 64-bit RBIO format    ;
;               Register address for PCIe capability structure           ;
;                                                                        ;
; OUTPUT:  R2 = Pointer updated by strings sequence read                 ;
;          R3 = Pointer updated by string write (this function reserved) ;
;------------------------------------------------------------------------;
Local_Inf_PCIe_1:
;--- Save registers ---
		PUSH64		R7
;--- Visual link width and speed ---
		MOVSNW		R2,R4,12h	; Base. Cap. + 12h = Link Status Register
		CALL32		Read_PCI_Cfg_16
		JMP8CS		Inf_PCIe_10	; Go silent error handling (skip data)

	;*	MOVIQW		R2,_String_PCIe_1
		MOVSNW		R2,R1,_String_PCIe_1	; Patch v0.21

		CALL32		Local_Inf_PCIe_2	; Built string: Width and Speed
;--- Visual current TLP ---
		PUSH64		R2
		MOVSNW		R2,R4,08h	; Base. Cap. + 08h = Device Control Register, Patch v0.21 (old 08h-12h)
		CALL32		Read_PCI_Cfg_16
		POP64		R2		; Patch v0.21

		JMP8CS		Inf_PCIe_10	; Go silent error handling (skip data)

		PUSH64		R2		; Patch v0.21
		MOVIQW		R2,5
		SHR32		R7,R2

		; MOVB		R7,R7		; Patch v0.21
		MOVIQW		R2,00000111b
		AND32		R7,R2

		CALL32		Local_Inf_PCIe_3	; Built string: TLP
		POP64		R2
;--- Visual ", max: x" ---
		CALL32		Copy_String
;--- Visual maximum supported width and speed ---
		MOVSNW		R2,R4,0Ch	; Base. Cap. + 0Ch = Link Capabilities Register, Patch v0.21 (old 0Ch-08h)
		CALL32		Read_PCI_Cfg_16
		JMP8CS		Inf_PCIe_10	; Go silent error handling (skip data)

	;*	MOVIQW		R2,_String_PCIe_1
		MOVSNW		R2,R1,_String_PCIe_1	; Patch v0.21

		CALL32		Local_Inf_PCIe_2
;--- Visual maximum supported TLP ---
		MOVSNW		R2,R4,04h	; Base. Cap. + 04h = Device Capabilities Register, Patch v0.21 (old 04h-0Ch)
		CALL32		Read_PCI_Cfg_16
		JMP8CS		Inf_PCIe_10	; Go silent error handling (skip data)

		MOVIQW		R2,00000111b
		AND32		R7,R2		; Patch v0.21, OLD R2,R7

		CALL32		Local_Inf_PCIe_3
;--- Terminate string ---
Inf_PCIe_10:	MOVIBW		@R3,0
		MOVSNW		R3,R3,1
;--- Restore registers and exit ---
		POP64		R7
		RET		


;--- Built part of PCIe device info string: width and speed -------------;
; INPUT:   R7 = Bit field from PCIe capability structure                 ;
;          R2 = Pointer to source text strings sequence                  ;
;          R3 = Pointer to destination string                            ;
;                                                                        ;
; OUTPUT:  R2 = Pointer updated by strings sequence read                 ;
;          R3 = Pointer updated by string write                          ;
;------------------------------------------------------------------------;
Local_Inf_PCIe_2:
;--- Save registers ---
		PUSH64		R4
		PUSH64		R7
;--- Visual link width ---
		PUSH64		R2
		MOVQ		R2,R3
		MOVQ		R3,R7
		MOVIQW		R4,4
		SHR32		R3,R4
		MOVIQW		R4,00111111b
		AND32		R3,R4
		XOR64		R4,R4			; Template mode = no template
		CALL32		String_Decimal32	; Write link width
		MOVQ		R3,R2
		POP64		R2
;--- Visual separator ", " ---
		CALL32		Copy_String
;--- Visual link speed ---
		PUSH64		R2
		MOVIQW		R4,00001111b
		AND32		R7,R4
		MOVSNW		R7,R7,-1		; R7-1, R7 = Selector
		CMPI32WUGTE	R7,2
		JMP8CS		Inf_PCIe_4
		MOVIQW		R7,3
Inf_PCIe_4:	MOVIQW		R6,3			; R6 = Limit
		
	;*	MOVIQW		R2,_String_25GTS	; R2 = Source strings pool
		MOVSNW		R2,R1,_String_25GTS	; Patch v0.21

		CALL32		Copy_Selected_String	; R3 = Destination
		POP64		R2
;--- Visual middle part "GT/s, TLP=" ---
		CALL32		Copy_String
;--- Restore registers and exit ---
Inf_PCIe_2:	POP64		R7
		POP64		R4
		RET


;--- Built part of PCIe device info string: TLP maximum size ------------;
; INPUT:   R7 = Bit field from PCIe capability structure                 ;
;          / NOT USED: R2 = Pointer to source text strings sequence      ;
;          R3 = Pointer to destination string                            ;
;                                                                        ;
; OUTPUT:  R2 = Pointer updated by strings sequence read                 ;
;          R3 = Pointer updated by string write                          ;
;------------------------------------------------------------------------;
Local_Inf_PCIe_3:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R4
		PUSH64		R7
;--- Check parameter valid, assume bits R7.[2-0] already masked ---
		CMPI32WULTE	R7,5
		JMP8CC		Inf_PCIe_5
;--- Reload parameter ---
		MOVQ		R2,R3
;--- Prepare number ---
		MOVIQW		R3,128			; Multiplier for calculate
		SHL32		R3,R7			; Calculate TLP size
		XOR64		R4,R4			; R4=0, template mode = no template
		CALL32		String_Decimal32	; Write TLP size
;--- Restore parameter and go to exit ---
		MOVQ		R3,R2
		JMP8		Inf_PCIe_6
;--- Branch for bad parameter ---
Inf_PCIe_5:	MOVIBW		@R3,'?'
		MOVSNW		R3,R3,1 		; R3+1, modify pointer
;--- Go to restore registers and exit ---
Inf_PCIe_6:	POP64		R7
		POP64		R4
		POP64		R2
		RET


;--- Wait user events after show system information screen --------------;
; Dynamical revisual date and time string,                               ;
; wait for key press, return scan code.                                  ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, use buffer on it     ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status, 64-bit or sign-extended 32 to 64 bit        ;
;          R2 = EFI_KEY, valid if Status=No errors                       ;
;          R2 bits [63-32] = 0, cleared                                  ;
;------------------------------------------------------------------------;
Wait_SysInfo:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6

;--- Revisual cycle ---
		XOR64		R5,R5
WSI_1:		PUSH64		R5

;--- Built date and time text string ---
		MOVSNW		R3,R1,Scratch_Pad_Size-80	; R3 = Pointer to destination text buffer
		CALL32		String_Date_Time		; Built date and time string
		MOVIBW		@R3,0				; Terminate date and time string
;--- Prepare parameters ---
;--- R2 = X base ---
		XOR64		R2,R2
;--- R3 = Y base ---
		MOVWW		R3,@R1,0,_Video_Ysize
		MOVSNW		R3,R3,-18
;--- R4 = X size ---
		MOVWW		R4,@R1,0,_Video_Xsize
;--- R5 = Y size ---
		MOVIQW		R5,18
;--- R6 = Memory base ---
		MOVQW		R6,@R1,0,_Primary_Memory_Base	; R6 = Transit buffer pointer
		MOVIQW		R7,4				; R7 = Bytes per pixel (BPP)
		MULU64		R7,R3				; R7 = 4 * Ysize
		MULU64		R7,R4				; R7 = 4 * Ysize * Xsize
		ADD64		R6,R7
;--- R7 = Delta ---
		MOVWW		R7,@R1,0,_Video_Xdelta	; R7 = Delta Y, bytes per string
;--- Blank transit buffer ---
		PUSH64		R4
		PUSH64		R6
		PUSH64		R7
		MOVIQW		R7,18*4/2		; 18 lines , 4 pixels per byte , 2 pixels per QWORD
		MULU64		R4,R7			; R4 = Qwords counter, multiplier=Xsize
		XOR64		R7,R7			; R7 = Data = 0
WSI_2:		MOVQ		@R6,R7			; Write QWORD = 2 pixels
		MOVSNW		R6,R6,8			; R6+8, modify pointer
		MOVSNW		R4,R4,-1		; R4-1, decrement counter
		CMPI32WEQ	R4,0
		JMP8CC		WSI_2			; Cycle for clear 18 lines
		POP64		R7
		POP64		R6
		POP64		R4
;--- Draw string in the transit buffer ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R7
		MOVIQW		R7,4*2			; 4 bytes per pixel, 2 lines
		MULU64		R7,R4			; Multiply by R4 = Xsize
		MOVSNW		R2,R7,3*8*4		; R2 = Block start + 2 lines down + 3 chars right
		ADD64		R2,R6			; Add buffer base address
		MOVSNW		R3,R1,Scratch_Pad_Size-80	; R3 = Pointer to string
		MOVIQD		R4,00D0E0D0h		; R4 = Front color, true color32: Reserved8.Red8.Green8.Blue8
		CALL32		Draw_String
		POP64		R7
		POP64		R4
		POP64		R3
		POP64		R2
;--- Update video memory, visual changes ---
		CALL32		GOP_BLT_Copy		

;--- Restore R5 ---
		POP64		R5

;--- Check keyboard status ---
WSI_6:		CALL32		Input_Check_Key
		CMPI64WEQ	R7,0
		JMP8CS		WSI_3			; Go if key pressed

;--- Check time changes ---
WSI_5:		CALL32		EFI_Get_Time		; Return R6 = Current date and time
		JMP8CS		WSI_6			; Go if error
		CMP64EQ		R5,R6			; Compare R5=Old time, R6=New time
		JMP8CS		WSI_6			; Go wait cycle if no changes
		MOVQ		R5,R6			; Update R5=Old time
WSI_4:		JMP8		WSI_1			; Go revisual date and time

;--- Restore registers ---
WSI_3:		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		RET

