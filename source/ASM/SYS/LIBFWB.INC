;===========================================================================;
;=                                                                         =;
;=          LIBRARY FOR FOR DETECT PLATFORM BY FIRMWARE DATA BASE.         =;
;= FW. DATA BASE FOR SUPPORT FIRMWARE VERSION-SPECIFIC FEATURES AND BUGS.  =;
;=                                                                         =;
;===========================================================================;

;--- Return firmware revision features/bugs list ------------------------;
; Detect platform F(firmware vendor and revision) return bitmap,         ;
; see DATAFWR.INC for bitmap firmware features/bugs layout.              ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
; OUTPUT:  R7 = Firmware features/bugs bitmap                            ;
;               if firmware not recognized, R7=0                         ;
;------------------------------------------------------------------------;
Firmware_Detect:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
;--- Load system parameters ---
		MOVQW		R2,@R1,0,_EFI_Table		; R2 = Pointer to EFI System Table
		MOVDW		R7,@R2,01,24			; R7 = Platform firmware revision dword
		MOVQW		R2,@R2,00,24			; R2 = Pointer to platform firmware string
;--- Recognize firmware vendor ---
		MOVSNW		R3,R1,_Firmware_Vendors		; R3 = Pointer to program data base
FWDT_2:		MOVQ		R4,R2				; R4 = Temporary copy of R2
FWDT_3:		MOVW		R5,@R4				; R5 = Word from firmware (UNICODE16)
		MOVSNW		R4,R4,2				; R4+2, modify firmware pointer
		MOVB		R6,@R3				; R6 = Byte from data base (ASCII)
		MOVSNW		R3,R3,1				; R3+1, modify data base pointer
		CMPI32WEQ	R5,0				; Check end of firmware string
		JMP8CC		FWDT_4				; Go if not end of firmware string
		CMPI32WEQ	R6,0				; Check end of data base string
		JMP8CS		FWDT_5				; Go if end of both strings
FWDT_4:		CMP32EQ		R5,R6				; Compare chars: word and zero-extended byte
		JMP8CS		FWDT_3				; Go cycle if chars match
;--- Skip tail if firmware string > data base string ---
FWDT_6:		MOVB		R6,@R3				; R6 = Byte from data base (ASCII)
		MOVSNW		R3,R3,1				; R3+1, modify data base pointer
		CMPI32WEQ	R6,0				; Check for string terminator
		JMP8CC		FWDT_6
;--- Go to compare with next string terminator not detected ---
		MOVSNW		R3,R3,2				; R3+2, skip data offset addend
		MOVB		R6,@R3				; R6 = Byte from data base (ASCII)
		CMPI32WEQ	R6,0				; Check for sequence terminator
		JMP8CC		FWDT_2				; Go if no sequence terminator
;--- Restore registers and exit ---
FWDT_0:		XOR64		R7,R7				; Entry 1: R7=0 if firmware not recognized
FWDT_1:		POP64		R6				; Entry 2: R7=Bitmap if firmware recognized
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET
;--- Continue if firmware vendor string match ---
FWDT_5:		EXTNDW64	R2,@R3				; R2 = Signed addend for R1
		ADD64		R2,R1				; R2 = Pointer to revisions list, data base
FWDT_7:		MOVD		R3,@R2				; R3 = Revision dword from data base
		CMP32EQ		R3,R7				; R7 = Revision dword from firmware, compare it
		JMP8CS		FWDT_8				; Go if revision match
		CMPI32WEQ	R3,0				; Compare with terminator code
		JMP8CS		FWDT_0				; Go if end of list, matches not found
		MOVSNW		R2,R2,8				; R2+8, modify pointer
		JMP8		FWDT_7
;--- Continue if firmware revision dword match ---
FWDT_8:		MOVDW		R7,@R2,0,4			; R7 = Firmware revision features/bugs list
		JMP8		FWDT_1				; Go exit with valid R7

