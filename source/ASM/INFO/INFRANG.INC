;--- Built string: Video memory attributes ---------------------------------;
; This parameters read by UEFI API during check configuration phase.        ;
; This parameters can be re-programming during set context phase.           ;
; For x86 machines (IA32 and x64) use MTRR attributes.                      ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Source sequence pointer, absolute 64-bit address            ;
;          R3 = Destination string pointer, absolute 64-bit address         ;
;          R4 = Current Y position offset at bits [15-00], other bits = 0   ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  Status not used, because this is only string built handler       ;
;          R2 = Current Y position offset addend                            ;
;          R3 = Updated destination string pointer, absolute 64-bit address ;
;               String located at [input R3], string is nul-terminated,     ;
;               but sequence termination executed by caller before show     ;
;---------------------------------------------------------------------------;

Info_Video_Memory_Attributes:
		MOVQW		R7,@R1,0,_Sys_Bitmap
		MOVIQW		R6,0800h			; D11=MTRR
		AND32		R7,R6
		CMPI32WEQ	R7,0
		JMP8CS		IVMA_4				; Go if MTRR not supported
;--- Check MTRR supported and detected ---
		MOVDW		R6,@R1,0,_MTRR_Used
		CMPI32WEQ	R6,0				; 0 means not supported
		JMP8CC		IVMA_0				; Go if MTRR supported and used
IVMA_4:		XOR64		R2,R2				; Set empty string
		RET
;--- Built left entry, parameter name ---
IVMA_0:		MOVIQW		R2,_String_MTRR			; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Start built right entry ---
		PUSH64		R3
		PUSH64		R4
;--- Assign destination address ---
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string
;--- Check MTRR used (vacant MTRR found) ---
		CMPI32WEQ	R6,1				; 1 means supported but no vacant MTRR
		JMP8CC		IVMA_1				; Go if MTRR used (vacant found)
		MOVSNW		R2,R1,_String_MTRR_N		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2 = Valid destination pointer
		JMP8		IVMA_2				; Go to handling this string
;--- Get and mask base address ---
IVMA_1:		CALL32		IVMA_20				; Accept R6=MSR Address, Return R7=Data
		PUSH64		R7				; (1)
		MOVIQQ		R5,00000000FFFFF000h
		AND64		R7,R5
		PUSH64		R7				; (2)
;--- Write base address ---
		PUSH64		R2				; R2 = Source pointer must be saved
		MOVQ		R2,R3				; R2 = Destination pointer for subroutine
		MOVQ		R3,R7				; R3 = Data for output
		CALL32		EBC_String_Hex64		; Built ASCII for Base Address
		MOVQ		R3,R2				; R3 = Destination pointer, updated after write
		POP64		R2
;--- Write middle string, here R2=Source, R3=Destination ---
		CALL32		Copy_String
;--- Get end address ---
		MOVSNW		R6,R6,1				; R6+1, modify MSR address
		CALL32		IVMA_20				; Accept R6=MSR Address, Return R7=Data
;--- Mask unused bits, D11=MTRR valid, D[10-0]=0 ---
		MOVIQW		R5,0F000h			; R5 = 0FFFFFFFFFFFFF000h
		AND64		R7,R5				; Clear non-address bits
;--- Invert number of bits N=address size) ---
		MOVIQW		R5,0FFFFh			; R5 = 0FFFFFFFFFFFFFFFFh
		MOVBW		R6,@R1,0,_CPU_Phys_Addr_Width	; R6 = Physical address width, bits
		SHL64		R5,R6				; R5 = Inverted bitmask for address
		NOT64		R5,R5				; R5 = Bitmask for address
		XOR64		R7,R5				; Invert address bits
;--- Add base address ---
		POP64		R5				; (2)
		OR64		R7,R5
;--- Write end address ---
		PUSH64		R2				; R2 = Source pointer must be saved
		MOVQ		R2,R3				; R2 = Destination pointer for subroutine
		MOVQ		R3,R7				; R3 = Data for output
		CALL32		EBC_String_Hex64		; Built ASCII for Base Address
		MOVQ		R3,R2				; R3 = Destination pointer, updated after write
		POP64		R2
;--- Write middle string, here R2=Source, R3=Destination ---
		CALL32		Copy_String
;--- Write memory type ---
		POP64		R7				; (1)
		MOVIQW		R6,0FFh
		AND64		R7,R6				; Mask type
		MOVIQW		R6,6				; R6 = Limit
		CMP32ULTE	R7,R6				; R7 = Value
		JMP8CS		IVMA_3
		MOVIQW		R7,2
IVMA_3:		CALL32		Copy_Selected_String
		MOVQ		R2,R3
;--- Go to terminate string, restore R3, go to set offset, built right part, set Y addend, return ---
; Here R2 must be valid destination pointer
;---
IVMA_2:		JMP32		ISM_0


;--- Local subroutine: gate for Read MSR ---
; INPUT:   R1 = Global variables pool base address
;          R6 = MSR index
; OUTPUT:  R7 = MSR data after Read
;---
IVMA_20:	XOR64		R7,R7				; Storage=0
		PUSH64		R7				; Storage for output
		MOVQ		R7,R0				; Address of storage = stack pointer
		PUSHN		R7				; Parameter#2 = Output address
		PUSHN		R6				; Parameter#1 = MSR address
		MOVINW		R7,1,0				; R7=4 if 32-bit, R7=8 if 64-bit
		CMPI32WEQ	R7,4				; 4 bytes = 32 bits , 8 bytes = 64 bit
		MOVIQW		R7,_IA32_Read_MSR		; This pointer for IA32 (native width=4)
		JMP8CS		IVMA_21				; Go if 32-bit
		MOVIQW		R7,_x64_Read_MSR		; This pointer for x64 (native width=8)
IVMA_21:	ADD64		R7,R1				; Add base address (R1) to offset (R7)
		CALL32EXA	R7				; Call native subroutine
		POPN		R6				; Remove parameter#1
		POPN		R7				; Remove parameter#2
		POP64		R7				; R7 = Output, remove storage
		RET

