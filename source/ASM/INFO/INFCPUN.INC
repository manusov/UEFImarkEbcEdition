;--- Built string: Native processor name string ----------------------------;
; This parameter read by native mode gate during check config. phase.       ;
; For x86 processors (IA32, x64) use CPUID instruction functions            ;
; 80000002h, 80000003h, 80000004h.                                          ;
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

Info_CPU_Name_String:
;--- Check this string availability ---
		MOVIQW		R2,0020h		; Bit D5 = CPU name string availability
		CALL32		ICVS_11
		JMP8CC		ICNS_0			; Go if string available
		RET					; Return, if control here, R2=0, used for output
;--- Built left entry, parameter name ---
ICNS_0:		MOVIQW		R2,_String_CPU_Name	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Get and reordering vendor string, use string saved at scratch pad ---
		MOVSNW		R2,R1,Scratch_Pad_Size-256+40h	; R2=Source
		MOVSNW		R5,R2				; R5=Destination
		XOR64		R6,R6				; R6=Counter
;--- Skip left spaces at CPU name string ---
ICNS_1:		MOVB		R7,@R2				; Load char from source
		CMPI32WEQ	R7,0
		JMP8CS		ICNS_3				; Go end if char=0
		MOVSNW		R2,R2,1				; Source address+1
		CMPI32WEQ	R7,' '
		JMP8CS		ICNS_1				; Go cycle for skip left spaces
		MOVSNW		R2,R2,-1			; Source address-1, for first char not ' '
;--- Copy string ---
ICNS_2:		MOVB		R7,@R2				; Load char from source
		CMPI32WEQ	R7,0
		JMP8CS		ICNS_3				; Go end if char=0
		MOVB		@R5,R7				; Store char to destination
		MOVSNW		R2,R2,1				; Source address+1
		MOVSNW		R5,R5,1				; Destination address+1
		MOVSNW		R6,R6,1				; Counter+1
		CMPI32WULTE	R6,48				; Maximum length = 48 bytes (3*16)
		JMP8CS		ICNS_2				; Cycle for chars in the string
ICNS_3:		XOR32		R7,R7				; R7=0
		MOVB		@R5,R7				; Write terminator = 0
;--- Set offset, go to built right part entry, set Y addend and return ---
		MOVIQW		R2,Scratch_Pad_Size-256+40h	; R2=Source, here relative R1
ICNS_4:		JMP8		ICVS_1

