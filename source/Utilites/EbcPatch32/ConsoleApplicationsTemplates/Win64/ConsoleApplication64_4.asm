;---------------------- Win64 console application -----------------------------;
; Used functions:
;
; GetStdHandle
; Input:  Parm#1 = Handle type code for retrieve
; Output: EAX = Handle, if error 0 or INVALID_HANDLE_VALUE 
;
; WriteConsole
; Input:  Parm#1 = Handle of output device
;         Parm#2 = Pointer to buffer
;         Parm#3 = Number of characters to write
;         Parm#4 = Pointer to returned number of successfully chars write
;         Parm#5 = Reserved parameters must be 0 (NULL)
; Output: Status, Nonzero=OK, 0=Error 
;
; ReadConsole
; Input:  Parm#1 = Handle of input device
;         Parm#2 = Pointer to buffer
;         Parm#3 = Number of chars to read (limit, but not for edit)
;         Parm#4 = Pointer to returned number of cars read (before ENTER)
;         Parm#5 = Pointer to CONSOLE_READCONSOLE_CONTROL structure, 0=None
; Output: Status, Nonzero=OK, 0=Error
;
; ExitProcess
; Input:  Parm#1 = Exit code for parent process
; No output, because not return control to caller
;
;------------------------------------------------------------------------------;


; ENABLE_LINE_INPUT  = 000000002h
; DISABLE_LINE_INPUT = 0FFFFFFFDh
; ENABLE_ECHO_INPUT  = 000000004h
; DISABLE_ECHO_INPUT = 0FFFFFFFBh
DISABLE_ECHO_ALL   = 0F9h

include 'win64a.inc'

format PE64 console
entry start

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:
;--- Prepare stack ---
sub	rsp,8*5 		 ; Reserve stack for API use and make stack dqword aligned
;--- Initialization ---
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov [OutputDevice],rax
invoke GetStdHandle,STD_INPUT_HANDLE
mov [InputDevice],rax

;--- Classic input-output methods ---
;- invoke WriteConsole, [OutputDevice], StringName, 9, CharInOut, 0
;- invoke ReadConsole,[InputDevice], ReadBuffer, 2, CharInOut, 0

;--- Alternative input-output methods for debug ---
;- lea rcx,[StringWait]
;- call StringWrite
;- call WaitKey                            ; Not any key, chars only !

;--- File operations for debug ---

; INT3

; MAKE SUBROUTINES READ_FILE, WRITE_FILE, USE THIS CODE AS BASE.

;--- Create file ---
lea rcx,[MyFileName]                   ; Parm#1 = RCX = Pointer to file name
mov edx,GENERIC_READ + GENERIC_WRITE   ; Parm#2 = RDX = Desired access
xor r8d,r8d                            ; Parm#3 = R8  = Share mode
xor r9d,r9d                            ; Parm#4 = R9  = Security attributes
push r9 r9                             ; Alignment + Parm#7 = Template File (no)
push FILE_ATTRIBUTE_NORMAL             ; Parm#6 = File attributes

; select F(Read or Write)
; This for WRITE
push CREATE_ALWAYS                   ; Parm#5 = Create disposition

; This for READ
; push OPEN_ALWAYS
;

sub rsp,32                             ; Create parameters shadow
call [CreateFile]                      ; Create file, ret. RAX=Handle or 0=Error
add rsp,32+32                          ; Remove shadow, 3 parameters, align
test rax,rax
jz @f                                  ; Go exit if error
xchg rbx,rax                           ; RBX = Handle (XCHG for compact code)
;--- Write file ---

; INT3
; This for WRITE
xor eax,eax
push rax rax                  ; Variable + Parm#5 = Pointer to overlapped (no) 
mov rbp,rsp
mov rcx,rbx                   ; Parm#1 = RCX = File Handle
lea rdx,[MyFileData]          ; Parm#2 = RDX = Buffer
mov r8d,[MyFileSize]          ; Parm#3 = R8  = File Size
lea r9,[rsp+8]                ; Parm#4 = R9  = Pointer to variable = write size
sub rsp,32
call [WriteFile]
add rsp,32+8
pop rdx                       ; RDX = Number of bytes write
; ... use RDX ...

; INT3
; This for READ
;xor eax,eax
;push rax rax                  ; Variable + Parm#5 = Pointer to overlapped (no) 
;mov rbp,rsp
;mov rcx,rbx                   ; Parm#1 = RCX = File Handle
;lea rdx,[MyFileBuffer]        ; Parm#2 = RDX = Buffer
;mov r8d,[MyFileSize]          ; Parm#3 = R8  = File Size
;lea r9,[rsp+8]                ; Parm#4 = R9  = Pointer to variable = write size
;sub rsp,32
;call [ReadFile]
;add rsp,32+8
;pop rdx                       ; RDX = Number of bytes write
; ... use RDX ...


;--- Close file ---
mov rcx,rbx                   ; Parm#1 = RCX = Handle for close (file handle)
xchg rbx,rax                  ; RBX = Save error code after write file
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


;--- Exit ---
invoke ExitProcess, 0

;---------- Subroutines for debug ---------------------------------------------;

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


;--- Wait for press any key -------------------;
; Echo and edit string mode disabled           ;
; INPUT:  None (but global variables used)     ;
; OUTPUT: RAX = Status                         ;
;         global variables updated)            ;
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

;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
;--- Keyboard and Display (console) support --- 
StringName    DB  'Console: '     ; Visualized string name
StringWait    DB  'Press any key (char)...',0
CharInOut     DQ  0               ; Variable for number of chars
OutputDevice  DQ  0               ; Handle for Output Device (example=display)
InputDevice   DQ  0               ; Handle for Input Device (example=keyboard)
ReadBuffer    DB  10 DUP (0)      ; Buffer for string 
;--- File operations support ---
MyFileName    DB  'myfile.txt',0
MyFileData    DB  '... content of my file ...'
MyFileSize    DD  26
MyFileBuffer  DB  32 DUP (11h)

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
