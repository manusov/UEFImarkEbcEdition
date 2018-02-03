;--- Built string: GOP.BLT.Fill, GOP.BLT.Copy video benchmarks results -----;
; Built text strings for video benchmarks passes #1-2 results:              ;
; Pass#1 use GOP.BLT.Fill for draw monotonous rectangle,                    ;
; Pass#2 use GOP.BLT.Copy for draw rectangle with picture sprite.           ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Source sequence pointer, absolute 64-bit address            ;
;          R3 = Destination string pointer, absolute 64-bit address         ;
;          R4 = Current Y position offset at bits [15-00], other bits = 0   ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
;          Benchmarks results data at scratch pad buffer,                   ;
;          see modules EXEBENC1.INC, EXEBENC2.INC                           ;
;                                                                           ;
; OUTPUT:  Status not used, because this is only string built handler       ;
;          R2 = Current Y position offset addend                            ;
;          R3 = Updated destination string pointer, absolute 64-bit address ;
;               String located at [input R3], string is nul-terminated,     ;
;               but sequence termination executed by caller before show     ;
;---------------------------------------------------------------------------;

Info_Benchmarks_1:
;--- Check this string availability ---
		MOVIQD		R2,01000000h			; Bit D24 = Benchmark 1 result valid
		CALL32		ICVS_11
		JMP8CC		IB1_1				; Go if string available
		RET						; Return, if control here, R2=0, used for output
;--- Built left entry, parameter name ---
IB1_1:		MOVIQW		R2,_String_GOP_BLT_FPS		; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Start built right entry with calculations ---
		PUSH64		R3
		PUSH64		R4

; *** MAKE SUBROUTINE OR COMMON BRANCH FOR N STRINGS ***

		MOVSNW		R5,R1,0A80h			; R5 = Status entry address for this test, use 2 entries

;--- Video frames per second ---
		XOR64		R4,R4				; R4=0, template for decimal print
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string
		MOVSNW		R2,R1,_String_Fill		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
		MOVQ		R3,@R5				; R3=Time per 1 frame (BLT.Fill), femtoseconds
		CALL32		String_Fs_Hz
		MOVQ		R3,R2
		MOVSNW		R2,R1,_String_Copy		; R2=Pointer to parameter name string
		CALL32		Copy_String
		MOVQ		R2,R3				; R2=Pointer for built string
 		MOVQW		R3,@R5,0,10h			; R3=Time per 1 frame (BLT.Copy), femtoseconds
		CALL32		String_Fs_Hz

;--- Go to terminate string, restore R3, go to set offset, built right part, set Y addend, return ---
; Here R2 must be valid destination pointer
;---
		JMP32		IVM_0



