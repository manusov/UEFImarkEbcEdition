;--- Built string: Program name, copyright and http ------------------------;
; This strings stored as text constants.                                    ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R2 = Source sequence pointer, absolute 64-bit address            ;
;          R3 = Destination string pointer, absolute 64-bit address         ;
;          R4 = Current Y position offset at bits [15-00], other bits = 0   ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  Status not used, because this is only string built handler       ;
;          R2 = Current Y position offset addend                            ;
;          R3 = Updated destination string pointer, absolute 64-bit address ;
;               String located at [input R3], string is nul-terminated,     ;
;               but sequence termination executed by caller before show     ;
;---------------------------------------------------------------------------;

Info_About:
;--- Built name + copyringt entry ---
		MOVIQW		R2,_String_Version	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00F0F0F000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Built http entry ---
		MOVIQW		R2,_Program_Vendor_Data	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,0000F00000000180h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Set Y-addend ---
		MOVIQW		R2,32
;--- Return ---
		RET







