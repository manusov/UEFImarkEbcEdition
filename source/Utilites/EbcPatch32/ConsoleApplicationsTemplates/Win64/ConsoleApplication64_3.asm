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
; Parameter #3 at application start = pointer to command line
; by WinMain convention.
; This doesn't work, probably for GUI only
;
;------------------------------------------------------------------------------;

format PE64 console
entry start
include 'win64a.inc'

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:

;--- Prepare stack ---
sub	rsp,8*5 			   ; Reserve stack for API use and make stack dqword aligned
;---

invoke GetCommandLineA
test rax,rax
jz SkipString
cld
mov rsi,rax
lea rdi,[StringGet]
@@:
lodsb
cmp al,0
je @f
stosb
jmp @b
@@:
SkipString:

;---
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov [OutputDevice],rax
invoke GetStdHandle,STD_INPUT_HANDLE
mov [InputDevice],rax
invoke WriteConsole, [OutputDevice], StringGet, 200, CharInOut, 0
invoke WriteConsole, [OutputDevice], StringWait, 10, CharInOut, 0
invoke ReadConsole,[InputDevice], ReadBuffer, 40, CharInOut, 0
invoke ExitProcess, 0

;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
StringGet     DB  200 DUP ('.')
StringWait    DB  0Ah, 0Dh, 'Wait... '  ; Visualized string name
CharInOut     DQ  0               ; Variable for number of chars
OutputDevice  DQ  0               ; Handle for Output Device (example=display)
InputDevice   DQ  0               ; Handle for Input Device (example=keyboard)
ReadBuffer    DB  100 DUP (0)     ; Buffer for string 

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
