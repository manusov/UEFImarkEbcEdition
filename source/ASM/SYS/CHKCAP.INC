;--- Initialization routine handler: Detect CPU Architectural Protocol -----;
; Use function Locate_Protocol for CPU Architectural Protocol (CAP),        ;
; if detected, get and save info.                                           ;
; Later, in benchmarks, CAP timers used for hardware-abstracted             ;
; time measurement.                                                         ;
; Yet used only timer index #0.                                             ;
;                                                                           ;
; INPUT:   R1 = Global variables pool base address                          ;
;          R6 = pre-initialized to 0000000000000001h for compact opcodes    ;
;          R7 = pre-initialized to 0000000000000000h for compact opcodes    ;
;                                                                           ;
; OUTPUT:  R7 = UEFI Status, include errors and warnings codes, and:        ;
;               0 = No errors, 00FFh = Not detalize as UEFI status          ;
;               For compact coding:                                         ;
;               R7 convert to 64-bit unified status by caller, not here.    ;
;               Set condition flag = F(R7) by caller, not here.             ;
;                                                                           ;
;               Global variables:                                           ;
;               DWORD  Info_Bitmap, bit 8 = CAP clock data valid            ;
;               QWORD  CAP_Period  = CAP timer period, femtoseconds         ;
;---------------------------------------------------------------------------;

Check_CAP:
;--- Detect protocol, save pointer ---
		MOVIQW		R2,_GuidCap
		CALL32		Locate_Protocol
		JMP8CS		CCAP_0			; Go if error: CAP not detected, R7=Status
		MOVQW		@R1,0,_CAP_Protocol,R2	; Save CAP interface pointer
;--- Read CAP timer ---
		CALL32		Read_CAP_Timer
		JMP8CS		CCAP_0			; Go if error: CAP timer read error, R7=Status
;--- Save CAP result, mark CAP result valid
		MOVQW		@R1,0,_CAP_Period,R6
		MOVIQQ		R6,10000000000h		; Bit D40=1, this bit is Info_Bitmap.8
;--- This point used from some other routines ---
CCAP_1:		CALL32		CUC_3			; This also return R7=0
;--- Exit, here valid R7=Status ---
CCAP_0:		RET

