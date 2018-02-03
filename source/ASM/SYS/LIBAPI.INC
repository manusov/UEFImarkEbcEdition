;===========================================================================;
;=                                                                         =;
;=           LIBRARY FOR ACCESS UEFI API AND MEMORY MANAGEMENT.            =;
;=                                                                         =;
;===========================================================================;

;--- Locate EFI protocol by GUID ----------------------------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Pointer to Protocol 128-bit GUID,                        ;
;               signed 16-bit offset relative R1                         ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0(CC) = No errors, R2 valid, 1(CS) = Error                    ;
;          R7 = UEFI Status                                              ;
;          R2 = Protocol pointer if operation successfully               ;
;------------------------------------------------------------------------;
Locate_Protocol:
;--- Calculate absolute pointer ---
		ADD64		R2,R1
;--- Save registers ---
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
;--- Built function stack frame ---
		XOR64		R3,R3
		PUSHN		R3		; Storage for output
		PUSHN		R0		; Parm#3 = Output pointer
		PUSHN		R3		; Parm#2 = Registration
		PUSHN		R2		; Parm#1 = Pointer to GUID
;--- Read pointer and handler call ---
		MOVNW		R3,@R1,0,_EFI_Table	; R3 = SysTable
		MOVNW		R3,@R3,9,24		; R3 = BootServices
		CALL32EXA	@R3,37,24		; Entry #37 = Locate Protocol
;--- Remove stack frame ---
		POPN		R2		; Parm#1
		POPN		R3		; Parm#2
		POPN		R3		; Parm#3
		POPN		R2		; Protocol pointer
;--- Check status and result ---
		MOVSNW		R7,R7
		CMPI64WUGTE	R7,1		; Check status
		JMP8CS		LP_0
		CMPI64WEQ	R2,0		; Check protocol pointer
;--- Restore registers and return ---
LP_0:		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R3
		RET

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

EFI_Allocate_Pages:
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
EAP_0:		MOVSNW		R7,R7		; Determine D[63-32]
		CMPI64WUGTE	R7,1		; Check status, set CS if R7>0
;--- Return, here Condition Flag=F(R7)  ---
		RET


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

EFI_Free_Pages:
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
		JMP8		EAP_0


;--- Get UEFI firmware memory map ---------------------------------------;
; Use 64-bit flat memory addressing even if IA32.                        ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Base address for return memory map                       ;
;          R3 = Memory map size limit                                    ;
;                                                                        ;
; OUTPUT:  Condition flag: 0(CC)=Successfully, 1(CS)=Failed              ;
;          R7 = UEFI Status                                              ;
;          R2 = Base address for return memory map (unchanged)           ;
;          R3 = Size updated, can be >Input or <Input                    ;
;               if Size>Input, biggest buffer req.                       ;
;          R4 = Map key                                                  ;
;          R5 = One descriptor size                                      ;
;          R6 = Descriptor version                                       ;
;          Memory map returned if status OK                              ;
;------------------------------------------------------------------------;

EFI_Get_Memory_Map:
;--- Reserve stack space for output parameters ---
		PUSH64		R3		; [R0+32] = Map Size = Input R3
		XOR64		R7,R7		; R7=0
		PUSH64		R7		; [R0+16] = Map Key
		PUSH64		R7		; [R0+08] = Descriptor Size
		PUSH64		R7		; [R0+00] = Descriptor Version
;--- Built function stack frame ---
		MOVQ		R7,R0
		PUSHN		R7		; Parm#5=Pointer to Descriptor Version
		MOVNW		R7,R7,0,8	; R7+8
		PUSHN		R7		; Parm#4=Pointer to Descriptor Size
		MOVNW		R7,R7,0,8	; R7+8
		PUSHN		R7		; Parm#3=Pointer to Map Key
		PUSHN		R2		; Parm#2=Map Pointer = Input R2
		MOVNW		R7,R7,0,8	; R7+8
		PUSHN		R7		; Parm#1=Pointer to Map Size
;--- Read pointer and handler call ---
		MOVNW		R7,@R1,0,_EFI_Table	; R7=SysTable
		MOVNW		R7,@R7,9,24		; R7=BootServices
		CALL32EXA	@R7,4,24		; Header=24, Element=4
;--- Remove stack frame ---
		POPN		R3		; Remove pointer Parm#1
		POPN		R2		; Read Parm#2: R2 = Map Pointer
		POPN		R3		; Remove pointer Parm#3
		POPN		R3		; Remove pointer Parm#4
		POPN		R3		; Remove pointer Parm#5
;--- Read updated parameters fields ---
		POP64		R6		; R6 = Descriptor Version
		POP64		R5		; R5 = Descriptor Size
		POP64		R4		; R4 = Map Key
		POP64		R3		; R3 = Map Size
;--- Go to set Condition Flag=F(R7) and return ---
		JMP8		EAP_0

