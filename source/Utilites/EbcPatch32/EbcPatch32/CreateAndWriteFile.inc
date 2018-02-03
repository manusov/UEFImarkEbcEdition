;--- Create and Write file --------------------;
; INPUT:   ECX = Pointer to file name          ;
;          EDX = Pointer to file content data  ;
;          EAX = File size, bytes              ;
; OUTPUT:  EAX = Status                        ;
;          ECX = File size write               ;
;          BL  = Error phase indicator,        ;
;                valid if error,               ;
;                8=Open, 9=Write, 10=Close     ;
;          subset of global errors numeration  ;
;----------------------------------------------;
CreateAndWriteFile:
push esi edi
mov esi,eax
mov edi,edx
;--- Create file ---
mov bh,8                               ; BH=8 , Phase = CREATE
xor eax,eax                            ; EAX = 0
push eax                               ; Parm#7 = Template File (no)
push FILE_ATTRIBUTE_NORMAL             ; Parm#6 = File attributes
push CREATE_ALWAYS                     ; Parm#5 = Create disposition
push eax                               ; Parm#4 = Security attributes
push eax                               ; Parm#3 = Share mode
push GENERIC_READ + GENERIC_WRITE      ; Parm#2 = RDX = Desired access
push ecx                               ; Parm#1 = RCX = Pointer to file name
call [CreateFile]                      ; Create file, ret. RAX=Handle or 0=Error
test eax,eax
jz @f                                  ; Go exit if error
xchg ebx,eax                           ; EBX = Handle (XCHG for compact code)
;--- Write file ---
xor eax,eax
push eax                      ; Variable 
mov edx,esp                   ; EDX = Address of variable
push eax                      ; Parm#5 = Pointer to overlapped (no)
push edx                      ; Parm#4 = Pointer to variable = write size
push esi                      ; Parm#3 = File Size 
push edi                      ; Parm#2 = Buffer
push ebx                      ; Parm#1 = File Handle
call [WriteFile]
pop esi                       ; ESI = Number of bytes write
;--- Close file ---
push ebx                      ; Parm#1 = Handle for close (file handle)
xchg ebx,eax                  ; EBX = Save error code after write file
call [CloseHandle]
;--- Error code = F(restore, input) ---
test ebx,ebx                  ; Check status after read/write file 
setnz bl                      ; BL=0 if read/write error, BL=1 if r/w OK
test eax,eax                  ; Check status after close file
setnz al                      ; AL=0 if close error, AL=1 if close OK
and al,bl                     ; AL=1 only if both operations status OK
and eax,1                     ; Bit EAX.0=Valid, bits EAX.[31-1]=0
;--- Adjust error phase indicator ---
mov bh,9                      ; BH=9 means WRITE
mov ecx,esi                   ; ECX = Write data size
jecxz @f                      ; Return BH=9 (WRITE phase) if size=0
add bh,bl                     ; If Read OK, set BH=10 means CLOSE
;--- Exit ---
@@:
mov bl,bh                     ; BL = Error phase indicator
pop edi esi
ret

