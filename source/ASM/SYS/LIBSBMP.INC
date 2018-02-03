;===========================================================================;
;                                                                           ;
;          SUBROUTINES FOR SAVE SCREEN IMAGE AS BMP GRAPHICS FILE.          ;
;                                                                           ;
;===========================================================================;

;---------- Save graphics screen image to BMP file ----------------------;
; Don't direct read video memory, read transit buffer, optimal for       ;
; BltOnly mode.                                                          ;
; Yet fixed for true color mode, video mode=32BPP, file=24BPP.           ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          Global variables                                              ;
; OUTPUT:  Global variables                                              ;
;------------------------------------------------------------------------;

Save_Screen_to_BMP:

;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7

;--- Checkpoint 05Ch = Entry procedure save screen to BMP file ---
		CHK		05Ch

;--- Set current action type for errors reporting ---
		MOVIBW		@R1,0,_EFI_Action,2		; 2 = Save graphics screen by F10 key

;--- Adjust BMP header to current video mode parameters ---
		MOVSNW		R2,R1,_Save_BMP_Header
		MOVWW		R3,@R1,0,_Video_Xsize		; Get video mode X size, source=word
		MOVWW		R4,@R1,0,_Video_Ysize		; Get video mode Y size, source=word
		MOVDW		@R2,00,18,R3			; Set Xsize, dword
		MOVDW		@R2,00,22,R4			; Set Ysize, dword
		MUL64		R3,R4				; R3 = Screen size, pixels
		MOVIQW		R4,3				; BPP = 24, means 3 bytes per pixel
		MUL64		R3,R4				; R3 = Screen size, bytes
		MOVDW		@R2,00,34,R3			; Set image size w/o header, destination=dword
		MOVIQW		R4,14+40			; Prefix=14, Header=40 bytes
		ADD64		R3,R4
		MOVDW		@R2,00,02,R3			; Set file total size, destination=dword

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
;--- Label of cycle: free name find from PICT****.BMP ---
SSTB_0:		MOVSNW		R2,R1,_Name_BMP+4		; R2 = Destination pointer
		MOVQ		R3,R5				; R3 = Number value, 0000-9999
		MOVIQW		R4,4				; R4 = Template size = 4
		CALL32		String_Decimal32		; Update name numeric field: "0000" - "9999"
		MOVSNW		R2,R1,_Name_BMP
		CALL32		EFI_File_Path_Built		; Pointers: R2 = Name, R3 = Buffer
		MOVQ		R3,R2				; R3 = Pointer to file name
		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVIQW		R4,0000000000000003h		; R4 = Open mode: Open for Read and Write
		PUSH64		R5
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File		; This for check file exist
		POP64		R5
		JMP8CC		SSTB_1				; Go close file if no errors, but file exist
		MOVSNW		R7,R7				; Unify for 32 and 64
		MOVIQQ		R4,0FFFFFFFF00000000h
		AND64		R4,R7
		CMPI64WEQ	R4,0
		JMP8CS		SSTB_21				; Go file open error handling if unexpected error code
		MOVIQD		R4,07FFFFFFFh
		AND64		R7,R4
		CMPI32WEQ	R7,00000000Eh			; Compare with error code EFI_NOT_FOUND
		JMP8CS		SSTB_2				; Go create new file if expected error: file not exist
		JMP8		SSTB_21				; Go if file open error with unexpected error code

;--- Close existed file, go here if no errors but name exist, need try next name ---
SSTB_1:		MOVQ		R2,R6				; Set R2=Handle for close file
		CALL32		EFI_Close_File			; Close this existed file
		JMP8CS		SSTB_22				; Go if error close file
		MOVSNW		R5,R5,1				; R5+1, next number for file name PICT****.BMP
		CMPI32WULTE	R5,9999				; Compare with maximum acceptable value
		JMP8CC		SSTB_24				; Go error if file number overflow, above 9999
		JMP8		SSTB_0				; Go try next name after close existed

;--- Create new file, here R3=Pointer to name ---		; R3 = Pointer to name
SSTB_2:		MOVQW		R2,@R1,0,_File_Volume_Protocol	; R2 = Protocol (parent) handle
		MOVIQQ		R4,8000000000000003h		; R4 = Open mode: Create for Read and Write
		XOR64		R5,R5				; R5 = Attributes = 0
		CALL32		EFI_Open_Create_File		; This for create new file, return R6=Handle
		JMP8CS		SSTB_21				; Go if file create error

;--- Save BMP header ---
; Can't use complex open-write-close subroutines, because some writes, header + screen blocks
;---
		MOVQ		R2,R6				; R2 = Handle for file access
		MOVSNW		R3,R1,_Save_BMP_Header		; R3 = Buffer base address (buffer is BMP header)
		MOVIQW		R4,Prefix_and_Header_Size	; R4 = Buffer size
		MOVQ		R5,R4				; R5 = Save required size for compare
		CALL32		EFI_Write_File
		JMP8CS		SSTB_23				; Go if file write error
		CMP64EQ		R4,R5				; Compare reported and required size
		JMP8CC		SSTB_25				; Go if disk full error

;--- Prepare cycle for save BMP pixels array ---
; Note 1. Cannot direct read video memory, because BltOnly mode possible.
; Note 2. Must not corrupt R6=Handle.
;---
		MOVQW		R2,@R1,0,_Primary_Memory_Base	; R2 = Source base, transit buffer
		MOVWW		R3,@R1,0,_Video_Xsize		; R3 = Video mode X size, source=word
		MOVWW		R4,@R1,0,_Video_Ysize		; R4 = Video mode Y size, source=word
		MOVSNW		R5,R4,-1			; R5 = R4-1, means Y-1		
		MUL64		R4,R3				; R4 = Total pixels per screen
		MUL64		R5,R3				; R5 = Total pixels per screen - 1 string
		MOVIQW		R7,4				; BPP = 32 means 4 bytes per pixel
		MUL64		R5,R7				; R5 = Total bytes per screen - 1 string
		ADD64		R2,R5				; R2 = Positioning pointer to last line

;--- Start cycle for save BMP pixels array, write file ---
;--- Copy block from transit video buffer to file I/O buffer ---
; Note. Palette convert during copy, for minimize delays
; Note. Block size limited to 64KB, after porting from DOS, 
;       re-optimizing required
; Registers usage:
; R2 = Source pointer
; R3 = Video mode X size, pixels, updated as counter
; R4 = Total pixels per screen, updated as counter
; R5 = Destination pointer
; R6 = Handle, must not corrupt, saved when used as scratch pad
; R7 = Pixels per pass counter

;--- Cycle for all blocks ---
SSTB_3:		MOVQW		R5,@R1,0,_Secondary_Memory_Base
		MOVIQW		R7,21844			; 21844 pixels = 65532 bytes, BPP = 24
		CMP64ULTE	R4,R7
		JMP8CS		SSTB_4				; Go if this pass is last
		SUB64		R4,R7				; This for not last pass of copy
		JMP8		SSTB_5
SSTB_4:		MOVQ		R7,R4				; This for last pass of copy
		XOR64		R4,R4

;--- Start cycle for one block ---
SSTB_5:		PUSH64		R6				; Save file handle (1)
		PUSH64		R7				; Save pixels per pass (2)

;--- First check end of line ---		
SSTB_6:		MOVSNW		R3,R3,-1			; R3-1, decrement Xsize
		CMPI64WEQ	R3,-1
		JMP8CC		SSTB_7				; Go if not done yet
		MOVWW		R3,@R1,0,_Video_Xsize		; R3 = Video mode X size, source=word
		MOVIQW		R6,4*2				; 4 bytes per pixel, 2 strings: done + repositioning
		MUL64		R6,R3				; R6 = Xsize, bytes
		SUB64		R2,R6
		MOVSNW		R3,R3,-1			; R3-1, because number of passes is R3+1

;--- Copy with conversion ---
SSTB_7:		MOVD		R6,@R2				; Read pixel from source buffer (video transit)
		MOVD		@R5,R6				; Write pixel to destination buffer (file buffer)
		MOVSNW		R2,R2,4				; R2+4 , 4 bytes per pixel for video transit buffer
		MOVSNW		R5,R5,3				; R5+3 , 3 bytes per pixel for video file buffer

;--- End of one RGB conversion, cycle for low buffer ---
		MOVSNW		R7,R7,-1			; R7-1 , decrement counter
		CMPI64WEQ	R7,0
		JMP8CC		SSTB_6

;--- Restore this block size ---
		POP64		R7				; Restore pixels per pass (2)
		POP64		R6				; Restore file handle (1)

;--- Save buffer to file ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		MOVQ		R2,R6				; R2 = Handle for file access
		MOVQW		R3,@R1,0,_Secondary_Memory_Base	; R3 = Buffer base address
		MOVIQW		R4,3				; BPP = 24 means 3 bytes per pixel
		MUL64		R4,R7				; R4 = Block size for this pass, bytes
		MOVQ		R5,R4				; R5 = Save required size for compare
		CALL32		EFI_Write_File
		SUB64		R5,R4				; R5 = Required size, R4 = Executed size
		POP64		R4
		POP64		R3
		POP64		R2
		JMP8CS		SSTB_23				; Go if file write error
		CMPI64WEQ	R5,0				; Check reported and required size mismatch
		JMP8CC		SSTB_25				; Go if disk full error

;--- Cycle for entire video array ---
		CMPI64WEQ	R4,0				; R4 = Total size, pixels, counter
		JMP8CC		SSTB_3				; Go cycle if not all screen done

;--- Close file, input R6=Handle ---
		MOVQ		R2,R6				; Copy handle to R2
		CALL32		EFI_Close_File
		JMP8CS		SSTB_22				; Go if file close error

;--- Reserved for context restore after errors ---
SSTB_28:

;--- Checkpoint 05Fh = Exit procedure save screen to BMP file ---
		CHK		05Fh

;--- Restore registers and exit ---
		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET

;---------- Errors handling ---------------------------------------------;

SSTB_20:							; Entry for Memory errors
		; Reserved
SSTB_29:							; Common entry for some errors branches
		MOVSNW		R7,R7				; Unify status for 32 and 64
		MOVWW		@R1,0,_EFI_Act_Op,R6
		MOVQW		@R1,0,_EFI_Status,R7
		JMP8		SSTB_28
SSTB_21:							; Entry for File Open or Create Error
		MOVIQW		R6,0B03h			; Action=3, Operation=11
		JMP8		SSTB_29
SSTB_22:							; Entry for File Close Error
		MOVIQW		R6,0C03h
		JMP8		SSTB_29
SSTB_23:							; Entry for File Write Error
		MOVIQW		R6,0E03h
		JMP8		SSTB_29
SSTB_24:							; Entry for can't assign number, exist 10000 files
		MOVIQD		R7,080000009h			; Error=OUT_OF_RESOURCES (32-bit)
		JMP8		SSTB_23
SSTB_25:							; Entry for Disk Full Error
		MOVIQD		R7,08000000Bh			; Error=VOLUME_FULL (32-bit)
		JMP8		SSTB_23
