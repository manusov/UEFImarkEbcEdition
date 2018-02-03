;--- Execute benchmarks: UEFI API Block Fill video benchmarks --------------;
; Video benchmarks pass#3:                                                  ;
; Operation = Fill video memory with constant pattern.                      ;
; Method    = UEFI API, function Memory Fill.                               ;
; Visualized result is built monotonous rectangles.                         ;
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
;          Bit Info_Bitmap.26 = validity flag for benchmarks result         ;
;          Benchmarks results stored at Scratch_Pad multifunctional buffer, ;
;          6 subroutines use 128 bytes at R1 + [0A80h-0AFFh], this store:   ;
;          QWORD R1+0AA0h = Time per 1 frame, femtoseconds (10^-15 sec)     ;
;          QWORD R1+0AA8h = Time per 1 kilobyte, femtoseconds (10^-15 sec)  ;
;                                                                           ;
;          Transit variable (unified for all passes #1-6):                  ;
;          QWORD R1+0AE0h = CAP timer value at benchmark start, clocks      ;
;---------------------------------------------------------------------------;

Execute_Benchmarks_3:

		JMP8		EB1_0

;*** DEBUG ***

		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
		CALL32		Setup_Video_Mode

	;*	BREAK 3


;--- Prepare parameters ---
		MOVQW		R7,@R1,0,_Primary_Memory_Base	; R7 = Block base address, use allocated
		MOVIQW		R6,0020h			; R6 = Block size, use constant 32 Bytes
		MOVIQW		R5,0011h			; R5 = Data value for block fill
;--- Built input parameters frame ---
		PUSHN		R5				; Parameter #3 = Data for fill
		PUSHN		R6				; Parameter #2 = Block size
		PUSHN		R7				; Parameter #1 = Block address
;--- Read pointer and handler call ---
		MOVNW		R4,@R1,0,_EFI_Table		; R4 = SysTable
		MOVNW		R4,@R4,9,24			; R4 = BootServices (entry=9, header size=24)
		CALL32EXA	@R4,42,24			; Call SetMem (entry=42, header size=24)
;--- Remove input parameters frame ---
		POPN		R7				; Parameter #1
		POPN		R6				; Parameter #2
		POPN		R5				; Parameter #3


;--- Exit ---
		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		JMP8		EB1_0

