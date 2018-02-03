;===========================================================================;
;                                                                           ;
;   DISPATCHER AND LIBRARY FOR EXECUTE PROGRAM START AND STOP SEQUENCES.    ;
;                                                                           ;
;===========================================================================;

;--- Dispatcher for program start and stop sequences execution ----------;
; Handlers can corrupt registers R2-R7, dispatcher must save-restore it  ;
; if required for sequence execution, but not saved for caller.          ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address, use buffer on it     ;
;          R2 = Pointer to sequence control table, offset relative R1    ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, can continue                               ;
;          1(CS) = Error, program exit required                          ;
;          R2 = Updated pointer to sequence control table,               ;
;               offset relative R1                                       ;
;          R3-R7 corrupted                                               ;
;------------------------------------------------------------------------;
Dispatcher_Start_Stop:
;--- Prepare parameters ---
		ADD64		R2,R1		; R2 = Pointer to sequence control table
;--- Start of work cycle, prepare assist parameters ---
DSS_0:		MOVIQW		R6,1		; Assist for handlers, R6=1
		XOR64		R7,R7		; Assist for handlers, R7=0
;--- Get checkpoint byte, check end of sequence ---
		MOVB		R4,@R2		; R4 = Checkpoint byte
		CMP32UGTE	R4,R6		; 0 means end of sequence, other=debug code
		JMP8CC		DSS_1		; Go if end of sequence with Condition Flag=0
		CALL32		Send_Checkpoint_R4	; Conditional send checkpoint to debug port
;--- Get check bit number byte, check bitmap ---
		MOVBW		R4,@R2,0,1	; R4 = Bit number byte
		MOVIQW		R5,0040h	; Bit R5.6 = Unconditional mode selector
		AND32		R5,R4
		CMP32EQ		R5,R7		; Here R7=0
		JMP8CC		DSS_2		; Go if bit R4.6 = 1, for unconditional
		MOVIQW		R5,001Fh	; Bits R5.[4-0] = Shifts count
		AND32		R5,R4		; R5 = Shifts count
		MOVQ		R3,R6		; R3 = Shifts argument, here R6=1
		SHL32		R3,R5		; R3 = Mask for check
		MOVDW		R5,@R1,0,_Sys_Bitmap
		AND32		R5,R3
		CMP32EQ		R5,R7		; Here R7=0
		JMP8CC		DSS_2		; Go if selected bit = 1, make operation
;--- Cycle for sequence ---
DSS_5:		MOVSNW		R2,R2,8		; R2+8, address next element of sequence
		JMP8		DSS_0		; Go cycle for next element of sequence
;--- Start operation if enable bit=1, prepare parameters and select mode ---
DSS_2:		EXTNDW64	R5,@R2,0,4	; R5 = Offset for subroutine call
		ADD64		R5,R1		; Add R1=Base64 and R5=SignedOffset16
		MOVIQW		R3,0080h	; Bit R3.7 = means SG mode, last graphics
		AND32		R3,R4		; Mask bit D7
		CMP32EQ		R3,R7		; Here R7=0
		JMP8CC		DSS_3		; Go if bit R4.7=1, means last graphics
		MOVIQW		R3,0020h	; Bit R3.5 = means SS mode, last text
		AND32		R3,R4		; Mask bit D5
		CMP32EQ		R3,R7		; Here R7=0
		JMP8CC		DSS_4		; Go if bit R4.5=1, means last text

;--- Normal branch: D7=0, D5=0 ---
; First write message, then call target procedure, then analyse status (OK,Failed)
;--- 
		CALL32		DSS_11		; Write text message
		CALL32		DSS_10		; Call target procedure
DSS_8:		PUSH64		R7		; R7 must be saved, it is status
		PUSH64		R2		; R2 must be saved, it is sequence pointer
		MOVIQW		R2,_Msg_OK
		CMPI32WEQ	R7,0
		JMP8CS		DSS_6		; Go if status=0 means OK
		MOVIQW		R2,_Msg_FAILED
DSS_6:		CALL32		String_Write	; Write result string (right part)
		MOVIQW		R2,_Msg_CRLF	; CRLF = Carriage return, Line feed
		CALL32		String_Write	; Next string
		POP64		R2
		POP64		R7
		CMPI32WEQ	R7,0
		JMP8CC		DSS_7		; Go if error
		JMP8		DSS_5		; Go cycle for next element of sequence

;---------- Special branches -----------------------------------------------;

;--- Special branch 1 (D7=1), for step, switching to new video mode ---
; Support last executed in the new graphics mode, set original mode (SG)
; First call target procedure, then write message, then analyse status (OK,Failed)
; Required for correct video output at this step
;---
DSS_3:		CALL32		DSS_10		; Call target procedure
		PUSH64		R7		; Twice for alignment
		PUSH64		R7
		CALL32		DSS_11		; Write text message
		POP64		R7
		POP64		R7
		JMP8		DSS_8		; Go to analyse status and sequence cycle

;--- Special branch 2 (D5=1), for step, switching to new video mode ---
; Support last executed in the original mode, set new graphics mode (SS)
; First write message, then call target procedure, skip analyse status (OK,Failed)
; Required for correct video output at this step
;---
DSS_4:		CALL32		DSS_11		; Write text message
		CALL32		DSS_10		; Call target procedure
		JMP8		DSS_5		; Go to sequence cycle

;---------- Exit points ----------------------------------------------------;

;--- Write error message and exit, start write ---
; Note. Stack alignment required because called subroutines can use UEFI API.
;---
DSS_7:		MOVQ		R6,R7		; Save status after handler call
		MOVQ		R3,R2		; Save sequence pointer
		MOVIQW		R2,0Ch		; Color=Bold Red
		CALL32		Output_Attribute  ; Set color
		MOVIQW		R2,_Msg_Error	; String "ERROR: "
		CALL32		String_Write
		EXTNDW64	R2,@R3,0,6	; R2 = Offset for error string
		CALL32		String_Write
		MOVQ		R7,R6		; Restore status
;--- Decode UEFI status ---
		MOVSNW		R2,R1,256	; R2 = Scratch pad, skip used by String_Write
		CALL32		Strings_Errors	; This receive R2=Absolute address
		MOVIBW		@R2,0		; 0=Terminator byte
		MOVIQW		R2,256		; R2 = Scratch pad, skip used by String_Write
		CALL32		String_Write	; This receive R2=Relative address
;--- Restore attribute, pointer and terminate ---
		MOVIQW		R2,07h		; Color=White
		CALL32		Output_Attribute  ; Set color
		MOVSNW		R2,R3,8		; Restore and modify pointer, R2=R3+8
;--- Return, here must be valid Condition Flag (CF) ---
		CMPI32WUGTE	R1,0		; This for set Condition Flag=1
DSS_1:		RET

;---------- Local subroutines ----------------------------------------------;
; Note. Stack alignment required because called subroutines can use UEFI API.

;--- Local subroutine: call target procedure ---
; Input:   R5 = Address for call
;          R6 = 1, code assist
;          R6 = 0, code assist
; Output:  R7 = Status, converted to 64-bit
;---
DSS_10:		PUSH64		R2		; R2 must be saved, it is sequence pointer
		PUSH64		R2		; Twice for alignment
		XOR64		R7,R7		; Restore R7=0 after String_Write
		CALL32A		R5		; Call handler, built text string or benchmark
		MOVSNW		R7,R7		; R7 = Status, 64-bit
DSS_12:		POP64		R2
		POP64		R2		; Twice for alignment
		RET

;--- Local subroutine: write text message ---
; Input:   R2 = Sequence pointer
; Output:  R7 = Status
;---
DSS_11:		PUSH64		R2		; R2 must be saved, it is sequence pointer
		PUSH64		R2		; Twice for alignment
		EXTNDW64	R2,@R2,0,2	; R2 = Offset for message string
		CALL32		String_Write	; Write message string (left part)
		JMP8		DSS_12


