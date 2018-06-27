;--- Locate EFI protocol by GUID ----------------------------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Pointer to Protocol 128-bit GUID                         ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, key ready, 1(CS) = Error (reserved)        ;
;          R7 = UEFI Status                                              ;
;          R2 = Protocol pointer if operation successfully               ;
;------------------------------------------------------------------------;
EBC_Locate_Protocol:
;--- Save registers ---
		;
;--- Built function stack frame ---
		XOR64		R3,R3
		PUSHN		R3		; Storage for output
		PUSHN		R0		; Parm#3 = Output pointer
		PUSHN		R3		; Parm#2 = Registration
		PUSHN		R2		; Parm#1 = Pointer to GUID
;--- Read pointer and handler call ---
		MOVNW		R3,@R1,0,_EFI_Table	; R3 = SysTable
		MOVNW		R3,@R3,9,24		; R3 = BootServices
		CALL32EXA	@R3,37,24
;--- Remove stack frame ---
		POPN		R2		; Parm#1
		POPN		R3		; Parm#2
		POPN		R3		; Parm#3
		POPN		R2		; Protocol pointer
;--- Check status and result ---
		MOVSNW		R7,R7
		CMPI64WUGTE	R7,1		; Check status
		JMP8CS		EBC_LP_0
		CMPI64WEQ	R2,0		; Check protocol pointer
;--- Restore registers and return ---
EBC_LP_0:	;
		RET