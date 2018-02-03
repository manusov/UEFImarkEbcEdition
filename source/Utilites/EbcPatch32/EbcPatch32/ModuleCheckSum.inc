;--- Calculate module checksum by specific algorithm --------------------;
; Checksum field in the module must be blank before call this subroutine ;
; INPUT:   ESI = Pointer to module                                       ;
;          ECX = Size of module, bytes                                          ;
; OUTPUT:  EDX = Checksum                                                ;
;------------------------------------------------------------------------;
ModuleCheckSum:
push ecx ebx esi edi
cld
mov edi,ecx   ; EDI = Size, in bytes
shr ecx,1     ; ECX = Size, in 16-bit words
;---
xor ebx,ebx
xor edx,edx   ; BX:DX=Storage
@@:
lodsw         ; AX=Get current word
add dx,ax     ; Add to storage.low
adc bx,0      ; Carry to storage.high
mov ax,dx     ; Save storage.low
mov dx,bx     ; Shift right storage by 16
xor bx,bx     ; Also for shift
add dx,ax     ; Get carry
adc bx,0
loop @b        ; Cycle for entire module
add dx,di      ; Add file size to checksum
adc bx,0       ; Carry to storage.high
;---
movzx edx,dx   ; Clear high 16 bits of EDX
shl ebx,16     ; Write high 16 bits of EDX , checksum.high 
or edx,ebx
pop edi esi ebx ecx
ret

; Code, ported from 16-bit application, can be optimized
; Original 16-bit version of checksum calculation routine.

;--- Calculate module checksum by specific algorithm --------------------;
; Checksum field in the module must be blank before call this subroutine ;
; INPUT:   DS = Segment of module, assume start offset=0                 ;
;          CX = Size of module image                                     ;
; OUTPUT:  BX:DX = Checksum, BX=High, DX=Low                             ;
;------------------------------------------------------------------------;
;Module_Check_Sum  PROC NEAR
;push	ax cx si di
;cld
;xor si,si  ; DS:SI=Pointer, CX=Size
;mov di,cx  ; DI=Size, bytes
;shr cx,1   ; CX=Size, words
;xor bx,bx
;xor dx,dx  ; BX:DX=Storage
;Sum_Cycle:	lodsw  ; AX=Get current word
;add dx,ax  ; Add to storage.low
;adc bx,0   ; Carry to storage.high
;mov ax,dx  ; Save storage.low
;mov dx,bx  ; Shift right storage by 16
;xor bx,bx  ; Also for shift
;add dx,ax  ; Get carry
;adc bx,0
;loop Sum_Cycle  ; Cycle for entire module
;add dx,di       ; Add file size to checksum
;adc bx,0        ; Carry to storage.high
;pop di si cx ax
;ret
;Module_Check_Sum  ENDP
