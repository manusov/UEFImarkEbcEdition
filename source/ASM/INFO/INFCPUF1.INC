; *** UNSTABLE LOCKED, YET AVX2 ONLY VERIFIED ***

;--- Built string: Native processor supported features list ----------------;
; This parameter read by native mode gate during check config. phase.       ;
; For x86 processors (IA32, x64) use CPUID instruction functions            ;
; 00000001h, 80000001h, 00000007h                                           ;
;                                                                           ;
; This module additional for AVX512 support, 1 of 2                         ;
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

Info_CPU_Supported_Features_1:

;--- Check this string availability ---
		MOVIQW		R2,0040h			; Bit D6 = CPU features string availability
		CALL32		ICVS_11
		JMP8CC		ICSF_100			; Go if string available
		RET						; Return, if control here, R2=0, used for output

;--- Make extension for AVX512, group 1 of 2 ---
ICSF_100:	PUSH64		R7				; (+1) R7 = Used as scratch pad
		PUSH64		R3				; (+2) Save base dest. address, for restore if empty
		MOVIQW		R2,_String_CPU_Features_1
		CALL32		ICSF_50
		MOVSNW		R2,R1,_AVX512_CPU_Features_1
ICSF_101:
		PUSH64		R3		; (+3)
		CALL32		ICSF_52
		POP64		R7		; (-3)
		SUB64		R7,R3
		CMPI32DEQ	R7,-9		; Detect empty string condition
		POP64		R7		; (-2)
		JMP8CC		ICSF_102
		XOR64		R2,R2		; Delta Y = 0 if string empty
		MOVQ		R3,R7		; Destination pointer = start value if string empty
		
ICSF_102:	POP64		R7		; (-1) Restore R7 (redundant)
		RET
