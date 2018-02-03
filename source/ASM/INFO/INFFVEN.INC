;--- Built string: UEFI firmware vendor ------------------------------------;
; Pointer to this string get from EFI System Table during check cfg. phase. ;
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

Info_UEFI_Firmware_Vendor_String:
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_Firmware_Vendor	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Prepare value ---
		PUSH64		R3
		PUSH64		R4
		MOVQW		R2,@R1,0,_Firmware_Vendor	; R2=Pointer to vendor name
		MOVNW		R3,R1,0,Scratch_Pad_Size-128	; R3=Pointer for built string
		CALL32		Convert_String_2		; Get string, save at ASCII
		MOVQ		R3,R2				; This for right termination
;--- Go to terminate string, built right entry (parameter value), set Y addend and retrn ---
; Here for target fragment must be R4=0
; This point used from some other subroutines
;---
IUFVS_1:	XOR64		R4,R4
		JMP8		IUSR_1
