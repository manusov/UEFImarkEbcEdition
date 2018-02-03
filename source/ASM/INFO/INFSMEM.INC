;--- Built string: System memory info, memory used by UEFI and ACPI --------;
; This parameters extracted from UEFI memory map, requested during check    ;
; config. phase.                                                            ;
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

Info_System_Memory:
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_System_Memory	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Start built right entry ---
		PUSH64		R3
		PUSH64		R4
;--- UEFI memory size ---
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string
		MOVSNW		R2,R1,_String_U_Mem		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
		MOVDW		R3,@R1,0,_EFI_Memory_Total	; R3=UEFI firmware memory size, KB
		XOR64		R4,R4				; R4=0, template for decimal print
		CALL32		String_Decimal32		; Built ASCII for UEFI memory size
;--- ACPI memory size ---
		MOVQ		R3,R2				; R3=Pointer for built string
		MOVSNW		R2,R1,_String_A_Mem		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3
		MOVDW		R3,@R1,0,_EFI_Memory_ACPI	; R3=UEFI ACPI-reserved memory size, KB
		CALL32		String_Decimal32		; Built ASCII for UEFI memory size
;--- Go to terminate string, restore R3, go to set offset, built right part, set Y addend, return ---
; Here R2 must be valid destination pointer
;---
ISM_0:		JMP8		IVMP_0

