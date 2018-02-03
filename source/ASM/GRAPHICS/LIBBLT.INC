;===========================================================================;
;                                                                           ;
;                   LIBRARY FOR USE GOP.BLT FUNCTIONS.                      ;
;                                                                           ;
;===========================================================================;

;--- Video Block Transfer (BLT) operation: fill block -------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R2 = Base X coordinate, for up/left point                     ;
;          R3 = Base Y coordinate, for down/right point                  ;
;          R4 = X size (pixels), width                                   ;
;          R5 = Y size (pixels), height                                  ;
;          R6 = True color:                                              ;
;               D[63-32] = Not used                                      ;
;               D[31-24] = Reserved                                      ;
;               D[23-16] = Red color                                     ;
;               D[15-08] = Green color                                   ;
;               D[07-00] = Blue color                                    ;
;                                                                        ;
; OUTPUT:  Condition flag: 0(CC)=Successfully, 1(CS)=Failed              ;
;          R7 = UEFI Status, 64-bit or sign-extended 32 to 64 bit        ;
;------------------------------------------------------------------------;
GOP_BLT_Fill:
;--- Prepare parameters ---
		XOR64		R7,R7			; R7 = 0
		PUSH64		R6			; This for stack alignment
		PUSH64		R6			; Color pattern
;--- Built stack frame for function ---
		PUSHN		R7			; Parm#10 = Delta = 0
		PUSHN		R5			; Parm#9  = Height = Input
		PUSHN		R4			; Parm#8  = Width = Input
		PUSHN		R3			; Parm#7  = Destination Y = Input
		PUSHN		R2			; Parm#6  = Destination X = Input
		PUSHN		R7			; Parm#5  = Source Y = 0
		PUSHN		R7			; Parm#4  = Source X = 0
		PUSHN		R7			; Parm#3  = BLT Operation = Fill = 0
		MOVNW		R7,R0,8,0		; R7 = Pointer to color pattern
		PUSHN		R7			; Parm#2  = BLT Buffer Pointer
		MOVNW		R7,@R1,0,_GOP_Protocol
		PUSHN		R7			; Parm#1  = Protocol Pointer
;--- Call function ---
		CALL32EXA	@R7,2,0			; GopTable.Blt
;--- Restore parameters after built stack frame for function ---
		POPN		R2			; Remove only Parm#1
		POPN		R2			; Remove only Parm#2
		POPN		R2			; Remove only Parm#3
		POPN		R2			; Remove only Parm#4
		POPN		R2			; Remove only Parm#5
		POPN		R2			; Restore Parm#6
		POPN		R3			; Restore Parm#7
		POPN		R4			; Restore Parm#8
		POPN		R5			; Restore Parm#9
		POPN		R6			; Remove only Parm#10
;--- Restore parameters after built transit frame ---
		POP64		R6			; This for stack alignment
		POP64		R6			; Restore color pattern
;--- Convert 32/64-bit results to 64-bit, analyse status and exit ---
		MOVSNW		R7,R7		; Determine D[63-32]
		CMPI64WUGTE	R7,1		; Check status, set CS if R7>0
;--- Return, here Condition Flag=F(R7)  ---
		RET


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
GOP_BLT_Copy:
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
;--- Analuse status at R7 ---
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

