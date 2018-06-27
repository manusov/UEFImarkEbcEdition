;--- Check input status and get pressed key, without wait key -----------;
; Use Simple Text Input Protocol.                                        ;
; This procedure return also if no keys pressed, check only.             ;
; Key code cleared from buffer, unlike BIOS INT 16h function 1.          ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;                                                                        ;
; OUTPUT:  R7 = UEFI Status, 64-bit or sign-extended 32 to 64 bit        ;
;          R2 = EFI_KEY, valid if Status=No errors                       ;
;          R2 bits [63-32] = 0, cleared                                  ;
;------------------------------------------------------------------------;

EBC_Input_Check_Key:
		MOVNW		R2,@R1,0,_EFI_Table	; R3=SysTable
		MOVNW		R2,@R2,3,24		; R3=ConIn
		PUSHN		R2		; Storage for key code
		PUSHN		R0 	    	; Parameter #2 for protocol
		PUSHN		R2 	    	; Parameter #1 for protocol
		CALL32EXA	@R2,1,0
		POPN		R2		; Remove parameter #1
		POPN		R2		; Remove parameter #2
		POPN		R2		; Read key code
		EXTNDD64	R7,R7
		RET
