;===========================================================================;
;=                                                                         =;
;=               TEXT STRINGS AND TEXT OUTPUT CONTROL DATA.                =;
;=                                                                         =;
;===========================================================================;

;---------- Equations for data access --------------------------------------;

_Msg_Starting		= Msg_Starting - Global_Variables_Pool
_Msg_Exit		= Msg_Exit - Global_Variables_Pool
_Msg_CRLF_2		= Msg_CRLF_2 - Global_Variables_Pool
_Msg_CRLF		= Msg_CRLF - Global_Variables_Pool
_Msg_Error		= Msg_Error - Global_Variables_Pool
_Msg_Press		= Msg_Press - Global_Variables_Pool
_Msg_OK			= Msg_OK - Global_Variables_Pool
_Msg_FAILED		= Msg_FAILED - Global_Variables_Pool

;---------- Data -----------------------------------------------------------;

Msg_Starting	DB  0Dh,0Ah,'Starting...',0Dh,0Ah,0

;--- Special strings ---

Msg_Exit	DB  'Exit to UEFI.',0Dh,0Ah,0Ah,0
Msg_CRLF_2	DB  0Dh,0Ah
Msg_CRLF	DB  0Dh,0Ah,0
Msg_Error	DB  0Dh,0Ah,'ERROR: ',0
Msg_Press	DB  0Dh,0Ah,'Press any key...',0

Msg_OK		DB  'OK',0
Msg_FAILED	DB  'FAILED',0

