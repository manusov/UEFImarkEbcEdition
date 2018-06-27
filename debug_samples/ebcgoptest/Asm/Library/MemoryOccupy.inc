;--- Allocate memory pages from the UEFI firmware -----------------------;
; Use 64-bit flat memory addressing even if IA32.                        ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = EFI Memory Allocate Type                                 ;
;          R3 = EFI Memory Type                                          ;
;          R4 = Number of contiguous 4KB pages                           ;
;          R5 = Address Value or Limit, used only for                    ;
;               AllocateAddress, AllocateMaxAddress                      ;
;                                                                        ;
; OUTPUT:  Condition flag: 0(CC)=Successfully, 1(CS)=Failed              ;
;          R7 = UEFI Status                                              ;
;          R5 = Base address, if allocate OK                             ;
;------------------------------------------------------------------------;

EBC_Allocate_Pages:
;--- Built parameter storage, addressed by Parm#4 ---
		PUSH64		R5		; Address field for Parm#4 (QWORD)
;--- Built function stack frame ---
		PUSHN		R0		; Parm#4=Pointer to address
		PUSHN		R4		; Parm#3=Number of Pages
		PUSHN		R3		; Parm#2=Memory Type
		PUSHN		R2		; Parm#1=Allocate Type
;--- Read pointer and handler call ---
		MOVNW		R7,@R1,0,_EFI_Table	; R7=SysTable
		MOVNW		R7,@R7,9,24		; R7=BootServices
		CALL32EXA	@R7,2,24		; Header=24, Element=2
;--- Remove stack frame ---
		POPN		R2		; Remove Parm#1
		POPN		R3		; Remove Parm#2
		POPN		R4		; Remove Parm#3
		POPN		R5		; Remove Parm#4
;--- Get data from parameter storage, addressed by Parm#4 ---
		POP64		R5		; Allocated base address
;--- Convert 32/64-bit results to 64-bit, analyse status ---
		MOVNW		R5,R5		; Make address 64-bit
EBC_AP_0:	MOVSNW		R7,R7		; Determine D[63-32]
		CMPI64WUGTE	R7,1		; Check status, set CS if R7>0
;--- Return, here Condition Flag=F(R7)  ---
		RET
