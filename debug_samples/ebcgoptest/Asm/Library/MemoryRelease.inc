;--- Free memory pages from the UEFI firmware ---------------------------;
; Use 64-bit flat memory addressing even if IA32.                        ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Base Address of memory block                             ;
;          R3 = Number of contiguous 4KB pages                           ;
;                                                                        ;
; OUTPUT:  Condition flag: 0(CC)=Successfully, 1(CS)=Failed              ;
;          R7 = UEFI Status                                              ;
;------------------------------------------------------------------------;

EBC_Free_Pages:
;--- Built function stack frame ---
		PUSHN		R3		; Parm#2=Number of Pages
		PUSH64		R2		; Parm#1=Base address of block (QWORD)
;--- Read pointer and handler call ---
		MOVNW		R7,@R1,0,_EFI_Table	; R7=SysTable
		MOVNW		R7,@R7,9,24		; R7=BootServices
		CALL32EXA	@R7,3,24		; Header=24, Element=3
;--- Remove stack frame ---
		POP64		R2		; Remove Parm#1
		POPN		R3		; Remove Parm#2
;--- Go to set Condition Flag=F(R7) and return ---
		JMP8		EBC_AP_0
