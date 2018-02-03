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
;------------------------------------------------------------------------------;

format PE console
entry start
include 'win32ax.inc'

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:
invoke GetStdHandle,STD_OUTPUT_HANDLE
mov [OutputDevice],eax
invoke GetStdHandle,STD_INPUT_HANDLE
mov [InputDevice],eax
invoke WriteConsole, [OutputDevice], StringName, 9, CharInOut, 0
invoke ReadConsole,[InputDevice], ReadBuffer, 2, CharInOut, 0
invoke ExitProcess, 0

;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
StringName    DB  'Console: '     ; Visualized string name
CharInOut     DD  0               ; Variable for number of chars
OutputDevice  DD  0               ; Handle for Output Device (example=display)
InputDevice   DD  0               ; Handle for Input Device (example=keyboard)
ReadBuffer    DB  10 DUP (0)      ; Buffer for string 

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
