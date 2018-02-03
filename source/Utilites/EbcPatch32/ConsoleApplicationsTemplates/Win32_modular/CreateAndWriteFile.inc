;--- Create and Write file --------------------;
; INPUT:   ECX = Pointer to file name          ;
;          EDX = Pointer to file content data  ;
;          EAX = File size, bytes              ;
; OUTPUT:  EAX = Status                        ;
;----------------------------------------------;
CreateAndWriteFile:
push esi edi
mov esi,eax
mov edi,edx
;--- Create file ---
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
push 0                        ; Variable 
mov edx,esp                   ; EDX = Address of variable
push 0                        ; Parm#5 = Pointer to overlapped (no)
push edx                      ; Parm#4 = Pointer to variable = write size
push esi                      ; Parm#3 = File Size 
push edi                      ; Parm#2 = Buffer
push ebx                      ; Parm#1 = File Handle
call [WriteFile]
pop edx                       ; EDX = Number of bytes write
; ... use RDX ...
; ...
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
;--- Exit ---
@@:
pop edi esi
ret

