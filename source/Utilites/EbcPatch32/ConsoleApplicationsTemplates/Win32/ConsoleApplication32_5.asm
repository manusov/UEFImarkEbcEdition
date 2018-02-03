
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
;invoke GetStdHandle,STD_OUTPUT_HANDLE
;mov [OutputDevice],eax
;invoke GetStdHandle,STD_INPUT_HANDLE
;mov [InputDevice],eax
;---
; invoke WriteConsole, [OutputDevice], StringGet, 200, CharInOut, 0
; invoke WriteConsole, [OutputDevice], StringWait, 10, CharInOut, 0
; invoke ReadConsole,[InputDevice], ReadBuffer, 40, CharInOut, 0
;---

;mov eax,[MyFileSize]
;lea edx,[MyFileData]
;lea ecx,[MyFileName1]
;call OpenAndReadFile
;
;lea eax,[MyFileData]
;mov byte [eax+24],'*'
;
;mov eax,[MyFileSize]
;lea edx,[MyFileData]
;lea ecx,[MyFileName2]
;call CreateAndWriteFile

;lea ecx,[MyString]
;call StringWrite
;call WaitKey

ErrorHandling:
ErrorSilent:

invoke ExitProcess, 0

;---------- Subroutines for debug ---------------------------------------------;

;--- Scan for space in the command line ---------;
; INPUT:   ESI = Source pointer, scanned string  ;
;          ECX = Size limit for cycle counter    ;
; OUTPUT:  ESI = Updated pointer by string scan  ;
;          AL  = Last detected ASCII char        ;  
;------------------------------------------------;
ScanForSpace:
@@:
lodsb
cmp al,' '
loopne @b
ret

;--- Skip spaces in the command line ------------;
; INPUT:   ESI = Source pointer, scanned string  ;
;          ECX = Size limit for cycle counter    ;
; OUTPUT:  ESI = Updated pointer by string scan  ;
;          AL  = Last detected ASCII char        ;  
;------------------------------------------------;
SkipExtraSpaces:
@@:
lodsb
cmp al,0
je @f 
cmp al,' '
loope @b
@@:
dec esi
ret

;--- Copy parameter from the command line -------;
; INPUT:   ESI = Source pointer, scanned string  ;
;          EDI = Destination pointer, buffer     ;
; OUTPUT:  ESI = Updated pointer by string copy  ;
;          EDI = Updated pointer by string copy  ;
;          AL  = Last detected ASCII char        ;
;                examples: 20h=SPACE=Next parm.  ;
;                          00h=End of string     ;
;------------------------------------------------;
ExtractParameter:
mov ecx,256-6
@@:
lodsb
cmp al,0
je @f
cmp al,' '
je @f
cmp al,'"'
je @b
stosb
jmp @b
@@:
push eax
mov al,0
stosb
pop eax
ret

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


;--- String write for errors messages ---------;
; INPUT:  AL = Error code selector, 0-based    ;
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

;--- Create and Write file --------------------;
; INPUT:   ECX = Pointer to file name          ;
;          EDX = Pointer to file content data  ;
;          EAX = File size, bytes              ;
; OUTPUT:  EAX = Status                        ;
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
; OUTPUT:  EAX = Status                        ;
;          ECX = File size                     ;
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
pop esi                       ; ESI = Number of bytes read
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
mov ecx,esi
pop edi esi
ret


;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable

VariablesPool:

InputDevice   DD  0               ; Handle for Input Device (example=keyboard)
OutputDevice  DD  0               ; Handle for Output Device (example=display)

StringGet     DB  200 DUP ('.')
StringWait    DB  0Ah,0Dh,'Wait... '  ; Visualized string name
CharInOut     DD  0                   ; Variable for number of chars

ReadBuffer    DB  100 DUP (0)     ; Buffer for string 
;---
MyString      DB  0Ah,0Dh,'Diagnostics message, press ENTER ...',0
;---
; MyFileName1   DB  'myfile1.txt',0
; MyFileName2   DB  'myfile2.txt',0
; MyFileData  DB  '[ ] Win32 file operations.'
; MyFileSize    DD  22+4  

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

InputFileName   DB  256 DUP (?)
OutputFileName  DB  256 DUP (?)
MyFileData      DB  256 DUP (?)

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
