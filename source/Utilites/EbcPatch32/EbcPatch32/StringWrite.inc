;--- String write for errors messages ---------;
; INPUT:  AL  = Error code selector, 0-based   ;
;         ECX = Strings pool base address      ;
; OUTPUT: EAX = OS Status                      ;
;----------------------------------------------;
ErrorStringWrite:
cmp al,0
je ErrorWrite
SkipStrings:
cmp byte [ecx],0
je NextString
inc ecx
jmp SkipStrings
NextString:
dec al
jnz SkipStrings  
inc ecx
;---
ErrorWrite:
push ecx
lea ecx,[CrLf]
call StringWrite
pop ecx
call StringWrite
lea ecx,[CrLf]
; No RET, continue in the next subroutine

;--- String write in ASCII --------------------;
; INPUT:  ECX = Pointer to string              ;
;         ASCII string at ECX memory address   ;
;         string is null-terminated            ;
;         also global variables used           ;
; OUTPUT: EAX = OS Status                      ;
;----------------------------------------------;
StringWrite:
;--- Entry ---
push ebp eax                  ; EBP = non-volatile, EAX = for storage
mov ebp,esp                   ; RBP = storage for RSP and pointer to frame
;--- Calculate string length ---
xor eax,eax                   ; EAX = Number of chars
@@:
cmp byte [ecx+eax],0          ; Check current char from string
je @f                         ; Exit cycle if terminator (byte=0) found
inc eax                       ; Chars counter + 1
jmp @b                        ; Go next iteration
@@:
;--- Write console ---
push 0                        ; Parm#5 = Reserved
push ebp                      ; Parm#4 = Pointer to out. variable, count
push eax                      ; Parm#3 = Number of chars 
push ecx                      ; Parm#2 = Pointer to string
push [OutputDevice]           ; Parm#1 = Input device handle 
call [WriteConsole]           ; Display output
;--- Exit ---
pop ebp ebp
ret

