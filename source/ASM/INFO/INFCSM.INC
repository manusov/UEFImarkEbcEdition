;--- Built string: Compatibility Support Module presence -------------------;
; This is result of detection Legacy BIOS Protocol during check cfg. phase. ;
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

Info_CSM_Presence:
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_CSM		; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Prepare value ---
		MOVIQW		R7,0002h		; Bit D1=1
		AND32		R7,@R1,0,_Info_Bitmap	; Masking
		CMPI32WEQ	R7,0			; Make CF = f(masking)
		MOVIQW		R2,_String_Not_Found
		JMP8CS		ICP_0			; Go if CSM not found
		MOVIQW		R2,_String_Found
;--- Set coordinates + color and write ---
ICP_0:		MOVIQQ		R5,00F0F000000000A8h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Go to set Y addend and return ---
ICP_1:		JMP8		IUSR_0