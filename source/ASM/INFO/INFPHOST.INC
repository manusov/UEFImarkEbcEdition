;--- Built string: PCIe Host features: Link Width, Speed, TLP size ---------;
; This parameters read from PCIe Host during check config. phase.           ;
; TLP parameter can be re-programming during set context phase.             ;
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

Info_PCIe_Video_Host_Parameters:

;------------------------------------------------------------------------------ Patch v0.21 ---
;
;		XOR64		R2,R2			; Unlocked at v0.21
;;--- Check bridge presence ---
;		MOVBW		R7,@R1,0,_PCIe_H_Reg
;		CMPI32WEQ	R7,0
;		JMP8CS		Inf_PCIe_20		; Go if PCIe bridge absent
;;--- Write left part of string ---
;		
;	;*	MOVIQW		R2,_String_PCIe_B
;		MOVSNW		R2,R1,_String_PCIe_B	; Patch v0.21
;		
;		CALL32		Copy_String
;;--- Write parameters for host ---
;		MOVQW		R4,@R1,0,_Host_Cfg	; Bus/Device/Function
;		MOVBW		R7,@R1,0,_PCIe_H_Reg	; Register
;		ADD64		R4,R7
;		CALL32		Local_Inf_PCIe_1
;;--- Exit ---
;		MOVIQW		R2,16
;Inf_PCIe_20:	RET
;
;------------------------------------------------------------------------------


;--- Check bridge presence ---
		XOR64		R2,R2
		MOVBW		R7,@R1,0,_PCIe_H_Reg
		CMPI32WEQ	R7,0
		JMP8CS		Inf_PCIe_20		; Go if PCIe bridge absent
;--- Built left entry, parameter name ---
		MOVIQW		R2,_String_G_Bus	; R2=Source pointer, signed offset relative R1
		MOVIQQ		R5,00C0C0C000000000h	; ReservedColor8.Red8.Green8.Blue8.Yadd16.Xadd16
		CALL32		Built_Info_Entry
;--- Built right entry, parameter value ---
		PUSH64		R3
		PUSH64		R4
		MOVSNW		R3,R1,Scratch_Pad_Size-128	; R3=Pointer for built string
		MOVSNW		R2,R1,_String_PCIe_B		; R2=Pointer to parameter name string, B=Bridge
		CALL32		Copy_String
;--- Variable part for host and device, this=host ---
		MOVQW		R4,@R1,0,_Host_Cfg	; Bus/Device/Function
		MOVBW		R7,@R1,0,_PCIe_H_Reg	; Register
		ADD64		R4,R7
		CALL32		Local_Inf_PCIe_1	; Built string for PCIe parameters: Link width, Link speed
		POP64		R4
		POP64		R3
;--- Exit points ---
		JMP32		IUSR_2
Inf_PCIe_20:	RET
