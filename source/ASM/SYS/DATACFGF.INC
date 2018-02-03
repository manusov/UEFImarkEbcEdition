;===========================================================================;
;                                                                           ;
;          TABLES FOR CREATE AND INTERPRETING CONFIGURATION FILE.           ;
;                                                                           ;
;===========================================================================;

;---------- Equations for data access --------------------------------------;
; For compact coding, use 16-bit offsets relative Global_Variables_Pool
; instead full 64-bit flat addresses. This method also eliminate relocation
; elements with some bugs in the old IA32 EFI implementations.

;--- Data ---
_CfgChar_00		= CfgChar_00 - Global_Variables_Pool
_CfgChar_01		= CfgChar_01 - Global_Variables_Pool
_CfgChar_02		= CfgChar_02 - Global_Variables_Pool
_CfgChar_03		= CfgChar_03 - Global_Variables_Pool
_CfgChar_04		= CfgChar_04 - Global_Variables_Pool
_Cfg_File_Name		= Cfg_File_Name - Global_Variables_Pool
_Cfg_File_Ext		= Cfg_File_Ext - Global_Variables_Pool
_Bak_File_Ext		= Bak_File_Ext - Global_Variables_Pool
_Cfg_File_Header	= Cfg_File_Header - Global_Variables_Pool
_Cfg_File_Header_Stop	= Cfg_File_Header_Stop - Global_Variables_Pool
_CfgStr_00		= CfgStr_00 - Global_Variables_Pool
_CfgStr_01		= CfgStr_01 - Global_Variables_Pool
_CfgStr_02		= CfgStr_02 - Global_Variables_Pool
_Keywords_List		= Keywords_List - Global_Variables_Pool
_Keywords_List_1	= Keywords_List_1 - Global_Variables_Pool

;--- Code ---
_HL_Space		= HL_Space - Global_Variables_Pool
_HL_Tab			= HL_Tab - Global_Variables_Pool
_HL_LF			= HL_LF - Global_Variables_Pool
_HL_CR			= HL_CR - Global_Variables_Pool
_HL_Comment		= HL_Comment - Global_Variables_Pool
_HL_Xsize		= HL_Xsize - Global_Variables_Pool
_HL_Ysize		= HL_Ysize - Global_Variables_Pool
_HL_Pause		= HL_Pause - Global_Variables_Pool
_HS_Space		= HS_Space - Global_Variables_Pool
_HS_Tab			= HS_Tab - Global_Variables_Pool
_HS_LF			= HS_LF - Global_Variables_Pool
_HS_CR			= HS_CR - Global_Variables_Pool
_HS_Comment		= HS_Comment - Global_Variables_Pool
_HS_Xsize		= HS_Xsize - Global_Variables_Pool
_HS_Ysize		= HS_Ysize - Global_Variables_Pool
_HS_Pause		= HS_Pause - Global_Variables_Pool

;---------- Data -----------------------------------------------------------;

;--- Common block ---
; CFGL.X1=String or char (data pointer is R1-relative offset, 16-bit)
;--- Loader block (used by LOADCFG.INC) ---
; CFGL.X2=Handler for loader (code pointer is R1-relative offset, 16-bit )
; CFGL.X3=Offset of parameter value in the Global_Variables_Pool
; CFGL.X4=Parameter width, bits: 8=Byte, 16=Word, 32=Dword, 64=Qword,
;         0=Entry not used, other values reserved
;         Qword mode reserved, 64-bit parameters not supported yet
;         for configuration file
;         Reserved functionality for bytes X4, X5:
;         parameter offset can be 14-bit, parameter width select bits
;         can be at D[15-14], this expands offsets from
;         [-128...+127] to [-16384...+16383].
;--- Saver block (used by SAVECFG.INC) ---
; CFGS.X1=Handler for saver (code pointer is R1-relative offset, 16-bit)
; CFGS.X2=Offset of parameter value, see comments for CFGL.X3
; CFGS.X3=Parameter width, see comments for CFGL.X4
;---

;--- Keywords (size, chars) ---
; Used for key words comparing and write configuration file

;--- Chars (size=1, char) ---
CfgChar_00	DB  ' '  , 0		; Space
CfgChar_01	DB  9    , 0		; Tab
CfgChar_02	DB  10   , 0		; Line feed
CfgChar_03	DB  13   , 0		; Carriage return
CfgChar_04	DB  ';'  , 0		; Comments strings

;--- Numbers 0-9 ---
; Reserved, handled by code, no data

;--- List for chars and keywords handling ---
; "HL" means handler for loader
; "HS" means handler for saver
; Start chars and keywords list, entire list

;--- Configuration options names ---

;--- Keywords (size, chars) ---
; Used for key words comparing and write config. file
CfgStr_00	DB  'Xsize'   , 0	; Video mode X size, pixels
CfgStr_01	DB  'Ysize'   , 0	; Video mode Y size, pixels
CfgStr_02	DB  'Pause'   , 0	; Pause during program start

;--- Common and Loader blocks, 18 bytes ---
MACRO  CFGL  x1, x2, x3, x4
{
DW  x1, x2, x3
DB  x4
}

;--- Saver block, 10 bytes ---
MACRO  CFGS  x1, x2, x3
{
DW  x1, x2
DB  x3
}

;--- List Terminator ---
MACRO  CFGSTOP
{
DW  0
}

;--- List for chars and keywords handling ---
; "HL" means handler for loader
; "HS" means handler for saver
; Start chars and keywords list, entire list

Keywords_List:
CFGL  _CfgChar_00 , _HL_Space   , 0 , 0
CFGS                _HS_Space   , 0 , 0
CFGL  _CfgChar_01 , _HL_Tab     , 0 , 0
CFGS                _HS_Tab     , 0 , 0
CFGL  _CfgChar_02 , _HL_LF      , 0 , 0
CFGS                _HS_LF      , 0 , 0
CFGL  _CfgChar_03 , _HL_CR      , 0 , 0
CFGS                _HS_CR      , 0 , 0
CFGL  _CfgChar_04 , _HL_Comment , 0 , 0
CFGS                _HS_Comment , 0 , 0
;--- Start keywords list, used also for parameters update, part of list ---
Keywords_List_1:
CFGL  _CfgStr_00  , _HL_Xsize   ,  _Video_Xsize  , 16
CFGS                _HS_Xsize   ,  _Video_Xsize  , 16
CFGL  _CfgStr_01  , _HL_Ysize   ,  _Video_Ysize  , 16
CFGS                _HS_Ysize   ,  _Video_Ysize  , 16
CFGL  _CfgStr_02  , _HL_Pause   ,  _Start_Pause  , 8
CFGS                _HS_Pause   ,  _Start_Pause  , 8
CFGSTOP

;--- Configuration file support ---

Cfg_File_Name  DB  'UEFIMARK.'
Cfg_File_Ext   DB  'CFG',0
Bak_File_Ext   DB  'BAK',0

; Note need write prefix = String_Version.
Cfg_File_Header:
DB  '; Program configuration file, must be not larger 60KB.',13,10
DB  '; Comments started with ";" char.',13,10
DB  '; Xsize = Video mode horizontal resolution, pixels, 640-2048.',13,10
DB  '; Ysize = Video mode vertical resolution, pixels, 400-2048.',13,10
DB  '; Pause = Pause during program start for messages view.',13,10
Cfg_File_Header_Stop:
DB  0

