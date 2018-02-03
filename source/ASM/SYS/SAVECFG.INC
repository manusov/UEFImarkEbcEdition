;--- Save configuration file ProgramName.CFG, with user dialogue -----------;
; For UEFImark file name is UEFIMARK.CFG                                    ;
; File created if not exist or updated if exist                             ;
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
;---------------------------------------------------------------------------;

Save_Configuration:
;--- Dialogue with user, send message ---
		CALL32		Local_Question			; Question to user (Y/N)
		JMP8CC		SCFG_0				; Exit if user press "N"
		MOVIQW		R2,_Msg_Y_Save			; Message "Saving..."
		CALL32		String_Write
;--- Get program run path for .EFI file, built path for .CFG file ---
; Note yet used root directory.
;---
		MOVSNW		R2,R1,_Cfg_File_Name
		CALL32		EFI_File_Path_Built
;--- Save and prepare parameters, use Scratch Pad ---
		MOVQ		@R1,R2				; Save pointer to path\name string
		MOVQW		@R1,00,08,R3			; Save pointer to file I/O buffer

;--- Load current configuration file ---
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
		JMP8CS		SCFG_3				; Go create new file if not exist or error
		MOVQ		R2,R6				; R2 = File handle
		MOVIQD		R4,61440+1			; R4 = Maximum size+1 = 60 KB + 1 byte
		MOVQ		R5,R4				; This for compare later
		CALL32		EFI_Read_File			; If 60KB+1 = too big size
		JMP8CS		SCFG_2				; Go if Read file error
		CMP64UGTE	R4,R5
		JMP8CS		SCFG_2				; Go if file too big, if 60KB+1 serviced
		CALL32		EFI_Close_File			; Close file
		JMP8CS		SCFG_2				; Go if Close file error

;--- Save old file size, use Scratch Pad ---
		MOVQW		@R1,00,16,R4			; R4 = File size returned after read

;--- Modify file name to .BAK ---
		MOVSNW		R2,R1,_Bak_File_Ext		; R2 = Pattern for replace extension
		MOVQ		R3,@R1				; R3 = Pointer to file path\name string
		CALL32		Local_Change_Ext
		JMP8CS		SCFG_2				; Go if path parse error

;--- Write current configuration file backup ---
; Here actual after path built and file read, stored in the Scratch pad qwords:
; Qword #0 = Pointer to file path\name string
; Qword #1 = Pointer to buffer with current configuration file
; Qword #2 = Current configuration file size
;--- Check previous BAK exist ---
		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVQ		R3,@R1				; R3 = Pointer to path\name string
		MOVIQW		R4,0000000000000003h		; R4 = Open mode: Open for Read and Write
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File
		MOVQ		R2,R6				; R2 = Handle, if no errors

;--- Delete previous .BAK if it exist ---
		JMP8CC		SCFG_4				; Go delete .BAK file if it found OK
		MOVSNW		R7,R7				; Unify status for 32 and 64
		MOVIQQ		R6,0FFFFFFFF00000000h
		AND64		R6,R7
		CMPI64WEQ	R6,0				; CF=1 if unexpected error or warning
		JMP8CS		SCFG_2				; Go error with valid R7 = Status
		MOVIQD		R6,07FFFFFFFh
		AND64		R6,R7
		CMPI32WEQ	R6,EFI_NOT_FOUND AND 00FFh
		JMP8CS		SCFG_5				; Skip delete if not exist
		JMP8		SCFG_2				; Otherwise error
SCFG_4:		CALL32		EFI_Delete_File			; Delete file
		JMP8CS		SCFG_2				; Go if delete error, valid R7 = Status

;--- End of delete section, start open-write-close new .BAK ---
SCFG_5:		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVQ		R3,@R1				; R3 = Pointer to file path\name string
		MOVIQQ		R4,8000000000000003h		; R4 = Open mode: Create
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File		; Create file *.BAK
		JMP8CS		SCFG_2				; Go if create error, valid R7 = Status
		MOVQ		R2,R6				; R2 = Handle
		MOVQW		R3,@R1,00,08			; R3 = Pointer to file I/O buffer
		MOVQW		R4,@R1,00,16			; R4 = File size returned after previous read
		MOVQ		R5,R4				; R5 = Copy size for compare
		CALL32		EFI_Write_File
		JMP8CS		SCFG_2				; Go if write error, valid R7 = Status
		CALL32		EFI_Close_File
		JMP8CS		SCFG_2				; Go if close error, valid R7 = Status
		CMP64EQ		R4,R5
		MOVIQQ		R7,EFI_VOLUME_FULL
		JMP8CC		SCFG_2				; Go if disk full, with loaded R7 = Status

;--- Modify file name to .CFG (return) ---
		MOVSNW		R2,R1,_Cfg_File_Ext		; R2 = Pattern for replace extension
		MOVQ		R3,@R1				; R3 = Pointer to file path\name string
		CALL32		Local_Change_Ext
		JMP8CS		SCFG_2				; Go if path parse error

;--- Create data block for new configuration file ---
SCFG_3:		MOVQW		R3,@R1,00,08			; Get pointer to file I/O buffer
;--- Header ---
		MOVIQW		@R3,'; '			; Begin string as comment
		MOVSNW		R3,R3,2				; R3+2, modify pointer
		MOVSNW		R2,R1,_String_Version
		CALL32		Copy_String
		MOVSNW		R2,R1,_Program_Vendor_Data_1
		CALL32		Copy_String
		MOVSNW		R2,R1,_Report_Middle
		CALL32		Copy_String
		MOVSNW		R2,R1,_Cfg_File_Header
		CALL32		Copy_String

;--- Built parameters list, cycle for all supported parameters ---
		MOVSNW		R2,R1,_Keywords_List_1		; R2 = Pointer to control table
SCFG_8:		MOVIWW		@R3,0A0Dh			; CR, LF, write end of previous string
		MOVSNW		R3,R3,2				; R3+2, modify pointer
		EXTNDW64	R7,@R2				; R7 = Current string pointer (R1-relative) from control table
		CMPI32WEQ	R7,0				; 0 means end of list
		JMP8CS		SCFG_9				; Go if end of list
		ADD64		R7,R1				; Convert from R1-relative to absolute
		CALL32		Local_Name_Parm			; Write parameter name
		EXTNDW64	R6,@R2,0,7			; R6 = Current routine pointer (R1-relative) from control table
		ADD64		R6,R1				; Convert from R1-relative to absolute
		CALL32		Local_Get_Mask_Parm		; Get parameter and mask it valid bits
		CALL32		Local_Value_Parm		; Write parameter value as ASCII string, plus CR, LF
		CALL32A		R6				; Call parameter additional routine
		MOVSNW		R2,R2,12			; R2+12, skip both entries: CFGL(7) + CFGS(5)
		JMP8		SCFG_8				; Cycle for all parameters

;--- Write new configuration file ---
; Here actual after path built and file read,
; Stored in the Scratch pad qwords:
; Qword #0 = Pointer to file path\name string
; Qword #1 = Pointer to buffer with current configuration file, start position
; Stored in the registers:
; R3 = Pointer to current buffer position, size = Current - Start
;--- Calculate and store size ---
SCFG_9:		SUB64		R3,@R1,00,08			; Subtract pointer to file I/O buffer
		MOVQW		@R1,00,16,R3			; Save generated file size
;--- Check previous .CFG exist ---
		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVQ		R3,@R1				; R3 = Pointer to path\name string
		MOVIQW		R4,0000000000000003h		; R4 = Open mode: Open for Read and Write
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File
		MOVQ		R2,R6				; R2 = Handle, if no errors
;--- Delete previous .CFG ---
		JMP8CC		SCFG_6				; Go delete .CFG file if it found OK
		MOVSNW		R7,R7				; Unify status for 32 and 64
		MOVIQQ		R6,0FFFFFFFF00000000h
		AND64		R6,R7
		CMPI64WEQ	R6,0				; CF=1 if unexpected error or warning
		JMP8CS		SCFG_2				; Go error with valid R7 = Status
		MOVIQD		R6,07FFFFFFFh
		AND64		R6,R7
		CMPI32WEQ	R6,EFI_NOT_FOUND AND 00FFh
		JMP8CS		SCFG_7				; Skip delete if not exist
		JMP8		SCFG_2				; Otherwise error
SCFG_6:		CALL32		EFI_Delete_File			; Delete file
		JMP8CS		SCFG_2				; Go if delete error, valid R7 = Status
;--- End of delete section, start open-write-close ---
SCFG_7:		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVQ		R3,@R1				; R3 = Pointer to file path\name string
		MOVIQQ		R4,8000000000000003h		; R4 = Open mode: Create
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File		; Create file *.CFG
		JMP8CS		SCFG_2				; Go if create error, valid R7 = Status
		MOVQ		R2,R6				; R2 = Handle
		MOVQW		R3,@R1,00,08			; R3 = Pointer to file I/O buffer
		MOVQW		R4,@R1,00,16			; R4 = File size returned after previous read
		MOVQ		R5,R4				; R5 = Copy size for compare
		CALL32		EFI_Write_File
		JMP8CS		SCFG_2				; Go if write error, valid R7 = Status
		CALL32		EFI_Close_File
		JMP8CS		SCFG_2				; Go if close error, valid R7 = Status
		CMP64EQ		R4,R5
		MOVIQQ		R7,EFI_VOLUME_FULL
		JMP8CC		SCFG_2				; Go if disk full, with loaded R7 = Status

;--- Exit points ---
SCFG_1:		XOR64		R7,R7				; Status = 0 for no errors
SCFG_2:		RET						; Status = R7 if jump here
SCFG_0:		MOVIQW		R2,_Msg_N_Save			; Message "Skipping..."
		CALL32		String_Write
		XOR64		R7,R7
		RET



;---------- Local subroutines ----------------------------------------------;


;--- Functionality reserved for complex parameters support ---
HS_Space:
HS_Tab:
HS_LF:
HS_CR:
HS_Comment:
HS_Xsize:
HS_Ysize:
HS_Pause:
		RET


;--- Write parameter name, tabulate, equality, space ---------------;
; INPUT:   R1 = Global variables pool base address                  ;
;          R7 = Pointer to parameter name, nul-terminated string    ;
;          R3 = Pointer to destination buffer                       ;
; OUTPUT:  R3 = Updated by write                                    ;
;-------------------------------------------------------------------;
Local_Name_Parm:
		PUSH64		R2
;--- Write parameter name and calculate number of added spaces to format ---
		MOVQ		R2,R7		; R2 = Destination pointer
		CALL32		Copy_String
		SUB64		R2,R7		; R2 = Executed write size
		MOVIQW		R7,9		; Template size = 9 chars
		SUB64		R7,R2		; R7 = Number of spaces required for add
		CMPI64WUGTE	R7,9
		JMP8CS		SvCfg_1		; Overflow protect, go if overflow
;--- Blank template field ---
SvCfg_20:	MOVIBW		@R3,' '		; Write space in cycle
		MOVSNW		R3,R3,1		; R3+1, modify pointer
		MOVSNW		R7,R7,-1	; R7-1, decrement counter
		CMPI32WEQ	R7,0
		JMP8CC		SvCfg_20	; Cycle for required number of spaces
;--- Char "=" ---
SvCfg_1:	MOVIWW		@R3,'= '	; Store "=" and " " chars
		MOVSNW		R3,R3,2		; R3+2, modify pointer
;--- Exit ---
		POP64		R2
		RET

;--- Get parameter and mask it valid bits --------------------------;
; INPUT:   R1 = Global variables pool base address                  ;
;          R2 = Pointer to control data:                            ;
;                 WORD at [R2+09] = Parameter signed offset         ;
;                 BYTE at [R2+11] = Parameter width, bits           ;
;          Variables at Global Variables Pool                       ;
; OUTPUT:  R7 = Parameter value, valid bits masked                  ;
;-------------------------------------------------------------------;
Local_Get_Mask_Parm:
;--- Prepare parameter address ---
		EXTNDW64	R7,@R2,00,09	; Parameter signed offset
		ADD64		R7,R1		; Convert R1-relative to absolute
;--- Prepare parameter bitmask ---
		MOVBW		R5,@R2,00,11	; Parameter width, bits
		MOVIBW		R4,32
		SUB32		R4,R5		; R5 = Number of shifts
		MOVIQW		R5,0FFFFh	; R5 = Mask
		SHR32		R5,R4		; R5 = Shifted mask
;--- Read and mask parameter ---
		MOVD		R7,@R7		; R7 = Dword include target parameter
		AND32		R7,R5		; Mask valid bits: 8/16/32
;--- Exit ---
		RET

;--- Write parameter value as ASCII number string ------------------;
; INPUT:   R7 = Parameter value, already masked                     ;
;          R3 = Pointer to destination buffer                       ;
; OUTPUT:  R3 = Updated by write                                    ;
;-------------------------------------------------------------------;
Local_Value_Parm:
		PUSH64		R2
		MOVQ		R2,R3		; R2 = Destination pointer
		MOVQ		R3,R7		; R3 = Numeric value
		XOR64		R4,R4		; R4 = 0, means no template
		CALL32		String_Decimal32
		MOVQ		R3,R2		; Change destination pointer
		POP64		R2
		RET

;--- Replace extension for path ------------------------------------;
; Notes. Need find last "." char, because directories namse also    ;
;        can have extensions.                                       ;
;        This procedure assume use 3-char extensions (.CFG/.BAK).   ;
;                                                                   ;
; INPUT:   R2 = Pointer to extension string, nul-terminated, ASCII  ;
;          R3 = Pointer to path string for update it, UNICODE16     ;
; OUTPUT:  CF  = Error flag: 0(CC)=OK, 1(CS)=Bad path string        ;
;-------------------------------------------------------------------;
Local_Change_Ext:
;--- Save path start pointer ---
		MOVQ		R4,R3		; R4 = Copy of pointer
;--- Find end of string, word=00h ---
SvCfg_2:	MOVW		R7,@R3		; Get current char (UNICODE16)
		MOVSNW		R3,R3,2		; R3+2 (2 bytes per char), modify pointer for access new char
		CMPI32WEQ	R7,0
		JMP8CC		SvCfg_2		; Scan for 0000h, end of string, go if 0 not found
		MOVSNW		R3,R3,-2	; R3-2 (2 bytes per char), return pointer to last char 0000h
;--- Find last occured byte="." ---
SvCfg_3:	MOVW		R7,@R3		; Get current char (UNICODE16)
		MOVSNW		R3,R3,-2	; R3-2 (2 bytes per char), for scan chars backward
		CMPI32WEQ	R7,'.'
		JMP8CS		SvCfg_4		; Go if "." found
		CMP32UGTE	R3,R4
		JMP8CC		SvCfg_9		; Go if error, "." not found
		JMP8		SvCfg_3		; Cycle for search "."
;--- Replace extension ---
SvCfg_4:	MOVSNW		R3,R3,4		; R3 = Destination path after "." , R2 = Source
SvCfg_11:	MOVB		R7,@R2		; Read source char (byte), zero-extended to R7 (64)
		MOVSNW		R2,R2,1		; R2+1, modify source pointer (elements=bytes)
		CMPI32WEQ	R7,0
		JMP8CS		SvCfg_12	; Go to sequence termination if source char = 0
		MOVW		@R3,R7		; Write destination char (word)
		MOVSNW		R3,R3,2		; R3+1, modify destination pointer (elements=words)
		JMP8		SvCfg_11	; Cycle for copy all chars of extension
;--- Check 3-char extensions requirements, by check last word ---
SvCfg_12:	MOVW		R7,@R3		; Get last destination char
		CMPI32WEQ	R7,0
		JMP8CC		SvCfg_9		; Go error if last destination char not 0000h
;--- Exit points ----
SvCfg_10:	CMP64EQ		R1,R4		; This for clear condition flag only, for no errors
		RET
SvCfg_9:	CMP64EQ		R1,R1		; This for set condition flag only, for errors
		RET

;--- Write question message about save game and wait for keys ------;
; INPUT:   R1 = Global variables pool base address                  ;
; OUTPUT:  CF = Mode flag:                                          ;
;                0(CC)=User press "N", means NO, skip operation     ;
;                1(CS)=User press "Y", means YES, make operation    ;
;-------------------------------------------------------------------;
Local_Question:
;--- Message ---
		MOVIQW		R2,_Msg_Q_Save
		CALL32		String_Write
;--- Wait for key press: acceptable "Y" or "N" ---
Quest_Wait:	CALL32		Input_Wait_Key		; Wait for key press
;--- Analysing input ---
		MOVIQW		R3,16
		SHR32		R2,R3			; Move scan code to bits R2.[7-0]
		MOVIQW		R3,00100000b
		OR32		R2,R3			; Set lower case, D5=1
		CMPI32WEQ	R2,06Eh			; Compare with ASCII code "n"
		JMP8CS		Quest_Ok		; Go handling if "n"
		CMPI32WEQ	R2,079h			; Compare with ASCII code "y"
		JMP8CC		Quest_Wait		; Go still wait if not "y"
;--- Write char "Y" or "N" ---
Quest_Ok:	PUSH64		R2
		MOVDW		R2,R2			; R2.[31-16] = 0 for high word as terminator
		MOVD		@R1,R2
		XOR64		R2,R2			; Offset=0 relative R1
		CALL32		String_Write
		POP64		R2
		CMPI32WEQ	R2,079h			; Compare with ASCII code "y"
		RET


