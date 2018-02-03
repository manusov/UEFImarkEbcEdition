;--- String write for errors messages ---------;
; INPUT:  AL  = Error code selector, 0-based   ;
;         RCX = Strings pool base address      ; 
; OUTPUT: RAX = OS Status                      ;
;----------------------------------------------;
ErrorStringWrite:
cmp al,0
je ErrorWrite
SkipStrings:
cmp byte [rcx],0
je NextString
inc rcx
jmp SkipStrings
NextString:
dec al
jnz SkipStrings  
inc rcx
;---
ErrorWrite:
push rcx
lea rcx,[CrLf]
call StringWrite
pop rcx
call StringWrite
lea rcx,[CrLf]
; No RET, continue in the next subroutine

;--- String write in ASCII --------------------;
; INPUT:  RCX = Pointer to string              ;
;         ASCII string at RCX memory address   ;
;         string is null-terminated            ;
;         also global variables used           ;
; OUTPUT: RAX = OS Status                      ;
;----------------------------------------------;
StringWrite:
;--- Entry ---
push rbp rax                  ; RBP = non-volatile, RAX = for storage
mov rbp,rsp                   ; RBP = storage for RSP and pointer to frame
and rsp,0FFFFFFFFFFFFFFF0h    ; Align stack
;--- Calculate string length ---
mov rdx,rcx                   ; RDX = Parm#2 = Pointer to string
xor r8d,r8d                   ; R8  = Parm#3 = Number of chars
@@:
cmp byte [rcx+r8],0           ; Check current char from string
je @f                         ; Exit cycle if terminator (byte=0) found
inc r8d                       ; Chars counter + 1
jmp @b                        ; Go next iteration
@@:
;--- Write console ---
mov rcx,[OutputDevice]        ; RCX = Parm#1 = Input device handle
mov r9,rbp                    ; R9  = Parm#4 = Pointer to out. variable, count
xor eax,eax                   ; EAX = 0
push rax rax                  ; Align stack + Parm#5 = Reserved
sub rsp,32                    ; Create parameters shadow
call [WriteConsole]           ; Display output
add rsp,32+16                 ; Remove parameters shadow, parm#5, stack align
;--- Exit ---
mov rsp,rbp
pop rax rbp
ret
