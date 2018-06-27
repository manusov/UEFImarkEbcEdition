;--- Video Block Transfer (BLT) operation: copy block -------------------;
; This BLT operation is copy block from System RAM to Video RAM,         ;
; (EfiBltBufferToVideo) note API functionality support some other modes. ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Base X coordinate, for up/left point                     ;
;          R3 = Base Y coordinate, for down/right point                  ;
;          R4 = X size (pixels), width                                   ;
;          R5 = Y size (pixels), height                                  ;
;          R6 = Pointer to transit buffer                                ;
;          R7 = Delta, string size in the transit buffer                 ;
;                                                                        ;
; OUTPUT:  Condition flag: 0(CC)=Successfully, 1(CS)=Failed              ;
;          Note output R7 not a status, contain input R7=Delta parameter ;
;------------------------------------------------------------------------;
EBC_GOP_BLT_Copy:
;--- Built stack frame for function ---
		PUSHN		R7			; Parm#10 = Delta = Input
		PUSHN		R5			; Parm#9  = Height = Input
		PUSHN		R4			; Parm#8  = Width = Input
		PUSHN		R3			; Parm#7  = Destination Y = Input
		PUSHN		R2			; Parm#6  = Destination X = Input
		XOR64		R7,R7
		PUSHN		R7			; Parm#5  = Source Y = 0
		PUSHN		R7			; Parm#4  = Source X = 0
		MOVIQW		R7,2			; EfiBltBufferToVideo = 2
		PUSHN		R7			; Parm#3  = BLT Operation = 2
		PUSHN		R6			; Parm#2  = BLT Buffer Pointer
		MOVNW		R7,@R1,0,_GOP_Protocol
		PUSHN		R7			; Parm#1  = Protocol Pointer
;--- Call function ---
		CALL32EXA	@R7,2,0			; GopTable.Blt
;--- Analuuse status at R7 ---
		MOVSNW		R7,R7			; Determine D[63-32]
		CMPI64WUGTE	R7,1			; Check status, set CS if R7>0
;--- Restore parameters after built stack frame for function ---
		POPN		R7			; Remove only Parm#1
		POPN		R6			; Restore Parm#2
		POPN		R7			; Remove only Parm#3
		POPN		R7			; Remove only Parm#4
		POPN		R7			; Remove only Parm#5
		POPN		R2			; Restore Parm#6
		POPN		R3			; Restore Parm#7
		POPN		R4			; Restore Parm#8
		POPN		R5			; Restore Parm#9
		POPN		R7			; Restore Parm#10
;--- Return ---
		RET

