;--- Built string: Currently used video mode info --------------------------;
; Xsize, Ysize get by GOP mode info tables. Bits Per Pixel (BPP) = 32       ;
; for all GOP modes.                                                        ;
; This parameters detected during check config. phase and after video mode  ;
; set. Note some GOP video mode parameters available only after mode set.   ;
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

Info_Video_Mode_Parameters:
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_Graphics	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Start built right entry ---
		PUSH64		R3
		PUSH64		R4
;--- Video mode number ---
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string
		MOVSNW		R2,R1,_String_GOP_Mode
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
		MOVDW		R3,@R1,0,_Program_Video_Mode	; R3=Video mode number
		XOR64		R4,R4				; R4=0, template for decimal print
		CALL32		String_Decimal32		; Built ASCII for video mode number
;--- Xsize ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_Xsize
		CALL32		Copy_String
		MOVQ		R2,R3
		MOVWW		R3,@R1,0,_Video_Xsize		; R3=Video Xsize, pixels
		CALL32		String_Decimal32		; Built ASCII for Xsize
;--- Ysize ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_Ysize
		CALL32		Copy_String
		MOVQ		R2,R3
		MOVWW		R3,@R1,0,_Video_Ysize		; R3=Video Ysize, pixels
		CALL32		String_Decimal32		; Built ASCII for Ysize
;--- BPP (Bits per Pixel) ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_BPP
		CALL32		Copy_String
		MOVQ		R2,R3
		MOVBW		R3,@R1,0,_Video_BPP		; R3=Video BPP, pixels
		CALL32		String_Decimal32		; Built ASCII for BPP
;--- Go to terminate string, restore R3, go to set offset, built right part, set Y addend, return ---
; Here R2 must be valid destination pointer
;---
IVMP_0:		POP64		R4
		JMP8		ICCC_2

