;===========================================================================;
;=                                                                         =;
;=    DATA BASE FOR SUPPORT FIRMWARE VERSION-SPECIFIC FEATURES AND BUGS.   =;
;=                                                                         =;
;===========================================================================;

;--- Firmware vendor template ---
; X1 = Firmware vendor string, ASCII, but compared with UNICODE16
; X2 = Pointer to firmware revisions data, 16-bit signed offset relative R1=Global_Variables_Pool

MACRO FWVEN x1, x2
{
DB x1 , 0
DW x2 - Global_Variables_Pool
}

;--- Terminator ---
MACRO TFWVEN
{
DB 0
}

;--- Firmware revision features/bugs list template ---
; X1 = Firmware revision dword from EFI System Table
; X2 = Firmware features and bugs list
; bits:
; X2.00 = System is x86-compatible, but IA32/x64 must be additional differentiate by MOVINW
; X2.[01-31] = Reserved for future use

MACRO FWREV x1, x2
{
DD x1 , x2
}

;--- Terminator ---
MACRO TFWREV
{
DD 0
}

;---------- Equations for data access --------------------------------------;
; For compact coding, use 16-bit offsets relative Global_Variables_Pool
; instead full 64-bit flat addresses. This method also eliminate relocation
; elements with some bugs in the old IA32 EFI implementations.

_Firmware_Vendors		= Firmware_Vendors - Global_Variables_Pool

;---------- Firmware vendors list ------------------------------------------;

Firmware_Vendors:
FWVEN  'EDK II'              , Firmware_EDK_II
FWVEN  'American Megatrends' , Firmware_American_Megatrends
TFWVEN

;---------- Firmware revisions list for vendors ----------------------------;

Firmware_EDK_II:
FWREV  00010000h , 00000000h
TFWREV

Firmware_American_Megatrends:
FWREV  00050008h , 00000001h
TFWREV

