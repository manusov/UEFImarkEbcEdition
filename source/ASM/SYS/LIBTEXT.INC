;===========================================================================;
;=                                                                         =;
;=          LIBRARY FOR TEXT CHARS AND STRINGS INPUT AND OUTPUT.           =;
;=                                                                         =;
;===========================================================================;

;--- Check input status and get pressed key, without wait key -----------;
; Use Simple Text Input Protocol.                                        ;
; This procedure return also if no keys pressed, check only.             ;
; Key code cleared from buffer, unlike BIOS INT 16h function 1.          ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status, 64-bit or sign-extended 32 to 64 bit        ;
;          R2 = EFI_KEY, valid if Status=No errors                       ;
;          R2 bits [63-32] = 0, cleared                                  ;
;------------------------------------------------------------------------;

Input_Check_Key:
		MOVNW		R2,@R1,0,_EFI_Table	; R3=SysTable
		MOVNW		R2,@R2,3,24		; R3=ConIn
		PUSHN		R2		; Storage for key code
		PUSHN		R0 	    	; Parameter #2 for protocol
		PUSHN		R2 	    	; Parameter #1 for protocol
		CALL32EXA	@R2,1,0
		POPN		R2		; Remove parameter #1
		POPN		R2		; Remove parameter #2
		POPN		R2		; Read key code
		EXTNDD64	R7,R7
		RET

;--- Check input status and get pressed key, with wait key --------------;
; Use Simple Text Input Protocol.                                        ;
; This procedure not return if no keys pressed, wait for key press.      ;
; Key code cleared from buffer, like BIOS INT 16h function 0.            ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0 = No errors, key ready, 1 = Error (reserved)                ;
;          R7 = UEFI Status                                              ;
;          R2 = EFI_KEY, valid if Status=No errors                       ;
;          R2 bits [63-32] = 0, cleared                                  ;
;------------------------------------------------------------------------;

Input_Wait_Key:
IWK_0:		CALL32		Input_Check_Key
		CMPI32WEQ	R7,0			; 0 if key ready
		JMP8CC		IWK_0			; Wait if no keys
		RET

;--- Output text string -------------------------------------------------;
; Use Simple Text Output Protocol, string must be in UNICODE16.          ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Pointer to string, flat 64                               ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;

Output_String:
		PUSH64		R3
		PUSH64		R2		; R2 for alignment only
		MOVNW		R3,@R1,0,_EFI_Table	; R3=SysTable
		MOVNW		R3,@R3,5,24		; R3=ConOut
;--- Start built frame ---
		PUSHN		R2 	    	; Parameter #2 for protocol
		PUSHN		R3 	    	; Parameter #1 for protocol
;--- Here R0 must be align 16 ---
; OK for x64 (N=8), need correct for IA32 (N=4) ---
;---
		CALL32EXA	@R3,1,0
;--- Externally used exit point ---
OS_0:		POPN		R3		; Remove parameter #1
		POPN		R2		; Remove parameter #2
		POP64		R2		; R2 for alignment only
		POP64		R3
		RET

;--- Set video attribute for next string writes -------------------------;
; Use Simple Text Output Protocol.                                       ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Video attribute                                          ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;

Output_Attribute:
		PUSH64		R3
		PUSH64		R2		; R2 for Align 16 only
		MOVNW		R3,@R1,0,_EFI_Table	; R3=SysTable
		MOVNW		R3,@R3,5,24		; R3=ConOut
;--- Start built frame ---
		PUSHN		R2 	    	; Parameter #2 for protocol
		PUSHN		R3 	    	; Parameter #1 for protocol
;--- Here R0 must be align 16 ---
; OK for x64 (N=8), need correct for IA32 (N=4) ---
;---
		CALL32EXA	@R3,5,0
		JMP8		OS_0

;--- Clear text screen, positioning cursor to X=0, Y=0  -----------------;
; Use Simple Text Output Protocol.                                       ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;

; *** UNDER TEST ***
; Where align, before built frame or before call ?
; Use special alignment rules for single parameter, frame SIZE must be aligned too ?
; Must be aligned AFTER push, this is frame start (R0 value).


Output_Clear:
		MOVNW		R3,@R1,0,_EFI_Table	; R3=SysTable
		MOVNW		R3,@R3,5,24		; R3=ConOut
;--- Start built frame ---
		PUSHN		R3 	    	; This for alignment
		PUSHN		R3 	    	; Parameter #1 for protocol
;--- Here R0 must be align 16 ---
; OK for x64 (N=8), need correct for IA32 (N=4) ---
;---
		CALL32EXA	@R3,6,0
		POPN		R3
		POPN		R3
		RET


;--- Output text string -------------------------------------------------;
; Use conversion ASCII->UNICODE16 and Simple Text Output Protocol,       ;
; string must be in ASCII.                                               ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, use buffer on it     ;
;          R2 = Relative pointer to string, signed offset relative R1    ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;

String_Write:
		PUSH64		R2
		PUSH64		R3
		ADD64		R2,R1			; R2=Source, add R1 to relative offset
		MOVQ		R3,R1			; R3=Destination, use scratch pad at R1
		CALL32		Convert_String_1	; Convert ASCII-UNICODE16
		MOVQ		R2,R1
;--- Here R0 must be align 16 ---
		CALL32		Output_String		; Write UNICODE16
		POP64		R3
		POP64		R2
		RET

;--- Convert string from ASCII to UNICODE16 -----------------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to nul-terminated source string, flat 64         ;
;          R3 = Pointer destination string, flat 64                      ;
;          Source string is ASCII (1 byte per char)                      ;
;          Destination string is UNICODE (2 bytes per char)              ;
;                                                                        ;
; OUTPUT:  R2 = Modified by read source string                           ;
;          R3 = Modified by write destination string                     ;
;------------------------------------------------------------------------;

Convert_String_1:
		PUSH64		R4
CS1_2:		MOVB		R4,@R2
		CMPI64WEQ	R4,0
		JMP8CS		CS1_0		; Go if end of string
		CMPI64WEQ	R4,0Ah
		JMP8CS		CS1_1		; Go if char=Line Feed
		CMPI64WEQ	R4,0Dh
		JMP8CS		CS1_1		; Go if char=Carriage Return
		CMPI64WLTE	R4,' '
		JMP8CS		CS1_1		; Go if legal char
		CMPI64WLTE	R4,'}'
		JMP8CS		CS1_1		; Go if legal char
		MOVIQW		R4,'.'
CS1_1:		MOVW		@R3,R4
		MOVSNW		R2,R2,1
		MOVSNW		R3,R3,2
		JMP8		CS1_2		; Cycle for all string
CS1_0:		MOVW		@R3,R4		; Write terminator=0
		POP64		R4
		RET

;--- Convert string from UNICODE16 to ASCII -----------------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to nul-terminated source string, flat 64         ;
;          R3 = Pointer destination string, flat 64                      ;
;          Source string is UNICODE (2 bytes per char)                   ;
;          Destination string is ASCII (1 byte per char)                 ;
;                                                                        ;
; OUTPUT:  R2 = Modified by read source string                           ;
;          R3 = Modified by write destination string                     ;
;------------------------------------------------------------------------;

Convert_String_2:
		PUSH64		R4
CS2_2:		MOVW		R4,@R2
		CMPI64WEQ	R4,0
		JMP8CS		CS2_0		; Go if end of string
		CMPI64WEQ	R4,0Ah
		JMP8CS		CS2_1		; Go if char=Line Feed
		CMPI64WEQ	R4,0Dh
		JMP8CS		CS2_1		; Go if char=Carriage Return
		CMPI64WLTE	R4,' '
		JMP8CS		CS2_1		; Go if legal char
		CMPI64WLTE	R4,'}'
		JMP8CS		CS2_1		; Go if legal char
		MOVIQW		R4,'.'
CS2_1:		MOVB		@R3,R4
		MOVSNW		R2,R2,2
		MOVSNW		R3,R3,1
		JMP8		CS2_2		; Cycle for all string
CS2_0:		MOVB		@R3,R4		; Write terminator=0
		POP64		R4
		RET

;--- Select string from strings sequence --------------------------------;
; INPUT:   R7 = Selector, number of strings to skip                      ;
;          R6 = Selector Limit, skip move if AL>AH                       ;
;          R2 = Source Buffer, pointer to nul-terminated strings pool    ;
;               Both memory pointers is flat 64-bit                      ;
;                                                                        ;
; OUTPUT:  R2 = Pointer to selected string                               ;
;------------------------------------------------------------------------;
String_Selector:
;--- Check limit ---
		CMP32ULTE	R7,R6
		JMP8CC		SS_0		; Go if Selector > Limit
;--- Skip N strings, N=Selector ---
		PUSH64		R6
		PUSH64		R7
SS_3:		CMPI32WEQ	R7,0		; Located here for support R7=0
		JMP8CS		SS_2		; Exit cycle of nul-terminated strings
SS_1:		MOVB		R6,@R2		; Get current char
		MOVSNW		R2,R2,1		; R2+1, modify pointer
		CMPI32WEQ	R6,0
		JMP8CC		SS_1		; Go search cycle if char not zero
		MOVSNW		R7,R7,-1	; R7-1, decrement selector
		JMP8		SS_3
SS_2:		POP64		R6
		POP64		R7
;--- Exit ---
SS_0:		RET

;--- Select string from strings sequence and copy it to destination -----;
; ASCII edition                                                          ;
; INPUT:   R1 = Global variables pool base address, use as base          ;
;          R7 = Selector, number of strings to skip                      ;
;          R6 = Selector Limit, skip move if R7>R6                       ;
;          R2 = Source Buffer, pointer to nul-terminated strings pool    ;
;          R3 = Pointer to destination string                            ;
;               Both memory pointers is flat 64-bit                      ;
;                                                                        ;
; OUTPUT:  R2 = Modified by read source string                           ;
;          R3 = Modified by write destination string                     ;
;------------------------------------------------------------------------;

Copy_Selected_String:
		CALL32		String_Selector
; No RET, continue in the next subroutine

;--- Copy ASCII string --------------------------------------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to nul-terminated source string, absolute 64     ;
;          R3 = Pointer destination string, absolute 64                  ;
;          Source and destination strings is ASCII (1 byte per char)     ;
;                                                                        ;
; OUTPUT:  R2 = Modified by read source string                           ;
;          R3 = Modified by write destination string                     ;
;------------------------------------------------------------------------;

Copy_String:
		PUSH64		R4
CS_2:		MOVB		R4,@R2
		MOVSNW		R2,R2,1		; R2+1
		CMPI64WEQ	R4,0
		JMP8CS		CS_0		; Go if end of string
		CMPI64WEQ	R4,0Ah
		JMP8CS		CS_1		; Go if char=Line Feed
		CMPI64WEQ	R4,0Dh
		JMP8CS		CS_1		; Go if char=Carriage Return
		CMPI64WLTE	R4,' '
		JMP8CS		CS_1		; Go if legal char
		CMPI64WLTE	R4,'}'
		JMP8CS		CS_1		; Go if legal char
		MOVIQW		R4,'.'
CS_1:		MOVB		@R3,R4
		MOVSNW		R3,R3,1		; R3+1
		JMP8		CS_2		; Cycle for all string
CS_0:		MOVB		@R3,R4		; Write terminator=0
		POP64		R4
		RET

;--- Write error or warning string by UEFI status code ------------------;
; Built string only, caller must visual by String_Write.                 ;
; String not terminated, caller must terminate it (for flexibility).     ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Pointer to destination string, flat 64                   ;
;          R7 = UEFI Status for decoding as text string,                 ;
;               converted to 64-bit                                      ;
;                                                                        ;
; OUTPUT:  R2 = Modified by write destination string                     ;
;          ASCII text string at [input R2]                               ;
;------------------------------------------------------------------------;

Strings_Errors:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
;--- Reload destination pointer ---
		MOVQ		R3,R2		; R3=Destination
;--- Select error, warning or unsupported code, mask R7 ---
		MOVIQQ		R2,8000000000000000h
		AND64		R2,R7
		CMPI64WEQ	R2,0		; Set Condition Flag = F(R7.63)
		MOVIQW		R2,00FFh
		AND64		R7,R2		; Mask bits R7.[7-0]
		JMP8CS		SE_3		; If bit R7.63=0, go WARNING mode
		MOVSNW		R4,R1,_EFI_Errors_Names
		CMPI32WULTE	R7,NUMBER_OF_EFI_ERRORS
		JMP8CC		SE_1		; Go skip if wrong ERROR number
		JMP8		SE_2		; Otherwise go execute
SE_3:		MOVSNW		R4,R1,_EFI_Warnings_Names
		CMPI32WULTE	R7,NUMBER_OF_EFI_WARNINGS
		JMP8CC		SE_1		; Go skip if wrong WARNING number
;--- Built left constant part ", status = " ---
SE_2:		MOVSNW		R2,R1,_Msg_Addend
		CALL32		Copy_String	; R2=Source, R3=Destination
;--- Cycle for select message, count nul-terminated strings ---
		MOVQ		R2,R4		; R2=Pointer
		CMPI32WEQ	R7,0		; Check status=0
		JMP8CS		SE_4		; Skip scan if argument=0
SE_0:		MOVB		R4,@R2		; Load current char to R4
		MOVSNW		R2,R2,1		; R2+1, increment chars pointer
		CMPI32WEQ	R4,0		; Check char is nul
		JMP8CC		SE_0		; Cycle if char not nul
		MOVSNW		R7,R7,-1	; R7-1, decrement string number
		CMPI32WEQ	R7,0		; Check count is done
		JMP8CC		SE_0		; Cycle if count not done
;--- Built right variable part: error name ---
SE_4:		CALL32		Copy_String	; R2=Source, R3=Destination
;--- Set output parameter ---
SE_1:		MOVQ		R2,R3		; R2=Modified destination pointer
;--- Restore registers and return ---
		POP64		R4
		POP64		R3
		RET

;--- Output 32-bit unsigned decimal number as ASCII text string ---------;
;                                                                        ;
; INPUT:   R2 = Pointer to destination string, flat 64                   ;
;          R3 = Number, valid bits 0-31                                  ;
;          R4 = Template size, chars, 0 means no template                ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

String_Decimal32:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
;--- Prepare start parameters ---
		EXTNDD64	R3,R3
		MOVIQQ		R5,1000000000
;--- Generate current digit ---
SD_0:		PUSH64		R3
		DIVU64		R3,R5
		MOVIQW		R6,0Fh
		AND64		R3,R6
		CMPI64WGTE	R4,10
		JMP8CS		SD_1
		CMPI64WEQ	R5,1
		JMP8CS		SD_1
		CMPI64WEQ	R3,0
		JMP8CS		SD_2
;--- Write ASCII char ---
SD_1:		MOVSNW		R3,R3,30h
		MOVB		@R2,R3
		MOVSNW		R2,R2,1		; R2+1
		MOVIQW		R4,11
SD_2:		POP64		R3
;--- Generate modulus for next digit ---
		MODU64		R3,R5
;--- Generate divisor for next digit ---
		MOVIQW		R6,10
		DIVU64		R5,R6
;--- Cycle for entire number ---
		MOVSNW		R4,R4,1		; R4+1
		CMPI64WEQ	R5,0
		JMP8CC		SD_0
;--- Restore registers and exit ---
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		RET

;--- Output 64-bit hex number as ASCII text string ----------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to string                                        ;
;          R3 = Number, valid bits 0-63                                  ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

EBC_String_Hex64:
		PUSH64		R3
		PUSH64		R4
		MOVIQW		R4,32		; Shift count
		ASHR64		R3,R4
		CALL32		EBC_String_Hex32
		POP64		R4
		POP64		R3
; No RET, continue in the next subroutine

;--- Output 32-bit hex number as ASCII text string ----------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to string                                        ;
;          R3 = Number, valid bits 0-31                                  ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

EBC_String_Hex32:
		PUSH64		R3
		PUSH64		R4
		MOVIQW		R4,16		; Shift count
		ASHR64		R3,R4
		CALL32		EBC_String_Hex16
		POP64		R4
		POP64		R3
; No RET, continue in the next subroutine

;--- Output 16-bit hex number as ASCII text string ----------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to string                                        ;
;          R3 = Number, valid bits 0-15                                  ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

EBC_String_Hex16:
		PUSH64		R3
		PUSH64		R4
		MOVIQW		R4,08		; Shift count
		ASHR64		R3,R4
		CALL32		EBC_String_Hex8
		POP64		R4
		POP64		R3
; No RET, continue in the next subroutine

;--- Output 8-bit hex number as ASCII text string -----------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to string                                        ;
;          R3 = Number, valid bits 0-7                                   ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

EBC_String_Hex8:
		PUSH64		R3
		PUSH64		R4
		MOVIQW		R4,04		; Shift count
		ASHR64		R3,R4
		CALL32		EBC_String_Hex4
		POP64		R4
		POP64		R3
; No RET, continue in the next subroutine

;--- Output 4-bit hex number as ASCII text string -----------------------;
;                                                                        ;
; INPUT:   R2 = Pointer to string                                        ;
;          R3 = Number, valid bits 0-3                                   ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

EBC_String_Hex4:
		PUSH64		R3
		PUSH64		R4
		MOVIQW		R4,0Fh		; Select bits [3-0]
		AND64		R3,R4
		MOVIQW		R4,'0'		; 0-9 convert to 30h-39h
		CMPI32WLTE	R3,9
		JMP8CS		EBC_SH_0
		MOVIQW		R4,'A'-10	; A-F convert to 41h-46h
EBC_SH_0:	ADD64		R3,R4
		MOVB		@R2,R3		; Write ASCII char
		MOVSNW		R2,R2,1		; String pointer+1
		POP64		R4
		POP64		R3
		RET

;--- Convert femtoseconds (fs) to MHz and write as ASCII ----------------;
; String not terminated, for flexibility, string can be not a last,      ;
; terminator byte 00h must be added at [R2] by caller, if required.      ;
;                                                                        ;
; INPUT:   R2 = Pointer to destination string                            ;
;          R3 = Number (femtoseconds), valid bits 0-63                   ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

String_Fs_MHz:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
;--- Verify input, prepare parameters ---
		XOR64		R4,R4			; R4=Template=0=None
		MOVQ		R5,R3
		CMP64EQ		R5,R4			; Here R3=Period (fs), R4=0
		JMP8CS		SFM_1			; Go if error: divisor=0
		MOVIQQ		R3,1000000000000000	; R3=femtoseconds per Second
;--- Calculate and write integer part of Frequency, MHz ---
		DIVU64		R3,R5			; R5=Result frequency, Hz
		PUSH64		R3			; Save Result frequency, Hz
		MOVIQD		R5,1000000		; R5=Hz per MHz
		DIVU64		R3,R5			; R3=Result, MHz (integer)
		CALL32		String_Decimal32	; Note here required R4=0
;--- Write decimal point ---
		MOVIBW		@R2,'.'			; Write decimal point
		MOVSNW		R2,R2,1			; R2+1, advance string pointer
;--- Calculate and write float part of Frequency, MHz ---
		POP64		R3			; Restore to R3=Result, Hz
		MODU64		R3,R5			; R3=Remainder after divide by 1000000
		MOVIQD		R5,100000
		DIVU64		R3,R5			; R3=First digit of float part
SFM_2:		CALL32		String_Decimal32	; Note here required R4=0
;--- Restore registers and exit ---
SFM_0:		POP64		R5
		POP64		R4
		POP64		R3
		RET
;--- Handling errors: time value zero ---
SFM_1:		MOVIBW		@R2,'?'			; Write "?" char
		MOVSNW		R2,R2,1			; R2+1, advance string pointer		
		JMP8		SFM_0

;--- Convert femtoseconds (fs) to Hz and write as ASCII -----------------;
; String not terminated, for flexibility, string can be not a last,      ;
; terminator byte 00h must be added at [R2] by caller, if required.      ;
;                                                                        ;
; INPUT:   R2 = Pointer to destination string                            ;
;          R3 = Number (femtoseconds), valid bits 0-63                   ;
;                                                                        ;
; OUTPUT:  R2 = Modified pointer by string write, current position       ;
;------------------------------------------------------------------------;

String_Fs_Hz:
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
;--- Verify input, prepare parameters ---
		XOR64		R4,R4			; R4=Template=0=None
		MOVQ		R5,R3
		CMP64EQ		R5,R4			; Here R3=Period (fs), R4=0
		JMP8CS		SFM_1			; Go if error: divisor=0
		MOVIQQ		R3,10000000000000000	; R3=femtoseconds per Second * 10
;--- Calculate and write integer part of Frequency, Hz ---
		DIVU64		R3,R5			; R5=Result frequency, Hz/10
		PUSH64		R3			; Save Result frequency, Hz/10
		MOVIQW		R5,10			; R5=Divisor for units Hz/10
		DIVU64		R3,R5			; R3=Result, Hz (integer)
		CALL32		String_Decimal32	; Note here required R4=0
;--- Write decimal point ---
		MOVIBW		@R2,'.'			; Write decimal point
		MOVSNW		R2,R2,1			; R2+1, advance string pointer
;--- Calculate and write float part of Frequency, MHz ---
		POP64		R3			; Restore to R3=Result, Hz/10
		MODU64		R3,R5			; R3=Remainder after divide by 10
;--- Go to write number ---
		JMP8		SFM_2

;--- Write success or error message -------------------------------------;
; Used for deferred status reporting when exit program,                  ;
; status can be result of F9=Save TXT or F10=Save BMP.                   ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, use buffer on it     ;
;          Status variables                                              ;
; OUTPUT:  None                                                          ;
;------------------------------------------------------------------------;
Status_Msg_Write:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
;--- Analysing status ---
		MOVBW		R2,@R1,0,_EFI_Action
		CMPI32WEQ	R2,0
		JMP8CS		SMW_0			; Exit if no actions 
;--- Skip one string ---
		MOVIQW		R2,_Msg_CRLF		
		CALL32		String_Write
;--- Built and Write action name string ---
		MOVSNW		R2,R1,_Save_File_Status	; R2 = Source string
		MOVSNW		R3,R1,256		; R3 = Destination string, +256 otherwise overlap
		MOVIQW		R6,4			; R6 = Limit
		MOVBW		R7,@R1,0,_EFI_Action	; R7 = Selector
		CALL32		Copy_Selected_String
		MOVSNW		R2,R1,_Name_TXT
		CMPI32WEQ	R6,1
		JMP8CS		SMW_2
		MOVSNW		R2,R1,_Name_BMP
		CMPI32WEQ	R6,2
		JMP8CC		SMW_1
SMW_2:		CALL32		Copy_String
SMW_1:		MOVIQW		R2,256			; Offset = 256 relative R1
		CALL32		String_Write		
		MOVIQW		R2,_Msg_CRLF		
		CALL32		String_Write
;--- Check errors detected ---
		MOVBW		R7,@R1,0,_EFI_Action	; R7 = Selector
		CMPI32WULTE	R7,2
		JMP8CS		SMW_0			; Go if no errors
;--- Built and Write UEFI API operation name string ---
		MOVSNW		R2,R1,_Str_Operation	; R2 = Source string
		MOVSNW		R3,R1,256		; R3 = Destination string, +256 otherwise overlap
		CALL32		Copy_String
		MOVSNW		R2,R1,_Operations_Names	; R2 = Source string
		MOVIQW		R6,14			; R6 = Limit
		MOVBW		R7,@R1,0,_EFI_Operation	; R7 = Selector		
		CALL32		Copy_Selected_String
		MOVIQW		R2,256			; Offset = 256 relative R1
		CALL32		String_Write
		MOVIQW		R2,_Msg_CRLF		
		CALL32		String_Write
;--- Built and Write UEFI Error name string ---
		MOVSNW		R2,R1,_Str_Status	; R2 = Source string
		MOVSNW		R3,R1,256		; R3 = Destination string, +256 otherwise overlap
		CALL32		Copy_String
		MOVQW		R7,@R1,0,_EFI_Status
		MOVIQQ		R6,0FFFFFFFF00000000h	; D[63-32]=Sign extend: 0=Warning, 1=Error
		AND64		R6,R7
		CMPI64WEQ	R6,0
		MOVSNW		R2,R1,_EFI_Warnings_Names
		MOVIQW		R6,4			; Last warning=4
		JMP8CS		SMW_3
		MOVSNW		R2,R1,_EFI_Errors_Names
		MOVIQW		R6,32			; Last error=32
		MOVIQW		R5,00FFh
		AND64		R7,R5			; Mask status code
SMW_3:		CALL32		Copy_Selected_String
		MOVIQW		R2,256			; Offset = 256 relative R1
		CALL32		String_Write
		MOVIQW		R2,_Msg_CRLF		
		CALL32		String_Write
;--- Restore registers and exit ---
SMW_0:		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET

