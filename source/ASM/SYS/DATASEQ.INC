;===========================================================================;
;                                                                           ;
;        TEXT AND CONTROL DATA FOR PROGRAM START AND STOP SEQUENCES.        ;
;                                                                           ;
;===========================================================================;

;---------- Macro for start and stop control tables ------------------------;

; S=Sequence element
; X1=Checkpoint Code (Port 80h), X2=Check bit
; X3=Subroutine, X4=Message string, X5=Error string 
; X3, X4, X5 is signed 16-bit offsets relative Global_Variables_Pool
; CheckBit special value: 0FFh=Stop
MACRO  S  x1, x2, x3, x4, x5
{
DB  x1, x2
DW  x4, x3, x5
}

; SG=Sequence special element, last executed during graphics mode, 
; text out not available before execution, available after execution
; This element used for restore text mode when exit to OS.
; CheckBit.D7=1 means this special mode
; X1=Checkpoint Code (Port 80h), X2=Check bit
; X3=Subroutine, X4=Message string, X5=Error string
; X3, X4, X5 is signed 16-bit offsets relative Global_Variables_Pool
; CheckBit special value: 0FFh=Stop
MACRO  SG  x1, x2, x3, x4, x5
{
DB  x1, x2+80h		; Bit X2.7=1
DW  x4, x3, x5
}

; CheckBit.D6=0 means bits [0-31] of _Sys_Bitmap used as run enable,
; bit selected by CheckBit.D[4-0]
; CheckBit.D6=1 means unconditional run, NC means No Conditions
NC = 040h		; Bit X2.6=1

; SS=Sequence special element, executed for switch to (new) graphics mode,
; text out not available after execution, available before execution
; This element used for set (new) graphics mode when run program,
; need skip text write OK/FAILED for this element.
; CheckBit.D5=1 means this special mode
; X1=Checkpoint Code (Port 80h), X2=Check bit
; X3=Subroutine, X4=Message string, X5=Error string
; X3, X4, X5 is signed 16-bit offsets relative Global_Variables_Pool
; CheckBit special value: 0FFh=Stop
MACRO  SS  x1, x2, x3, x4, x5
{
DB  x1, x2+20h		; Bit X2.5=1
DW  x4, x3, x5
}

; Checkpoint Code=00h means stop
MACRO  STOPS
{
DB  00h
}

;---------- Equations for data access --------------------------------------;
; For compact coding, use 16-bit offsets relative Global_Variables_Pool
; instead full 64-bit flat addresses. This method also eliminate relocation
; elements with some bugs in the old IA32 EFI implementations.

;--- Start phase routines ---

_Check_CPU_Architecture		= Check_CPU_Architecture - Global_Variables_Pool
_Check_UEFI_Configuration	= Check_UEFI_Configuration - Global_Variables_Pool
_Check_Shell_Configuration	= Check_Shell_Configuration - Global_Variables_Pool
_Check_CAP			= Check_CAP - Global_Variables_Pool
_Check_Native_CPU		= Check_Native_CPU - Global_Variables_Pool
_Allocate_Memory		= Allocate_Memory - Global_Variables_Pool
_Load_Config			= Load_Config - Global_Variables_Pool
_Check_GOP_Video		= Check_GOP_Video - Global_Variables_Pool
_Check_Video_Mode		= Check_Video_Mode - Global_Variables_Pool
_Check_ACPI			= Check_ACPI - Global_Variables_Pool
_Check_PCI_Bus			= Check_PCI_Bus - Global_Variables_Pool
_Check_Active_Video		= Check_Active_Video - Global_Variables_Pool
_Check_Video_BAR		= Check_Video_BAR - Global_Variables_Pool
_Check_AGP			= Check_AGP - Global_Variables_Pool
_Check_PCIe			= Check_PCIe - Global_Variables_Pool
_Check_Video_Range		= Check_Video_Range - Global_Variables_Pool
_Setup_AGP			= Setup_AGP - Global_Variables_Pool
_Setup_PCIe			= Setup_PCIe - Global_Variables_Pool
_Setup_Video_Range		= Setup_Video_Range - Global_Variables_Pool
_Check_Mouse			= Check_Mouse - Global_Variables_Pool
_Measure_Clock			= Measure_Clock - Global_Variables_Pool
_Setup_Wait			= Setup_Wait - Global_Variables_Pool
_Setup_Video_Mode		= Setup_Video_Mode - Global_Variables_Pool

;--- Stop phase routines ---

_Restore_Video_Mode		= Restore_Video_Mode - Global_Variables_Pool
_Restore_Video_Range		= Restore_Video_Range - Global_Variables_Pool
_Restore_Video_PCIe		= Restore_Video_PCIe - Global_Variables_Pool
_Restore_Video_AGP		= Restore_Video_AGP - Global_Variables_Pool
_Save_Configuration		= Save_Configuration - Global_Variables_Pool
_Release_Memory			= Release_Memory - Global_Variables_Pool

;--- Start phase messages ---

_Msg_C_Arch			= Msg_C_Arch - Global_Variables_Pool
_Msg_C_UEFI			= Msg_C_UEFI - Global_Variables_Pool
_Msg_C_Shell			= Msg_C_Shell - Global_Variables_Pool
_Msg_C_CAP			= Msg_C_CAP - Global_Variables_Pool
_Msg_C_NCPU			= Msg_C_NCPU - Global_Variables_Pool
_Msg_A_Mem			= Msg_A_Mem - Global_Variables_Pool
_Msg_L_Cfg			= Msg_L_Cfg - Global_Variables_Pool
_Msg_C_Video			= Msg_C_Video - Global_Variables_Pool
_Msg_C_VMode			= Msg_C_VMode - Global_Variables_Pool
_Msg_C_ACPI			= Msg_C_ACPI - Global_Variables_Pool
_Msg_C_PCI			= Msg_C_PCI - Global_Variables_Pool
_Msg_C_VGA			= Msg_C_VGA - Global_Variables_Pool
_Msg_C_VBAR			= Msg_C_VBAR - Global_Variables_Pool
_Msg_C_AGP			= Msg_C_AGP - Global_Variables_Pool
_Msg_C_PCIe			= Msg_C_PCIe - Global_Variables_Pool
_Msg_C_Range			= Msg_C_Range - Global_Variables_Pool
_Msg_S_AGP			= Msg_S_AGP - Global_Variables_Pool
_Msg_S_PCIe			= Msg_S_PCIe - Global_Variables_Pool
_Msg_S_Range			= Msg_S_Range - Global_Variables_Pool
_Msg_C_Mouse			= Msg_C_Mouse - Global_Variables_Pool
_Msg_M_Clock			= Msg_M_Clock - Global_Variables_Pool
_Msg_S_Wait			= Msg_S_Wait - Global_Variables_Pool
_Msg_S_Vmode			= Msg_S_Vmode - Global_Variables_Pool

;--- Stop phase messages ---

_Msg_R_Vmode			= Msg_R_Vmode - Global_Variables_Pool
_Msg_R_Range			= Msg_R_Range - Global_Variables_Pool
_Msg_R_PCIe			= Msg_R_PCIe - Global_Variables_Pool
_Msg_R_AGP			= Msg_R_AGP - Global_Variables_Pool
_Msg_S_Cfg			= Msg_S_Cfg - Global_Variables_Pool
_Msg_R_Smem			= Msg_R_Smem - Global_Variables_Pool

;--- Errors ---

_Msg_CE_Arch			= Msg_CE_Arch - Global_Variables_Pool
_Msg_CE_UEFI			= Msg_CE_UEFI - Global_Variables_Pool
_Msg_CE_Shell			= Msg_CE_Shell - Global_Variables_Pool
_Msg_CE_CAP			= Msg_CE_CAP - Global_Variables_Pool
_Msg_CE_NCPU			= Msg_CE_NCPU - Global_Variables_Pool
_Msg_AE_Mem			= Msg_AE_Mem - Global_Variables_Pool
_Msg_LE_Cfg			= Msg_LE_Cfg - Global_Variables_Pool
_Msg_CE_Video			= Msg_CE_Video - Global_Variables_Pool
_Msg_CE_VMode			= Msg_CE_VMode - Global_Variables_Pool
_Msg_CE_ACPI			= Msg_CE_ACPI - Global_Variables_Pool
_Msg_CE_PCI			= Msg_CE_PCI - Global_Variables_Pool
_Msg_CE_VGA			= Msg_CE_VGA - Global_Variables_Pool
_Msg_CE_VBAR			= Msg_CE_VBAR - Global_Variables_Pool
_Msg_CE_AGP			= Msg_CE_AGP - Global_Variables_Pool
_Msg_CE_PCIe			= Msg_CE_PCIe - Global_Variables_Pool
_Msg_CE_Range			= Msg_CE_Range - Global_Variables_Pool
_Msg_SE_PCIe			= Msg_SE_PCIe - Global_Variables_Pool
_Msg_SE_AGP			= Msg_SE_AGP - Global_Variables_Pool
_Msg_SE_Range			= Msg_SE_Range - Global_Variables_Pool
_Msg_CE_Mouse			= Msg_CE_Mouse - Global_Variables_Pool
_Msg_ME_Clock			= Msg_ME_Clock - Global_Variables_Pool
_Msg_SE_Wait			= Msg_SE_Wait - Global_Variables_Pool
_Msg_SE_Vmode			= Msg_SE_Vmode - Global_Variables_Pool
_Msg_RE_Vmode			= Msg_RE_Vmode - Global_Variables_Pool
_Msg_RE_Range			= Msg_RE_Range - Global_Variables_Pool
_Msg_RE_PCIe			= Msg_RE_PCIe - Global_Variables_Pool
_Msg_RE_AGP			= Msg_RE_AGP - Global_Variables_Pool
_Msg_SE_Cfg			= Msg_SE_Cfg - Global_Variables_Pool
_Msg_RE_Smem			= Msg_RE_Smem - Global_Variables_Pool

;--- Additional strings ---

_Msg_Addend			= Msg_Addend - Global_Variables_Pool
_Msg_E_Line			= Msg_E_Line - Global_Variables_Pool
_Msg_E_Line_1			= Msg_E_Line_1 - Global_Variables_Pool
_Msg_E_File			= Msg_E_File - Global_Variables_Pool
_Msg_Q_Save			= Msg_Q_Save - Global_Variables_Pool
_Msg_Y_Save			= Msg_Y_Save - Global_Variables_Pool
_Msg_N_Save			= Msg_N_Save - Global_Variables_Pool

;--- Sequences ---

_Start_Program_Sequence		= Start_Program_Sequence - Global_Variables_Pool
_Stop_Program_Sequence		= Stop_Program_Sequence - Global_Variables_Pool

;---------- Control table for run sequence of handlers: start program ------;
; Note 1. 
; Unlike x64 edition, SSE context initialized before run SSE benchmarks,
; restored after benchmarks, but not in the start phase. It supported only
; if native IA32 or x64 CPU detected by EBC Code, during common
; initialization, usage of native gates must be minimal.
; Note 2. 
; Setup MTRR moved to SETVMODE.INC, execute after make  GOP parameters valid.
; Note 3. 
; Native CPU clock measurement and mouse detect located at end
; of list, because big delay possible at this steps, it make better
; view of messages.
;---

Start_Program_Sequence:
S   001h , NC , _Check_CPU_Architecture    , _Msg_C_Arch  , _Msg_CE_Arch
S   002h , NC , _Check_UEFI_Configuration  , _Msg_C_UEFI  , _Msg_CE_UEFI
S   003h , NC , _Check_Shell_Configuration , _Msg_C_Shell , _Msg_CE_Shell
S   004h , NC , _Check_CAP                 , _Msg_C_CAP   , _Msg_CE_CAP
S   005h , 01 , _Check_Native_CPU          , _Msg_C_NCPU  , _Msg_CE_NCPU
S   006h , NC , _Allocate_Memory           , _Msg_A_Mem   , _Msg_AE_Mem
S   007h , 13 , _Load_Config               , _Msg_L_Cfg   , _Msg_LE_Cfg
S   008h , NC , _Check_GOP_Video           , _Msg_C_Video , _Msg_CE_Video
S   009h , NC , _Check_Video_Mode          , _Msg_C_VMode , _Msg_CE_VMode
S   00Ah , NC , _Check_ACPI                , _Msg_C_ACPI  , _Msg_CE_ACPI	; This unlocked at v0.24
S   00Bh , NC , _Check_PCI_Bus             , _Msg_C_PCI   , _Msg_CE_PCI
S   00Ch , 04 , _Check_Active_Video        , _Msg_C_VGA   , _Msg_CE_VGA
S   00Dh , 05 , _Check_Video_BAR           , _Msg_C_VBAR  , _Msg_CE_VBAR
;S   00Eh , 06 , _Check_AGP                 , _Msg_C_AGP   , _Msg_CE_AGP
S   00Fh , 07 , _Check_PCIe                , _Msg_C_PCIe  , _Msg_CE_PCIe	; This unlocked at v0.21 (OK)
S   010h , 08 , _Check_Video_Range         , _Msg_C_Range , _Msg_CE_Range
;S   011h , 09 , _Setup_AGP                 , _Msg_S_AGP   , _Msg_SE_AGP
;S   012h , 10 , _Setup_PCIe                , _Msg_S_PCIe  , _Msg_SE_PCIe
;S   013h , 11 , _Setup_Video_Range         , _Msg_S_Range , _Msg_SE_Range	; This planned unlocked at v0.22 (OK, make call)
;S   01Ch , 12 , _Check_Mouse               , _Msg_C_Mouse , _Msg_CE_Mouse
S   01Dh , 02 , _Measure_Clock             , _Msg_M_Clock , _Msg_ME_Clock
S   01Eh , 31 , _Setup_Wait                , _Msg_S_Wait  , _Msg_SE_Wait
SS  01Fh , NC , _Setup_Video_Mode          , _Msg_S_Vmode , _Msg_SE_Vmode
STOPS

;---------- Control table for run sequence of handlers: stop program -------;

Stop_Program_Sequence:
SG  060h , 15 , _Restore_Video_Mode        , _Msg_R_Vmode , _Msg_RE_Vmode
S   061h , 11 , _Restore_Video_Range       , _Msg_R_Range , _Msg_RE_Range
;S   062h , 10 , _Restore_Video_PCIe        , _Msg_R_PCIe  , _Msg_RE_PCIe
;S   063h , 09 , _Restore_Video_AGP         , _Msg_R_AGP   , _Msg_RE_AGP
S   064h , 14 , _Save_Configuration        , _Msg_S_Cfg   , _Msg_SE_Cfg
S   065h , 03 , _Release_Memory            , _Msg_R_Smem  , _Msg_RE_Smem
STOPS

;---------- Text messages --------------------------------------------------;

;--- Start phase messages ---

Msg_C_Arch	DB  'Check hardware type...',0
Msg_C_UEFI	DB  'Check UEFI firmware configuration...',0
Msg_C_Shell	DB  'Analysing UEFI environment...',0
Msg_C_CAP	DB  'Check CPU Architectural Protocol...',0
Msg_C_NCPU	DB  'Identify native processor...',0
Msg_A_Mem	DB  'Allocate memory buffer...',0
Msg_L_Cfg	DB  'Load configuration file...',0
Msg_C_Video	DB  'Check video configuration...',0
Msg_C_VMode	DB  'Check video modes list...',0
Msg_C_ACPI	DB  'Check ACPI...',0
Msg_C_PCI	DB  'Check PCI bus...',0
Msg_C_VGA	DB  'Check legacy PC resources...',0
Msg_C_VBAR	DB  'Check PCI PnP video memory...',0
Msg_C_AGP	DB  'Check AGP capabilities...',0
Msg_C_PCIe	DB  'Check PCI Express capabilities...',0
Msg_C_Range	DB  'Check MMIO attribute capabilities...',0
Msg_S_AGP	DB  'Programming AGP fast write mode...',0
Msg_S_PCIe	DB  'Programming PCIe TLP size...',0
Msg_S_Range	DB  'Programming video memory attributes...',0
Msg_C_Mouse	DB  'Analysing input devices...',0
Msg_M_Clock	DB  'Measure native CPU clock...',0
Msg_S_Wait	DB  0Dh,0Ah,'Press any key to continue start. N=No wait...',0
Msg_S_Vmode	DB  'Set graphics video mode...',0

;--- Stop phase messages ---

Msg_R_Vmode	DB  'Continue in the firmware original video mode...',0
Msg_R_Range	DB  'Restore attributes for video memory range...',0
Msg_R_PCIe	DB  'Restore PCI Express settings...',0
Msg_R_AGP	DB  'Restore AGP settings...',0
Msg_S_Cfg	DB  'Analyse configuration updates...',0
Msg_R_Smem	DB  'Release system memory...',0

;--- Errors ---

Msg_CE_Arch	DB  'Unknown firmware implementation',0
Msg_CE_UEFI	DB  'Inconsistent firmware parameters',0
Msg_CE_Shell	DB  'Inconsistent UEFI shell parameters',0
Msg_CE_CAP	DB  'CAP access failed',0
Msg_CE_NCPU	DB  'Native processor detection failed',0
Msg_AE_Mem	DB  'Memory allocation failed',0
Msg_LE_Cfg	DB  'Load configuration failed',0
Msg_CE_Video	DB  'GOP access failed',0
Msg_CE_VMode	DB  'GOP video modes list inconsistent',0
Msg_CE_ACPI	DB  'ACPI failed',0
Msg_CE_PCI	DB  'PCI bus detection failed',0
Msg_CE_VGA	DB  'Legacy PC hardware failed',0
Msg_CE_VBAR	DB  'Video memory PCI BAR failed',0
Msg_CE_AGP	DB  'AGP capabilities failed',0
Msg_CE_PCIe	DB  'PCI Express capabilities failed',0
Msg_CE_Range	DB  'MMIO attribute capabilities failed',0
Msg_CE_Mouse	DB  'Mouse detection failed',0
Msg_ME_Clock	DB  'Native CPU clock measurement failed',0
Msg_SE_Wait	DB  '?',0
Msg_SE_Vmode:	; alias
Msg_RE_Vmode	DB  'Set video mode failed',0
Msg_SE_Range:	; alias
Msg_RE_Range	DB  'Set video memory range attribute failed',0
Msg_SE_PCIe:	; alias
Msg_RE_PCIe	DB  'PCI express capabilities programming failed',0
Msg_SE_AGP:	; alias
Msg_RE_AGP	DB  'AGP capabilities programming failed',0
Msg_SE_Cfg	DB  'File I/O error',0
Msg_RE_Smem	DB  'Memory de-allocation failed',0

;--- Additional string ---

Msg_Addend	DB  ', status = ',0

;--- Errors, associated with load/save configuration file ---

;--- Non-fatal errors messages (not cause program exit) ---

Msg_E_Line	DB  'Error in the configuration file, line # '
Msg_E_Line_1	DB  5 DUP (20h),0
Msg_E_File	DB  0Dh,0Ah,'File operations error.',0Dh,0Ah,07h,0

;--- Dialogue questions messages ---

Msg_Q_Save	DB  0Dh, 0Ah, 0Ah, 'Save configuration (y/n) ? : ',0
Msg_Y_Save	DB  0Dh, 0Ah, 'Saving...', 0
Msg_N_Save	DB  0Dh, 0Ah, 'Skipping...', 0


