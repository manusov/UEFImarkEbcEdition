;--- Built string: Native processor data width -----------------------------;
; This parameter read by MOVINW instruction during check config. phase.     ;
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

Info_Native_Width:
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_CPU_Width	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Prepare value ---
		PUSH64		R3
		PUSH64		R4
		MOVNW		R2,R1,0,Scratch_Pad_Size-128	; R2=Pointer for built string
		MOVBW		R3,@R1,0,_CPU_Native_Width	; R3=Native width, bytes
		MOVIQW		R4,3				; Number of shifts for multiply by 8
		SHL64		R3,R4				; R3=Native width, bits
		XOR64		R4,R4				; R4=Template
		CALL32		String_Decimal32		; Built ASCII for data width value
;--- Check additional value (CPU physical address width) string availability ---
		PUSH64		R2
		MOVIQW		R2,0200h			; Bits D9 = Physical address width
		CALL32		ICVS_11
		POP64		R2
		JMP8CS		INW_0				; Go if string not available
;--- Additional value: CPU physical address width ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_Phys_Addr
		CALL32		Copy_String
		MOVQ		R2,R3
		MOVBW		R3,@R1,0,_CPU_Phys_Addr_Width
		CALL32		String_Decimal32		; Built ASCII for address width value
;--- Go to terminate string, write entry, set Y addend and exit ---
INW_0:		JMP8		IEIR_0
