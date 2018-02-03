;===========================================================================;
;=                                                                         =;
;=                       CONSTANTS AND VARIABLES.                          =;
;=                                                                         =;
;===========================================================================;

;---------- Equations for data access --------------------------------------;
; For compact coding, use 16-bit offsets relative Global_Variables_Pool
; instead full 64-bit flat addresses. This method also eliminate relocation
; elements with some bugs in the old IA32 EFI implementations.

_EFI_Handle		= EFI_Handle - Global_Variables_Pool
_EFI_Table		= EFI_Table - Global_Variables_Pool

_Both_Bitmaps		= Both_Bitmaps - Global_Variables_Pool
_Sys_Bitmap		= Sys_Bitmap - Global_Variables_Pool
_Info_Bitmap		= Info_Bitmap - Global_Variables_Pool

_Shell_Protocol		= Shell_Protocol - Global_Variables_Pool
_GOP_Protocol		= GOP_Protocol - Global_Variables_Pool
_Legacy_BIOS_Protocol	= Legacy_BIOS_Protocol - Global_Variables_Pool
_CAP_Protocol		= CAP_Protocol - Global_Variables_Pool
_PCI_RB_IO_Protocol	= PCI_RB_IO_Protocol - Global_Variables_Pool
_File_System_Protocol	= File_System_Protocol - Global_Variables_Pool
_File_Volume_Protocol	= File_Volume_Protocol - Global_Variables_Pool

_EFI_Memory_Total	= EFI_Memory_Total - Global_Variables_Pool
_EFI_Memory_ACPI	= EFI_Memory_ACPI - Global_Variables_Pool
_Primary_Memory_Base	= Primary_Memory_Base - Global_Variables_Pool
_Primary_Memory_Size	= Primary_Memory_Size - Global_Variables_Pool
_Secondary_Memory_Base	= Secondary_Memory_Base - Global_Variables_Pool
_Secondary_Memory_Size	= Secondary_Memory_Size - Global_Variables_Pool

_EFI_Revision		= EFI_Revision - Global_Variables_Pool
_Firmware_Revision	= Firmware_Revision - Global_Variables_Pool
_Firmware_Vendor	= Firmware_Vendor - Global_Variables_Pool
_EBC_VM_Version		= EBC_VM_Version - Global_Variables_Pool
_CPU_Native_Width	= CPU_Native_Width - Global_Variables_Pool

_Firmware_Video_Mode	= Firmware_Video_Mode - Global_Variables_Pool
_Program_Video_Mode	= Program_Video_Mode - Global_Variables_Pool
_Video_X_Y_sizes	= Video_Xsize - Global_Variables_Pool	; alias for dword
_Video_Xsize		= Video_Xsize - Global_Variables_Pool
_Video_Ysize		= Video_Ysize - Global_Variables_Pool
_Video_Xdelta		= Video_Xdelta - Global_Variables_Pool
_Video_BPP		= Video_BPP - Global_Variables_Pool
_Video_Memory_Base_GOP	= Video_Memory_Base_GOP - Global_Variables_Pool
_Video_Memory_Size_GOP	= Video_Memory_Size_GOP - Global_Variables_Pool
_Video_Memory_Base_HW	= Video_Memory_Base_HW - Global_Variables_Pool
_Video_Memory_Size_HW	= Video_Memory_Size_HW - Global_Variables_Pool

_TSC_Period		= TSC_Period - Global_Variables_Pool
_CAP_Period		= CAP_Period - Global_Variables_Pool
_CPU_Phys_Addr_Width	= CPU_Phys_Addr_Width - Global_Variables_Pool

_Last_PCI_Bus		= Last_PCI_Bus - Global_Variables_Pool
_PBAR_Reg		= PBAR_Reg - Global_Variables_Pool
_Act_Video_Cfg		= Act_Video_Cfg - Global_Variables_Pool
_Host_Cfg		= Host_Cfg - Global_Variables_Pool
_Adapter_Cfg		= Adapter_Cfg - Global_Variables_Pool
_PBAR_Base		= PBAR_Base - Global_Variables_Pool
_PBAR_Size		= PBAR_Size - Global_Variables_Pool

_AGP_H_Reg		= AGP_H_Reg - Global_Variables_Pool
_AGP_H_Stat		= AGP_H_Stat - Global_Variables_Pool
_AGP_H_Cmd		= AGP_H_Cmd - Global_Variables_Pool
_AGP_A_Reg		= AGP_A_Reg - Global_Variables_Pool
_AGP_A_Stat		= AGP_A_Stat - Global_Variables_Pool
_AGP_A_Cmd		= AGP_A_Cmd - Global_Variables_Pool
_AGP_Rsvd_1		= AGP_Rsvd_1 - Global_Variables_Pool

_PCIe_H_Reg		= PCIe_H_Reg - Global_Variables_Pool
_PCIe_H_Caps		= PCIe_H_Caps - Global_Variables_Pool
_PCIe_H_Ctrl		= PCIe_H_Ctrl - Global_Variables_Pool
_PCIe_A_Reg		= PCIe_A_Reg - Global_Variables_Pool
_PCIe_A_Caps		= PCIe_A_Caps - Global_Variables_Pool
_PCIe_A_Ctrl		= PCIe_A_Ctrl - Global_Variables_Pool
_PCIe_Rsvd_1		= PCIe_Rsvd_1 - Global_Variables_Pool

_MTRR_Area		= MTRR_Used - Global_Variables_Pool	; alias for 24 bytes
_MTRR_Used		= MTRR_Used - Global_Variables_Pool
_MTRR_Rsvd_1		= MTRR_Rsvd_1 - Global_Variables_Pool
_MTRR_S_Base		= MTRR_S_Base - Global_Variables_Pool
_MTRR_S_Mask		= MTRR_S_Mask - Global_Variables_Pool

_SSE_Area		= SSE_S_CR4 - Global_Variables_Pool	; alias for 16 bytes
_SSE_S_CR4		= SSE_S_CR4 - Global_Variables_Pool
_SSE_S_MXCSR		= SSE_S_MXCSR - Global_Variables_Pool
_SSE_Rsvd_1		= SSE_Rsvd_1 - Global_Variables_Pool

_EFI_Act_Op		= EFI_Action - Global_Variables_Pool	; alias for word
_EFI_Action		= EFI_Action - Global_Variables_Pool
_EFI_Operation		= EFI_Operation - Global_Variables_Pool
_EFI_Status		= EFI_Status - Global_Variables_Pool

_Start_Pause		= Start_Pause - Global_Variables_Pool

;---------- Data -----------------------------------------------------------;

;---------- Multifunctional buffer -----------------------------------------;
; Use fact: Scratch_Pad = Global_Variables_Pool (pointers equal),
; this address loaded into R1 by first EBC instruction MOVRELW.
; Additional restructions for 3072 bytes, 0000h-0BFFh
; 1) 0000h-0A7Fh (2688 bytes) = Miscellaneous area
; 2) 0A80h-0AFFh (128 bytes)  = Benchmarks results
; 3) 0B00h-0B7Fh (128 bytes)  = CPUID results storage
; 4) 0B80h-0BFFh (128 bytes)  = Built ASCII numbers and other sys. info strings

Scratch_Pad_Size	=   3072
Scratch_Pad		DB  Scratch_Pad_Size DUP (?)

;---------- Save parameters, received from UEFI firmware or UEFI shell -----;

EFI_Handle		DQ  ?		; This application handle
EFI_Table		DQ  ?		; System table address

;---------- System control bitmap ------------------------------------------;
; Described operations enabled if associated bit=1

Both_Bitmaps:		; Alias for QWORD access Sys_Bitmap and Info_Bitmap

; D0  = Enable send checkpoints
; D1  = Enable native CPU identification
; D2  = Enable native CPU clock measurement
; D3  = Enable memory release, because requested OK
; D4  = Enable scan for active legacy VGA adaptor
; D5  = Enable scan for video memory BAR
; D6  = Enable scan for AGP video capabilities
; D7  = Enable scan for PCI Express video capabilities
; D8  = Enable scan for video memory range with attributes capability
; D9  = Enable AGP video capabilities reprogramming and restore
; D10 = Enable PCI Express video capabilities reprogramming and restore
; D11 = Enable video memory range attributes reprogramming and restore
; D12 = Enable mouse
; D13 = Enable load configuration file
; D14 = Enable and required save configuration file
; D15 = Enable video mode restore, because switched OK
; D16 = 
; D17 = 
; D18 = 
; D19 = 
; D20 = 
; D21 = 
; D22 = 
; D23 = 
; D24 = Flag means Adapter_Cfg is address of 3D Controller, not VGA Controller, this disable extreme mode
; D25 = 
; D26 = 
; D27 = 
; D28 = 
; D29 = 
; D30 = 
; D31 = Enable wait key after start messages before set video mode

Sys_Bitmap		DD  ?		; System control bitmap

;---------- System info bitmap ---------------------------------------------;
; Object present if associated bit=1

; D0  = UEFI Shell present
; D1  = CSM present
; D2  = Native CPU is x86, firmware mode is IA32 EFI
; D3  = Native CPU is x86, firmware mode is x64 UEFI
; D4  = Native CPU is x86, CPU vendor string supported by CPUID function #1
; D5  = Native CPU is x86, CPU name string supported by CPUID functions #80000002h-80000004h
; D6  = Native CPU is x86, CPU features string supported by CPUID functions #00000001h and/or 80000001h
; D7  = Native CPU is x86, TSC (Time Stamp Counter) clock measurement results valid
; D8  = CAP (CPU Architectural Protocol) clock data valid
; D9  = CPU physical address width valid
; D10 = Video memory base and size, read from GOP table, valid (it also additional enable for HW scan) 
; D11 = Video memory base and size, detected by hardware scan, valid 
; D12 = 
; D13 = 
; D14 = 
; D15 = 
; D16 = 
; D17 = 
; D18 = 
; D19 = 
; D20 = 
; D21 = 
; D22 = 
; D23 = 
; D24 = Benchmark 1 result valid
; D25 = Benchmark 2 result valid
; D26 = Benchmark 3 result valid
; D27 = Benchmark 4 result valid
; D28 = Benchmark 5 result valid
; D29 = Benchmark 6 result valid
; D30 = Benchmark 7 result valid
; D31 = Benchmark 8 result valid

Info_Bitmap		DD  ?		; System information bitmap

;---------- Protocols interface blocks pointers ----------------------------;

Shell_Protocol		DQ  ?		; EFI Shell Protocol
GOP_Protocol		DQ  ?		; Graphics Output Protocol (GOP)
Legacy_BIOS_Protocol	DQ  ?		; Compatibility Support Module (CSM) interface
CAP_Protocol		DQ  ?		; CPU Architectural Protocol (CAP)
PCI_RB_IO_Protocol	DQ  ?		; PCI Root Bridge I/O Protocol (RBIO)
File_System_Protocol	DQ  ?		; Simple File System protocol
File_Volume_Protocol	DQ  ?		; File protocol for used volume

;---------- Memory allocation control --------------------------------------;

EFI_Memory_Total	DQ  ?		; Total memory reported by firmware, KB
EFI_Memory_ACPI		DQ  ?		; ACPI memory reported by firmware, KB
Primary_Memory_Base	DQ  ?		; Base address of this application pool
Primary_Memory_Size	DQ  ?		; Size of of this application pool
Secondary_Memory_Base	DQ  ?		; Base address of separate part of pool
Secondary_Memory_Size	DQ  ?		; Size of separate part of pool

;---------- UEFI firmware and shell parameters -----------------------------;

EFI_Revision		DD  ?		; UEFI specification revision, major16:minor16
Firmware_Revision	DD  ?		; Firmware revision, major16:minor16
Firmware_Vendor		DQ  ?		; Pointer to firmware vendor string
EBC_VM_Version		DD  ?		; EBC virtual machine version
CPU_Native_Width	DB  ?		; CPU width, units=bytes: 4 means 32, 8 means 64

;---------- Video subsystem control ----------------------------------------;

Firmware_Video_Mode	DD  ?		; GOP video mode number, for mode, used by firmware
Program_Video_Mode	DD  ?		; GOP video mode number, for mode, set by this program
Video_Xsize		DW  ?		; GOP.BLT X size, pixels
Video_Ysize		DW  ?		; GOP.BLT Y size, pixels
Video_Xdelta		DW  ?		; GOP.BLT Y line length, bytes (1 pixel = 4 bytes)
Video_BPP		DB  ?		; Bits per pixel, equal 32 for all GOP modes
Video_Memory_Base_GOP	DQ  ?		; Video memory base physical address, detected by GOP
Video_Memory_Size_GOP	DQ  ?		; Video memory size, detected by GOP, Bytes
Video_Memory_Base_HW	DQ  ?		; Video memory base physical address, by hardware
Video_Memory_Size_HW	DQ  ?		; Video memory size, by hardware, Bytes

;---------- CPU and CPU Architectural Protocol (CAP) parameters ------------;

TSC_Period		DQ  ?		; TSC clock period, femtoseconds (10^-15 sec)
CAP_Period		DQ  ?		; CAP timer clock period, femtoseconds
CPU_Phys_Addr_Width	DB  ?		; Physical address width by CPUID, bits

;---------- PCI PnP control ------------------------------------------------;

Last_PCI_Bus		DB  ?		; Number of PCI buses minus 1
PBAR_Reg		DB  ?		; Video RAM BAR register address, 0=None
Act_Video_Cfg		DQ  ?		; Active video adapter bus/device/function (RBIO64)
Host_Cfg		DQ  ?		; Host component bus/device/function (RBIO64)
Adapter_Cfg		DQ  ?		; PCIe video adapt./3D controller bus/dev/fnc (RBIO64)
PBAR_Base		DQ  ?		; Video RAM prefetchable BAR base address
PBAR_Size		DQ  ?		; Video RAM prefetchable BAR size

;---------- AGP control ----------------------------------------------------;

AGP_H_Reg		DB  ?		; AGP Host Cap. Reg. Address (at Host_Cfg)
AGP_H_Stat		DB  ?		; AGP H.Stat. D[7-0]=FW,AGP3,0,0,8x,4x,2x,1x
AGP_H_Cmd		DD  ?		; Old state of Host AGP Command Register
AGP_A_Reg   		DB  ?		; AGP Adapter Cap. Reg. Addr. (at Adapt._Cfg)
AGP_A_Stat  		DB  ?           ; AGP A.Stat. D[7-0]=FW,AGP3,0,0,8x,4x,2x,1x
AGP_A_Cmd  		DD  ?           ; Old state of Adapter AGP Command Register    
AGP_Rsvd_1		DD  ?		; Reserved for extensions and alignment 8

;---------- PCI Express control --------------------------------------------;

PCIe_H_Reg		DB  ?		; PCIe Host Cap. Reg. Address (at Host_Cfg)
PCIe_H_Caps		DB  ?		; PCIe H. Device Cap. Register, bits [7-0]
PCIe_H_Ctrl		DW  ?		; Old state of Host PCIe Device Control Reg.
PCIe_A_Reg		DB  ?		; PCIe Adapter Cap. Reg. Addr. (at Host_Cfg)
PCIe_A_Caps		DB  ?           ; PCIe A. Device Cap. Register, bits [7-0]     
PCIe_A_Ctrl		DW  ?           ; Old state of A. PCIe Device Control Reg.
PCIe_Rsvd_1		DQ  ?		; Reserved for extensions

;---------- MTRR variables -------------------------------------------------;
; Formatting of this 4 variables (24 bytes) block is used by native subroutine,
; access by _MTRR_Area pointer addend

MTRR_Used		DD  ?		; MSR address for used MTRR
					; Spec. values: 0=Not supported, 1=No vacant
MTRR_Rsvd_1		DD  ?		; Reserved for extensions and alignment 8
MTRR_S_Base		DQ  ?		; Save old MTRR state, Base(i) MSR
MTRR_S_Mask		DQ  ?		; Save old MTRR state, Mask(i) MSR

;---------- SSE context variables ------------------------------------------;
; Formatting of this 3 variables (16 bytes) block is used by native subroutine,
; access by _SSE_Area pointer addend

SSE_S_CR4		DQ  ?		; Save old CR4 state
SSE_S_MXCSR		DD  ?		; Save old MXCSR state
SSE_Rsvd_1		DD  ?		; Reserved for extensions and alignment 8

;---------- Errors reporting support ---------------------------------------;

EFI_Action		DB  ?		; 0=None, 1=Save TXT, 2=Save BMP
EFI_Operation		DB  ?		; Current operation
EFI_Status		DQ  ?		; UEFI Status, unified for 32 and 64

;---------- Control for pause after show start messages --------------------;

Start_Pause		DB  ?		; 0=Don't make pause, 1=Make pause


