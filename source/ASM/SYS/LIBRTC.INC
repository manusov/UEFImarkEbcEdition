;===========================================================================;
;                                                                           ;
;     SUBROUTINES FOR GET DATE AND TIME, AS NUMBERS AND AS TEXT STRINGS.    ;
;                                                                           ;
;===========================================================================;

;--- EFI Time structures, stack frame used, not declared as variables ---;
; EFI_Time_Value:
; EFI_Time_Year		DW  ?	; +0
; EFI_Time_Month	DB  ?	; +2
; EFI_Time_Day		DB  ?	; +3
; EFI_Time_Hour		DB  ?	; +4
; EFI_Time_Minute	DB  ?	; +5
; EFI_Time_Second	DB  ?	; +6
; EFI_Time_Pad1		DB  ?	; +7
; EFI_Time_Nanosecond	DD  ?	; +8
; EFI_Time_TimeZone	DW  ?	; +12
; EFI_Time_DayLight	DB  ?	; +14
; EFI_Time_Pad2		DB  ?	; +15
;------------------------------------------------------------------------;
; EFI_Time_Capabilities:
; EFI_TCap_Resolution	DD  ?	; +0
; EFI_TCap_Accuracy	DD  ?	; +4
; EFI_TCap_SetsToZero	DD  ?	; +8
;------------------------------------------------------------------------;

;---------- Get Date and Time, use EFI Runtime Services -----------------;
; INPUT:   R1 = Global variables pool base address                       ;
;                                                                        ;
; OUTPUT:  CF  = Error flag: 0(CC)=No errors, 1(CS)=Error                ;
;          R7 = EFI_STATUS                                               ;
;          Next parameters valid if no errors (R6 bit fields):           ;
;                                                                        ;
;          Parameters read from output (bytes 0-6 of qword):             ;
;          R6.[15-00] = Year                                             ;
;          R6.[23-16] = Month        , 1-12                              ;
;          R6.[31-24] = Day of month , 1-31                              ;
;          R6.[39-32] = Hours        , 0-23                              ;
;          R6.[47-40] = Minutes      , 0-59                              ;
;          R6.[55-48] = Seconds      , 0-59                              ;
;                                                                        ;
;          Additionally calculated parameters (byte 7 of qword):         ;
;          R6.[63-56] = Day of week  , 0=Sunday, ... 6=Saturday          ;
;                                                                        ;
; Invalid value encoding: For 8-bit = 0FFh, for 16-bit = 0FFFFh.         ;
;------------------------------------------------------------------------;
EFI_Get_Time:
;--- Save registers ---
		PUSH64		R1
		PUSH64		R2
		PUSH64		R3
		PUSH64		R4
		PUSH64		R5
;--- Built scratchs pad in the stack ---
		MOVQW		R2,@R1,0,_EFI_Table	; R2 = Pointer to EFI System Table
		MOVNW		R2,@R2,08,24		; R2 = Pointer to EFI Runtime Services Table, N=8, C=24
		XOR64		R3,R3			; R3 = 0
		PUSH64		R3
		PUSH64		R3			; Two push instructions blank 16-byte block EFI_TIME
		MOVQ		R4,R0			; R4 = Pointer to EFI_TIME output field (R0 is stack pointer)
		PUSH64		R3
		PUSH64		R3			; Two push instructions blank 16-byte block EFI_TCAP
		MOVQ		R3,R0			; R3 = Pointer to EFI_TCAP output field (R0 is stack pointer)
;--- Make parameters frame, function call, remove frame ---
; R3 = Pointer to structure EFI_Time_Capabilities in the stack frame
; R4 = Pointer to structure EFI_Time_Value in the stack frame
;---

		PUSHN		R3		; Added at v0.30, shift stack before EBC-to-native call, under debug

		PUSHN		R3		; Parameter #2 = Pointer to EFI_TCAP
		PUSHN		R4		; Parameter #1 = Pointer to EFI_TIME
		CALL32EXA	@R2,00,24	; call RuntimeServices.GetTime, N=0, C=24
		POPN		R4		; Remove parameter #1
		POPN		R3		; Remove parameter #2

		POPN		R3		; Added at v0.30, shift stack before EBC-to-native call, under debug

;--- Check status ---
EGT_1:		MOVSNW		R7,R7			; Unify for 32 and 64
		CMPI64WUGTE	R7,1			; Condition Flag = 1 if R7>0 (unsigned)
		JMP8CS		EGT_0
;--- Verify hours, minutes, seconds valitity ---
		MOVBW		R2,@R4,0,4
		CMPI32WULTE	R2,23
		JMP8CC		EGT_2			; Go if error, hours > 23
		MOVBW		R2,@R4,0,5
		CMPI32WULTE	R2,59
		JMP8CC		EGT_2			; Go if error, minutes > 59
		MOVBW		R2,@R4,0,6
		CMPI32WULTE	R2,59
		JMP8CC		EGT_2			; Go if error, seconds > 59
;--- Prepare for calculate day of week, use fact: in this point R7=0, use R7 as sum of days ---
		MOVW		R2,@R4			; R2 = year, word
		MOVBW		R3,@R4,0,2		; R3 = month, byte
		MOVBW		R5,@R4,0,3		; R5 = day of month, byte
		MOVIQW		R6,1980			; R6 = First valid year, 1980
		CMP32UGTE	R2,R3
		JMP8CC		EGT_2			; Go if error, year < 1980
		CMPI32WUGTE	R2,3001
		JMP8CS		EGT_2			; Go if error, year > 3000
		CMPI32WEQ	R3,0
		JMP8CS		EGT_2			; Go if error, month = 0
		CMPI32WULTE	R3,12
		JMP8CC		EGT_2			; Go if error, month > 12
		CMPI32WEQ	R5,0
		JMP8CS		EGT_2			; Go if error, day of month = 0
		CMPI32WULTE	R5,31
		JMP8CC		EGT_2			; Go if error, day of month > 31
;--- Start calculate cycle, update February days in the months-days table, here R7=0 ---
		PUSH64		R1
		PUSH64		R4
		MOVSNW		R1,R1,_January_Days	; This label = years calculation cycle
;--- Years cycle---
EGT_3:		MOVIBW		@R1,0,1,28
		PUSH64		R1
		MOVIBW		R1,0003h
		AND32		R1,R6			; Bits R1.[1,0]=00b if year with February 29
		CMPI32WEQ	R1,0			; CF=1 if February 29
		POP64		R1
		JMP8CC		EGT_4			; Go label after determine February 28/29 days
		MOVIBW		@R1,0,1,29
;--- Prepare for months cycle for given year, cycle for R6=Years ---
EGT_4:		MOVIQW		R4,1			; R4 = Months counter, 1-12
;--- This label for not this year+month, cycle for R4=months ---
EGT_5:		CMP32UGTE	R6,R2			; R6=Incremented from 1980, R2=This year
		JMP8CC		EGT_8			; Go if not this year
		CMP32UGTE	R4,R3			; Compare months is this year reached
		JMP8CS		EGT_6			; Go if year+month reached
;--- Add number of days per months ---
EGT_8:		PUSH64		R1			; This label for not this year+month
		ADD64		R1,R4,-1
		MOVB		R1,@R1			; R1 = Zero-extended addend, 8-bit
		ADD64		R7,R1			; R7 = Sum of days, 64-bit addition
		POP64		R1
;--- Next month ---
		MOVSNW		R4,R4,1			; R4+1, next month
		CMPI32WULTE	R4,12
		JMP8CS		EGT_5			; Cycle for next month
		MOVSNW		R6,R6,1			; R6+1, next year
		JMP8		EGT_3			; Cycle for next year
;--- This label for year+month reached by increments ---
EGT_6:		ADD64		R7,R5,-1		; Add last month, -1 because day of month 1-based
		MOVIQW		R1,7			; Days per week = 7
		MODU64		R7,R1			; R7 = Modulo, day of week
		MOVSNW		R7,R7,2			; R7+2, because 2=Tuesday, 01.01.1980 is Tuesday
		CMP32UGTE	R7,R1
		JMP8CC		EGT_7			; Go if not week days overflow
		SUB32		R7,R1			; Prevent week days overflow
EGT_7:		POP64		R4
		POP64		R1
;--- Update result (day of week) in the stack frame ---
		MOVBW		@R4,0,7,R7
;--- Clear status, clear CF, this branch for OK ---
		XOR64		R7,R7
		CMP32EQ		R6,R7
;--- Extract parameters from scratch pads, later used R6 only ---
; R4,R3 = EFI_TCAP , R4=low, R3=high
; R6,R5 = EFI_TIME , R6=low, R5=high
;---
EGT_0:		POP64		R4
		POP64		R3
		POP64		R6
		POP64		R5
;--- Restore registers and exit ---
EGT_9:		POP64		R5
		POP64		R4
		POP64		R3
		POP64		R2
		POP64		R1
		RET
;--- Exit branch for wrong date or time value ---
EGT_2:		MOVIQW		R7,00FFh		; R7 = Status, unknown error
		JMP8CS		EGT_1

;---------- Prepare date and time string for report file ----------------;
; This subroutine don't check time and data values validity before       ;
; write, because subroutine EFI_Get_Time verify it.                      ;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          R3 = Pointer to destination buffer                            ;
;                                                                        ;
; OUTPUT:  R3 = Modified by string write                                 ;
;          Scratch Pad Buffer at [input R2] contain string               ;
;          Maximum total string(s) size is 128 chars, possible CR, LF    ;
;          String is not nul-terminated, it make caller,                 ;
;          for flexibility if strings sequence required                  ;
;------------------------------------------------------------------------;
String_Date_Time:
;--- Save registers ---
		PUSH64		R2
		PUSH64		R4
		PUSH64		R5
		PUSH64		R6
		PUSH64		R7
;
;--- Blank string ---
;		MOVQ		R2,R3			; Make copy of destination base
;		XOR32		R4,R4			; R4 = Counter
;SDT_0:		MOVIBW		@R2,' '			; Write SPACE char
;		MOVSNW		R2,R2,1			; R2+1, modify buffer pointer
;		MOVSNW		R4,R4,1			; R4+1, modify counter
;		CMPI32WUGTE	R4,128
;		JMP8CC		SDT_0			; Cycle for blank buffer
;

;--- Get date and time by EFI API ---
		CALL32		EFI_Get_Time
		JMP8CS		SDT_1			; Go if error
		MOVQ		R5,R6			; R5 = Storage
;--- Write Month and ", " ---
		MOVQ		R7,R5			; R7 = Date and time qword
		MOVIQW		R2,16			; R2 = Shifts count for required parameter
		SHR64		R7,R2
		MOVB		R7,R7			; R7 = Selector, byte
		MOVIQW		R6,12			; R6 = Limit, maximum valid selector
		MOVSNW		R2,R1,_String_Months	; R2 = Pointer to strings pool
		CALL32		Copy_Selected_String	; String write at [R3]
		MOVIWW		@R3,', '
		MOVSNW		R2,R3,2			; +2 and change pointer = R2
;--- Write Day of month and "  " ---
		XOR64		R4,R4			; Template=0, no template
		MOVIQW		R6,24			; R6 = Shifts count for required parameter
		CALL32		Parm_String_Decimal8
		MOVIWW		@R2,'  '
		MOVSNW		R2,R2,2
;--- Write Year  and "  " ---
		MOVW		R3,R5			; R3 = Date and time qword, with mask D[15-0]
		CALL32		String_Decimal32	; Here R4=0 means no template
		MOVIWW		@R2,'  '
		MOVSNW		R3,R2,2			; +2 and change pointer = R3
;--- Write Week day name and "  " ---
		MOVQ		R7,R5			; R7 = Date and time qword
		MOVIQW		R2,56			; R2 = Shifts count for required parameter
		SHR64		R7,R2
		MOVB		R7,R7			; R7 = Selector, byte
		MOVSNW		R7,R7,1			; R7+1, 0=wrong, 1=Sunday...7=Saturday, 8=wrong
		MOVIQW		R6,8			; R6 = Limit, maximum valid selector
		MOVSNW		R2,R1,_String_Days	; R2 = Pointer to strings pool
		CALL32		Copy_Selected_String	; String write at [R3]
		MOVIWW		@R3,'  '
		MOVSNW		R2,R3,2			; +2 and change pointer = R2
;--- Write Hours and ":" ---
		MOVIQW		R6,32			; R6 = Shifts count for required parameter
		MOVIQW		R4,2			; Template=2
		CALL32		Parm_String_Decimal8
		MOVIBW		@R2,':'
		MOVSNW		R2,R2,1
;--- Write Minutes ---
		MOVIQW		R6,40			; R6 = Shifts count for required parameter
		CALL32		Parm_String_Decimal8
		MOVIBW		@R2,':'
		MOVSNW		R2,R2,1
;--- Write Seconds ---
		MOVIQW		R6,48			; R6 = Shifts count for required parameter
		CALL32		Parm_String_Decimal8
;--- Set output parameter R3=Modified pointer ---
		MOVQ		R3,R2			; Change pointer = R3
;--- Restore registers and exit ---
SDT_2:		POP64		R7
		POP64		R6
		POP64		R5
		POP64		R4
		POP64		R2
		RET
;--- Write string if error ---
SDT_1:		MOVIQW		R2,_String_Bad_Time_Date
		CALL32		Copy_String
		JMP8		SDT_2


;---------- Local subroutines ----------------------------------------------;

;--- Extract and write 8-bit parameter ---------;
; INPUT:   R2 = Destination pointer             ;
;          R4 = Template size, 0=no template    ;
;          R5 = Parameters qword                ;
;          R6 = Shifts count                    ;
;                                               ;
; OUTPUT:  R2 = Updated by string write         ;
;-----------------------------------------------;
Parm_String_Decimal8:
		MOVQ		R3,R5			; R3 = Date and time qword
		SHR64		R3,R6
		MOVB		R3,R3			; R3 = Value, byte
		CALL32		String_Decimal32	; Here R4=0 means no template
		RET

