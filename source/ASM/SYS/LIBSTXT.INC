;===========================================================================;
;                                                                           ;
;       SUBROUTINES FOR SAVE SYSTEM INFORMATION REPORT AS TEXT FILE.        ;
;                                                                           ;
;===========================================================================;

;---------- Save report to TXT file -------------------------------------;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          Global variables                                              ;
; OUTPUT:  Global variables                                              ;
;------------------------------------------------------------------------;

Save_Report_to_TXT:

;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7

;--- Checkpoint 058h = Entry procedure save report to TXT file ---
		CHK		058h

;--- Set current action type for errors reporting ---
		MOVIBW		@R1,0,_EFI_Action,1		; 1 = Save text report by F9 key

;--- Check free memory ---
		;
		; Reserved
		;
;--- Setup file path and try generating unique file name ---
; Subroutine Path_Built return:
; R2 = Pointer to file path\name string, UNICODE
; R3 = Pointer to free buffer for file I/O
;----------
		XOR64		R5,R5				; R5 = File Number Value, start=0
;--- Label of cycle: free name find from TEXT****.TXT ---
SRTT_0:		MOVSNW		R2,R1,_Name_TXT+4		; R2 = Destination pointer
		MOVQ		R3,R5				; R3 = Number value, 0000-9999
		MOVIQW		R4,4				; R4 = Template size = 4
		CALL32		String_Decimal32		; Update name numeric field: "0000" - "9999"
		MOVSNW		R2,R1,_Name_TXT
		CALL32		EFI_File_Path_Built		; Pointers: R2 = Name, R3 = Buffer
		MOVQ		R3,R2				; R3 = Pointer to file name
		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVIQW		R4,0000000000000003h		; R4 = Open mode: Open for Read and Write
		PUSH64		R5
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File		; This for check file exist
		POP64		R5
		JMP8CC		SRTT_1				; Go close file if no errors, but file exist
		MOVSNW		R7,R7				; Unify for 32 and 64
		MOVIQQ		R4,0FFFFFFFF00000000h
		AND64		R4,R7
		CMPI64WEQ	R4,0
		JMP8CS		SRTT_21				; Go file open error handling if unexpected error code
		MOVIQD		R4,07FFFFFFFh
		AND64		R7,R4
		CMPI32WEQ	R7,00000000Eh			; Compare with error code EFI_NOT_FOUND
		JMP8CS		SRTT_2				; Go create new file if expected error: file not exist
		JMP8		SRTT_21				; Go if file open error with unexpected error code

;--- Close existed file, go here if no errors but name exist, need try next name ---
SRTT_1:		MOVQ		R2,R6				; Set R2=Handle for close file
		CALL32		EFI_Close_File			; Close this existed file
		JMP8CS		SRTT_22				; Go if error close file
		MOVSNW		R5,R5,1				; R5+1, next number for file name TEXT****.TXT
		CMPI32WULTE	R5,9999				; Compare with maximum acceptable value
		JMP8CC		SRTT_24				; Go error if file number overflow, above 9999
		JMP8		SRTT_0				; Go try next name after close existed

;--- Create new file, here R3=Pointer to name ---		; R3=Pointer to name
SRTT_2:		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVIQQ		R4,8000000000000003h		; R4 = Open mode: Create for Read and Write
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File		; This for create new file, return R6=Handle
		JMP8CS		SRTT_21				; Go if file create error

;--- Initialize base address for built text report ---
		MOVQW		R3,@R1,0,_Secondary_Memory_Base
		MOVQ		R5,R3

;--- Built text report header, R3=Destination base ---
		MOVSNW		R2,R1,_String_Version
		CALL32		Copy_String
		MOVSNW		R2,R1,_Report_Middle
		CALL32		Copy_String			; Add CR, LF between strings
		MOVSNW		R2,R1,_Program_Vendor_Data
		CALL32		Copy_String
	;*	MOVSNW		R2,R1,_Program_Vendor_Data_1
	;*	CALL32		Copy_String
		MOVSNW		R2,R1,_Report_Middle_2
		CALL32		Copy_String
	;*	MOVSNW		R2,R1,_Report_Title
	;*	CALL32		Copy_String

;--- Built date and time string ---
		CALL32		String_Date_Time
		MOVSNW		R2,R1,_Report_Middle_2
		CALL32		Copy_String

;--- Start built report strings, copy from system information screen results ---
; Skip 2 first strings, correspond to report format
; Cannot handling empty list, correspond to report format
; 2 strings per group: left and right, correspond to report format

;--- Skip 2 first header strings ---
		MOVQ		R2,R1				; R2 = Source, R3 = Destination
		XOR32		R4,R4				; R4 = Strings counter for skip
SRTT_11:	MOVSNW		R2,R2,8				; R2+8, skip string header
SRTT_10:	MOVB		R7,@R2				; Read current byte from buffer, sign-extend to R4
		MOVSNW		R2,R2,1				; R2+1
		CMPI32WEQ	R7,0
		JMP8CC		SRTT_10				; Go cycle if current string continued
		MOVSNW		R4,R4,1				; R5+1, count if end of current string
		CMPI32WUGTE	R4,2				; Number of first strings to skip = 2
		JMP8CC		SRTT_11				; Go cycle if 2 strings in progress
;--- Built report strings ---
SRTT_12:	MOVQ		R4,R3				; Save previous destination pointer
		MOVSNW		R2,R2,8				; R2+8, skip string header
		CALL32		Copy_String			; Copy current string, left part
		MOVSNW		R2,R2,8				; R2+8, skip string header
		CALL32		Copy_String			; Copy current string, right part
		PUSH64		R2
		MOVSNW		R2,R1,_Report_Middle
		CALL32		Copy_String			; Add CR, LF between strings
		POP64		R2
		MOVW		R7,@R2
		CMPI32DUGTE	R7,00000FFFFh			; Detect terminator
		JMP8CC		SRTT_12
		MOVQ		R3,R4				; Restore previous destination pointer

;--- Last chars: CR, LF ---
	;*	MOVSNW		R2,R1,_Report_Middle_2
	;*	CALL32		Copy_String

;--- Save report file ---
; Here must be valid R5,R3 = generated text block start,end address
; Here must be valid R6 = Handle after create file
;---
		MOVQ		R2,R6				; R2 = Handle for file access
		SUB64		R3,R5				; R3 = Size, R5 = Base
		MOVQ		R4,R3				; R4 = Size of file
		MOVQ		R3,R5				; R3 = Base address of buffer
		MOVQ		R5,R4				; R5 = Save required size for compare
		CALL32		EFI_Write_File
		JMP8CS		SRTT_23				; Go if file write error
		CMP64EQ		R4,R5				; Compare reported and required size
		JMP8CC		SRTT_25				; Go if disk full error

;--- Close file, here valid R2=Handle ---
		CALL32		EFI_Close_File
		JMP8CS		SRTT_22				; Go if file close error

;--- Reserved for context restore after errors ---
SRTT_28:


;--- Checkpoint 05Bh = Exit procedure save screen to TXT file ---
		CHK		05Bh
;--- Restore registers and exit ---
		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET

;---------- Errors handling ---------------------------------------------;

SRTT_20:							; Entry for Memory errors
		; Reserved
SRTT_29:							; Common entry for some errors branches
		MOVSNW		R7,R7				; Unify status for 32 and 64
		MOVWW		@R1,0,_EFI_Act_Op,R6
		MOVQW		@R1,0,_EFI_Status,R7
		JMP8		SRTT_28
SRTT_21:							; Entry for File Open or Create Error
		MOVIQW		R6,0B03h			; Action=3, Operation=11
		JMP8		SRTT_29
SRTT_22:							; Entry for File Close Error
		MOVIQW		R6,0C03h
		JMP8		SRTT_29
SRTT_23:							; Entry for File Write Error
		MOVIQW		R6,0E03h
		JMP8		SRTT_29
SRTT_24:							; Entry for can't assign number, exist 10000 files
		MOVIQD		R7,080000009h			; Error=OUT_OF_RESOURCES (32-bit)
		JMP8		SRTT_23
SRTT_25:							; Entry for Disk Full Error
		MOVIQD		R7,08000000Bh			; Error=VOLUME_FULL (32-bit)
		JMP8		SRTT_23

