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

;--- Initialization routine handler: Allocate memory, get memory info ------;
; Use EFI memory control API, built map, save summary info and pointers.    ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;               Global variables:                                           ;
;                QWORD  EFI_Memory_Total      , total memory used by UEFI   ;
;                QWORD  EFI_Memory_ACPI       , memory used by ACPI         ;
;                QWORD  Primary_Memory_Base   , base address of allocated   ;
;                QWORD  Primary_Memory_Size   , size of of allocated        ;
;                QWORD  Secondary_Memory_Base , base of separate part       ;
;                QWORD  Secondary_Memory_Size , size of separate part       ;
;---------------------------------------------------------------------------;

Allocate_Memory:
;--- Try memory allocate ---
		XOR64		R2,R2		; R2 = EFI Memory Allocate Type = 0
		MOVIQW		R3,4		; R3 = EFI Memory Type = BS DATA = 4
		MOVIQW		R4,8192		; R4 = Num. of contiguous 4KB pages = 8129 (32MB)
		XOR64		R5,R5		; R5 = Limit, here not used = 0
		CALL32		EFI_Allocate_Pages
		JMP8CS		AM_0		; CC condition if error
;--- Calculate results, here R5=Base Address, R4=Size (pages), R2=0 ---
		MOVIQW		R7,12
		SHL64		R4,R7		; Pages->Bytes
		MOVIQW		R7,0Fh		; Mask for check align 16
		MOVQ		R6,R7
		AND64		R6,R5
		CMP64EQ		R6,R2		; Here R2=0
		JMP8CS		AM_1		; Go if already aligned
		MOVSNW		R6,R7,1		; R6 = R7+1   = 0000000000000010h
		NOT64		R7,R7		; R7 = NOT R7 = FFFFFFFFFFFFFFF0h
		AND64		R5,R7		; R5 Align 16, clear bits [3-0]
		ADD64		R5,R6		; R5 + 16
;--- Save results: Base and Size, include for file operations ---
; Note primary buffer used for video graphics data transit
; Note secondary buffer used for file operations and for generating memory map
; Note secondary buffer is last 128KB of primary buffer
AM_1:		MOVQW		@R1,0,_Primary_Memory_Base,R5
		MOVQW		@R1,0,_Primary_Memory_Size,R4
		MOVIQD		R7,131072
		ADD64		R5,R4		
		SUB64		R5,R7
		MOVQW		@R1,0,_Secondary_Memory_Base,R5
		MOVQW		@R1,0,_Secondary_Memory_Size,R7
;--- Set flag for memory release required ---
		MOVDW		R2,@R1,0,_Sys_Bitmap
		MOVIQW		R3,0008h
		OR32		R2,R3
		MOVDW		@R1,0,_Sys_Bitmap,R2
;--- Generating memory map ---
; Subroutine return parameters:
; R2 = Base address for return memory map (input unchanged)
; R3 = Size updated, can be >Input or <Input
; R4 = Map key
; R5 = One descriptor size
; R6 = Descriptor version
; R7 = UEFI Status
;---
		MOVQW		R2,@R1,0,_Secondary_Memory_Base
		MOVQW		R3,@R1,0,_Secondary_Memory_Size
		CALL32		EFI_Get_Memory_Map
		JMP8CS		AM_0
;--- Start analysing memory map, calculate normal memory size ---
		MOVIQW		R4,00FEh	; Mark types [1-7] = 1
		CALL32		Local_AM_9
		MOVQW		@R1,0,_EFI_Memory_Total,R7
;--- Calculate ACPI-reserved memory size ---
		MOVIQW		R4,0600h	; Mark types [9-10] = 1
		CALL32		Local_AM_9
		MOVQW		@R1,0,_EFI_Memory_ACPI,R7
;--- Clear status ---
AM_2:		XOR64		R7,R7
;--- Return ---
AM_0:		RET


;---------- Local subroutine -----------------------------------------------;

;--- Calculate total size of selected type memory pages -----------------;
; INPUT:   R2 = Base of memory map                                       ;
;          R3 = Size of memory map, bytes                                ;
;          R4 = Type selection bitmap                                    ;
;          R5 = One descriptor (map element) size, bytes                 ;
;                                                                        ;
; OUTPUT:  R7 = Calculated size, kilobytes                               ;
;------------------------------------------------------------------------;
Local_AM_9:	PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
;--- Work cycle ---
		XOR64		R7,R7		; R7=0, clear sum=size
AM_10:		MOVD		R6,@R2		; R6=Type dword
		CMPI32WULTE	R6,31		; Type limit
		JMP8CC		AM_11		; Skip if Type>Limit
		PUSH64		R7
		MOVIQW		R7,0001h
		SHL64		R7,R6		; Positioning mask bit
		AND64		R7,R4		; Check selected bit
		CMPI64WEQ	R7,0000h
		POP64		R7
		JMP8CS		AM_11		; Skip if selected bit=0
		ADD64		R7,@R2,0,18h	; Add block size
AM_11:		ADD64		R2,R5		; Select next descriptor
		SUB64		R3,R5
		CMPI64WLTE	R3,0
		JMP8CC		AM_10		; Cycle for all descriptors
		MOVIQW		R6,2
		SHL64		R7,R6		; 4KB Pages -> Kilobytes
;--- Restore registers and exit ---
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET

