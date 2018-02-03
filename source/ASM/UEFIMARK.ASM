;===========================================================================;
;=                                                                         =;
;=    UEFIMARK. MAIN MODULE. SYSTEM INFORMATION AND BENCHMARK UTILITY.     =;
;=                       EFI Byte Code Edition.                            =;
;=                     (C)2014-2017 IC Book Labs.                          =;
;=                                                                         =;
;===========================================================================;


;---------------------------------------------------------------------------;
;                              GLOBAL MACRO.                                ;
;---------------------------------------------------------------------------;

include 'EBCMACRO\ebcmacro.inc' 	; Macro for assembling EBC instructions
include 'EBCMACRO\dbgmacro.inc' 	; Macro for assembling debug checkpoints
include 'NATIVE\x86macro.inc'		; Macro for assembling EBC-Native x86 gates

;---------------------------------------------------------------------------;
;                       CODE SECTION DEFINITIONS.                           ;
;---------------------------------------------------------------------------;

format pe64 dll efi
entry main
section '.text' code executable readable
main:

;---------------------------------------------------------------------------;
;                  CODE SECTION \ MAIN EBC ROUTINE.                         ;
;---------------------------------------------------------------------------;

;---------- Determine R1 = Data pool address -------------------------------;
; Don't use direct assign "MOVIQQ R1,Global_Variables_Pool"
; because some IA32 EFI implementations has bug with 
; relocation elements for PE64 format.
; Additionally, MOVRELW is compact:
; MOVRELW use 16-bit operand = IP-relative offset
; MOVIQQ  use 64-bit operand = Absolute address.
;---
		MOVRELW 	R1,Global_Variables_Pool - Anchor_IP
Anchor_IP:
;---------- Get and save parent parameters, initializing variables ---------;

		MOVNW		R2,@R0,0,16				; R2=Handle
		MOVNW		R3,@R0,1,16				; R3=Table
		MOVQW		@R1,0,_EFI_Handle,R2			; Save Handle
		MOVQW		@R1,0,_EFI_Table,R3			; Save Table
		MOVIQQ		@R1,0,_Both_Bitmaps,0000000080002000h	; Default bitmaps
		MOVIBW		@R1,0,_EFI_Action,0			; No actions yet

;---------- First message strings ------------------------------------------;

		MOVIQW		R2,0Fh				; Color=Bold White
		CALL32		Output_Attribute		; Set color
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write			; Skip 1 string
		MOVIQW		R2,_String_Version
		CALL32		String_Write			; Write Program Version
		MOVIQW		R2,0Ah				; Color=Bold Green
		CALL32		Output_Attribute		; Set color
		MOVIQW		R2,_Program_Vendor_Data_1
		CALL32		String_Write			; Write Vendor Name
		MOVIQW		R2,07h				; Color=White
		CALL32		Output_Attribute		; Set color
		MOVIQW		R2,_Msg_Starting
		CALL32		String_Write			; Write "Starting..."

;---------- Program Initialization Sequence execution ----------------------;
; Checkpoints 001h-01Fh (in the subroutine)
;---
		MOVIQW		R2,_Start_Program_Sequence
		CALL32		Dispatcher_Start_Stop
		JMP8CS		Bad_Config

;---------- System Info and Benchmarks with visualize parameters -----------;
; Checkpoints 020h-04Fh (in the subroutine)
;---
		MOVIQW		R2,_Info_Sequence
		CALL32		Dispatcher_SysInfo

;---------- Wait key press or mouse click ----------------------------------;
; Checkpoint 050h
;---
		CHK		050h

	;*      CALL32          Input_Wait_Key          ; Wait for key
		CALL32		Wait_SysInfo		; Wait for key + revisual date and time

;---------- Conditionally save sysinfo-benchmarks results to TXT file ------;
; Checkpoints 058h-05Bh (in the subroutine, if F9 pressed)
;---
		CMPI32WEQ	R2,0013h		; F9 key = 13h
		JMP8CC		No_Save_TXT		; Go skip if not F9 key
		CALL32		Save_Report_to_TXT
No_Save_TXT:

;---------- Conditionally save sysinfo-benchmarks screen to BMP file -------;
; Checkpoints 05Ch-05Fh (in the subroutine, if F10 pressed)
;---
		CMPI32WEQ	R2,0014h		; F10 key = 14h
		JMP8CC		No_Save_BMP		; Go skip if not F10 key
		CALL32		Save_Screen_to_BMP
No_Save_BMP:

;---------- Exit Program Sequence execution --------------------------------;
; Checkpoints 060h-07Fh (in the subroutine)
;---
		MOVIQW		R2,_Stop_Program_Sequence
ESeq_Cycle:	CALL32		Dispatcher_Start_Stop
		JMP8CC		ESeq_Stop		; Go if no errors
		PUSH64		R2
		PUSH64		R2			; Twice for alignment
		MOVIQW		R2,_Msg_Press
		CALL32		String_Write
		CALL32		Input_Wait_Key		; Wait for key press
		MOVIQW		R2,_Msg_CRLF		; Next string
		CALL32		String_Write		
		POP64		R2
		POP64		R2			; Twice for alignment
		JMP8		ESeq_Cycle
ESeq_Stop:

;---------- Send last message to screen, before program vendor data --------;

		CALL32		Status_Msg_Write	; Results of F9=TXT, F10=BMP
		MOVIQW		R2,_Msg_Exit
		CALL32		String_Write

;---------- Message with program vendor data and/or requirements -----------;

		MOVIQW		R2,0Ah			; Color=Bold Green
		CALL32		Output_Attribute	; Set color
		MOVIQW		R2,_Program_Vendor_Data
		CALL32		String_Write
		MOVIQW		R2,07h			; Color=White
		CALL32		Output_Attribute	; Set color
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write		; Carriage Return, Line Feed

;---------- Exit to parent task (Shell or Firmware) if no errors -----------;
; Checkpoint 0F0h

		CHK		0F0h
Exit_Status_0:	XOR64		R7,R7			; UEFI Status = 0
		RET					; Return to parent task, IP<-[R0]


;---------- Exit if errors detected during check configuration -------------;
; Checkpoint 0F1h
;---
Bad_Config:	CHK		0F1h
		MOVIQW		R2,_Msg_Press		; "Press any key"
		CALL32		String_Write
		CALL32		Input_Wait_Key		; Wait for key
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write		; Carriage Return, Line Feed

;---------- Restore state for error branch ---------------------------------;
; Checkpoints 060h-07Fh (in the subroutine)
;---
		MOVIQW		R2,_Stop_Program_Sequence
ESeq_Cycle_1:	CALL32		Dispatcher_Start_Stop
		JMP8CC		ESeq_Stop_1		; Go if done without errors
		PUSH64		R2
		PUSH64		R2			; Twice for alignment
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write		; Carriage Return, Line Feed
		POP64		R2
		POP64		R2			; Twice for alignment
		JMP8		ESeq_Cycle_1		; Cycle after error, for all sequence
ESeq_Stop_1:

;---------- Message with program vendor data and/or requirements -----------;

		MOVIQW		R2,_Msg_CRLF		; Next string
		CALL32		String_Write		
		MOVIQW		R2,0Ah			; Color=Bold Green
		CALL32		Output_Attribute	; Set color
		MOVIQW		R2,_Program_Vendor_Data
		CALL32		String_Write
		MOVIQW		R2,07h			; Color=White
		CALL32		Output_Attribute	; Set color
		MOVIQW		R2,_Msg_CRLF
		CALL32		String_Write		; Carriage Return, Line Feed

;---------- Exit to parent task (Shell or Firmware) if starting error ------;
; Checkpoint 0F2h
;---
		CHK		0F2h
		JMP8		Exit_Status_0		; Return to parent task, IP<-[R0]

;---------------------------------------------------------------------------;
;                CODE SECTION \ EBC-EXECUTABLE SUBROUTINES.                 ;
;---------------------------------------------------------------------------;

include 'SYS\chkarch.inc'		; Check instruction set architecture
include 'SYS\chkuefi.inc'		; Check UEFI config
include 'SYS\chkshell.inc'		; Check UEFI Shell presence and config
;- relocated to prevent 8-bit JMP8 offsets overflows -
;- include 'SYS\chkacpi.inc'		; Check ACPI config
include 'SYS\allocmem.inc'		; Allocate system memory, analyse map
include 'SYS\chkgop.inc'		; Check support Graphics Output Protocol
include 'SYS\chkvmode.inc'		; Check video configuration, select video mode
include 'SYS\chkvrang.inc'		; Check video memory range attributes
include 'SYS\chkpci.inc'		; Check PCI bus support
include 'SYS\chkavga.inc'		; Check legacy VGA and detect active adaptor
include 'SYS\chkvbar.inc'		; Check PCI base address register for video memory
include 'SYS\chkagp.inc'		; Check AGP bus support for video subsystem
include 'SYS\chkpcie.inc'		; Check PCI Express bus support for video subsystem
include 'SYS\chkcap.inc'		; Check CPU Architectural Protocol, get timer period
include 'SYS\chkncpu.inc'		; Identify native CPU
include 'SYS\chkmouse.inc'		; Check mouse present
include 'SYS\chkclk.inc'		; Check and measure native CPU clock
include 'SYS\loadcfg.inc'		; Load and interpreting configuration file
include 'SYS\savecfg.inc'		; Save updated configuration file
include 'SYS\setvrang.inc'		; Set optimal attribute for video memory range
include 'SYS\setagp.inc'		; Set AGP options: Fast Write at optimal 2x/4x/8x
include 'SYS\setpcie.inc'		; Set PCIe options: TLP size
include 'SYS\setvmode.inc'		; Set target video mode, use GOP
include 'SYS\setwait.inc'		; Wait key from user in the setup phase
include 'SYS\resvrang.inc'		; Restore attribute for video memory range
include 'SYS\resagp.inc'		; Restore AGP options
include 'SYS\relsmem.inc'		; Restore system memory context: release memory
include 'SYS\respcie.inc'		; Restore PCIe options
include 'SYS\resvmode.inc'		; Restore firmware video mode
;- relocated to prevent 8-bit JMP8 offsets overflows -
include 'SYS\chkacpi.inc'		; Check ACPI config

include 'SYS\libapi.inc'		; Library for access UEFI API
include 'SYS\libpci.inc'		; Library for access PCI Configuration Space
include 'SYS\libpcie.inc'		; Library for support PCI Express interface
include 'SYS\libagp.inc'		; Library for support AGP interface
include 'SYS\libacpi.inc'		; Library for support ACPI interface
include 'SYS\libstsp.inc'		; Library for execute program start and stop sequences
include 'SYS\libtext.inc'		; Library for chars and text i/o: screen and keyboard
include 'SYS\libmouse.inc'		; Library for mouse support
include 'SYS\libfile.inc'		; Library for file operations
include 'SYS\librtc.inc'		; Library for Real Time Clock API support
include 'SYS\libstxt.inc'		; Library for save text report (F9 TXT)
include 'SYS\libsbmp.inc'		; Library for save graphics screen image (F10 BMP)
include 'SYS\libfwb.inc'		; Library for detect firmware vendors and revisions
include 'SYS\libchkp.inc'		; Library for send checkpoints to debug ports

include 'GRAPHICS\libmcurs.inc' 	; Library for graphics mouse cursor support
include 'GRAPHICS\libblt.inc'		; Library for GOP.BLT.Fill and GOP.BLT.Copy
include 'GRAPHICS\libmfmc.inc'		; Library for Memory Fill and Memory Copy by UEFI API
include 'GRAPHICS\libpgen.inc'		; Library for Pictures gen. by EBC instructions
include 'GRAPHICS\libnmfmc.inc' 	; Library (gate) for Mem. Fill, Mem. Copy by Native CPU
include 'GRAPHICS\libnpgen.inc' 	; Library (gate) for Pictures gen. by Native CPU

include 'INFO\infabout.inc'		; Constant info about this application
include 'INFO\infspec.inc'		; Info about UEFI specification, include ACPI version
include 'INFO\inffrev.inc'		; Info about UEFI firmware revision
include 'INFO\inffven.inc'		; Info about UEFI firmware vendor string
include 'INFO\infsrev.inc'		; Info about UEFI shell revision
include 'INFO\infsven.inc'		; Info about UEFI shell vendor string
include 'INFO\infebc.inc'		; Info about UEFI EBC interpreter revision
include 'INFO\infcsm.inc'		; Info about UEFI CSM presence
include 'INFO\infnat.inc'		; Info about Processor native width
include 'INFO\infcpuv.inc'		; Info about Processor vendor string
include 'INFO\infcpun.inc'		; Info about Processor name string
include 'INFO\infcpuf.inc'		; Info about Processor supported features list
include 'INFO\infcpuf1.inc'		; Info about Processor supported features list, add AVX512 at v0.25
include 'INFO\infcpuf2.inc'		; Info about Processor supported features list, add AVX512 at v0.25
include 'INFO\infclk.inc'		; Info about Processor clock, TSC and CAP
include 'INFO\infvmod.inc'		; Info about Video mode parameters
include 'INFO\infahost.inc'		; Info about AGP video host parameters
include 'INFO\infadevs.inc'		; Info about AGP video card parameters
include 'INFO\infphost.inc'		; Info about PCIe video host parameters
include 'INFO\infpdevs.inc'		; Info about PCIe video card parameters
include 'INFO\infsmem.inc'		; Info about System memory, UEFI and ACPI
include 'INFO\infvmem.inc'		; Info about Video memory, GOP and Hardware
include 'INFO\infrang.inc'		; Info about Video memory range attributes

include 'INFO\infbenc1.inc'		; Info about Benchmarks GOP.BLT.Fill, GOP.BLT.Copy (FPS)
include 'INFO\infbenc2.inc'		; Info about Benchmarks UEFIAPI.MemFill, UEFIAPI.MemCopy (FPS)
include 'INFO\infbenc3.inc'		; Info about Benchmarks Native.MemFill, Native.MemCopy (FPS)
include 'INFO\infbenc4.inc'		; Info about Benchmarks Native.MemFill, Native.MemCopy (MBPS)

include 'INFO\exebenc1.inc'		; Execution Benchmarks GOP.BLT.Fill
include 'INFO\exebenc2.inc'		; Execution Benchmarks GOP.BLT.Copy
include 'INFO\exebenc3.inc'		; Execution Benchmarks UEFIAPI.MemFill
include 'INFO\exebenc4.inc'		; Execution Benchmarks UEFIAPI.MemCopy
include 'INFO\exebenc5.inc'		; Execution Benchmarks Native.MemFill, 386 instructions
include 'INFO\exebenc6.inc'		; Execution Benchmarks Native.MemFill, SSE instructions
include 'INFO\exebenc7.inc'		; Execution Benchmarks Native.MemCopy, 386 instructions
include 'INFO\exebenc8.inc'		; Execution Benchmarks Native.MemCopy, SSE instructions

include 'INFO\infpress.inc'		; Constant info about keys press for continue
include 'INFO\showrslt.inc'		; Show system info and benchmarks results
include 'INFO\libinfo.inc'		; Library for system info handlers

include 'EXTREME\remap.inc'		; Library for remap video RAM above 4GB

;---------------------------------------------------------------------------;
;                CODE SECTION \ X86-EXECUTABLE SUBROUTINES.                 ;
;---------------------------------------------------------------------------;

include 'NATIVE\x86id.inc'		; x86 routines: CPUID and 
include 'NATIVE\x86mtrr.inc'		; x86 routines: Direct programming CPU MTRR
include 'NATIVE\x86vram.inc'		; x86 routines: VRAM blocks fill/copy benchmarks, sizing
include 'NATIVE\x86dtask.inc'		; Daughter task for detect instruction set

;---------------------------------------------------------------------------;
;          DATA IN THE CODE SECTION (Note EBC code is native data).         ;
;---------------------------------------------------------------------------;

include 'NAMES\dataname.inc'		; Program and vendor strings
include 'SYS\dataseq.inc'		; Data for program start and stop sequences control
include 'SYS\datatext.inc'		; Data for text strings and text control info
include 'SYS\datacfgf.inc'		; Data for interpreting configuration file
include 'SYS\datartc.inc'		; Data for built time and date strings
include 'SYS\datastxt.inc'		; Data for save text report (F9 TXT)
include 'SYS\datasbmp.inc'		; Data for save graphics screen image (F10 BMP)
include 'SYS\dataerrs.inc'		; Data for UEFI errors type ordering and visual
include 'SYS\dataguid.inc'		; Data for used GUID list
include 'SYS\datafwr.inc'		; Data for firmware vendors and revisions
include 'GRAPHICS\font8x16.inc' 	; Data for graphics visual text font 8x16 
include 'INFO\datainfo.inc'		; Data for system info handlers

;---------- Base point for fields addressing -------------------------------;
Global_Variables_Pool:

; This file must be last to eliminate reserve space for non-predefined fields
include 'SYS\data.inc'			; Scratch pad and variables

;---------------------------------------------------------------------------;
;                            DATA SECTION.                                  ;
;---------------------------------------------------------------------------;

; Not used, data located in the code section, note EBC code is native data.
; This design for old IA32 EFI compatibility and use compact 16-bit offsets.
section '.data' data readable writeable
; This for prevent zero number of relocatable elements
Dummy_Label	DQ  Dummy_Label

;---------------------------------------------------------------------------;
;                     RELOCATION ELEMENTS SECTION.                          ;
;---------------------------------------------------------------------------;

; Not used, data located in the code section, note EBC code is native data.
; This design for old IA32 EFI compatibility and use compact 16-bit offsets.
section '.reloc' fixups data discardable

