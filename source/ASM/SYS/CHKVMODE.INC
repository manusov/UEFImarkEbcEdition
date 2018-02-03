;----------------------------------------------------------------;
; EFI_GRAPHICS_OUTPUT_MODE_INFORMATION, returned by function
; EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode()
; Offset(hex)  Size     Comments
;----------------------------------------------------------------;
; 00-03        dword    Structure version, yet used 0
; 04-07        dword    Horizontal resolution, pixels
; 08-0B        dword    Vertical resolution, pixels
; 0C-0F        dword    Bitmap EFI_GRAPHICS_PIXEL_FORMAT
; 10-1F     16 bytes    Color bitmaps EFI_PIXEL_BITMASK
; 20-23        dword    Pixels per scan line, logical length
;----------------------------------------------------------------;
; EFI_GRAPHICS_PIXEL_FORMAT bitmap:
; D00 = PixelRedGreenBlueReserved8BitPerColor, fixed mode 1
; D01 = PixelBlueGreenRedReserved8BitPerColor, fixed mode 2
; D02 = PixelBitMask, variable mode
; D03 = PixelBltOnly, "1" means direct VRAM access not supported
; D04 = PixelFormatMax
; D05-31=Limit
;----------------------------------------------------------------;
; EFI_PIXEL_BITMASK bitmaps: dwords Blue, Red, Green, Reserved
;----------------------------------------------------------------;
;
; EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE, accessible by GOP entry #4,
; information about current video mode
; Offset(hex)  Size     Comments
;----------------------------------------------------------------;
; 00-03        dword    MaxMode, maximum mode number
; 04-07        dword    Mode, current mode number
; 08-0F        qword    Addr. EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
; 10-17        qword    Size EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
; 18-1F        qword    FrameBufferBase, flat 64-bit address
; 20-27        qword    FrameBufferSize, flat 64-bit address
;----------------------------------------------------------------;


;--- Scan UEFI video modes list, find compatible video mode ----------------;
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
;                DWORD  Program_Video_Mode = Used video mode number (GOP)   ;
;---------------------------------------------------------------------------;

Check_Video_Mode:
;--- Detect number of supported GOP video modes, assign start mode = 0 ---
		MOVQW		R2,@R1,0,_GOP_Protocol		; R2 = Protocol pointer
		MOVNW		R6,@R2,3,0			; R6 = ModeTable

;--- Corrections at v0.20 ---

		MOVDW		R3,@R6,0,4			; R3 = Current mode
		MOVDW		@R1,0,_Firmware_Video_Mode,R3	; Set variable used when restore video mode

;----------------------------

		MOVD		R6,@R6				; R6 = MaxMode
		XOR64		R3,R3				; R3 = Start video mode number
;--- Setup default video mode ---
		MOVDW		@R1,0,_Program_Video_Mode,R3	; Default mode = 0
		MOVIWW		@R1,0,_Video_Xdelta,0FFFFh	; Set approximation = bad (this storage temp. used)
;--- Save mode limit ---
CVM_0:		PUSH64		R6
;--- Prepare parameters for query video mode info by GOP.QueryMode function ---
		PUSH64		R3				; Scratch pad for info size
		MOVQ		R4,R0				; R4 = Pointer to info size (note R0 is stack pointer)
		PUSH64		R3				; Scratch pad for info base
		MOVQ		R5,R0				; R4 = Pointer to info base (note R0 is stack pointer)
;--- Built stack frame with input parameters ---
		PUSHN		R4				; Parameter #4 = Info size (address for store it)
		PUSHN		R5				; Parameter #3 = Info base (address for store it)
		PUSHN		R3				; Parameter #2 = Video mode number by GOP
		PUSHN		R2				; Parameter #1 = Protocol pointer
;--- Call firmware function ---
		CALL32EXA	@R2,0,0				; GopTable.QueryMode
;--- Read and remove function parameters ---
		POPN		R2
		POPN		R3
		POPN		R4
		POPN		R5
;--- Read and remove scratch pads ---
		POP64		R4				; R4 = Info size (updated by function)
		POP64		R5				; R5 = Info base (updated by function)
;--- Restore mode limit ---
		POP64		R6
;--- Check status ---
		MOVSNW		R7,R7
		CMPI64WEQ	R7,0
		JMP8CC		CVM_1				; Exit if error returned
;--- Check info size validity ---
		CMPI64WUGTE	R4,12
		JMP8CC		CVM_2				; Exit if output data too small
;--- Verify Ysize ---
		MOVDW		R4,@R5,0,8			; R4 = Ysize for this mode
		CMPI32WUGTE	R4,Ymin
		JMP8CC		CVM_3				; Skip this mode if Xsize < Xmin
		CMPI32WULTE	R4,Ymax
		JMP8CC		CVM_3				; Skip this mode if Xsize > Xmax
;--- Verify Xsize, after Ysize because R4 used ---
		MOVDW		R4,@R5,0,4			; R4 = Xsize for this mode
		CMPI32WUGTE	R4,Xmin
		JMP8CC		CVM_3				; Skip this mode if Xsize < Xmin
		CMPI32WULTE	R4,Xmax
		JMP8CC		CVM_3				; Skip this mode if Xsize > Xmax
;--- Select optimal mode with current and previous ---
		MOVWW		R7,@R1,0,_Video_Xsize		; R7 = Xideal, R4 = Xcurrent
		MOVWW		R5,@R1,0,_Video_Xdelta		; R5 = Xideal-Xcurrent by previous settings (this storage temp. used)
		SUB32		R7,R4				; R7=R7-R4, Xideal - Xcurrent
		CMPI32WGTE	R7,0
		JMP8CC		CVM_3				; Go skip current if Xideal < Xcurrent
		CMP32UGTE	R7,R5
		JMP8CS		CVM_3				; Go skip current if this is not a better approximation

;--- Replace by new video mode ---
		MOVDW		@R1,0,_Program_Video_Mode,R3
		MOVWW		@R1,0,_Video_Xdelta,R7		; Update Xexist - Xideal (this storage temp. used)

;--- Cycle for check all supported GOP modes ---
CVM_3:		MOVSNW		R3,R3,1				; R3+1, try next mode
		CMP64UGTE	R3,R6				; R3 = Current, R6 = Limit
		JMP8CC		CVM_0

;--- Exit points ---
		XOR64		R7,R7				; Set status for no errors
CVM_1:		RET						; Here R7 = Status
CVM_2:		MOVIQW		R7,0FFh				; Set status for unknown errors
		RET

