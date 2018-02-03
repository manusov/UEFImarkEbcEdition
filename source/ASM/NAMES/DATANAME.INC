;===========================================================================;
;=                                                                         =;
;=                       PROGRAM AND VENDOR STRINGS.                       =;
;=                                                                         =;
;===========================================================================;

;---------- Equations for data access --------------------------------------;

_String_Version		= String_Version - Global_Variables_Pool
_String_Copyright	= String_Copyright - Global_Variables_Pool
_Program_Vendor_Data_1	= Program_Vendor_Data_1 - Global_Variables_Pool
_Program_Vendor_Data	= Program_Vendor_Data - Global_Variables_Pool

;---------- Data -----------------------------------------------------------;

; This 2 strings must be sequental, without separation by 0.

String_Version		DB  'UEFImark EBC Edition v0.26. '
String_Copyright	DB  '(C) IC Book Labs.',0

; This string X position must be conform with previous string length.
Program_Vendor_Data_1	DB  '  '
Program_Vendor_Data	DB  'http://jelezo.com.ua/' , 0

