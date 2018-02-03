;--- Load configuration file ProgramName.CFG -------------------------------;
; This subroutine also detect File I/O protocol for other usages,           ;
; save report and save graphics procedures use this result.                 ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;               Global variables:                                           ;
;                QWORD File_System_Protocol = Simple File System Protocol   ;
;                QWORD File_Volume_Protocol = Used volume File protocol     ;
;
;---------------------------------------------------------------------------;

;--- Equations ---

Xdefault	= 1024
Ydefault	= 768
Xmin		= 640
Ymin		= 400
Xmax		= 2048
Ymax		= 2048
X_Y_default	= 03000400h	; Ydefault=768  , Xdefault=1024
Pause_default	= 1

;--- Procedure ---

Load_Config:

;--- Detect File Protocol ---
		MOVIQW		R2,_GuidFileSystem
		CALL32		Locate_Protocol		; Return R2 = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
		JMP8CS		LCFG_0			; Go if error detected
		MOVQW		@R1,0,_File_System_Protocol,R2

;--- Prepare for call Open volume, built scratch pad for output ---
		XOR64		R7,R7
		PUSHN		R7			; Create scratch pad for output parameter
		MOVQ		R3,R0			; R0 (Stack pointer) = Output parameter address

;--- Built input parameters stack frame ---
		PUSHN		R3			; Parameter #2 = Output parameter address
		PUSHN		R2			; Parameter #1 = Protocol

;--- Call function ---
		CALL32EXA	@R2,0,8			; EFI_SIMPLE_FILE_SYSTEM_PROTOCOL.OpenVolume (N=0, C=8)

;--- Remove input parameters stack frame ---
		POPN		R2
		POPN		R3

;--- Read and save output parameter, remove scratch pad ---
		POPN		R2			; R2 = EFI_FILE_PROTOCOL, for this volume
		MOVQW		@R1,0,_File_Volume_Protocol,R2

;--- Verify function status ---
		MOVSNW		R7,R7			; Unify for 32 and 64
		CMPI64WUGTE	R7,1			; Condition Flag=1 if R7>0
		JMP8CS		LCFG_0			; Go if error detected, with CF=1

;--- Pre-set parameters for prevent unexpected values ---
		MOVIDD		@R1,0,_Video_X_Y_sizes, X_Y_default
		MOVIBW		@R1,0,_Start_Pause, Pause_default

;--- Get program run path for .EFI file, built path for .CFG file --- 
; Yet used first volume only, without scan run path
;---
		MOVSNW		R2,R1,_Cfg_File_Name
		CALL32		EFI_File_Path_Built

;--- Load configuration file ---
; Here actual after Path_Built
; R2  = Pointer to file path\name string
; R3 = Pointer to free buffer for file I/O
;---
		PUSH64		R3
		MOVQ		R3,R2				; R3 = Pointer to file name
		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVIQW		R4,0000000000000003h		; R4 = Open mode: Open for Read and Write
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File
		POP64		R3				; R3 = Restore buffer address

	;*	JMP8CS		LCFG_1				; Go for interpreting as non-fatal error (silent)
		JMP8CS		LCFG_6				; Go for interpreting as non-fatal error (silent) + create file request

		MOVQ		R2,R6				; R2 = File handle
		MOVIQD		R4,61440+1			; R4 = Maximum size+1 = 60 KB + 1 byte
		MOVQ		R5,R4				; This for compare later
		CALL32		EFI_Read_File			; If 60KB+1 = too big size
		JMP8CS		LCFG_0				; Go if Read file error
		CMP64UGTE	R4,R5
		JMP8CS		LCFG_0				; Go if file too big, if 60KB+1 serviced
		CALL32		EFI_Close_File			; Close file
		JMP8CS		LCFG_0				; Go if Close file error

;--- Prepare for interpreting configuration file ---
; Here valid: R3 = Pointer to file buffer, R4 = File size
;---
		MOVIDW		@R1,1				; Setup lines counter, use scratch pad, dword [0-3]
		ADD64		R4,R3				; R4 = Size + Base = Limit, R4 = First not-existed offset

;--- Start interpreting cycle ---
LCFG_2:		CMP64UGTE	R3,R4				; R3 = Pointer, R4 = Limit, check limit
		JMP8CS		LCFG_4				; Go if end of file detected
		MOVSNW		R2,R1,_Keywords_List
;--- Start keywords comparision cycle ---
; R5 = Pointer to keyword nul-terminated string
; R6 = Pointer to keyword handling subroutine
;---
LCFG_3:		EXTNDW64	R5,@R2				; R5 = Data pointer: keyword or char
		EXTNDW64	R6,@R2,0,2			; R6 = Code pointer: handler
		MOVSNW		R2,R2,12			; R2+12, skip both entries: CFGL(7) + CFGS(5)
		CMPI64WEQ	R5,0
		JMP8CS		LCFG_5				; Go error if bad keyword
		ADD64		R5,R1				; Convert R5 to absolute address
		ADD64		R6,R1				; Convert R6 to absolute address
		CALL32		Local_Cmp_String
		JMP8CC		LCFG_3				; Go compare next keyword if mismatch

;--- End of compare cycle, CF=1 means keyword match ---
		MOVSNW		R2,R2,-12			; R2-12, return to current structure
		CMP64UGTE	R3,R4				; R3 = Pointer, R4 = Limit, check limit after keyword
		JMP8CS		LCFG_4				; Go if end of file detected
		PUSH64		R2
		MOVD		R2,@R1				; R2 = Current string number, 1-based
		CALL32A		R6				; Call handler for detected keyword, return R7 = Value
		MOVD		@R1,R2
		POP64		R2
		JMP8CS		LCFG_5				; Go if error detected by keyword handler

;--- Parameter update ---
		CALL32		Local_Set_Mask_Parm		; Accept R7 = Value

;--- Cycle ---
		JMP8		LCFG_2				; Continue if no errors

;--- End of keyword comparision cycle ---
; control here if keywords not recognized (bad keyword)
LCFG_5:		MOVD		R2,@R1				; R2 = Current string number, 1-based
		CALL32		Local_Error_String
		JMP8		LCFG_6

;------- Verify validity of loaded parameters, make warnings if required ---;
LCFG_4:
;--- Xsize ---
		MOVWW		R2,@R1,0,_Video_Xsize
		MOVIQW		R3,Xmin
		MOVIQW		R4,Xmax
		MOVIQW		R5,Xdefault
		CALL32		Local_Parms_Limit
		MOVWW		@R1,0,_Video_Xsize,R2
;--- Ysize ---
		MOVWW		R2,@R1,0,_Video_Ysize
		MOVIQW		R3,Ymin
		MOVIQW		R4,Ymax
		MOVIQW		R5,Ydefault
		CALL32		Local_Parms_Limit
		MOVWW		@R1,0,_Video_Ysize,R2
;--- BPP (bits per pixel) ---
; Reserved, BPP=32 for all GOP modes.
;--- Start pause, include update Sys_Bitmap ---
		MOVDW		R2,@R1,0,_Sys_Bitmap
		MOVIDD		R3,7FFFFFFFh
		AND32		R2,R3				; D31=0, clear pause flag
		MOVBW		R4,@R1,0,_Start_Pause
		CMPI32WEQ	R4,0
		JMP8CS		LCFG_7				; Go skip if pause disabled
		NOT32		R3,R3
		OR32		R2,R3				; D31=1, set pause flag
LCFG_7:		MOVDW		@R1,0,_Sys_Bitmap,R2
;--- Exit points ---

LCFG_1:		XOR64		R7,R7		; This label for non-falal errors: clear status
LCFG_0:		RET				; This label for exit with R7 = Status
LCFG_6:		MOVIQW		R6,4000h	; D14 = 1, request save/create configuration file
		CALL32		CUC_3
		RET				; Subroutine also return R7=0


;---------- Main local subroutine ------------------------------------------;


;---------- Handlers for config file interpreting ------------------;
; INPUT:   R1 = Global variables pool base address                  ;
;          R2 = Current text string number in the text file         ;
;          R3 = Pointer to analysed text file Name.CFG              ;
;          R4 = Text file limit                                     ;
;                                                                   ;
; OUTPUT:  CF = Error flag: 0(CC)=No errors, 1(CS)=Syntax error     ;
;          R2 = String number modified by text scan                 ;
;          R3 = Text pointer modified by text scan                  ;
;          R7 = Parameter value for parameters strings              ;
;-------------------------------------------------------------------;

HL_Parms:
;--- Handling  LINE FEED ---
HL_LF:		MOVSNW		R2,R2,1			; R2+1, string number +1
;--- Handling SPACE ---
;--- Handling  TAB ---
;--- Handling CARRIAGE RETURN ---
HL_Space:
HL_Tab:
HL_CR:
HL_C_1:		CMPI64WEQ	R2,0			; This for set CF=0(CC), means no errors
		RET
;--- Handling  ";" ---
HL_Comment:	CMP64UGTE	R3,R4			; Compare pointer with limit
		JMP8CS		HL_C_1			; Go if end of file detected
		MOVB		R7,@R3			; Get char from string
		CMPI32WEQ	R7,10			; 10=0Ah=Line Feed
		JMP8CS		HL_LF			; Go if line feed
		CMPI32WEQ	R7,13			; 13=0Dh=Carriage Return
		JMP8CS		HL_CR			; Go if carriage return
		MOVSNW		R3,R3,1			; R3+1, select next char
HL_Comment_2:	JMP8		HL_Comment		; Cycle for skip this string

;--- Handling XSIZE ---
HL_Xsize:	
;--- Handling YSIZE ---
HL_Ysize:
;--- Handling BPP (BITS PER PIXEL) ---
HL_BPP:
;--- Handling START PAUSE ---
HL_Pause:
HL_X_1:		CALL32		Local_Skip_Space	; Skip spaces before "="
		JMP8CC		HL_X_7			; Go if error: no parameter means error
		CMPI32WEQ	R7,'='			; Required ParmName=Value
		JMP8CC		HL_X_1			; Go if not "="
		MOVSNW		R3,R3,1			; R3+1, skip "="
		CALL32		Local_Skip_Space	; Skip spaces after "="
		XOR64		R7,R7			; Accumulator for number = 0
HL_X_5:		MOVB		R6,@R3			; Interpreting ASCII number
		CMPI32WUGTE	R6,'0'
		JMP8CC		HL_X_9			; Go if error: wrong char
		CMPI32WULTE	R6,'9'
		JMP8CS		HL_X_8			; Go if no error: acceptable char
HL_X_9:		PUSH64		R7
		CALL32		Local_Skip_Space	; Skip spaces after number
		POP64		R7
		JMP8CS		HL_X_7			; Go if error: extra chars
		JMP8		HL_X_4			; End of string or file
HL_X_8:		MOVIQW		R5,10
		MULU64		R7,R5			; Set power of 10 for digits sequence
		MOVIQW		R5,0Fh
		AND32		R6,R5			; Acceptable: 0-9
		ADD32		R7,R6			; Add parsed digit
		MOVSNW		R3,R3,1			; R3+1, advance string pointer
		CMP64UGTE	R3,R4
		JMP8CC		HL_X_5			; Go if R3<R4, means pointer in the valid range
HL_X_4:		JMP8		HL_C_1			; R7=Parameter value, go to set CF=0 and return
HL_X_7:		CMP64EQ		R1,R1			; This for set CF=1(CS) means char found
		RET

;---------- Local subroutines ----------------------------------------------;


;--- Compare text strings, support upper/lower case ------;
; INPUT:   R5 = Pointer to nul-terminated pattern         ;
;          R3 = Pointer to analysed text from file        ;
; OUTPUT:  CF flag = compare result:                      ;
;                    0(CC)=Mismatch, 1(CS)=Match          ;
;          R3 = Modified by analysed text scan            ;
;               but saved if mismatch                     ;
;---------------------------------------------------------;
Local_Cmp_String:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R4
		PUSH64		R6
		MOVQ		R2,R3			; Save buffer address for restore if mismatch
		MOVIQW		R4,11011111b		; Mask for make upper case, bit D5=0
;--- Compare with keyword, required lower/upper case support ---
Key_Cmp_4:	MOVB		R7,@R5			; R7 = Char from pattern
		MOVSNW		R5,R5,1			; R5+1, modify pattern pointer
		CMPI32WEQ	R7,0
		JMP8CS		Key_Cmp_3		; Go if end of string, 0=terminator
		MOVB		R6,@R3			; R6 = Char from file
		MOVSNW		R3,R3,1			; R3+1, modify file pointer
;--- Work with pattern char ---
		CMPI32WUGTE	R7,'a'
		JMP8CC		Key_Cmp_1
		CMPI32WULTE	R7,'z'
		JMP8CC		Key_Cmp_1
		AND32		R7,R4			; Make upper case, bit D5=0
;--- Work with file char ---
Key_Cmp_1:	CMPI32WUGTE	R6,'a'
		JMP8CC		Key_Cmp_2
		CMPI32WULTE	R6,'z'
		JMP8CC		Key_Cmp_2
		AND32		R6,R4			; Make upper case, bit D5=0
;--- Compare chars ---
Key_Cmp_2:	CMP32EQ		R7,R6
		JMP8CS		Key_Cmp_4		; Go continue compare if match
		MOVQ		R3,R2			; Restore buffer address if mismatch
;--- Restore registers and exit ---
; Here CF=0(CC) if matches not found, CF=1(CS) if matches found
Key_Cmp_3:	POP64		R6
		POP64		R4
		POP64		R2
		RET

;--- Mask parameter valid bits and set parameter ---------;
; If parameter offset=0, then not update, this is not     ;
; parameter keywords, for example comments or empty str.  ;
; INPUT:   R1 = Pointer to global variables pool          ;
;          R2 = Pointer to control data:                  ;
;                WORD at [R2+0] = Parameter signed offset ;
;                BYTE at [R2+2] = Parameter width, bits   ;
;          R7 = Parameter value, valid bits masked here   ;
;          Variables at Global Variables Pool             ;
; OUTPUT:  Parameter updated                              ;
;---------------------------------------------------------;
Local_Set_Mask_Parm:
;--- Prepare parameter mask ---
		MOVBW		R5,@R2,0,6		; R5 = Parameter width
		MOVIBW		R6,32
		SUB32		R6,R5			; R6 = Mask shifts count
		MOVIQW		R5,0FFFFh		; R5 = Mask
		SHR32		R5,R6			; R5 = Shifted mask
;--- Prepare parameter address ---
		EXTNDW64	R6,@R2,0,4		; R6 = Parameter offset
		CMPI32WEQ	R6,0			; Check this paramter exist
		JMP8CS		LdCfg_Parm_5		; Skip if this is no parameter
		ADD64		R6,R1			; R6 = Parameter absolute address
;--- Update parameter and exit ---
		AND32		R7,R5			; Mask valid bits: 8/16/32
		NOT32		R5,R5			; Mask for parameter variable
		AND32		@R6,R5			; Pre-blank parameter variable
		OR32		@R6,R7			; Set parameter variable
LdCfg_Parm_5:	RET

;--- Scan string for ASCII chars, skip spaces ------------;
; INPUT:   R1 = Pointer to global variables pool,         ;
;               here for compare only                     ;
;          R3 = Pointer to string                         ;
;          R4 = Buffer limit for compare                  ;
; OUTPUT:  CF = Flag:                                     ;
;                0(CC)=Char not found, end of string      ;
;                      or end of text file                ;
;                1(CS)=Char found                         ;
;          R7 = Last read char                            ;
;---------------------------------------------------------;
Local_Skip_Space:
LdCfg_Skip_3:	MOVB		R7,@R3			; Get char from text file
		CMPI32WEQ	R7,10
		JMP8CS		LdCfg_Skip_1		; Go if LINE FEED, end string
		CMPI32WEQ	R7,13
		JMP8CS		LdCfg_Skip_1		; Go if CARRIAGE RETURN, end string
		CMPI32WEQ	R7,' '
		JMP8CS		LdCfg_Skip_2		; Go if SPACE, continue string
		CMPI32WEQ	R7,9
		JMP8CC		LdCfg_Skip_4		; Go if not TAB, char detected
LdCfg_Skip_2:	MOVSNW		R3,R3,1			; R3+1, modify address
		CMP64UGTE	R3,R4
		JMP8CC		LdCfg_Skip_3		; Go if no end of file
LdCfg_Skip_1:	CMP64EQ		R1,R3			; This for set CF=0(CC) means char not found or end of file
		RET
LdCfg_Skip_4:	CMP64EQ		R1,R1			; This for set CF=1(CS) means char found
		RET

;--- Write error message about analysed string -----------;
; INPUT:   R1 = Pointer to global variables pool          ;
;          R2 = String number in the file                 ;
; OUTPUT:  None                                           ;
;          Corrupted: R2-R7                               ;
;---------------------------------------------------------;
Local_Error_String:
		MOVQ		R3,R2			; R3 = Number, here max. 65535
		MOVSNW		R2,R1,_Msg_E_Line_1	; R2 = Destination string
		XOR64		R4,R4			; R4 = 0, means no template
		MOVIDD		R5,'    '		; Need blank field
		MOVD		@R2,R5			; Blank chars 1-4
		MOVBW		@R2,R5,0,4		; Blank char 5
		CALL32		String_Decimal32	; Prepare line number
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write		; Carriage Return, Line Feed, next string
		MOVIQW		R2,_Msg_E_Line		; Error msg. with line number
		CALL32		String_Write		; Carriage Return, Line Feed
		MOVIQW		R2,_Msg_Press		; "Press any key..."
		CALL32		String_Write		; Carriage Return, Line Feed, next string
		CALL32		Input_Wait_Key		; Wait for key press
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write		; Carriage Return, Line Feed, next string
		RET

;--- Compare parameters and make limits ------------------;
; INPUT:   R2 = Parameter value                           ;
;          R3 = Minimum limit                             ;
;          R4 = Maximum limit                             ;
; OUTPUT:  R2 = Corrected by limits, if limits violated   ;
;                or parameter not defined (R2=0)          ;
;---------------------------------------------------------;
Local_Parms_Limit:
		CMPI64WEQ	R2,0
		JMP8CS		LdCfg_Parm_4		; Go if parameter not defined
		CMP64UGTE	R2,R3			; Compare with minimum limit
		JMP8CS		LdCfg_Parm_1		; Go if parameter >= min, OK
		MOVQ		R2,R3			; Correction if P < Pmin
LdCfg_Parm_1:	CMP64ULTE	R2,R4			; Compare with maximum limit
		JMP8CS		LdCfg_Parm_2		; Go if parameter <= max, OK
LdCfg_Parm_3:	MOVQ		R2,R4			; Correction if P > Pmax
LdCfg_Parm_2:	RET
LdCfg_Parm_4:	MOVQ		R2,R5			; Set default if not defined
		RET
