;===========================================================================;
;                                                                           ;
;            LIBRARY FOR ACPI DETECTION AND TABLES INTERPRETING.            ;
;                                                                           ;
;===========================================================================;

; Subroutines located especially for 8-bit offsets sufficient,
; don't change location without verify offsets in the JMP8xx instructions.


;--- Scan Root System Description Table, find table by signature -----------;
; This subroutine use 32-bit entries for ACPI 1 , see also Scan_XSDT        ;
; Checksum verified for RSDT and target table if found                      ;
;                                                                           ;
; INPUT:   R2 = Base address of RSDT                                        ;
;          R3 = Signature for find required table                           ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = Pointer to found table, valid if no errors (CC condition)   ;
;          If required table not found, but no errors detected, returns:    ;
;          CF=CC , R7=0                                                     ; 
;---------------------------------------------------------------------------;
Scan_RSDT:
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		MOVQ		R4,R3			; R4 = Save required signature
;--- Verify signature of RSDT ---
		MOVIQD		R3,'RSDT'
		CMP32EQ		R3,@R2			; Compare RSDT signature
		JMP8CC		GACR_6			; Go if RSDT signature mismatch
;--- Get RSDT length, verify checksum of RSDT ---
		MOVDW		R3,@R2,0,4		; R3 = Table length
		CALL32		Checksum_ACPI		; Checksum by R2=Base, R3=Length
		JMP8CS		GACR_6			; Go if RSDT checksum mismatch
;--- Calculate base and limit for scan region ---
		ADD64		R3,R2			; R3 = Limit = Length + Base = Limit (exclusive)
		MOVSNW		R2,R2,36		; R7 = New base = Pointer to entries list
;--- Cycle for scan RSDT for required table ---
SRSD_0:		MOVD		R7,@R2			; R7 = Pointer to current checked table
		CMP32EQ		R4,@R7			; R4 = Required signature, @R7 = Scanned table
		JMP8CS		SRSD_1			; Go if signature match, required table found
		MOVSNW		R2,R2,4			; R2 = Pointer to next 32-bit entry
		CMP64UGTE	R2,R3
		JMP8CC		SRSD_0			; Cycle for RSDT entries scan
		JMP8		GACR_7			; Go if not found, make CF=CC, R7=0
;--- Clear CF flag if R7 valid and go return ---
SRSD_1:		MOVQ		R2,R7			; R2 = Table base
		MOVDW		R3,@R2,0,4		; R3 = Table length
		CALL32		Checksum_ACPI		; Checksum by R2=Base, R3=Length
		JMP8		GACR_5


;--- Scan Extended System Description Table, find table by signature -------;
; This subroutine use 64-bit entries for ACPI 2+ , see also Scan_RSDT       ;
; Checksum verified for XSDT and target table if found                      ;
;                                                                           ;
; INPUT:   R2 = Base address of XSDT                                        ;
;          R3 = Signature for find required table                           ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R7 = Pointer to found table, valid if no errors (CC condition)   ;
;          If required table not found, but no errors detected, returns:    ;
;          CF=CC , R7=0                                                     ; 
;---------------------------------------------------------------------------;
Scan_XSDT:
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
		MOVQ		R4,R3			; R4 = Save required signature
;--- Verify signature of XSDT ---
		MOVIQD		R3,'XSDT'
		CMP32EQ		R3,@R2			; Compare XSDT signature
		JMP8CC		GACR_6			; Go if RSDP signature mismatch
;--- Get XSDT length, verify checksum of XSDT ---
		MOVDW		R3,@R2,0,4		; R3 = Table length
		CALL32		Checksum_ACPI		; Checksum by R2=Base, R3=Length
		JMP8CS		GACR_6			; Go if RSDP checksum mismatch
;--- Calculate base and limit for scan region ---
		ADD64		R3,R2			; R3 = Limit = Length + Base = Limit (exclusive)
		MOVSNW		R2,R2,36		; R7 = New base = Pointer to entries list
;--- Cycle for scan XSDT for required table ---
SXSD_0:		MOVQ		R7,@R2			; R7 = Pointer to current checked table
		CMP32EQ		R4,@R7			; R4 = Required signature, @R7 = Scanned table
		JMP8CS		SRSD_1			; Go if signature match, required table found
		MOVSNW		R2,R2,8			; R2 = Pointer to next 64-bit entry
		CMP64UGTE	R2,R3
		JMP8CC		SXSD_0			; Cycle for XSDT entries scan
		JMP8		GACR_7			; Go if not found, make CF=CC, R7=0


;--- Detect ACPI support, return root tables pointers ----------------------;
; First try find ACPI 2.0 , if not found try ACPI 1.0.                      ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address, not used                ;
;                                                                           ;
; OUTPUT:  CF = Status flag: 0(CC)=No errors, 1(CS)=Error                   ;
;          R6 = Pointer to RSDT (Root System Description Table (ACPI 1)     ;
;          R7 = Pointer to XSDT (Extended System Desc. Table (ACPI 2+)      ;
;          Pointers at R6, R7 can be analused only if CF=0=CC (No errors),  ;
;          plus additional validation used: if Pointer=0 means not valid    ;
;          Required 2 checks: 1) Check CF , 2) Check R6/R7                  ;
;---------------------------------------------------------------------------;
Get_ACPI_Root:
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
;--- Load start parameters ---
		XOR64		R4,R4			; R4=0, pass 1/2 count
		MOVSNW		R3,R1,_GuidAcpi20	; First try ACPI 2.0
;--- Start cycle for try ACPI 1.0 if ACPI 2.0 not found ---
GACR_0:		MOVNW		R2,@R1,0,_EFI_Table
		MOVNW		R5,@R2,10,24		; R5=Count
		MOVNW		R2,@R2,11,24		; R2=Base
;--- Start cycle for scan EFI Configuration Table ---
GACR_1:		MOVQ		R6,@R2			; Get Low 64 bits of GUID
		CMP64EQ		R6,@R3			; Low 64 bits of 128 comparision
		JMP8CC		GACR_2			; Go if mismatch
		MOVQQ		R6,@R2,0,8		; Get High 64 bits of GUID
		CMP64EQ		R6,@R3,0,8		; High 64 bits of 128 comparision
		JMP8CS		GACR_3			; Go if match
;--- Make cycle for scan EFI Configuration Table ---
GACR_2:		MOVINW		R6,1,16			; Natural=1, Const=16
		ADD64		R2,R6			; To next GUID
		MOVSNW		R5,R5,-1		; Decrement Count
		CMPI32WEQ	R5,0
		JMP8CC		GACR_1			; Cycle for all GUIDs
;--- Make cycle for try ACPI 1.0 if ACPI 2.0 not found ---
		MOVSNW		R3,R1,_GuidAcpi10	; Next try ACPI 1.0
		MOVSNW		R4,R4,1	
		CMPI32WEQ	R4,2
		JMP8CC		GACR_0

;--- Entry point if not found, here CC condition cleared (no errors) ---
; ACPI absense not interpreted as error
; This 3 entry points (Set CF=CC, Transit CF , Set CF=CS) also used by other subroutines
;---
GACR_4:		XOR64		R6,R6			; RSDT pointer set not valid
GACR_7:		XOR64		R7,R7			; XSDT pointer set not valid
		CMPI32WUGTE	R7,1			; This for clear CF=0,make CC = no errors
;--- Common entry point with registers restore, CF unchanged ---
GACR_5:		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET
;--- Entry point with set error condition ---
GACR_6:		XOR64		R2,R2			; R2=0 for condition set
		CMPI64WEQ	R2,0			; This for set CF=1,make CS = error
		JMP8		GACR_5

;--- Handling if ACPI GUID match found ---

GACR_3:		MOVNW		R2,@R2,0,16		; R2 = Pointer to RSDP
		MOVIQQ		R3,'RSD PTR '		; R3 = RSDP signature for comparision
		CMP64EQ		R3,@R2			; Compare RSDP signature
		JMP8CC		GACR_6			; Go error return if RSDP signature mismatch
		MOVBW		R4,@R2,0,15		; R4 = RSDP structure revision byte
		CMPI32WUGTE	R4,2
		JMP8CS		GACR_20			; Go if revision 2 or higher

;--- Support ACPI 1.0 mode ---
GACR_10:	MOVIQW		R3,20			; R3 = ACPI 1.0 RSDP fixed length=20 bytes , R2 = Base
		CALL32		Checksum_ACPI		; Result = condition flag
		JMP8CS		GACR_5			; Return with error if RSDP checksum is wrong
		MOVDW		R6,@R2,0,16		; R6 = RSDT pointer
		XOR64		R7,R7			; R7 = 0, mark XSDT not valid for ACPI 1
		JMP8		GACR_5			; Return with RSDT available, XSDT not available, no errors

;--- Support ACPI 2.0+ mode ---
GACR_20:	MOVDW		R3,@R2,0,20		; R3 = ACPI 2.0 RSDP variable length , R2 = Base
		CALL32		Checksum_ACPI		; Result = condition flag
		JMP8CS		GACR_5			; Return with error if RSDP checksum is wrong
		MOVDW		R6,@R2,0,16		; R6 = RSDT pointer, 32-bit variant
		MOVQW		R7,@R2,0,24		; R7 = XSDT pointer, 64-bit variant
		JMP8		GACR_5			; Return with RSDT and XSDT available, no errors

;--- Calculate memory region checksum mod 256 for ACPI structures verify ---;
;                                                                           ;
; INPUT:   R2 = Base address of checksummed memory region                   ;
;          R3 = Length of checksummed memory region, bytes                  ;
;                                                                           ;
; OUTPUT:  CF = Status flag:                                                ;
;               0(CC) = No errors, checksum mod 256 is zero                 ;
;               1(CS) = Error , checksum mod 256 is non-zero                ;
;---------------------------------------------------------------------------;
Checksum_ACPI:
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
;--- Calculate sum ---
		XOR64		R4,R4			; Clear sum
CSACP_0:	MOVB		R5,@R2			; Get current byte
		MOVSNW		R2,R2,1			; Advance pointer to next byte
		ADD64		R4,R5			; Add byte to sum
		MOVSNW		R3,R3,-1		; Length counter -1
		CMPI32WEQ	R3,0			; Check length counter
		JMP8CC		CSACP_0			; Go cycle if length not done
;--- Verify sum ---
		MOVIQW		R2,0FFh			; R2 = Mask for low byte
		AND32		R2,R4			; R2 = Result of masking
		CMPI32WUGTE	R2,1			; Compare sum, modify condition flag
;--- Registers restore and return ---
		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		RET

