;--- Execute benchmarks: Native Fill video benchmarks, SSE -----------------;
; Video benchmarks pass#6:                                                  ;
; Operation = Fill video memory with constant pattern.                      ;
; Method    = Native CPU instructions.                                      ;
; Visualized result is built monotonous rectangles.                         ;
; For x86 machines (IA32 and x64) native Fill and Copy use SSE instruct.    ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  Status not used, because it saved at result variables            ;
;          R2 = Current Y position offset addend, here must be 0            ;
;          R3 = Updated destination string pointer, here must be saved      ;
;                                                                           ;
;          Result variables:                                                ;
;          Bit Info_Bitmap.29 = validity flag for benchmarks result         ;
;          Benchmarks results stored at Scratch_Pad multifunctional buffer, ;
;          6 subroutines use 128 bytes at R1 + [0A80h-0AFFh], this store:   ;
;          QWORD R1+0AC0h = Time per 1 frame, femtoseconds (10^-15 sec)     ;
;          QWORD R1+0AC8h = Time per 1 kilobyte, femtoseconds (10^-15 sec)  ;
;                                                                           ;
;          Transit variable (unified for all passes #1-8):                  ;
;          QWORD R1+0AE0h = CAP timer value at benchmark start, clocks      ;
;---------------------------------------------------------------------------;

Execute_Benchmarks_6:

	;*	JMP8		EB1_0
		JMP32		EB1_0



