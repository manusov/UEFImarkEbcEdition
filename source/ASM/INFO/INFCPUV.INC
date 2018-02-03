;--- Built string: Native processor vendor string --------------------------;
; This parameter read by native mode gate during check config. phase.       ;
; For x86 processors (IA32, x64) use CPUID instruction function 00000000h.  ;
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

Info_CPU_Vendor_String:
;--- Check this string availability ---
		MOVIQW		R2,0010h		; Bit D4 = CPU vendor string availability
		CALL32		ICVS_11
		JMP8CC		ICVS_0			; Go if string available
		RET					; Return, if control here, R2=0, used for output
;--- Built left entry, parameter name ---
ICVS_0:		MOVIQW		R2,_String_CPU_Vendor	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Get and reordering vendor string, use string saved at scratch pad ---
		MOVSNW		R2,R1,Scratch_Pad_Size-256	; Pointers: R2=Source, R3=Destination
		MOVDW		R5,@R2,00,04
		MOVDW		R6,@R2,00,08
		MOVDW		R7,@R2,00,12
		MOVD		@R2,R5				; Store chars 1-4
		MOVSNW		R2,R2,4				; R2+4
		MOVD		@R2,R7				; Store chars 5-8
		MOVSNW		R2,R2,4				; R2+4
		MOVD		@R2,R6				; Store chars 9-12
		MOVSNW		R2,R2,4				; R2+4
		XOR32		R5,R5
		MOVD		@R2,R5
;--- Built right part entry ---
		MOVIQW		R2,Scratch_Pad_Size-256		; Pointers: R2=Source, R3=Destination
;--- This point used from some other routines ---
ICVS_1:		MOVIQQ		R5,00F0F000000000A8h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Set Y addend and exit ---
		MOVIQW		R2,16
		RET

;--- Local subroutine: check string availability ---
; Input:   R1 = Global variables pool base address
;          R2 = Bitmap for check variable DWORD:Info_Bitmap
; Output:  Condition Flag (CF):
;           0(CC) = String available, execute built string
;           1(CS) = String not available, skip built string
;          R2 = 0 if string not available, it used for output
;---
ICVS_11:
		AND32		R2,@R1,0,_Info_Bitmap
		CMPI32WEQ	R2,0
		RET
