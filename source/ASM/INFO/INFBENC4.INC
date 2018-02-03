;--- Built string: Native Fill, Native Copy video benchmarks results -------;
; Built text strings for video benchmarks pass #5-6 results:                ;
; Pass#5 use Native Fill with video memory for draw monotonous rectangle,   ;
; Pass#6 use Native Copy with video mem. for draw rect. with pict. sprite.  ;
; For x86 machines (IA32 and x64) native Fill and Copy use SSE instr.       ;
;                                                                           ;
; Difference from INFBENC3.INC: data visualized in megabytes per second.    ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Source sequence pointer, absolute 64-bit address            ;
;          R3 = Destination string pointer, absolute 64-bit address         ;
;          R4 = Current Y position offset at bits [15-00], other bits = 0   ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
;          Benchmarks results data at scratch pad buffer,                   ;
;          see modules EXEBENC5.INC, EXEBENC6.INC                           ;
;                                                                           ;
; OUTPUT:  Status not used, because this is only string built handler       ;
;          R2 = Current Y position offset addend                            ;
;          R3 = Updated destination string pointer, absolute 64-bit address ;
;               String located at [input R3], string is nul-terminated,     ;
;               but sequence termination executed by caller before show     ;
;---------------------------------------------------------------------------;

Info_Benchmarks_4:

		XOR64		R2,R2
		RET

