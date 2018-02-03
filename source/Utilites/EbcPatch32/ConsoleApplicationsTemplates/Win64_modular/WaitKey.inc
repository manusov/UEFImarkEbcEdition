;--- Wait for press any key -------------------;
; Echo and edit string mode disabled           ;
; INPUT:  None (but global variables used)     ;
; OUTPUT: RAX = Status                         ;
;         (global variables updated)           ;
;----------------------------------------------;
WaitKey:
;--- Entry ---
push rbx rbp rax rax          ; RBX, RBP = non-volatile, RAX = for storage
mov rbp,rsp                   ; RBP = storage for RSP and pointer to frame
and rsp,0FFFFFFFFFFFFFFF0h    ; Align stack
sub rsp,32                    ; Create parameters shadow
mov rbx,[InputDevice]         ; RBX = Storage for input device handle
;--- Get current console mode ---
mov rcx,rbx                   ; RCX = Parm#1 = Input device handle
mov rdx,rbp                   ; RDX = Parm#2 = Pointer to output variable 
call [GetConsoleMode]         ; Get current console mode
test rax,rax                  ; RAX = Status, 0 if error
jz @f                         ; Go exit function if error
;--- Change current console mode ---
mov rcx,rbx                   ; RCX = Parm#1 = Input device handle
mov edx,[rbp]                 ; RDX = Parm#2 = Console mode
and dl,DISABLE_ECHO_ALL       ; Disable echo and string in. (ret. after 1 char)
call [SetConsoleMode]         ; Get current console mode
test rax,rax                  ; RAX = Status, 0 if error
jz @f                         ; Go exit function if error
;--- Read console (wait only without echo) ---
mov rcx,rbx                   ; RCX = Parm#1 = Input device handle
lea rdx,[ReadBuffer]          ; RDX = Parm#2 = Pointer to in. buffer (not used)
mov r8d,1                     ; R8  = Parm#3 = Number of chars to Read
lea r9d,[rbp+8]               ; R9  = Parm#4 = Pointer to out. var., chars count
xor eax,eax                   ; EAX = 0
push rax rax                  ; Align stack + Parm#5 = InputControl
sub rsp,32                    ; Create parameters shadow
call [ReadConsole]            ; Keyboard input
add rsp,32+16                 ; Remove parameters shadow, parm#5, stack align
;--- Restore current console mode ---
mov rcx,rbx                   ; RCX = Parm#1 = Input device handle
xchg rbx,rax                  ; RBX = Save error code after input char
mov edx,[rbp]                 ; RDX = Parm#2 = Console mode
call [SetConsoleMode]         ; Get current console mode
;--- Error code = F(restore, input) ---
test rbx,rbx                  ; Check status after console input 
setnz bl                      ; BL=0 if input error, BL=1 if input OK
test rax,rax                  ; Check status after restore console mode
setnz al                      ; AL=0 if mode error, AL=1 if mode OK
and al,bl                     ; AL=1 only if both operations status OK
and eax,1                     ; Bit RAX.0=Valid, bits RAX.[63-1]=0
;--- Exit ---
@@:
mov rsp,rbp
pop rax rax rbp rbx
ret

