;--- Check input status and get pressed key, with wait key --------------;
; Use Simple Text Input Protocol.                                        ;
; This procedure not return if no keys pressed, wait for key press.      ;
; Key code cleared from buffer, like BIOS INT 16h function 0.            ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;                                                                        ;
; OUTPUT:  Condition Flag = Status:                                      ;
;          0 = No errors, key ready, 1 = Error (reserved)                ;
;          R7 = UEFI Status                                              ;
;          R2 = EFI_KEY, valid if Status=No errors                       ;
;          R2 bits [63-32] = 0, cleared                                  ;
;------------------------------------------------------------------------;

EBC_Input_Wait_Key:
EBC_IWK_0:	CALL32		EBC_Input_Check_Key
		CMPI32WEQ	R7,0			; 0 if key ready
		JMP8CC		EBC_IWK_0		; Wait if no keys
		RET
