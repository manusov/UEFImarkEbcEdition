;===========================================================================;
;                                                                           ;
;   TEXT AND CONTROL DATA FOR BUILT BLOCK OF SYSTEM INFORMATION STRINGS.    ;
;                                                                           ;
;===========================================================================;

;---------- Macro for system information control table ---------------------;

; SINF=System information sequence element
; X1=Checkpoint Code (Port 80h)
; X2=Subroutine, 16-bit signed offset relative Global_Variables_Pool
; Special X1 values: 00h=Stop

MACRO  SINF  x1, x2
{
DB  x1
DW  x2
}

MACRO  STOPSINF
{
DB  00h
}

;---------- Equations for subroutines call and data access -----------------;

_Execute_Benchmarks_1			= Execute_Benchmarks_1 - Global_Variables_Pool
_Execute_Benchmarks_2			= Execute_Benchmarks_2 - Global_Variables_Pool
_Execute_Benchmarks_3			= Execute_Benchmarks_3 - Global_Variables_Pool
_Execute_Benchmarks_4			= Execute_Benchmarks_4 - Global_Variables_Pool
_Execute_Benchmarks_5			= Execute_Benchmarks_5 - Global_Variables_Pool
_Execute_Benchmarks_6			= Execute_Benchmarks_6 - Global_Variables_Pool
_Execute_Benchmarks_7			= Execute_Benchmarks_7 - Global_Variables_Pool
_Execute_Benchmarks_8			= Execute_Benchmarks_8 - Global_Variables_Pool
_Info_About				= Info_About - Global_Variables_Pool
_Info_UEFI_Specification_Revision	= Info_UEFI_Specification_Revision - Global_Variables_Pool
_Info_UEFI_Firmware_Revision		= Info_UEFI_Firmware_Revision - Global_Variables_Pool
_Info_UEFI_Firmware_Vendor_String	= Info_UEFI_Firmware_Vendor_String - Global_Variables_Pool
_Info_UEFI_Shell_Revision		= Info_UEFI_Shell_Revision - Global_Variables_Pool
_Info_UEFI_Shell_Vendor_String		= Info_UEFI_Shell_Vendor_String - Global_Variables_Pool
_Info_EBC_Interpreter_Revision		= Info_EBC_Interpreter_Revision - Global_Variables_Pool
_Info_CSM_Presence			= Info_CSM_Presence - Global_Variables_Pool
_Info_Native_Width			= Info_Native_Width - Global_Variables_Pool
_Info_CPU_Vendor_String			= Info_CPU_Vendor_String - Global_Variables_Pool
_Info_CPU_Name_String			= Info_CPU_Name_String - Global_Variables_Pool
_Info_CPU_Supported_Features		= Info_CPU_Supported_Features - Global_Variables_Pool
_Info_CPU_and_CAP_Clocks		= Info_CPU_and_CAP_Clocks - Global_Variables_Pool
_Info_Video_Mode_Parameters		= Info_Video_Mode_Parameters - Global_Variables_Pool
_Info_AGP_Video_Host_Parameters		= Info_AGP_Video_Host_Parameters - Global_Variables_Pool
_Info_AGP_Video_Device_Parameters	= Info_AGP_Video_Device_Parameters - Global_Variables_Pool
_Info_PCIe_Video_Host_Parameters	= Info_PCIe_Video_Host_Parameters - Global_Variables_Pool
_Info_PCIe_Video_Device_Parameters	= Info_PCIe_Video_Device_Parameters - Global_Variables_Pool
_Info_System_Memory			= Info_System_Memory - Global_Variables_Pool
_Info_Video_Memory			= Info_Video_Memory - Global_Variables_Pool
_Info_Video_Memory_Attributes		= Info_Video_Memory_Attributes - Global_Variables_Pool
_Info_Benchmarks_1			= Info_Benchmarks_1 - Global_Variables_Pool
_Info_Benchmarks_2			= Info_Benchmarks_2 - Global_Variables_Pool
_Info_Benchmarks_3			= Info_Benchmarks_3 - Global_Variables_Pool
_Info_Benchmarks_4			= Info_Benchmarks_4 - Global_Variables_Pool
_Info_Press_Keys			= Info_Press_Keys - Global_Variables_Pool
_Show_Results				= Show_Results - Global_Variables_Pool

_Info_Sequence				= Info_Sequence - Global_Variables_Pool

_String_Benchmarking			= String_Benchmarking - Global_Variables_Pool
_String_UEFI				= String_UEFI - Global_Variables_Pool
_String_Firmware			= String_Firmware - Global_Variables_Pool
_String_Firmware_Vendor			= String_Firmware_Vendor - Global_Variables_Pool
_String_EBC				= String_EBC - Global_Variables_Pool
_String_CSM				= String_CSM - Global_Variables_Pool
_String_CPU_Width			= String_CPU_Width - Global_Variables_Pool
_String_Phys_Addr			= String_Phys_Addr - Global_Variables_Pool
_String_CPU_Vendor			= String_CPU_Vendor - Global_Variables_Pool
_String_CPU_Name			= String_CPU_Name - Global_Variables_Pool
_String_CPU_Features			= String_CPU_Features - Global_Variables_Pool
_String_Clocks				= String_Clocks - Global_Variables_Pool
_String_Not_Found			= String_Not_Found - Global_Variables_Pool
_String_Found				= String_Found - Global_Variables_Pool
_String_TSC				= String_TSC - Global_Variables_Pool
_String_CAP				= String_CAP - Global_Variables_Pool
_String_Interval			= String_Interval - Global_Variables_Pool
_String_Graphics			= String_Graphics - Global_Variables_Pool
_String_GOP_Mode			= String_GOP_Mode - Global_Variables_Pool
_String_Xsize				= String_Xsize - Global_Variables_Pool
_String_Ysize				= String_Ysize - Global_Variables_Pool
_String_BPP				= String_BPP - Global_Variables_Pool
_String_G_Bus				= String_G_Bus - Global_Variables_Pool
_String_G_Device			= String_G_Device - Global_Variables_Pool
_String_AGP_B				= String_AGP_B - Global_Variables_Pool
_String_AGP_A				= String_AGP_A - Global_Variables_Pool
_String_FastW				= String_FastW - Global_Variables_Pool
_String_NotOpt				= String_NotOpt - Global_Variables_Pool
_String_AGP_2				= String_AGP_2 - Global_Variables_Pool
_String_FW				= String_FW - Global_Variables_Pool
_String_PCIe_B				= String_PCIe_B - Global_Variables_Pool
_String_PCIe_A				= String_PCIe_A - Global_Variables_Pool
_String_PCIe_1				= String_PCIe_1 - Global_Variables_Pool
_String_PCIe_2				= String_PCIe_2 - Global_Variables_Pool
_String_PCIe_3				= String_PCIe_3 - Global_Variables_Pool
_String_25GTS				= String_25GTS - Global_Variables_Pool
_String_50GTS				= String_50GTS - Global_Variables_Pool
_String_80GTS				= String_80GTS - Global_Variables_Pool
_String_xGTS				= String_xGTS - Global_Variables_Pool
_String_System_Memory			= String_System_Memory - Global_Variables_Pool
_String_U_Mem				= String_U_Mem - Global_Variables_Pool
_String_A_Mem				= String_A_Mem - Global_Variables_Pool
_String_Video_Memory			= String_Video_Memory - Global_Variables_Pool
_String_G_Mem				= String_G_Mem - Global_Variables_Pool
_String_H_Mem				= String_H_Mem - Global_Variables_Pool
_String_Or_Above			= String_Or_Above - Global_Variables_Pool
_String_MTRR				= String_MTRR - Global_Variables_Pool
_String_MTRR_1				= String_MTRR_1 - Global_Variables_Pool
_String_MTRR_2				= String_MTRR_2 - Global_Variables_Pool
_String_MTRR_3				= String_MTRR_3 - Global_Variables_Pool
_String_MTRR_N				= String_MTRR_N - Global_Variables_Pool
_String_GOP_BLT_FPS			= String_GOP_BLT_FPS - Global_Variables_Pool
_String_API_FPS				= String_API_FPS - Global_Variables_Pool
_String_Native_FPS			= String_Native_FPS - Global_Variables_Pool
_String_Native_MBPS			= String_Native_MBPS - Global_Variables_Pool
_String_Fill				= String_Fill - Global_Variables_Pool
_String_Copy				= String_Copy - Global_Variables_Pool
_String_Press_Keys			= String_Press_Keys - Global_Variables_Pool
_String_Press_Keys_1			= String_Press_Keys_1 - Global_Variables_Pool

_Sign_CPU				= Sign_CPU - Global_Variables_Pool
_Sign_Intel				= Sign_Intel - Global_Variables_Pool
_Sign_AMD				= Sign_AMD - Global_Variables_Pool
_Selector_CPU_Features			= Selector_CPU_Features - Global_Variables_Pool
_String_EM64T				= String_EM64T - Global_Variables_Pool
_String_AMD64				= String_AMD64 - Global_Variables_Pool
_String_MMX				= String_MMX - Global_Variables_Pool
_String_SSE				= String_SSE - Global_Variables_Pool
_String_SSE2				= String_SSE2 - Global_Variables_Pool
_String_SSE3				= String_SSE3 - Global_Variables_Pool
_String_SSSE3				= String_SSSE3 - Global_Variables_Pool
_String_SSE41				= String_SSE41 - Global_Variables_Pool
_String_SSE42				= String_SSE42 - Global_Variables_Pool
_String_SSE4A				= String_SSE4A - Global_Variables_Pool
_String_SSE5				= String_SSE5 - Global_Variables_Pool
_String_3DNow				= String_3DNow - Global_Variables_Pool
_String_3DNowP				= String_3DNowP - Global_Variables_Pool
_String_AVX				= String_AVX - Global_Variables_Pool
_Intel_CPU_Features			= Intel_CPU_Features - Global_Variables_Pool
_AMD_CPU_Features			= AMD_CPU_Features - Global_Variables_Pool

; Added from v0.25

_String_AVX2				= String_AVX2 - Global_Variables_Pool
_String_AVX512F				= String_AVX512F - Global_Variables_Pool
_String_AVX512DQ			= String_AVX512DQ - Global_Variables_Pool
_String_AVX512IFMA			= String_AVX512IFMA - Global_Variables_Pool
_String_AVX512PF			= String_AVX512PF - Global_Variables_Pool
_String_AVX512ER			= String_AVX512CD - Global_Variables_Pool
_String_AVX512CD			= String_AVX512BW - Global_Variables_Pool
_String_AVX512BW			= String_AVX512VL - Global_Variables_Pool
_String_AVX512VL			= String_AVX512VL - Global_Variables_Pool
_String_AVX512VBM			= String_AVX512VBM - Global_Variables_Pool

_String_CPU_Features_1			= String_CPU_Features_1 - Global_Variables_Pool
_AVX512_CPU_Features_1			= AVX512_CPU_Features_1 - Global_Variables_Pool
_AVX512_CPU_Features_2			= AVX512_CPU_Features_2 - Global_Variables_Pool

_Info_CPU_Supported_Features_1		= Info_CPU_Supported_Features_1 - Global_Variables_Pool
_Info_CPU_Supported_Features_2		= Info_CPU_Supported_Features_2 - Global_Variables_Pool

;---------- Control table for run sequence of handlers ---------------------;

Info_Sequence:
SINF  020h , _Execute_Benchmarks_1
SINF  021h , _Execute_Benchmarks_2
SINF  022h , _Execute_Benchmarks_3
SINF  023h , _Execute_Benchmarks_4
SINF  024h , _Execute_Benchmarks_5
SINF  025h , _Execute_Benchmarks_6
SINF  026h , _Execute_Benchmarks_7
SINF  027h , _Execute_Benchmarks_8
; 28h-2Fh reserved for new performance tests
SINF  030h , _Info_About
SINF  031h , _Info_UEFI_Specification_Revision
SINF  032h , _Info_UEFI_Firmware_Revision
SINF  033h , _Info_UEFI_Firmware_Vendor_String
SINF  034h , _Info_UEFI_Shell_Revision
SINF  035h , _Info_UEFI_Shell_Vendor_String
SINF  036h , _Info_EBC_Interpreter_Revision
SINF  037h , _Info_CSM_Presence
SINF  038h , _Info_Native_Width
SINF  039h , _Info_CPU_Vendor_String
SINF  03Ah , _Info_CPU_Name_String
SINF  03Bh , _Info_CPU_Supported_Features
;- From v0.25, diagnostics code 03Bh repeats for AVX512 features 2 sublists
SINF  03Bh , _Info_CPU_Supported_Features_1
SINF  03Bh , _Info_CPU_Supported_Features_2
;-
SINF  03Ch , _Info_CPU_and_CAP_Clocks
SINF  03Dh , _Info_Video_Mode_Parameters
SINF  03Eh , _Info_AGP_Video_Host_Parameters
SINF  03Fh , _Info_AGP_Video_Device_Parameters
SINF  040h , _Info_PCIe_Video_Host_Parameters
SINF  041h , _Info_PCIe_Video_Device_Parameters
SINF  042h , _Info_System_Memory
SINF  043h , _Info_Video_Memory
SINF  044h , _Info_Video_Memory_Attributes
; 45h-47h reserved for new information strings
SINF  048h , _Info_Benchmarks_1
SINF  049h , _Info_Benchmarks_2
SINF  04Ah , _Info_Benchmarks_3
SINF  04Bh , _Info_Benchmarks_4
; 4Ch-4Dh reserved for new benchmarks strings
SINF  04Eh , _Info_Press_Keys
SINF  04Fh , _Show_Results
STOPSINF

;---------- Text strings for system information write ----------------------;

String_Benchmarking	DB  'Benchmarking, screen may blink, please wait...',0

String_UEFI		DB  'UEFI specification : ',0
String_Firmware		DB  'Firmware revision  : ',0
String_Firmware_Vendor	DB  'Firmware vendor    : ',0
String_EBC		DB  'EBC VM revision    : ',0
String_CSM		DB  'CSM module         : ',0
String_CPU_Width	DB  'Native width, bits : ',0

String_Phys_Addr	DB  ', physical address width=',0

String_CPU_Vendor	DB  'CPU vendor string  : ',0
String_CPU_Name		DB  'CPU name string    : ',0
String_CPU_Features	DB  'CPU features       : ',0
String_Clocks		DB  'Clocks, MHz        : ',0

String_CPU_Features_1	DB  '                   + ',0

String_Not_Found	DB  'not '
String_Found		DB  'found',0

String_TSC		DB  'TSC=',0
String_CAP		DB  'CAP=',0
String_Interval		DB  ', ',0

String_Graphics		DB  'Graphics mode      : ',0
String_GOP_Mode		DB  'GOP mode ',0
String_Xsize		DB  ', Xsize=',0
String_Ysize		DB  ', Ysize=',0
String_BPP		DB  ', BPP=',0

String_G_Bus		DB  'Graphics bus       : ',0
String_G_Device		DB  'Graphics device    : ',0

String_AGP_B		DB  'AGP  ',0		; Bridge
String_AGP_A		DB  'Card ',0		; Adapter
String_FastW		DB  'fast write at ',0
String_NotOpt		DB  'not optimized',0
String_AGP_2		DB  ', capable: ',0
String_FW		DB  'FW, ',0

String_PCIe_B		DB  'PCIe x',0		; Bridge
String_PCIe_A		DB  'Card x',0		; Adapter
String_PCIe_1		DB  ', ' ,0
String_PCIe_2		DB  'GT/s, TLP=',0
String_PCIe_3		DB  ', max: x',0
String_25GTS		DB  '2.5',0
String_50GTS		DB  '5.0',0
String_80GTS		DB  '8.0',0
String_xGTS		DB  '?',0

String_System_Memory	DB  'System memory, KB  : ',0
String_U_Mem		DB  'UEFI=',0
String_A_Mem		DB  ', reserved for ACPI=',0

String_Video_Memory	DB  'Video memory, KB   : ',0
String_G_Mem		DB  'GOP=',0
String_H_Mem		DB  ', hardware scan=',0
String_Or_Above		DB  ' or above',0

; This 4 strings must be sequental
String_MTRR		DB  'Video memory MTRR  : ',0
String_MTRR_1		DB  'h-',0
String_MTRR_2		DB  'h, ',0
String_MTRR_3:
DB  'UC' , 0		; 00=Uncacheable
DB  'WC' , 0		; 01=Write Combining
DB  '?'  , 0		; 02=Reserved
DB  '?'  , 0		; 03=Reserved
DB  'WT' , 0		; 04=Write Through
DB  'WP' , 0		; 05=Write Protected
DB  'WB' , 0		; 06=Write Back
String_MTRR_N		DB  'No vacant MTRR found',0

String_GOP_BLT_FPS	DB  'GOP.BLT, FPS       : ',0
String_API_FPS		DB  'Set/Copy API, FPS  : ',0
String_Native_FPS	DB  'Set/Copy CPU,  FPS : ',0
String_Native_MBPS	DB  'Set/Copy CPU, MBPS : ',0
String_Fill		DB  'Fill=',0
String_Copy		DB  ' , Copy=',0

String_Press_Keys	DB  'Press any key to continue, ',0
String_Press_Keys_1	DB  'F9=Save TXT, F10=Save BMP',0

;---------- Constants for CPU identification -------------------------------;

NUMBER_OF_CPU_VENDORS	= 2

Sign_CPU:
Sign_Intel	DB  'GenuineIntel'
Sign_AMD	DB  'AuthenticAMD'

;------------------------------------------
; Intel CPU features, selected for visual:
;------------------------------------------
; Intel EM64T = CPUID#80000001h, EDX.29
; MMX         = CPUID#1, EDX.23
; SSE         = CPUID#1, EDX.25
; SSE2        = CPUID#1, EDX.26
; SSE3        = CPUID#1, ECX.0
; SSSE3       = CPUID#1, ECX.9
; SSE4.1      = CPUID#1, ECX.19
; SSE4.2      = CPUID#1, ECX.20
; AVX         = CPUID#1, ECX.28
; AVX2        = CPUID#7, EBX.5
;-
; AVX512F     = CPUID#7, EBX.16    ; Foundation  
; AVX512DQ    = CPUID#7, EBX.17    ; Doublewords and Quadwords operations     
; AVX512IFMA  = CPUID#7, EBX.21    ; Integer Fused Multiply and Add 
; AVX512PF    = CPUID#7, EBX.26    ; Prefetch          
; AVX512ER    = CPUID#7, EBX.27    ; Exponential and Reciprocal  
; AVX512CD    = CPUID#7, EBX.28    ; Conflict Detection 
; AVX512BW    = CPUID#7, EBX.30    ; Bytes and Words Operations 
; AVX512VL    = CPUID#7, EBX.31    ; Vector Length Control 
; AVX512VBM   = CPUID#7, ECX.1     ; Vector Byte Manipulation   

;----------------------------------------
; AMD CPU features, selected for visual:
;----------------------------------------
; AMD64       = CPUID#80000001h, EDX.29
; MMX         = CPUID#1, EDX.23
; SSE         = CPUID#1, EDX.25
; SSE2        = CPUID#1, EDX.26
; SSE3        = CPUID#1, ECX.0
; SSSE3       = CPUID#1, ECX.9
; SSE4.1      = CPUID#1, ECX.19
; SSE4.2      = CPUID#1, ECX.20
; SSE4A       = CPUID#80000001h, ECX.6
; SSE5        = CPUID#80000001h, ECX.11
; 3DNow       = CPUID#80000001h, EDX.31
; 3DNow+      = CPUID#80000001h, EDX.30
; AVX         = CPUID#1, ECX.28
; AVX2        = CPUID#7, EBX.5
;-
; AVX512F     = CPUID#7, EBX.16    ; Foundation  
; AVX512DQ    = CPUID#7, EBX.17    ; Doublewords and Quadwords operations     
; AVX512IFMA  = CPUID#7, EBX.21    ; Integer Fused Multiply and Add 
; AVX512PF    = CPUID#7, EBX.26    ; Prefetch          
; AVX512ER    = CPUID#7, EBX.27    ; Exponential and Reciprocal  
; AVX512CD    = CPUID#7, EBX.28    ; Conflict Detection 
; AVX512BW    = CPUID#7, EBX.30    ; Bytes and Words Operations 
; AVX512VL    = CPUID#7, EBX.31    ; Vector Length Control 
; AVX512VBM   = CPUID#7, ECX.1     ; Vector Byte Manipulation   

;------------------------------------------
; Notes about same support for Intel, AMD:
;------------------------------------------
; Probably differences:
; SSE4A, 3DNow, 3DNow+ (3DNowExt)
;-----------------------------------------

String_EM64T	DB  'iEM64T',0
String_AMD64	DB  'AMD64',0
String_MMX	DB  'MMX',0
String_SSE	DB  'SSE',0
String_SSE2	DB  'SSE2',0
String_SSE3	DB  'SSE3',0
String_SSSE3	DB  'SSSE3',0
String_SSE41	DB  'SSE4.1',0
String_SSE42	DB  'SSE4.2',0
String_SSE4A	DB  'SSE4A',0
String_SSE5	DB  'SSE5',0
String_3DNow	DB  '3DNow',0
String_3DNowP	DB  '3DNow+',0
String_AVX	DB  'AVX',0
; This from v0.25
String_AVX2        DB  'AVX2',0
String_AVX512F     DB  'AVX512F',0
String_AVX512DQ    DB  'AVX512DQ',0
String_AVX512IFMA  DB  'AVX512IFMA',0
String_AVX512PF    DB  'AVX512PF',0
String_AVX512ER    DB  'AVX512ER',0
String_AVX512CD    DB  'AVX512CD',0
String_AVX512BW    DB  'AVX512BW',0
String_AVX512VL    DB  'AVX512VL',0
String_AVX512VBM   DB  'AVX512VBM',0

;--- Required upgrade for AVX2/AVX512 at v0.25 ---
; X1 = Strung type: 0=Unconditional, 1=Conditional
; X2 = CPUID Selector: 0=Standard, 1=Extended
; X3 = Register selector after CPUID: 0=EDX, 1=ECX
; X4 = Bit number for register EDX or ECX
; X5 = Text string, signed offset relative R1=Global_Variables_Pool
; Value 0FFh means stop
;
; MACRO  CFEAT  X1, X2, X3, X4, X5
; {
; DB X1 SHL 7 + X2 SHL 6 + X3 SHL 5 + X4
; DW X5
; }
;
; MACRO  CFEATSTOP
; {
; DB 0FFh
; }
;--- End of required upgrade for AVX2/AVX512 at v0.25 ---

; v0.25 modification, 16-bit tags for new CPUID functions support
; [n] means bitfield size, not-listed values reserved
; X1[4] = Strung type: 0=Unconditional, 1=Conditional
; X2[4] = CPUID Selector: 0=Standard (0), 1=Extended (80000000h), 2=Fnc. 7
; X3[3] = Register selector after CPUID: 0=EAX (n/a), 1=EBX, 2=ECX, 3=EDX 
; X4[5] = Bit number for register EAX/EBX/ECX/EDX
; X5[64] = Pointer to text string
; Tag value 0FFFFh means stop

MACRO  CPU_FEATURE  X1, X2, X3, X4, X5
{
DW  X1 SHL 12 + X2 SHL 8 + X3 SHL 5 + X4
DW  X5
}

MACRO  CPU_FEATURE_STOP
{
DW  0FFFFh
}

Selector_CPU_Features:
DW  _Intel_CPU_Features
DW  _AMD_CPU_Features

;--- Old format entries, required upgrade for AVX2/AVX512 at v0.25 ---
;
; Intel_CPU_Features:
; ; First string, removed
; ; Intel EM64T = CPUID#80000001h, EDX.29
; CFEAT  1 , 1 , 0 , 29 , _String_EM64T
; ; MMX = CPUID#1, EDX.23
; CFEAT  1 , 0 , 0 , 23 , _String_MMX
; ; SSE = CPUID#1, EDX.25
; CFEAT  1 , 0 , 0 , 25 , _String_SSE
; ; SSE2 = CPUID#1, EDX.26
; CFEAT  1 , 0 , 0 , 26 , _String_SSE2
; ; SSE3 = CPUID#1, ECX.0
; CFEAT  1 , 0 , 1 , 00 , _String_SSE3
; ; SSSE3 = CPUID#1, ECX.9
; CFEAT  1 , 0 , 1 , 09 , _String_SSSE3
; ; SSE4.1 = CPUID#1, ECX.19
; CFEAT  1 , 0 , 1 , 19 , _String_SSE41
; ; SSE4.2 = CPUID#1, ECX.20
; CFEAT  1 , 0 , 1 , 20 , _String_SSE42
; ; AVX = CPUID#1, ECX.28
; CFEAT  1 , 0 , 1 , 28 , _String_AVX
; ; Stop
; CFEATSTOP
;
; AMD_CPU_Features:
; ; First string, removed
; ; AMD64 = CPUID#80000001h, EDX.29
; CFEAT  1 , 1 , 0 , 29 , _String_AMD64
; ; MMX = CPUID#1, EDX.23
; CFEAT  1 , 0 , 0 , 23 , _String_MMX
; ; SSE = CPUID#1, EDX.25
; CFEAT  1 , 0 , 0 , 25 , _String_SSE
; ; SSE2 = CPUID#1, EDX.26
; CFEAT  1 , 0 , 0 , 26 , _String_SSE2
; ; SSE3 = CPUID#1, ECX.0
; CFEAT  1 , 0 , 1 , 00 , _String_SSE3
; ; SSSE3 = CPUID#1, ECX.9
; CFEAT  1 , 0 , 1 , 09 , _String_SSSE3
; ; SSE4.1 = CPUID#1, ECX.19
; CFEAT  1 , 0 , 1 , 19 , _String_SSE41
; ; SSE4.2 = CPUID#1, ECX.20
; CFEAT  1 , 0 , 1 , 20 , _String_SSE42
; ; SSE4A = CPUID#80000001h, ECX.6
; CFEAT  1 , 1 , 1 , 06 , _String_SSE4A
; ; SSE5 = CPUID#80000001h, ECX.11
; CFEAT  1 , 1 , 1 , 11 , _String_SSE5
; ; 3DNow = CPUID#80000001h, EDX.31
; CFEAT  1 , 1 , 0 , 31 , _String_3DNow
; ; 3DNow+ = CPUID#80000001h, EDX.30
; CFEAT  1 , 1 , 0 , 30 , _String_3DNowP
; ; AVX = CPUID#1, ECX.28
; CFEAT  1 , 0 , 1 , 28 , _String_AVX
; ; Stop
; CFEATSTOP
;
;--- End of required upgrade for AVX2/AVX512 at v0.25 ---


Intel_CPU_Features:
; First string, removed
; Intel EM64T = CPUID#80000001h, EDX.29
CPU_FEATURE  1 , 1 , 3 , 29 , _String_EM64T
; MMX = CPUID#1, EDX.23
CPU_FEATURE  1 , 0 , 3 , 23 , _String_MMX
; SSE = CPUID#1, EDX.25
CPU_FEATURE  1 , 0 , 3 , 25 , _String_SSE
; SSE2 = CPUID#1, EDX.26
CPU_FEATURE  1 , 0 , 3 , 26 , _String_SSE2
; SSE3 = CPUID#1, ECX.0
CPU_FEATURE  1 , 0 , 2 , 00 , _String_SSE3
; SSSE3 = CPUID#1, ECX.9
CPU_FEATURE  1 , 0 , 2 , 09 , _String_SSSE3
; SSE4.1 = CPUID#1, ECX.19
CPU_FEATURE  1 , 0 , 2 , 19 , _String_SSE41
; SSE4.2 = CPUID#1, ECX.20
CPU_FEATURE  1 , 0 , 2 , 20 , _String_SSE42
; AVX = CPUID#1, ECX.28
CPU_FEATURE  1 , 0 , 2 , 28 , _String_AVX
; AVX2        = CPUID#7, EBX.5                                    ; From v0.25
CPU_FEATURE  1 , 2 , 1 , 05 , _String_AVX2  
; Stop, end of list
CPU_FEATURE_STOP


AMD_CPU_Features:
; First string, removed
; AMD64 = CPUID#80000001h, EDX.29
CPU_FEATURE  1 , 1 , 3 , 29 , _String_AMD64
; MMX = CPUID#1, EDX.23
CPU_FEATURE  1 , 0 , 3 , 23 , _String_MMX
; SSE = CPUID#1, EDX.25
CPU_FEATURE  1 , 0 , 3 , 25 , _String_SSE
; SSE2 = CPUID#1, EDX.26
CPU_FEATURE  1 , 0 , 3 , 26 , _String_SSE2
; SSE3 = CPUID#1, ECX.0
CPU_FEATURE  1 , 0 , 2 , 00 , _String_SSE3
; SSSE3 = CPUID#1, ECX.9
CPU_FEATURE  1 , 0 , 2 , 09 , _String_SSSE3
; SSE4.1 = CPUID#1, ECX.19
CPU_FEATURE  1 , 0 , 2 , 19 , _String_SSE41
; SSE4.2 = CPUID#1, ECX.20
CPU_FEATURE  1 , 0 , 2 , 20 , _String_SSE42
; SSE4A = CPUID#80000001h, ECX.6
CPU_FEATURE  1 , 1 , 2 , 06 , _String_SSE4A
; SSE5 = CPUID#80000001h, ECX.11
CPU_FEATURE  1 , 1 , 2 , 11 , _String_SSE5
; 3DNow = CPUID#80000001h, EDX.31
CPU_FEATURE  1 , 1 , 3 , 31 , _String_3DNow
; 3DNow+ = CPUID#80000001h, EDX.30
CPU_FEATURE  1 , 1 , 3 , 30 , _String_3DNowP
; AVX = CPUID#1, ECX.28
CPU_FEATURE  1 , 0 , 2 , 28 , _String_AVX
; AVX2        = CPUID#7, EBX.5                                    ; From v0.25
CPU_FEATURE  1 , 2 , 1 , 05 , _String_AVX2  
; Stop, end of list
CPU_FEATURE_STOP


AVX512_CPU_Features_1:                                            ; From v0.25
; AVX512F     = CPUID#7, EBX.16    ; Foundation  
CPU_FEATURE  1 , 2 , 1 , 16 , _String_AVX512F
; AVX512IFMA  = CPUID#7, EBX.21    ; Integer Fused Multiply and Add 
CPU_FEATURE  1 , 2 , 1 , 21 , _String_AVX512IFMA 
; AVX512PF    = CPUID#7, EBX.26    ; Prefetch          
CPU_FEATURE  1 , 2 , 1 , 26 , _String_AVX512PF
; AVX512ER    = CPUID#7, EBX.27    ; Exponential and Reciprocal  
CPU_FEATURE  1 , 2 , 1 , 27 , _String_AVX512ER
; AVX512CD    = CPUID#7, EBX.28    ; Conflict Detection 
CPU_FEATURE  1 , 2 , 1 , 28 , _String_AVX512CD
; Stop, end of list
CPU_FEATURE_STOP


AVX512_CPU_Features_2:
; AVX512DQ    = CPUID#7, EBX.17    ; Doublewords and Quadwords operations     
CPU_FEATURE  1 , 2 , 1 , 17 , _String_AVX512DQ
; AVX512BW    = CPUID#7, EBX.30    ; Bytes and Words Operations 
CPU_FEATURE  1 , 2 , 1 , 30 , _String_AVX512BW
; AVX512VL    = CPUID#7, EBX.31    ; Vector Length Control 
CPU_FEATURE  1 , 2 , 1 , 31 , _String_AVX512VL
; AVX512VBM   = CPUID#7, ECX.1     ; Vector Byte Manipulation   
CPU_FEATURE  1 , 2 , 2 , 01 , _String_AVX512VBM
; Stop, end of list
CPU_FEATURE_STOP


