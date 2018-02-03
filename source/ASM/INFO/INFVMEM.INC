;--- Built string: Video memory info, by GOP and by Hardware ---------------;
; This parameters detected during check config. phase and after video mode  ;
; set. Note used video memory size for given GOP mode and video memory      ;
; size hardware detection available only after mode set.                    ;
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

Info_Video_Memory:
;--- Check this string availability ---
		MOVIQW		R2,0400h			; Bit D10 = Video memory access available
		CALL32		ICVS_11
		JMP8CC		IVM_1				; Go if string available
		RET						; Return, if control here, R2=0, used for output
;--- Built left entry, parameter name ---
IVM_1:		MOVIQW		R2,_String_Video_Memory		; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Start built right entry ---
		PUSH64		R3
		PUSH64		R4
;--- Video memory size, detected by GOP ---
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string
		MOVSNW		R2,R1,_String_G_Mem		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
		MOVDW		R3,@R1,0,_Video_Memory_Size_GOP	; R3=GOP video memory size, Bytes
		MOVIQW		R5,10				; R5=Shifts count
		SHR64		R3,R5				; R3=GOP video memory size, KB
		XOR64		R4,R4				; R4=0, template for decimal print
		CALL32		String_Decimal32		; Built ASCII for GOP video memory size
;--- Check hardware scan results availability ---
		PUSH64		R2
		MOVIQW		R2,0800h			; Bit D11 = Video memory hardware scan available
		CALL32		ICVS_11
		POP64		R2
		JMP8CS		IVM_0				; Go if results not available
;--- Video memory size, detected by hardware scan ---
		MOVQ		R3,R2				; R3=Pointer for built string
		MOVSNW		R2,R1,_String_H_Mem		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
		MOVDW		R3,@R1,0,_Video_Memory_Size_HW	; R3=Hardware video memory size, Bytes
		SHR64		R3,R5				; R3=Hardware video memory size, KB
		CALL32		String_Decimal32		; Built ASCII for GOP video memory size
		MOVQ		R3,R2				; R3=Pointer for built string
		MOVSNW		R2,R1,_String_Or_Above		; R2=Pointer to additional string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
;--- Go to terminate string, restore R3, go to set offset, built right part, set Y addend, return ---
; Here R2 must be valid destination pointer
;---
IVM_0:		JMP32		IVMP_0

