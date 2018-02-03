;--- Built string: UEFI firmware revision ----------------------------------;
; This parameter get from EFI System Table during check config. phase.      ;
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

Info_UEFI_Firmware_Revision:
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_Firmware	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Prepare value ---
		PUSH64		R3
		PUSH64		R4
		MOVNW		R2,R1,0,Scratch_Pad_Size-128	; R2=Pointer for built string
		MOVWW		R3,@R1,0,_Firmware_Revision+2	; R3=Major word
		XOR64		R4,R4				; R4=Template
		CALL32		String_Decimal32		; Built ASCII for major value
		MOVIBW		@R2,'.'				; Write "."
		MOVSNW		R2,R2,1				; Modify pointer, R2+1
		MOVWW		R3,@R1,0,_Firmware_Revision+0	; R3=Minor word
		CALL32		String_Decimal32		; Built ASCII for minor value
;--- Go to terminate string, built right entry (parameter value), set Y addend and retrn ---
; Here for target fragment must be R4=0
;---
		JMP8		IUSR_1
