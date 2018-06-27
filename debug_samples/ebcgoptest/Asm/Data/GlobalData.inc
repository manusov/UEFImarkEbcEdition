;===========================================================================;
;=                                                                         =;
;=                       CONSTANTS AND VARIABLES.                          =;
;=  WARNING: SOME OF FIELDS RESERVED AND NOT USED IN THE CURRENT VERSION.  =;
;=                                                                         =;
;===========================================================================;

;---------- Equations for access constants and variables -------------------;

;--- UEFI context analysing results ---

_EFI_Handle		= EFI_Handle - Global_Variables_Pool
_EFI_Table		= EFI_Table - Global_Variables_Pool
_Quad_Bitmaps		= Quad_Bitmaps - Global_Variables_Pool
_EFI_Revision		= EFI_Revision - Global_Variables_Pool
_Firmware_Revision	= Firmware_Revision - Global_Variables_Pool
_EBC_VM_Version		= EBC_VM_Version - Global_Variables_Pool
_Firmware_Vendor	= Firmware_Vendor - Global_Variables_Pool
_Legacy_BIOS_Protocol	= Legacy_BIOS_Protocol - Global_Variables_Pool
_GOP_Protocol		= GOP_Protocol - Global_Variables_Pool
_UGADRAW_Protocol	= UGADRAW_Protocol - Global_Variables_Pool
_ACPI_Pointer		= ACPI_Pointer - Global_Variables_Pool

;--- Memory allocation control ---

_EFI_Memory_Total	= EFI_Memory_Total - Global_Variables_Pool
_EFI_Memory_ACPI	= EFI_Memory_ACPI - Global_Variables_Pool
_Primary_Memory_Base	= Primary_Memory_Base - Global_Variables_Pool
_Primary_Memory_Size	= Primary_Memory_Size - Global_Variables_Pool
_Secondary_Memory_Base	= Secondary_Memory_Base - Global_Variables_Pool
_Secondary_Memory_Size	= Secondary_Memory_Size - Global_Variables_Pool


;--- Processor detection results ---

_CPUID_0_Data		= CPUID_0_Data - Global_Variables_Pool
_CPU_Native_Width	= CPU_Native_Width - Global_Variables_Pool
_CPU_Address_Width	= CPU_Address_Width - Global_Variables_Pool
_CPU_Name_String	= CPU_Name_String - Global_Variables_Pool
_CPU_Standard_Features	= CPU_Standard_Features - Global_Variables_Pool
_CPU_Extended_Features	= CPU_Extended_Features - Global_Variables_Pool
_TSC_Period		= TSC_Period - Global_Variables_Pool

;--- Video subsystem control ---

_Firmware_Video_Mode	= Firmware_Video_Mode - Global_Variables_Pool
_Program_Video_Mode	= Program_Video_Mode - Global_Variables_Pool
_Video_Xsize		= Video_Xsize - Global_Variables_Pool
_Video_Ysize		= Video_Ysize - Global_Variables_Pool
_Video_Xdelta		= Video_Xdelta - Global_Variables_Pool


;---------- Base point for addressing, offsets possible 0-7FFFh ------------;

Global_Variables_Pool:

;---------- Multifunctional buffer -----------------------------------------;
; Use fact: Multi_Buffer = Global_Variables_Pool

Multi_Buffer	DB  256 DUP (?)

;---------- Save parameters, received from UEFI firmware or UEFI shell -----;

EFI_Handle	DQ  ?		; This application handle
EFI_Table	DQ  ?		; System table address

;---------- Label for access 4 bitmaps as quad word ------------------------;

Quad_Bitmaps:

;---------- Supported features detection bitmap ----------------------------;
; D0  = Native CPU recognized, CPU specific support available if "1"
; D1  = Native CPU MTRR support available
; D2  = Low priveleged or emulated environment detected if "1"
; D3  = ACPI support available
; D4  = PCI bus support available
; D5  = AGP bus support available
; D6  = PCI Express bus-specific support available
; D7  = 0, reserved
; D8  = UEFI memory allocation service available if "1"
; D9  = UEFI Graphics Output Protocol (GOP) available if "1"
; D10 = UEFI UGADRAW Protocol available if "1"
; D11 = CSM (Legacy BIOS Protocol) available if "1"
; D12 = 0, reserved
; D13 = 0, reserved
; D14 = 0, reserved
; D15 = 0, reserved
;-------

Support_Bitmap	DW  ?

;---------- System context changes and restore requirements bitmap ---------;
; D0  = 0, reserved
; D1  = Native CPU MTRR context reprogrammed, restore required if "1"
; D2  = 0, reserved
; D3  = 0, reserved
; D4  = PCI bus context reprogrammed, restore required if "1"
; D5  = AGP bus context reprogrammed, restore required if "1"
; D6  = PCI Express bus context reprogrammed, restore required if "1"
; D7  = 0, reserved
; D8  = Memory requested by UEFI memory allocation service if "1"
; D9  = Video mode initialized by UEFI GOP if "1"
; D10 = Video mode initialized by UEFI UGADRAW if "1"
; D11 = 0, reserved
; D12 = 0, reserved
; D13 = 0, reserved
; D14 = 0, reserved
; D15 = 0, reserved
;-------

Changes_Bitmap	DW  ?

;---------- Program context control bitmap ---------------------------------;
; D0  = Enable pause for messages screen view during program start if "1"
; D1  = Enable save configuration file, changes detected if "1"
; D2  = Extreme edition (see also bit D3) if "1"
; D3  = Extreme edition and video memory remapped if "1"
; D4  = 0, reserved
; D5  = 0, reserved
; D6  = 0, reserved
; D7  = 0, reserved
; D8  = 0, reserved
; D9  = 0, reserved
; D10 = 0, reserved
; D11 = 0, reserved
; D12 = 0, reserved
; D13 = 0, reserved
; D14 = 0, reserved
; D15 = 0, reserved
;-------

Options_Bitmap	DW  ?

;---------- Platform compatibility bitmap ----------------------------------;
; D0  = PC platform,required additional differentiate IA32 and x64 by native size
; D1  = Itanium platform
; D2  = ARM platform
; D3  = 0, reserved
; D4  = 0, reserved
; D5  = 0, reserved
; D6  = 0, reserved
; D7  = 0, reserved
; D8  = 0, reserved
; D9  = 0, reserved
; D10 = 0, reserved
; D11 = 0, reserved
; D12 = 0, reserved
; D13 = 0, reserved
; D14 = 0, reserved
; D15 = 0, reserved
;-------

Platform_Bitmap	DW  ?

;---------- UEFI context analysing results ---------------------------------;

EFI_Revision		DD  ?		; UEFI specification revision
Firmware_Revision	DD  ?		; Firmware revision
EBC_VM_Version		DD  ?		; EBC virtual machine version
Firmware_Vendor		DQ  ?		; Pointer to nul-terminated string
Legacy_BIOS_Protocol	DQ  ?		; Pointers to protocol intf. blocks
GOP_Protocol		DQ  ?
UGADRAW_Protocol	DQ  ?
ACPI_Pointer		DQ  ?

;---------- Memory allocation control --------------------------------------;

EFI_Memory_Total	DQ  ?		; Total memory reported by firmware, bytes
EFI_Memory_ACPI		DQ  ?		; ACPI memory reported by firmware, bytes
Primary_Memory_Base	DQ  ?		; Base address of this application pool
Primary_Memory_Size	DQ  ?		; Size of of this application pool
Secondary_Memory_Base	DQ  ?		; Base address of separate part of pool
Secondary_Memory_Size	DQ  ?		; Size of separate part of pool

;---------- Processor detection results ------------------------------------;

CPUID_0_Data		DB  16 DUP (0)	; Results of CPUID function 0
CPU_Native_Width	DB  ?		; CPU native width by UEFI, units=bytes
CPU_Address_Width	DB  ?		; Address width by CPUID
CPU_Name_String		DB  50 DUP (?)	; CPU name string, size+2 for terminator+alignment
					; Also used as scratch pad for CPUID functions
CPU_Standard_Features	DQ  ?		; ECX:EDX after CPUID#00000001h, ECX=High, EDX=Low
CPU_Extended_Features	DQ  ?		; ECX:EDX after CPUID#80000001h, ECX=High, EDX=Low
TSC_Period		DQ  ?		; CPU Time Stamp Counter period, femptoseconds

;---------- Video subsystem control ----------------------------------------;

Firmware_Video_Mode	DD  ?		; GOP video mode number, for mode, used by firmware
Program_Video_Mode	DD  ?		; GOP video mode number, for mode, set by this program
Video_Xsize		DW  ?		; GOP.BLT X size, pixels
Video_Ysize		DW  ?		; GOP.BLT Y size, pixels
Video_Xdelta		DW  ?		; GOP.BLT Y line length, bytes (1 pixel = 4 bytes)

