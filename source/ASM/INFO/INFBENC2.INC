;--- Built string: MemFill, MemCopy video benchmarks results ---------------;
; Built text strings for video benchmarks passes #3-4 results:              ;
; Pass#3 use MemFill API with video memory for draw monotonous rectangle,   ;
; Pass#4 use MemCopy API with video mem. for draw rect. with pict. sprite.  ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Source sequence pointer, absolute 64-bit address            ;
;          R3 = Destination string pointer, absolute 64-bit address         ;
;          R4 = Current Y position offset at bits [15-00], other bits = 0   ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
;          Benchmarks results data at scratch pad buffer,                   ;
;          see modules EXEBENC3.INC, EXEBENC4.INC                           ;
;                                                                           ;
; OUTPUT:  Status not used, because this is only string built handler       ;
;          R2 = Current Y position offset addend                            ;
;          R3 = Updated destination string pointer, absolute 64-bit address ;
;               String located at [input R3], string is nul-terminated,     ;
;               but sequence termination executed by caller before show     ;
;---------------------------------------------------------------------------;

Info_Benchmarks_2:

		XOR64		R2,R2
		RET

