;--- Built string: System clocks -------------------------------------------;
; TSC clock measured relative RTC if x86 platform detected (IA32 or x64).   ;
; CAP (CPU Architectural Protocol) timer period read by CAP.                ;
; This parameters measured and read during check configuration phase.       ;
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

Info_CPU_and_CAP_Clocks:
;--- Check this string availability ---
		MOVIQW		R2,0180h		; Bits D[8,7] = CAP, TSC
		CALL32		ICVS_11
		JMP8CC		ICCC_0			; Go if string available
		RET					; Return, if control here, R2=0, used for output
;--- Built left entry, parameter name ---
ICCC_0:		MOVIQW		R2,_String_Clocks	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Start built right entry ---
		PUSH64		R3
;--- Check TSC result valid ---
		MOVIQW		R2,0080h			; Info_Bitmap.7 = TSC result validity
		CALL32		ICVS_11
		MOVSNW		R2,R1,Scratch_Pad_Size-128	; R2=Pointer for built string
		JMP8CS		ICCC_1				; Go if TSC data not valid
;--- Built TSC string ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_TSC
		CALL32		Copy_String			; Copy include last 0
		MOVQ		R2,R3				; R2=Buffer pointer
		MOVQW		R3,@R1,0,_TSC_Period		; R3=femtoseconds
		CALL32		String_Fs_MHz
;--- Insert spaces after TSC / before CAP string  ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_Interval
		CALL32		Copy_String
		MOVQ		R2,R3
;--- Check CAP result valid ---
ICCC_1:		PUSH64		R2
		MOVIQW		R2,0100h			; Info_Bitmap.8 = CAP result validity
		CALL32		ICVS_11
		POP64		R2
		JMP8CS		ICCC_2				; Go if CAP data not valid
;--- Built CAP string ---
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_CAP
		CALL32		Copy_String			; Copy include last 0
		MOVQ		R2,R3				; R2=Buffer pointer
		MOVQW		R3,@R1,0,_CAP_Period		; R3=femtoseconds
		CALL32		String_Fs_MHz
;--- Terminate string, restore R3, go to set offset, built right part, set Y addend, return ---
; Here R2 must be valid destination pointer
;---
ICCC_2:		MOVIBW		@R2,0
		POP64		R3
		JMP8		ICSF_3

