;===========================================================================;
;=                        UEFI GOP debug sample.                           =;
;=         Demo for EFI Byte Code (EBC) cross-platform programming.        =;
;=                      (C) 2013-2018 IC Book Labs.                        =;
;===========================================================================;

;---------------------------------------------------------------------------;
;                       MACRO FOR EBC INSTRUCIONS.                          ;
;---------------------------------------------------------------------------;

include 'MacroEbc\add32.inc'
include 'MacroEbc\add64.inc'
include 'MacroEbc\and32.inc'
include 'MacroEbc\and64.inc'
include 'MacroEbc\ashr32.inc'
include 'MacroEbc\ashr64.inc'
include 'MacroEbc\break.inc'
include 'MacroEbc\call32.inc'
include 'MacroEbc\call32a.inc'
include 'MacroEbc\call32ex.inc'
include 'MacroEbc\call32exa.inc'
include 'MacroEbc\call64.inc'
include 'MacroEbc\call64a.inc'
include 'MacroEbc\call64ex.inc'
include 'MacroEbc\call64exa.inc'
include 'MacroEbc\cmp32eq.inc'
include 'MacroEbc\cmp32gte.inc'
include 'MacroEbc\cmp32lte.inc'
include 'MacroEbc\cmp32ugte.inc'
include 'MacroEbc\cmp32ulte.inc'
include 'MacroEbc\cmp64eq.inc'
include 'MacroEbc\cmp64gte.inc'
include 'MacroEbc\cmp64lte.inc'
include 'MacroEbc\cmp64ugte.inc'
include 'MacroEbc\cmp64ulte.inc'
include 'MacroEbc\cmpi32deq.inc'
include 'MacroEbc\cmpi32dgte.inc'
include 'MacroEbc\cmpi32dlte.inc'
include 'MacroEbc\cmpi32dugte.inc'
include 'MacroEbc\cmpi32dulte.inc'
include 'MacroEbc\cmpi32eq.inc'
include 'MacroEbc\cmpi32gte.inc'
include 'MacroEbc\cmpi32lte.inc'
include 'MacroEbc\cmpi32ugte.inc'
include 'MacroEbc\cmpi32ulte.inc'
include 'MacroEbc\cmpi32weq.inc'
include 'MacroEbc\cmpi32wgte.inc'
include 'MacroEbc\cmpi32wlte.inc'
include 'MacroEbc\cmpi32wugte.inc'
include 'MacroEbc\cmpi32wulte.inc'
include 'MacroEbc\cmpi64deq.inc'
include 'MacroEbc\cmpi64dgte.inc'
include 'MacroEbc\cmpi64dlte.inc'
include 'MacroEbc\cmpi64dugte.inc'
include 'MacroEbc\cmpi64dulte.inc'
include 'MacroEbc\cmpi64eq.inc'
include 'MacroEbc\cmpi64gte.inc'
include 'MacroEbc\cmpi64lte.inc'
include 'MacroEbc\cmpi64ugte.inc'
include 'MacroEbc\cmpi64ulte.inc'
include 'MacroEbc\cmpi64weq.inc'
include 'MacroEbc\cmpi64wgte.inc'
include 'MacroEbc\cmpi64wlte.inc'
include 'MacroEbc\cmpi64wugte.inc'
include 'MacroEbc\cmpi64wulte.inc'
include 'MacroEbc\div32.inc'
include 'MacroEbc\div64.inc'
include 'MacroEbc\divu32.inc'
include 'MacroEbc\divu64.inc'
include 'MacroEbc\extndb32.inc'
include 'MacroEbc\extndb64.inc'
include 'MacroEbc\extndd32.inc'
include 'MacroEbc\extndd64.inc'
include 'MacroEbc\extndw32.inc'
include 'MacroEbc\extndw64.inc'
include 'MacroEbc\jmp32.inc'
include 'MacroEbc\jmp32cc.inc'
include 'MacroEbc\jmp32cs.inc'
include 'MacroEbc\jmp32a.inc'
include 'MacroEbc\jmp32acc.inc'
include 'MacroEbc\jmp32acs.inc'
include 'MacroEbc\jmp64.inc'
include 'MacroEbc\jmp64cc.inc'
include 'MacroEbc\jmp64cs.inc'
include 'MacroEbc\jmp64a.inc'
include 'MacroEbc\jmp64acc.inc'
include 'MacroEbc\jmp64acs.inc'
include 'MacroEbc\jmp8.inc'
include 'MacroEbc\jmp8cc.inc'
include 'MacroEbc\jmp8cs.inc'
include 'MacroEbc\loadsp.inc'
include 'MacroEbc\mod32.inc'
include 'MacroEbc\mod64.inc'
include 'MacroEbc\modu32.inc'
include 'MacroEbc\modu64.inc'
include 'MacroEbc\movb.inc'
include 'MacroEbc\movbd.inc'
include 'MacroEbc\movbw.inc'
include 'MacroEbc\movd.inc'
include 'MacroEbc\movdd.inc'
include 'MacroEbc\movdw.inc'
include 'MacroEbc\movibd.inc'
include 'MacroEbc\movibq.inc'
include 'MacroEbc\movibw.inc'
include 'MacroEbc\movidd.inc'
include 'MacroEbc\movidq.inc'
include 'MacroEbc\movidw.inc'
include 'MacroEbc\movind.inc'
include 'MacroEbc\movinq.inc'
include 'MacroEbc\movinw.inc'
include 'MacroEbc\moviqd.inc'
include 'MacroEbc\moviqq.inc'
include 'MacroEbc\moviqw.inc'
include 'MacroEbc\moviwd.inc'
include 'MacroEbc\moviwq.inc'
include 'MacroEbc\moviww.inc'
include 'MacroEbc\movn.inc'
include 'MacroEbc\movnd.inc'
include 'MacroEbc\movnw.inc'
include 'MacroEbc\movq.inc'
include 'MacroEbc\movqd.inc'
include 'MacroEbc\movqq.inc'
include 'MacroEbc\movqw.inc'
include 'MacroEbc\movreld.inc'
include 'MacroEbc\movrelq.inc'
include 'MacroEbc\movrelw.inc'
include 'MacroEbc\movsn.inc'
include 'MacroEbc\movsnd.inc'
include 'MacroEbc\movsnw.inc'
include 'MacroEbc\movw.inc'
include 'MacroEbc\movwd.inc'
include 'MacroEbc\movww.inc'
include 'MacroEbc\mul32.inc'
include 'MacroEbc\mul64.inc'
include 'MacroEbc\mulu32.inc'
include 'MacroEbc\mulu64.inc'
include 'MacroEbc\neg32.inc'
include 'MacroEbc\neg64.inc'
include 'MacroEbc\not32.inc'
include 'MacroEbc\not64.inc'
include 'MacroEbc\or32.inc'
include 'MacroEbc\or64.inc'
include 'MacroEbc\pop32.inc'
include 'MacroEbc\pop64.inc'
include 'MacroEbc\popn.inc'
include 'MacroEbc\push32.inc'
include 'MacroEbc\push64.inc'
include 'MacroEbc\pushn.inc'
include 'MacroEbc\ret.inc'
include 'MacroEbc\shl32.inc'
include 'MacroEbc\shl64.inc'
include 'MacroEbc\shr32.inc'
include 'MacroEbc\shr64.inc'
include 'MacroEbc\storesp.inc'
include 'MacroEbc\sub32.inc'
include 'MacroEbc\sub64.inc'
include 'MacroEbc\xor32.inc'
include 'MacroEbc\xor64.inc'

;---------------------------------------------------------------------------;
;                       CODE SECTION DEFINITIONS.                           ;
;---------------------------------------------------------------------------;

format pe64 dll efi
entry main
section '.text' code executable readable
main:

;---------------------------------------------------------------------------;
;                  CODE SECTION \ MAIN EBC ROUTINE.                         ;
;---------------------------------------------------------------------------;

;---------- Save parent parameters, initializing variables -----------------;

		MOVIQQ		R1,Global_Variables_Pool	; R1=Pool
		MOVNW		R2,@R0,0,16			; R2=Handle
		MOVNW		R3,@R0,1,16			; R3=Table
		MOVQW		@R1,0,_EFI_Handle,R2		; Save Handle
		MOVQW		@R1,0,_EFI_Table,R3		; Save Table
		MOVIQQ		@R1,0,_Quad_Bitmaps,0000000100000000h

;---------- Occupy memory --------------------------------------------------;

		CALL32		Check_Memory		; Check and occupy memory

;---------- Detect GOP -----------------------------------------------------;

		MOVIQQ		R2,GuidGop		
		CALL32		EBC_Locate_Protocol
		MOVQW		@R1,0,_GOP_Protocol,R2
		JMP8CS		Gop_Exit		; Go error if GOP detect failed

;---------- Get and save current video mode, use GOP -----------------------;
							; Here R2=Protocol pointer for GOP
		MOVNW		R2,@R2,3,0		; R2=GOP mode info table pointer, N=3, C=0
		MOVD		R3,@R2			; R3=Maximum video mode-1
		MOVDW		R4,@R2,0,4		; R4=Current (firmware) video mode
		CMP32ULTE	R3,R4
		JMP8CS		Gop_Exit		; Go error if Current Mode > Maximum Mode
		MOVDW		@R1,0,_Firmware_Video_Mode,R4

;---------- Set new video mode and save it parameters, use GOP -------------;


;=== FOR THIS DEBUG SAMPLE HERE EDIT R3 VALUE = TARGET VIDEO MODE NUMBER ===;*
                                                                            ;*
		MOVIQW		R3,0			; R3=Mode number    ;*
                                                                            ;*
;===========================================================================;*

		MOVQW		R2,@R1,0,_GOP_Protocol
		PUSH64		R3			; Parm#2=Mode number
		PUSHN		R2			; Parm#1=Protocol pointer
		CALL32EXA	@R2,1,0			; GopTable.ModeSet
		POPN		R2
		POP64		R3
		MOVNW		R2,@R2,3,0		; R2=GOP mode info table pointer, N=3, C=0
		MOVDW		R3,@R2,0,4		; R4=Current (program) video mode
		MOVDW		@R1,0,_Program_Video_Mode,R3
		MOVNW		R3,@R2,0,8		; R3=Pointer to mode info block, N=0, C=8
		MOVNW		R4,@R2,1,8		; R4=Size of mode info block, N=1, C=8
		; here reserved point, need check R4=24h or above
		MOVNW		R2,@R3,0,4		; R2=Horizontal Resolution, N=0, C=4
		MOVNW		R3,@R3,0,8		; R3=Vertical Resolution, N=0, C=8
		; here reserved point, need check R2, R3 with Xlimit, Ylimit
		MOVWW		@R1,0,_Video_Xsize,R2
		MOVWW		@R1,0,_Video_Ysize,R3
		MOVIQW		R3,2			; Shift left by 2 means *4
		SHL64		R2,R3			; Multiply by 4 bytes per pixel
		MOVWW		@R1,0,_Video_Xdelta,R2

;--- Built true color picture in the transit buffer ------------------------;

		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVWW		R3,@R1,0,_Video_Xsize
		MOVWW		R4,@R1,0,_Video_Ysize

		MOVQ		R6,R3
		ADD64		R6,R4			; R6=Maximum pixel sum X+Y
		MOVIQD		R7,65535
		DIVU64		R7,R6			; R7=Additions per coordinate
		MOVIQW		R5,16			; R5=Shifts counter for R,G,B
		MOVQ		R6,R7			; R6=Blue16
		SHL64		R7,R5
		OR64		R6,R7			; R6=Green16+Blue16
		SHL64		R7,R5
		OR64		R6,R7			; R6=Red16+Green16+Blue16
		MOVQ		R7,R6			; R6=Color X addend, R7=Color Y addend

		XOR64		R5,R5			; R5=Base Color: Red16:Green16:Blue16
		CALL32		EBC_Built_Color_Show

;---------- Built program info strings in the transit buffer ---------------;
; here required optimizing of operands location

		MOVQW		R2,@R1,0,_Primary_Memory_Base
		MOVWW		R3,@R1,0,_Video_Xsize
		MOVIQW		R4,100
		MUL64		R3,R4			; R3=Xsize*100
		ADD64		R3,R4			; R3=Xsize*100+100 (offsets x=100, y=100)
		ADD64		R2,R3
		MOVIQQ		R3,String_Version	; R3=Pointer to string
		MOVIQD		R4,00F0F0F0h		; R4=True color: Reserved8.Red8.Green8.Blue8
		CALL32		EBC_Draw_String		; String 1
		MOVWW		R5,@R1,0,_Video_Xdelta
		MOVIQW		R6,16
		MUL64		R5,R6
		ADD64		R2,R5			; R2=Pointer to video transit buffer, down 16 lines
		MOVIQQ		R3,String_Copyright	; R3=Pointer to string
		CALL32		EBC_Draw_String		; String 2
		ADD64		R2,R5			; R2=Pointer to video transit buffer, down 16 lines
		MOVIQQ		R3,String_Http		; R3=Pointer to string
		MOVIQD		R4,00F0F001h		; R4=True color: Reserved8.Red8.Green8.Blue8
		CALL32		EBC_Draw_String		; String 3

;---------- Prepare ASCII numbers string for video memory info strings -----;

		PUSH64		R2
		XOR64		R4,R4				; Template size, 0 means no restrictions
		MOVIQQ		R2,String_Video_Mode_1		; R2 = Pointer to destination string
		MOVDW		R3,@R1,0,_Program_Video_Mode	; R3=Number value
		CALL32		EBC_String_Decimal32		; Print number=R3 at string [R2]
		MOVIQQ		R2,String_Xsize_1
		MOVWW		R3,@R1,0,_Video_Xsize
		CALL32		EBC_String_Decimal32
		MOVIQQ		R2,String_Ysize_1
		MOVWW		R3,@R1,0,_Video_Ysize
		CALL32		EBC_String_Decimal32
		POP64		R2

;---------- Built video memory info strings in the transit buffer ----------;

		ADD64		R2,R5			; R2=Pointer to video transit buffer, down 3*16=48 lines
		ADD64		R2,R5
		ADD64		R2,R5
		MOVIQQ		R3,String_Video_Mode	; R3=Pointer to string
		MOVIQD		R4,0001F001h		; R4=True color: Reserved8.Red8.Green8.Blue8
		CALL32		EBC_Draw_String		; String Video Mode
		ADD64		R2,R5			; R2=Pointer to video transit buffer, down 16 lines
		MOVIQQ		R3,String_Xsize		; R3=Pointer to string
		CALL32		EBC_Draw_String		; String Xsize
		ADD64		R2,R5			; R2=Pointer to video transit buffer, down 16 lines
		MOVIQQ		R3,String_Ysize		; R3=Pointer to string
		CALL32		EBC_Draw_String		; String Ysize

;---------- Copy from transit buffer to video memory, use GOP.BLT ----------;
; Drawings is visible after this operation

		XOR64		R2,R2			; R2 = Start X = 0
		XOR64		R3,R3			; R3 = Start Y = 0
		MOVWW		R4,@R1,0,_Video_Xsize	; R4 = End X = Screen Xsize
		MOVWW		R5,@R1,0,_Video_Ysize	; R5 = End Y = Screen Ysize
		MOVQW		R6,@R1,0,_Primary_Memory_Base	; R6 = Transit buffer pointer
		MOVWW		R7,@R1,0,_Video_Xdelta	; R7 = Delta Y, bytes per string
		CALL32		EBC_GOP_BLT_Copy

;---------- Wait for user press any key ------------------------------------;

		CALL32		EBC_Input_Wait_Key	; Wait for key

;---------- Restore firmware video mode, use GOP ---------------------------;

		MOVDW		R3,@R1,0,_Firmware_Video_Mode	; R3=Mode number
		MOVQW		R2,@R1,0,_GOP_Protocol
		PUSH64		R3			; Parm#2=Mode number
		PUSHN		R2			; Parm#1=Protocol pointer
		CALL32EXA	@R2,1,0			; GopTable.ModeSet
		POPN		R2
		POP64		R3

;---------- Release memory -------------------------------------------------;
Gop_Exit:
		CALL32		Release_Memory		; Release occupied memory

;---------- Exit to parent task (Shell or Firmware) ------------------------;

		RET					; Return to parent

;---------------------------------------------------------------------------;
;                CODE SECTION \ EBC-EXECUTABLE SUBROUTINES.                 ;
;---------------------------------------------------------------------------;

include 'Library\StringDecimal.inc'	; Write decimal number as ASCII
include 'Library\ConvertString1.inc'	; Convert ASCII to UNICODE16
include 'Library\ConvertString2.inc'	; Convert UNICODE16 to ASCII
include 'Library\InputCheckKey.inc'	; Check keyboard input, no wait
include 'Library\InputWaitKey.inc'	; Wait keyboard input
include 'Library\LocateProtocol.inc'	; Locate UEFI protocol by GUID
include 'Library\MemoryOccupy.inc'	; Allocate memory pages by UEFI services
include 'Library\MemoryRelease.inc'	; Release memory pages by UEFI services
include 'Library\MemoryMap.inc'		; Built UEFI firmware memory map
include 'Library\BltCopy.inc'		; Copy block from system memory to video memory
include 'Library\BuiltColorShow.inc'	; Prepare color picture for benchmarks show
include 'Library\DrawChar.inc'		; Draw char in the graphics mode
include 'Library\DrawString.inc'	; Draw string of chars in the graphics mode

include 'Handlers\CheckMemory.inc'	; Check and occupy memory
include 'Handlers\ReleaseMemory.inc'	; Release occupied memory


;---------------------------------------------------------------------------;
;                            DATA SECTION.                                  ;
;---------------------------------------------------------------------------;

section '.data' data readable writeable

include 'Data\Names.inc'		; Program and vendor name
include 'Data\Guids.inc'		; GUID codes
include 'Data\SysInfoData.inc'		; System information strings
include 'Data\DrawData.inc'		; Data for text and graphics draw
include 'Data\GlobalData.inc'		; Variables

;---------------------------------------------------------------------------;
;                     RELOCATION ELEMENTS SECTION.                          ;
;---------------------------------------------------------------------------;

section '.reloc' fixups data discardable

