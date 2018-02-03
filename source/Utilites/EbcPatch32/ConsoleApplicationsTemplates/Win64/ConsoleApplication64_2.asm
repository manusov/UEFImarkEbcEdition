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

format PE64 console
entry start
include 'win64a.inc'

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:
;--- Prepare stack ---
sub	rsp,8*5 			   ; Reserve stack for API use and make stack dqword aligned
;---
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov [OutputDevice],rax
invoke GetStdHandle,STD_INPUT_HANDLE
mov [InputDevice],rax
;---
invoke WriteConsole, [OutputDevice], StringName, 9, CharInOut, 0
invoke ReadConsole,[InputDevice], ReadBuffer, 40, CharInOut, 0

;cld
;lea edi,[ReadBuffer]
;mov ecx,10
;mov al,0
;rep stosb
;mov [CharInOut],0

; Why this code exit without waiting input ?
; If number of accepted chars, extra chars accepted by next function.  

invoke WriteConsole, [OutputDevice], StringWait, 7, CharInOut, 0

;mov ecx,10000000h
;loop $

invoke WriteConsole, [OutputDevice], StringName2, 15, CharInOut, 0
invoke ReadConsole,[InputDevice], ReadBuffer, 2, CharInOut, 0

;---
invoke ExitProcess, 0

;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
StringName    DB  'Console: '     ; Visualized string name
StringWait    DB  'Wait...'
StringName2   DB  0Ah, 0Dh, 'Console (2): '
CharInOut     DQ  0               ; Variable for number of chars
OutputDevice  DQ  0               ; Handle for Output Device (example=display)
InputDevice   DQ  0               ; Handle for Input Device (example=keyboard)
ReadBuffer    DB  100 DUP (0)     ; Buffer for string 

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
