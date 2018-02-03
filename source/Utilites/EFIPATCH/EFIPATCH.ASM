_CODE  SEGMENT BYTE USE16
ASSUME CS:_CODE, SS:_STACK, DS:_DATA, ES:_DATA
ORG 0
PROGRAM PROC FAR
START:

;---------- Initializing stack ---------------------------------------------;
		mov	ax,_STACK
		mov	ss,ax
		mov	sp,Stack_Size

;---------- First message --------------------------------------------------;
		mov	dx,OFFSET Msg_1
		call	DOS_Message

;---------- Parse command string -------------------------------------------;
; Here DS=Segment PSP
; Scan for command line parameters and parse it if present
; Analysed Region = 65 bytes at PSP:0080h
		push	_DATA
		pop	es
		mov	bp,OFFSET Cmd_Parms_E_Str
		mov	si,0080h+1		; Skip Open=0Dh
		mov	di,OFFSET Src_File_Path
		mov	cx,65
		call	Scan_Name
		jc	Cmd_String_Error
		lea	bx,[di-1]		; ES:BX=Address of last byte
		mov	di,OFFSET Dst_File_Path
		call	Scan_Name
		jc	Cmd_String_Error
		dec	di			; ES:DI=Address of last byte

;---------- Message with input and output files names ----------------------;
		mov	dx,OFFSET Msg_2
		call	DOS_Message
		mov	dx,OFFSET Src_File_Path
		call	DOS_Message
		mov	dx,OFFSET Msg_3
		call	DOS_Message
		mov	dx,OFFSET Dst_File_Path
		call	DOS_Message

;---------- Message "Working..." -------------------------------------------;
		mov	dx,OFFSET Msg_4
		call	DOS_Message

;---------- Replace text strings terminators from 24h to 00h ---------------;
; Note. 24h=for DOS function string write, 00h=for DOS file functions
		mov	BYTE PTR es:[bx],0
		mov	BYTE PTR es:[di],0

;---------- Read input file ------------------------------------------------;
		mov	ax,3D00h		; Function=Open file for Read
		push	_DATA
		pop	ds
		mov	dx,OFFSET Src_File_Path   ; DS:DX=Pointer to name
		int	21h			  ; Open file
		mov	bp,OFFSET In_File_Open_E_Str
		jc	In_File_Open_Error
		xchg	bx,ax			; BX=Handle
		mov	ah,3Fh			; Function=Read file
		mov	cx,8000h+1		; Size maximum=32768 bytes
		push	_BUFFER
		pop	ds
		xor	dx,dx			; DS:DX=Pointer to buffer
		int	21h			; Read file
		mov	bp,OFFSET In_File_Read_E_Str
		jc	In_File_Read_Error
		push	ax
		mov	ah,3Eh			; Function=Close file
		int	21h			; Close file, need BX=Handle
		pop	ax
		mov	bp,OFFSET In_File_Close_E_Str
		jc	In_File_Close_Error
		mov	bp,OFFSET In_File_Bad_Size_E_Str
		cmp	ax,1024
		jb	In_File_Bad_Size
		cmp	ax,32768
		ja	In_File_Bad_Size

;---------- Patch buffer ---------------------------------------------------;

		mov	bp,OFFSET Patch_H_Str
		xchg	cx,ax			; CX=File size
		cmp	WORD PTR ds:[003Eh],0
		jne	Patch_Error
		mov	di,ds:[003Ch]
		cmp	di,0040h
		jb	Patch_Error
		cmp	di,cx
		ja	Patch_Error

		xor	ax,ax			; BP:AX = 0 for blank old sum
		xor	bp,bp
		xchg	ax,ds:[di+0058h+0]	; BP:AX = Old sum, DLL blank
		xchg	bp,ds:[di+0058h+2]
		call	DLL_Check_Sum		; Return BX:DX = Calc. sum
		cmp	ax,dx			; Compare checksum.low
		jne	Sum_Error
		cmp	bp,bx			; Compare checksum.high
Sum_Error:	mov	bp,OFFSET Patch_S_Str
		jne	Patch_Error

;---------- Patch module type ----------------------------------------------;

		mov	bx,000Ah		; New Subsystem = EFI
		xchg	bx,ds:[di+005Ch]	; Patch Subsystem Type word
		cmp	bx,0003h		; Old Subsystem = DLL
		mov	bp,OFFSET Patch_E_Str
		jne	Patch_Error

;---------- Patch checksum -------------------------------------------------;

		call	DLL_Check_Sum		; Return BX:DX = Calc. sum
		mov	ds:[di+0058h+0],dx	; Set checksum.low
		xchg	ds:[di+0058h+2],bx	; Set checksum.high

;---------- Create and Write output file -----------------------------------;
		push	cx			; Save CX=file size
		mov	ah,3Ch			; Function=Create file
		push	_DATA
		pop	ds
		mov	dx,OFFSET Dst_File_Path   ; DS:DX=Pointer to name
		xor	cx,cx			  ; CX=Attributes
		int	21h			; Create file
		pop	cx			; Restore CX=file size
		mov	bp,OFFSET Out_File_Create_E_Str
		jc	Out_File_Create_Error
		xchg	bx,ax			; BX=Handle
		mov	ah,40h			; Function=Write file
		push	_BUFFER
		pop	ds
		xor	dx,dx			; DS:DX=Pointer to buffer
		int	21h			; Write file
		mov	bp,OFFSET Out_File_Write_E_Str
		jc	Out_File_Write_Error
		mov	ah,3Eh			; Function=Close file
		int	21h			; Close file, need BX=Handle
		mov	bp,OFFSET Out_File_Close_E_Str
		jc	Out_File_Close_Error

;---------- Exit Points ----------------------------------------------------;
		mov	dx,OFFSET Msg_OK
		call	DOS_Message
		mov	ax,4C00h
		int	21h

In_File_Read_Error:
Out_File_Write_Error:
		mov	ah,3Eh			; Function=Close file
		int	21h			; Close file, need BX=Handle

Cmd_String_Error:
In_File_Open_Error:
In_File_Close_Error:
In_File_Bad_Size:
Patch_Error:
Out_File_Create_Error:
Out_File_Close_Error:

		mov	dx,bp
		call	DOS_Message
		mov	ax,4C01h
		int	21h

;---------- Library --------------------------------------------------------;

;---------- Output string, use DOS functions --------;
; INPUT:   DX = String offset, segment=_DATA         ;
; OUTPUT:  None                                      ;
;----------------------------------------------------;
DOS_Message  PROC NEAR
		push	ax ds
		mov	ax,_DATA
		mov	ds,ax
		mov	ah,9		; Function 9 = Output string
		int	21h
		pop	ds ax
		ret
DOS_Message  ENDP

;--- Parse command string parameters, extract and copy file names ---;
; INPUT:   DS:SI = Source (command string)                           ;
;          ES:DI = Destination (output buffer)                       ;
;          CX    = Size limit                                        ;
;                                                                    ;
; OUTPUT:  CF = Error flag: 0(NC)=Extracted OK, 1(C)=Error           ;
;          SI = Modify                                               ;
;          DI = Modify                                               ;
;--------------------------------------------------------------------;
Scan_Name  PROC NEAR
		jcxz	Empty_Name
Scan_Name_1:	lodsb
		cmp	al,0Dh
		je	Empty_Name		; Exit if Close=0Dh
		cmp	al,' '
		loope	Scan_Name_1
		je	Empty_Name		; Exit if SPACES only
		dec	si
		jcxz	Empty_Name
Move_Name:	lodsb
		cmp	al,0
		je	Name_End
		cmp	al,' '
		je	Name_End
		cmp	al,0Dh
		je	Name_End
		stosb
		loop	Move_Name
Empty_Name:	mov	ax,'$?'
		stosw
		stc
		ret
Name_End:	mov	al,'$'
		stosb
		clc
		ret
Scan_Name  ENDP

;--- Calculate DLL checksum by specific algorithm ----------------------;
; Checksum field in the DLL must be blank before call this subroutine   ;
; INPUT:   DS = Segment of DLL, assume start offset=0                   ;
;          CX = Size of DLL image                                       ;
; OUTPUT:  BX:DX = Checksum, BX=High, DX=Low                            ;
;-----------------------------------------------------------------------;
DLL_Check_Sum  PROC NEAR
		push	ax cx si di
		cld
		xor	si,si			; DS:SI=Pointer, CX=Size
		mov	di,cx			; DI=Size, bytes
		shr	cx,1			; CX=Size, words
		xor	bx,bx
		xor	dx,dx			; BX:DX=Storage
Sum_Cycle:	lodsw				; AX=Get current word
		add	dx,ax			; Add to storage.low
		adc	bx,0			; Carry to storage.high
		mov	ax,dx			; Save storage.low
		mov	dx,bx			; Shift right storage by 16
		xor	bx,bx			; Also for shift
		add	dx,ax			; Get carry
		adc	bx,0
		loop	Sum_Cycle		; Cycle for entire DLL
		add	dx,di			; Add file size to checksum
		adc	bx,0			; Carry to storage.high
		pop	di si cx ax
		ret
DLL_Check_Sum  ENDP

PROGRAM ENDP
_CODE ENDS

;---------- Data Segment ---------------------------------------------------;
_DATA  SEGMENT BYTE USE16
Msg_1		DB	0Ah, 0Dh, 'DLL32->EFI32 patcher for FASM. (C) IC Book Labs. v1.1' , 24h
		DB	0Ah, 0Dh, 'Starting...'   , 24h
Msg_2		DB	0Ah, 0Dh, 'Input file:  ' , 24h
Msg_3		DB	0Ah, 0Dh, 'Output file: ' , 24h
Msg_4		DB	0Ah, 0Dh, 'Working...'    , 24h
Msg_OK		DB	0Ah, 0Dh, 'OK.',0Ah,0Dh   , 24h
;--- Error strings ---
Cmd_Parms_E_Str		DB  0Ah,0Dh,'Command string error, '
			DB  'use EFIPATCH <InFile> <OutFile>.'
			DB  07h,0Ah,0Dh,24h
In_File_Open_E_Str	DB  0Ah,0Dh,'Open input file error.',07h,0Ah,0Dh,24h
In_File_Read_E_Str	DB  0Ah,0Dh,'Read input file error.',07h,0Ah,0Dh,24h
In_File_Close_E_Str	DB  0Ah,0Dh,'Close input file error.',07h,0Ah,0Dh,24h
In_File_Bad_Size_E_Str	DB  0Ah,0Dh
			DB  'Invalid input file size, support 1024-32768 bytes.'
			DB  07h,0Ah,0Dh,24h
Patch_H_Str		DB  0Ah,0Dh,'Input file bad, invalid offset at 003Ch.'
			DB  07h,0Ah,0Dh,24h
Patch_E_Str		DB  0Ah,0Dh,'Input file bad, need [Header+5Ch]=0003h.'
			DB  07h,0Ah,0Dh,24h
Patch_S_Str		DB  0Ah,0Dh,'Input file bad, wrong checksum.'
			DB  07h,0Ah,0Dh,24h
Out_File_Create_E_Str	DB  0Ah,0Dh,'Output file create error.',07h,0Ah,0Dh,24h
Out_File_Write_E_Str	DB  0Ah,0Dh,'Output file write error.',07h,0Ah,0Dh,24h
Out_File_Close_E_Str	DB  0Ah,0Dh,'Output file close error.',07h,0Ah,0Dh,24h
;--- Files names  from command string ---
Src_File_Path		DB  80 DUP (?)
Dst_File_Path		DB  80 DUP (?)
_DATA  ENDS

;---------- Buffer Segment -------------------------------------------------;
_BUFFER  SEGMENT PARA USE16
DB  32768+1 DUP (?)
_BUFFER  ENDS

;---------- Stack Segment --------------------------------------------------;
Stack_Size  EQU  2048
_STACK  SEGMENT PARA STACK 'STACK' USE16
DB  Stack_Size  DUP (?)
_STACK  ENDS

END START
