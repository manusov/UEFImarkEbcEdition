;--- Open and Read file -----------------------;
; INPUT:   RCX = Pointer to file name          ;
;          RDX = Pointer to file data buffer   ;
;          RAX = File size, bytes              ;
; OUTPUT:  RAX = Status                        ;
;          RCX = File size                     ;
;----------------------------------------------;
OpenAndReadFile:
push rbx rsi rdi rbp
mov rbp,rsp                   ; RBP = storage for RSP and pointer to frame
and rsp,0FFFFFFFFFFFFFFF0h    ; Align stack
mov rsi,rax                   ; RSI = File size
mov rdi,rdx                   ; RDI = Pointer to buffer
;--- Create file ---
mov edx,GENERIC_READ + GENERIC_WRITE   ; Parm#2 = RDX = Desired access
xor r8d,r8d                            ; Parm#3 = R8  = Share mode
xor r9d,r9d                            ; Parm#4 = R9  = Security attributes
push r9 r9                             ; Alignment + Parm#7 = Template File (no)
push FILE_ATTRIBUTE_NORMAL             ; Parm#6 = File attributes
push OPEN_ALWAYS                       ; Parm#5 = Create disposition
sub rsp,32                             ; Create parameters shadow
call [CreateFile]                      ; Create file, ret. RAX=Handle or 0=Error
add rsp,32+32                          ; Remove shadow, 3 parameters, align
test rax,rax
jz @f                                  ; Go exit if error
xchg rbx,rax                           ; RBX = Handle (XCHG for compact code)
;--- Write file ---
xor eax,eax
push rax rax                  ; Variable + Parm#5 = Pointer to overlapped (no) 
mov rcx,rbx                   ; Parm#1 = RCX = File Handle
mov rdx,rdi                   ; Parm#2 = RDX = Buffer
mov r8,rsi                    ; Parm#3 = R8  = File Size
lea r9,[rsp+8]                ; Parm#4 = R9  = Pointer to variable = read size
sub rsp,32
call [ReadFile]
add rsp,32+8
pop rsi                       ; RSI = Number of bytes write
;--- Close file ---
mov rcx,rbx                   ; Parm#1 = RCX = Handle for close (file handle)
xchg rbx,rax                  ; RBX = Save error code after read file
sub rsp,32
call [CloseHandle]
add rsp,32
;--- Error code = F(restore, input) ---
test rbx,rbx                  ; Check status after read/write file 
setnz bl                      ; BL=0 if read/write error, BL=1 if r/w OK
test rax,rax                  ; Check status after close file
setnz al                      ; AL=0 if close error, AL=1 if close OK
and al,bl                     ; AL=1 only if both operations status OK
and eax,1                     ; Bit RAX.0=Valid, bits RAX.[63-1]=0
;--- Exit point ---
@@:
mov rcx,rsi
mov rsp,rbp
pop rbp rdi rsi rbx
ret
