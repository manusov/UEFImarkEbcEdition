;--- Restore context handler: Release occupied memory -------------------;
;                                                                        ;
; INPUT:   R1 = Global variables pool base address                       ;
;          Global variables: QWORD EFI_Memory_Total                      ;
;                            QWORD Use_Memory_Base                       ;
;                            QWORD Use_Memory_Size                       ;
;                                                                        ;
; OUTPUT:  Condition flag: 0(CC)=Successfully, 1(CS)=Failed              ;
;------------------------------------------------------------------------;

Release_Memory:
		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVQW		R3,@R1,0,_Primary_Memory_Size
		MOVIQW		R7,12
		SHR64		R3,R7		; Bytes->Pages
		CALL32		EBC_Free_Pages
		RET