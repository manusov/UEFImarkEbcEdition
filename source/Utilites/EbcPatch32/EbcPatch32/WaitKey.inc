;--- Wait for press any key -------------------;
; Echo and edit string mode disabled           ;
; INPUT:  None (but global variables used)     ;
; OUTPUT: RAX = Status                         ; 
;         (global variables updated)           ;
;----------------------------------------------;
WaitKey:
;--- Entry ---
push ebx ebp                  ; EBX, EBP = non-volatile
mov ebx,[InputDevice]         ; EBX = Storage for input device handle
push 0 0                      ; Storages in the stack
mov ebp,esp                   ; EBP = Pointer to storage in the stack 
;--- Get current console mode ---
push ebp                      ; Parm#2 = Pointer to output variable 
push ebx                      ; Parm#1 = Input device handle
call [GetConsoleMode]         ; Get current console mode
test eax,eax                  ; EAX = Status, 0 if error
jz @f                         ; Go exit function if error
;--- Change current console mode ---
mov eax,[ebp]                 ; EAX = Console mode bitmap
and al,DISABLE_ECHO_ALL       ; Disable echo and string in. (ret. after 1 char)
push eax                      ; Parm#2 = Console mode
push ebx                      ; Parm#1 = Input device handle
call [SetConsoleMode]         ; Get current console mode
test eax,eax                  ; EAX = Status, 0 if error
jz @f                         ; Go exit function if error
;--- Read console (wait only without echo) ---
push 0                        ; Parm#5 = InputControl
lea eax,[ebp+4]
push eax                      ; Parm#4 = Pointer to out. var., chars count
push 1                        ; Parm#3 = Number of chars to Read
push ReadBuffer               ; Parm#2 = Pointer to in. buffer (not used)
push ebx                      ; Parm#1 = Input device handle
call [ReadConsole]            ; Keyboard input
;--- Restore current console mode ---
xchg ebx,eax                  ; EBX = Save error code after input char
push dword [ebp]              ; Parm#2 = Console mode
push ebx                      ; Parm#1 = Input device handle
call [SetConsoleMode]         ; Get current console mode
;--- Error code = F(restore, input) ---
test ebx,ebx                  ; Check status after console input 
setnz bl                      ; BL=0 if input error, BL=1 if input OK
test eax,eax                  ; Check status after restore console mode
setnz al                      ; AL=0 if mode error, AL=1 if mode OK
and al,bl                     ; AL=1 only if both operations status OK
and eax,1                     ; Bit EAX.0=Valid, bits EAX.[31-1]=0
;--- Exit ---
@@:
pop eax eax ebp ebx           ; EAX for remove parameters, EBP,EBX restored
ret

