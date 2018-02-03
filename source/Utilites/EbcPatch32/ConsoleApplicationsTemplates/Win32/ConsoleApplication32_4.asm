;---------------------- Win32 console application -----------------------------;
; Note. Win64 can run Win32 applications, but can't run Dos16 applications,
; compatibility mode can be used for 32-bit applications, but Virtual 8086
; mode not supported in the IA32e mode.
; For run Dos16 applications on Win64 platform, use virtualization technologies
; for run Win32 under Win64 and Dos16 under Win32 (nested virtualization).
;---
;
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
; Parameter #3 at application start = pointer to command line
; by WinMain convention.
; This doesn't work, probably for GUI only
;
;------------------------------------------------------------------------------;

; ENABLE_LINE_INPUT  = 000000002h
; DISABLE_LINE_INPUT = 0FFFFFFFDh
; ENABLE_ECHO_INPUT  = 000000004h
; DISABLE_ECHO_INPUT = 0FFFFFFFBh
DISABLE_ECHO_ALL   = 0F9h

format PE console
entry start
include 'win32ax.inc'

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:
;--- Initializing ---
; invoke GetCommandLineA
; test eax,eax
; jz SkipString
; cld
; mov esi,eax
; lea edi,[StringGet]
; @@:
; lodsb
; cmp al,0
; je @f
; stosb
; jmp @b
; @@:
; SkipString:
;---
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov [OutputDevice],eax
invoke GetStdHandle,STD_INPUT_HANDLE
mov [InputDevice],eax
;---
; invoke WriteConsole, [OutputDevice], StringGet, 200, CharInOut, 0
; invoke WriteConsole, [OutputDevice], StringWait, 10, CharInOut, 0
; invoke ReadConsole,[InputDevice], ReadBuffer, 40, CharInOut, 0
;---

mov eax,[MyFileSize]
lea edx,[MyFileData]
lea ecx,[MyFileName1]
call OpenAndReadFile

lea eax,[MyFileData]
mov byte [eax+24],'*'

mov eax,[MyFileSize]
lea edx,[MyFileData]
lea ecx,[MyFileName2]
call CreateAndWriteFile

lea ecx,[MyString]
call StringWrite

call WaitKey

invoke ExitProcess, 0

;---------- Subroutines for debug ---------------------------------------------;

;--- Wait for press any key -------------------;
; Echo and edit string mode disabled           ;
; INPUT:  None (but global variables used)     ;
; OUTPUT: Status                               ; 
;         but global variables updated)        ;
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

;--- String write in ASCII --------------------;
; INPUT:  ECX = Pointer to string              ;
;         ASCII string at ECX memory address   ;
;         string is null-terminated            ;
;         also global variables used           ;
; OUTPUT: OS Status                            ;
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

;--- Create and Write file --------------------;
; INPUT:   ECX = Pointer to file name          ;
;          EDX = Pointer to file content data  ;
;          EAX = File size, bytes              ;
; OUTPUT:  Status                              ;
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


;--- Open and Read file -----------------------;
; INPUT:   ECX = Pointer to file name          ;
;          EDX = Pointer to file data buffer   ;
;          EAX = File size, bytes              ;
; OUTPUT:  Status                              ;
;----------------------------------------------;
OpenAndReadFile:
push esi edi
mov esi,eax
mov edi,edx
;--- Create file ---
xor eax,eax                            ; EAX = 0
push eax                               ; Parm#7 = Template File (no)
push FILE_ATTRIBUTE_NORMAL             ; Parm#6 = File attributes
push OPEN_ALWAYS                       ; Parm#5 = Open disposition
push eax                               ; Parm#4 = Security attributes
push eax                               ; Parm#3 = Share mode
push GENERIC_READ + GENERIC_WRITE      ; Parm#2 = RDX = Desired access
push ecx                               ; Parm#1 = RCX = Pointer to file name
call [CreateFile]                      ; Create file, ret. RAX=Handle or 0=Error
test eax,eax
jz @f                                  ; Go exit if error
xchg ebx,eax                           ; EBX = Handle (XCHG for compact code)
;--- Read file ---
push 0                        ; Variable 
mov edx,esp                   ; EDX = Address of variable
push 0                        ; Parm#5 = Pointer to overlapped (no)
push edx                      ; Parm#4 = Pointer to variable = write size
push esi                      ; Parm#3 = File Size 
push edi                      ; Parm#2 = Buffer
push ebx                      ; Parm#1 = File Handle
call [ReadFile]
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


;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
StringGet     DB  200 DUP ('.')
StringWait    DB  0Ah, 0Dh, 'Wait... '  ; Visualized string name
CharInOut     DD  0               ; Variable for number of chars
OutputDevice  DD  0               ; Handle for Output Device (example=display)
InputDevice   DD  0               ; Handle for Input Device (example=keyboard)
ReadBuffer    DB  100 DUP (0)     ; Buffer for string 
;---
MyString      DB  'Diagnostics message, press ENTER ...',0
;---
MyFileName1   DB  'myfile1.txt',0
MyFileName2   DB  'myfile2.txt',0
; MyFileData  DB  '[ ] Win32 file operations.'
MyFileData    DB  32 DUP (0)
MyFileSize    DD  22+4  


;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
