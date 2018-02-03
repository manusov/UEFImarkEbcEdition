;--- Built string: Native processor supported features list ----------------;
; This parameter read by native mode gate during check config. phase.       ;
; For x86 processors (IA32, x64) use CPUID instruction functions            ;
; 00000001h, 80000001h, 00000007h                                           ;
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

Info_CPU_Supported_Features:

;--- Check this string availability ---
		MOVIQW		R2,0040h			; Bit D6 = CPU features string availability
		CALL32		ICVS_11
		JMP8CC		ICSF_0				; Go if string available
		RET						; Return, if control here, R2=0, used for output

;--- Sequence for first before AVX512 ---
ICSF_0:		MOVIQW		R2,_String_CPU_Features		; R2=Source pointer, signed offset relative R1
		CALL32		ICSF_50
		CALL32		ICSF_51
		RET

;--- Built left entry, parameter name ---
ICSF_50:	MOVIQQ		R5,00C0C0C000000000h		; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
		RET

;--- Start built CPU features list, recognize CPU vendor ---
ICSF_51:	MOVSNW		R2,R1,Scratch_Pad_Size-256  ; R2 = Pointer to vendor string, re-ordered by INFCPUN.INC
		MOVSNW		R5,R1,_Sign_CPU			; R5 = Pointer to vendor string patterns
		XOR64		R6,R6				; R6 = Counter
ICSF_1:		MOVQ		R7,@R2
		CMP64EQ		R7,@R5
		JMP8CC		ICSF_2				; Go next if pattern mismatch
		MOVDW		R7,@R2,0,8
		CMP32EQ		R7,@R5,0,8
		JMP8CS		ICSF_4				; Go if pattern match
ICSF_2:		MOVSNW		R5,R5,12			; Pattern pointer+12
		MOVSNW		R6,R6,1				; Counter+1
		CMPI32WULTE	R6,NUMBER_OF_CPU_VENDORS
		JMP8CS		ICSF_1
;--- CPU vendor not recognized, built empty features ---
		MOVSNW		R2,R1,Scratch_Pad_Size-128	; R2 = Destination pointer
		MOVIWW		@R2,'?'
		JMP8		ICSF_3
;--- CPU vendor recognized, built CPU features string ---
ICSF_4:		;* PUSH64	R3
		MOVSNW		R2,R1,_Selector_CPU_Features	; R2=Control tables directory pointer, relative
		ADD64		R6,R6				; Scale R6*2
		ADD64		R2,R6				; Add for select CPU vendor
		EXTNDW64	R2,@R2				; R2=Control entry pointer, relative
		ADD64		R2,R1				; R2=Control table pointer, absolute
ICSF_52:
		PUSH64		R3  ;*
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string

;--- Interpreting control table, upgraded for AVX2/AVX512 with 16-bit tags at v0.25 ---
ICSF_20:	MOVW		R7,@R2				; R7=Load tag word from control table entry
		CMPI32DEQ	R7,0FFFFh			; Check for value 0FFFFh=Terminator
		JMP8CS		ICSF_14				; Go if terminator detected, end of list
		MOVIQD		R6,0000F000h			; R6=Mask for bits D[15-12] = String type
		AND64		R6,R7				; Bit7 = String type: 1=conditional or 0=unconditional
		CMPI64WEQ	R6,0				; Make CF = f(compare result)
		JMP8CS		ICSF_22				; Go if unconditional string
;--- Select Standard, Extended or Structured CPUID ---
		MOVIQW		R6,0F00h			; R6=Mask for D[11-8] = CPUID selector
		AND64		R6,R7				; Bit6 = CPUID type: 0=Standard, 1=Extended
		MOVIQW		R7,Scratch_Pad_Size-256+10h	; Pointer to standard features list: EAX,EBX,ECX,EDX
		CMPI64WEQ	R6,0000h			; Make CF = f(compare result)
		JMP8CS		ICSF_21				; Go if Standard CPUID, check CF
		MOVIQW		R7,Scratch_Pad_Size-256+30h	; Pointer to extended features list: EAX,EBX,ECX,EDX
		CMPI64WEQ	R6,0100h			; Make CF = f(compare result)
		JMP8CS		ICSF_21				; Go if Extended CPUID, check CF
		MOVIQW		R7,Scratch_Pad_Size-256+70h	; Otherwise Pointer to Structured CPUID
ICSF_21:	ADD64		R7,R1				; Add buffer base address
;--- Select register in the CPUID results ---
		PUSH64		R5
		MOVW		R6,@R2				; R6=Reload tag word
		MOVIQW		R5,0060h
		AND64		R6,R5				; Masking used bits of register selector
		MOVIQW		R5,3
		SHR32		R6,R5				; Shift for bitfield indexing 32-bit dwords
		ADD64		R7,R6				; Add dword offset, register select EAX,EBX,ECX,EDX
;--- Prepare bitmask for select bit in the register ---
		MOVW		R5,@R2				; R5=Reload tag word
		MOVIQW		R6,001Fh			; R6=Mask for shifts count, bits [4-0]
		AND64		R5,R6				; R7=Shifts count
		MOVIQW		R6,0001h			; Pattern for shift
		SHL64		R6,R5				; Shift pattern for generating R6 = bitmask
		POP64		R5				; Restore R7=Pointer f(function, register)
;--- Check selected bit, skip string write if condition=false ---
		MOVD		R7,@R7			; Load bitmap from CPUID results
		AND64		R6,R7			; R6=Bitmask, R7=Bitmap
		CMPI64WEQ	R6,0			; Make CF = f(compare result)
		JMP8CS		ICSF_23			; Go if selected bit=0
;--- Write string if condition=true ---
ICSF_22:	PUSH64		R2			; Save control table pointer
		EXTNDW64	R2,@R2,0,2		; R2=Pointer to string, relative
		ADD64		R2,R1			; R2=Pointer to string, absolute		
		CALL32		Copy_String
		MOVIBW		@R3,' '			; Write interval space
		MOVSNW		R3,R3,1			; R3+1, advance built string pointer
		POP64		R2			; Restore control table pointer
;--- Skip string ---
ICSF_23:	MOVSNW		R2,R2,4			; Tag word (2) + Relative Pointer (2)
		JMP8		ICSF_20			; Cycle for checked features list
;--- End of upgraded for AVX2/AVX512 with 16-bit tags at v0.25 ---

;--- End of sequence ---
ICSF_14:	MOVIBW		@R3,0
		POP64		R3
;--- Set offset, go to built right part entry, set Y addend and return ---
ICSF_3:		MOVIQW		R2,Scratch_Pad_Size-128		; R2=Source, here relative R1
		JMP32		ICNS_4

