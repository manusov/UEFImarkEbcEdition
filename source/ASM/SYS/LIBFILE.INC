;===========================================================================;
;                                                                           ;
;               SUBROUTINES FOR UEFI FILE OPERATIONS SUPPORT.               ;
;                                                                           ;
;===========================================================================;

;--- Open or Create file for selected parent handle ---------------------;
; Note. Can create directory, if attributes means it.                    ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          R2 = Parent handle (example: directory for file)              ;
;          R3 = Pointer to NULL-terminated UNICODE16 string: file name   ;
;          R4 = Open mode: D63=Create, D1=Write, D0=Read                 ;
;                 D1,D0 must be "1" if D63=1, other bits reserved        ;
;          R5 = Attributes:                                              ;
;                 D0=Read Only, D1=Hidden, D2=System, D3=Reserved        ;
;                 D4=Directory, D5=Archive, other bits reserved          ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, 1(CS) = Error (reserved)                   ;
;          R7 = UEFI Status                                              ;
;          R6 = Opened/Created file handle, if no errors                 ;
;------------------------------------------------------------------------;
EFI_Open_Create_File:
;--- Built scratch pad for output parameters ---
		XOR64		R7,R7
		PUSHN		R7
		MOVQ		R7,R0		; R0 (Stack pointer) = Output parameter address
;--- Built alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Built input parameters stack frame ---
		PUSH64		R5		; Parameter #5 = Attributes, QWORD
		PUSH64		R4		; Parameter #4 = Open mode, QWORD
		PUSHN		R3		; Parameter #3 = String Pointer, DWORD if IA32, QWORD if x64
		PUSHN		R7		; Parameter #2 = Pointer to output: new handle
		PUSHN		R2		; Parameter #1 = Parent handle
;--- Call function ---
		CALL32EXA	@R2,0,8		; EFI_FILE_PROTOCOL.Open (N=0, C=8)
;--- Remove input parameters stack frame ---
		POPN		R2
		POPN		R6
		POPN		R3
		POP64		R4
		POP64		R5
;--- Remove alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Read output parameter, remove scratch pad ---
		POPN		R6
;--- Verify function status ---
EOCF_0:		MOVSNW		R7,R7		; Unify for 32 and 64
		CMPI64WUGTE	R7,1		; Condition Flag=1 if R7>0
;--- Exit ---
		RET


;--- Close file for selected file handle --------------------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          R2 = File handle                                              ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, 1(CS) = Error (reserved)                   ;
;          R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;
EFI_Close_File:
;--- Save registers ---				; R0 is stack pointer
		PUSH64		R4		; R1-R3 preserved across calls by specification
		PUSH64		R5		; R4-R6 need save-restore
		PUSH64		R6		; R7 is output parameter: UEFI status
;--- Built alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Built input parameters stack frame ---
		PUSHN		R2		; Parameter #1 = File handle
;--- Call function ---
		CALL32EXA	@R2,1,8		; EFI_FILE_PROTOCOL.Close (N=1, C=8)
;--- Remove input parameters stack frame ---
ECF_0:		POPN		R2
;--- Remove alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Restore registers and exit ---
		POP64		R6
		POP64		R5
		POP64		R4
		JMP8		EOCF_0

;--- Delete file for selected file handle -------------------------------;
; Note. File must be opened for delete, because handle must be valid.    ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          R2 = File handle                                              ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, 1(CS) = Error (reserved)                   ;
;          R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;
EFI_Delete_File:
;--- Save registers ---				; R0 is stack pointer
		PUSH64		R4		; R1-R3 preserved across calls by specification
		PUSH64		R5		; R4-R6 need save-restore
		PUSH64		R6		; R7 is output parameter: UEFI status
;--- Built alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Built input parameters stack frame ---
		PUSHN		R2		; Parameter #1 = File handle
;--- Call function ---
		CALL32EXA	@R2,2,8		; EFI_FILE_PROTOCOL.Delete (N=2, C=8)
;--- Go to remove frames, restore registers and exit ---
		JMP8		ECF_0

;--- Read file for selected file handle ---------------------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          R2 = File handle                                              ;
;          R3 = Buffer base address, flat 64-bit                         ;
;          R4 = Buffer size, allocated total                             ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, 1(CS) = Error (reserved)                   ;
;          R7 = UEFI Status                                              ;
;          R4 = Buffer size updated, really read size, valid if no errs. ;
;------------------------------------------------------------------------;
EFI_Read_File:
;--- Built scratch pad for input and output parameter ---
		PUSH64		R4
		MOVQ		R4,R0		; R0 (Stack pointer) = Parameter address
;--- Built alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Built input parameters stack frame ---
		PUSHN		R3		; Parameter #3 = Buffer base address, DWORD if IA32, QWORD if x64
		PUSHN		R4		; Parameter #2 = Buffer size, DWORD if IA32, QWORD if x64
		PUSHN		R2		; Parameter #1 = Protocol handle, DWORD if IA32, QWORD if x64
;--- Call function ---
		CALL32EXA	@R2,3,8		; EFI_FILE_PROTOCOL.Read (N=3, C=8)
;--- Remove input parameters stack frame ---
ERF_0:		POPN		R2
		POPN		R4
		POPN		R3
;--- Remove alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Read output parameter, remove scratch pad ---
		POP64		R4		; R4 = Updated by really size of read file
;--- Go to verify function status and exit ---
		JMP8		EOCF_0

;--- Write file for selected file handle --------------------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          R2 = File handle                                              ;
;          R3 = Buffer base address, flat 64-bit                         ;
;          R4 = Buffer size, allocated total                             ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, 1(CS) = Error (reserved)                   ;
;          R7 = UEFI Status                                              ;
;          R4 = Buffer size updated, really write size, valid if no err. ;
;------------------------------------------------------------------------;
EFI_Write_File:
;--- Built scratch pad for input and output parameter ---
		PUSH64		R4
		MOVQ		R4,R0		; R0 (Stack pointer) = Parameter address
;--- Built alignment pad ---
		;
		; UNDER CONSTRUCTION, need replace for adaptive to input R0
		;
;--- Built input parameters stack frame ---
		PUSHN		R3		; Parameter #3 = Buffer base address, DWORD if IA32, QWORD if x64
		PUSHN		R4		; Parameter #2 = Buffer size, DWORD if IA32, QWORD if x64
		PUSHN		R2		; Parameter #1 = Protocol handle, DWORD if IA32, QWORD if x64
;--- Call function ---
		CALL32EXA	@R2,4,8		; EFI_FILE_PROTOCOL.Write (N=4, C=8)
;--- Go to remove frames, restore registers and exit ---
		JMP8		ERF_0


;--- Built path for files in the program directory ----------------------;
; Convert path and file name string from ASCII to UNICODE                ;
; Set address for file I/O buffer                                        ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, flat 64-bit          ;
;          R2 = Pointer to file name for add, ASCII, flat 64-bit         ;
;          Global variables                                              ;
;                                                                        ;
; OUTPUT:  R2 = Pointer to path\name string, UNICODE , flat 64-bit       ;
;          R3 = Pointer to free buffer for file I/O, flat 64-bit         ;
;------------------------------------------------------------------------;
EFI_File_Path_Built:

		; UNDER CONSTRUCTION, YET NOT ADD PATH, OUTPUT STRING = INPUT STRING

		MOVQW		R3,@R1,0,_Secondary_Memory_Base
		CALL32		Convert_String_1

		MOVQW		R2,@R1,0,_Secondary_Memory_Base	; R2=Name string pointer
		MOVSNW		R3,R2,32			; R3=R2+32

		RET

