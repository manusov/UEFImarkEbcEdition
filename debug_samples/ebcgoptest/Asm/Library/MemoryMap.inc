;------------------------------------------------------------;
; EFI Memory Descriptor format, note alignment 8 for x64 UEFI
; Address, hex    Size       Comments
;------------------------------------------------------------;
; 00-03           Dword      Memory Type, see definition
; 04-07           Dword      Reserved for alignment
; 08-0F           Qword      EFI Physical Address
; 10-17           Qword      EFI Virtual Address
; 18-1F           Qword      Number of pages
; 20-27           Qword      Memory Attribute, see definition
;------------------------------------------------------------;
;
; Memory Type definition
; Num, hex    Type
;------------------------------------------------------------;
; 00          EfiReservedMemoryType 
; 01          EfiLoaderCode
; 02          EfiLoaderData
; 03          EfiBootServicesCode
; 04          EfiBootServicesData
; 05          EfiRuntimeServicesCode
; 06          EfiRuntimeServicesData
; 07          EfiConventionalMemory
; 08          EfiUnusableMemory
; 09          EfiAcpiReclaimMemory
; 0A          EfiAcpiMemoryNvs
; 0B          EfiMemoryMappedIo
; 0C          EfiMemoryMappedIoPortSpace
; 0D          EfiPalCode
; 0E          EfiMaxMemoryType (limit value only)
;
; Memory Allocate Type definition
; Num, hex    Type
;------------------------------------------------------------;
; 00          AllocateAnyPages
; 01          AllocateMaxAddress
; 02          AllocateAddress
; 03          MaxAllocateType (limit value only)
;
; Memory Attribute definition, not listed bits reserved
; Bit   Attribute
;------------------------------------------------------------;
; 00    UC = Uncacheable
; 01    WC = Write Combining
; 02    WT = Write Through (for cache)
; 03    WB = Write Back (for cache)
; 04    UCE = Uncacheable exported (for atomic ops. support)
; 12    WP = Write Protected (by hardware)
; 13    RP = Read Protected (by hardware)
; 14    XP = Execution Protected (for code fetch, by hardw.)
; 63    Runtime, means allocatable for paging
;------------------------------------------------------------;

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
		JMP8		EBC_AP_0

