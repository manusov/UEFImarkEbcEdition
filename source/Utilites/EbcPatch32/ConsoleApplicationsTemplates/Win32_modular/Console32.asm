
DISABLE_ECHO_ALL = 0F9h

format PE console
entry start
include 'win32ax.inc'

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:
;--- Initializing variables pointer ---
; lea ebx,[VariablesPool]
;--- Initializing input device handle (keyboard) ---
push STD_INPUT_HANDLE  
call [GetStdHandle]
test eax,eax
jz ErrorSilent
; mov [ebx+00],eax
mov [InputDevice],eax
;--- Initializing output device handle (display) ---
push STD_OUTPUT_HANDLE
call [GetStdHandle]
test eax,eax
jz ErrorSilent
; mov [ebx+04],eax
mov [OutputDevice],eax
;--- Visual first message and command line ---
lea ecx,[FirstMessage]
call StringWrite
;--- Get command line ---
call [GetCommandLineA]
test eax,eax
jz ErrorHandling
;--- Parse command line --- 
cld
xchg esi,eax
;--- Skip this program name ---
mov ecx,132
call ScanForSpace
cmp al,' '
jne ErrorHandling
;--- Skip extra spaces ---
call SkipExtraSpaces
cmp al,0
je ErrorHandling 
;--- Extract first name ---
lea edi,[InputFileName]
call ExtractParameter
cmp al,0
je ErrorHandling
;--- Skip extra spaces ---
call SkipExtraSpaces
cmp al,0
je ErrorHandling 
;--- Extract second name ---
lea edi,[OutputFileName]
call ExtractParameter
; This used if extra parameters is error
;- cmp al,0
;- jne ErrorHandling
;--- Visual input file name ---
lea ecx,[InputFileMessage]
call StringWrite
lea ecx,[InputFileName]
call StringWrite
;--- Visual output file name ---
lea ecx,[OutputFileMessage]
call StringWrite
lea ecx,[OutputFileName]
call StringWrite
;--- Message "Working..." ---
lea ecx,[WorkingMessage]
call StringWrite

;--- Read input file ---
lea ecx,[InputFileName]
lea edx,[MyFileData]
mov eax,256
call OpenAndReadFile
test eax,eax
jz ErrorHandling
;--- Modify buffer ---
; ...
; ...
;--- Write output file ---
mov eax,ecx
lea ecx,[OutputFileName]
lea edx,[MyFileData]
call CreateAndWriteFile
test eax,eax
jz ErrorHandling

;--- Final message ---
lea ecx,[ExecutedMessage]
call StringWrite

; DEBUG

;--- Error message ---
;lea ecx,[ErrorMessages]
;mov al,1
;call ErrorStringWrite


ErrorHandling:
ErrorSilent:

invoke ExitProcess, 0

;---------- Subroutines for debug ---------------------------------------------;
include 'ScanForSpace.inc'
include 'SkipExtraSpaces.inc'
include 'ExtractParameter.inc'
include 'WaitKey.inc'
include 'StringWrite.inc'
include 'CreateAndWriteFile.inc'
include 'OpenAndReadFile.inc'

;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
InputDevice   DD  0               ; Handle for Input Device (example=keyboard)
OutputDevice  DD  0               ; Handle for Output Device (example=display)
;---
FirstMessage:
DB  0Ah,0Dh,'Program first message at this point.',0
InputFileMessage:
DB  0Ah,0Dh,'Input file:  ',0
OutputFileMessage:
DB  0Ah,0Dh,'Output file: ',0
WorkingMessage:
DB  0Ah,0Dh,'Working...',0
ExecutedMessage:
DB  0Ah,0Dh,'Executed OK.',0Ah,0Dh,0
ErrorMessages:
DB  'Command line error, use PROGRAM inputfile.ext outputfile.ext',0
DB  'Error reading input file',0
DB  'Error input file format',0
DB  'Error writing output file',0
CrLf:
DB  0Ah,0Dh,0
ReadBuffer      DB  100 DUP (?)     ; Buffer for string , old fill 0
InputFileName   DB  256 DUP (?)
OutputFileName  DB  256 DUP (?)
MyFileData      DB  256 DUP (?)

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
