;------------------------------------------------------------------------------;
;              EFI Byte Code (EBC) applications patcher for Win32.             ;
;                      Can be run under Win32 and Win64.                       ;
;------------------------------------------------------------------------------; 

; Bug1. 
; If input file not exist, it created. 
; Can prevent this ? Or manually delete required ?
; OK, OPEN_ALWAYS replaced to OPEN_EXISTING, subroutine OpenReadFile.

DISABLE_ECHO_ALL = 0F9h

format PE console
entry start
include 'win32ax.inc'

;---------- Code section ------------------------------------------------------;
section '.code' code readable executable
start:
;--- Initializing console devices handles ---
push STD_INPUT_HANDLE       ; Request standard input device - keyboard  
call [GetStdHandle]
test eax,eax                ; 0 means error
jz ErrorSilent              ; Go if invalid input device handle
mov [InputDevice],eax       ; Store input device handle
push STD_OUTPUT_HANDLE      ; Request standard output device - display 
call [GetStdHandle]
test eax,eax                ; 0 means error
jz ErrorSilent              ; Go if invalid output device handle
mov [OutputDevice],eax      ; Store output device handle
;--- First message ---
lea ecx,[Msg1]
call StringWrite
;--- Get and scan command line, extract input and output files names ---
call [GetCommandLineA]
test eax,eax                ; 0 means error
mov bl,0                    ; Error phase = 0, bad command line
jz ErrorHandling            ; Go if get command line function error
cld
xchg esi,eax                ; ESI = Pointer for start parse command line 
mov ecx,200
call ScanForSpace           ; Skip this program name 
cmp al,' '
jne ErrorHandling           ; Go error if space (names separator) not found
call SkipExtraSpaces        ; Skip extra spaces 
cmp al,0
je ErrorHandling            ; Go error if unexpected end of line (no names) 
lea edi,[SrcFilePath]
call ExtractParameter       ; Extract first name 
cmp al,0
je ErrorHandling            ; Go error if unexp. end of line (no second name) 
call SkipExtraSpaces        ; Skip extra spaces 
cmp al,0
je ErrorHandling            ; Go error if unexp. end of line (no second name) 
lea edi,[DstFilePath]       ; Extract second name 
call ExtractParameter
;--- Visual extracted input and output files names ---
lea ecx,[Msg2]
call StringWrite
lea ecx,[SrcFilePath]
call StringWrite            ; Write input file name
lea ecx,[Msg3]
call StringWrite
lea ecx,[DstFilePath]
call StringWrite            ; Write output file name 
;--- Message "Working..." ---
lea ecx,[Msg4]
call StringWrite
;--- Open, read, close input file (UEFI x64 application) ---
lea ecx,[SrcFilePath]       ; ECX = Pointer to input file name ASCII string 
lea edx,[FileBuffer]        ; EDX = Pointer to buffer for read file
mov esi,edx                 ; ESI = Address, non volatile
mov eax,32768+1             ; EAX = File size limit
call OpenAndReadFile
test eax,eax                ; EAX = Status
jz ErrorHandling            ; Go if read file error, BL = phase indicator
jecxz ErrorHandling         ; Go if data size=0, OS can ret. no err. but size=0
;--- Check input file size ---
mov bl,4                    ; BL=4 means invalid file size error
cmp ecx,1024
jb ErrorHandling            ; Go error if input file size < 1024
cmp ecx,32768
ja ErrorHandling            ; Go error if input file size > 32768
;--- Check input file (UEFI x64 application) standard fields --- 
inc ebx                     ; BL=5 means invalid pointer in the input file
mov edi,[esi+003Ch]         ; EDI = Pointer to required block in the file
cmp edi,64
jb ErrorHandling            ; Go error if block offset < 64
cmp edi,ecx
ja ErrorHandling            ; Go error if block offset > file size
;--- Clear and calculate checksum ---
inc ebx                     ; BL=6 means wrong checksum
xor ebp,ebp                 ; EBP = 0 for blank and store checksum
xchg [esi+edi+0058h],ebp    ; Get and blank checksum field in the file
call ModuleCheckSum         ; EDX = Checksum for unchanged module
cmp edx,ebp                 ; EAX=Calculated checksum , EBP=Stored checksum
jne ErrorHandling           ; Go error if calculated and stored sum mismatch
;--- Patch module type ---
inc ebx                     ; BL=7 means wrong module signature
mov ax,00EBCh               ; New Machine Type = EBC
xchg ax,[esi+edi+0004h]     ; Patch Machine Type word
cmp ax,08664h               ; Old Machine Type must be = x64
jne ErrorHandling           ; Go error if current module singature mismatch
;--- Patch checksum ---     ; At this point: ESI=Pointer, ECX=Size
call ModuleCheckSum         ; EDX = Checksum for changed module
mov [esi+edi+0058h],edx     ; Patch checksum
;--- Create, write, close output file (UEFI EBC application)
xchg eax,ecx                ; EAX = File size, bytes
mov edx,esi                 ; EDX = Pointer to buffer
lea ecx,[DstFilePath]       ; ECX = Pointer to output file name ASCII string
call CreateAndWriteFile
test eax,eax                ; EAX = Status
jz ErrorHandling            ; Go if read file error, BL = phase indicator
jecxz ErrorHandling         ; Go if data size=0, OS can ret. no err. but size=0
;--- Last message ---
lea ecx,[MsgOK]
call StringWrite
;--- Exit point ---
ErrorSilent:
push 0                      ; Parm#1 = Exit code = 0 
call [ExitProcess]
;--- Error handling ---
ErrorHandling:
lea ecx,[ErrorStringsPool]  ; ECX = Pointer to errors messages strings pool
xchg eax,ebx                ; AL = Error string index
call ErrorStringWrite
jmp ErrorSilent 

;---------- Subroutines for debug ---------------------------------------------;
include 'ScanForSpace.inc'        ; Used for command line parse
include 'SkipExtraSpaces.inc'     ; Used for command line parse
include 'ExtractParameter.inc'    ; Used for command line parse
; include 'WaitKey.inc'           ; Wait for keyboard input (yet not used)
include 'StringWrite.inc'         ; Output string to display
include 'CreateAndWriteFile.inc'  ; Create new file and write
include 'OpenAndReadFile.inc'     ; Open existed file and read
include 'ModuleCheckSum.inc'      ; Module special checksum calculation  

;---------- Data section ------------------------------------------------------;
section '.data' data readable writeable
;--- Text data ---
CrLf    DB  0Ah,0Dh,0
Msg1    DB  0Ah,0Dh, 'UEFI x64 -> EBC patcher for FASM.'
        DB           ' (C) IC Book Labs. v2.0'
        DB  0Ah,0Dh, 'Starting...'    ,0
Msg2    DB  0Ah, 0Dh, 'Input file:  ' ,0
Msg3    DB  0Ah, 0Dh, 'Output file: ' ,0
Msg4    DB  0Ah, 0Dh, 'Working...'    ,0
MsgOK   DB  0Ah, 0Dh, 'OK.',0Ah,0Dh   ,0
;--- Error strings ---
ErrorStringsPool:
CmdParmsEStr:         ; BL=0
DB  0Ah,0Dh, 'Command string error, use EBCPATCH <InFile> <OutFile>.'
DB  07h,0Ah,0Dh,0
InFileOpenEStr:       ; BL=1
DB  0Ah,0Dh, 'Open input file error.',07h,0Ah,0Dh,0
InFileReadEStr:       ; BL=2
DB  0Ah,0Dh, 'Read input file error.',07h,0Ah,0Dh,0
InFileCloseEStr:      ; BL=3
DB  0Ah,0Dh, 'Close input file error.',07h,0Ah,0Dh,0
InFileBadSizeEStr:    ; BL=4
DB  0Ah,0Dh, 'Invalid input file size, support 1024-32768 bytes.'
DB  07h,0Ah,0Dh,0
PatchHStr:            ; BL=5
DB  0Ah,0Dh,'Input file bad, invalid offset at 003Ch.'
DB  07h,0Ah,0Dh,0
PatchSStr:            ; BL=6 
DB  0Ah,0Dh,'Input file bad, wrong checksum.'
DB  07h,0Ah,0Dh,0
PatchEStr:            ; BL=7
DB  0Ah,0Dh,'Input file bad, need [Header+04h]=8664h.'
DB  07h,0Ah,0Dh,0
OutFileCreateEStr:    ; BL=8
DB  0Ah,0Dh,'Output file create error.',07h,0Ah,0Dh,0
OutFileWriteEStr:     ; BL=9
DB  0Ah,0Dh,'Output file write error.',07h,0Ah,0Dh,0
OutFileCloseEStr:     ; BL=10
DB  0Ah,0Dh,'Output file close error.',07h,0Ah,0Dh,0
;--- Console devices handles ---
InputDevice   DD  ?             ; Handle for Input Device (example=keyboard)
OutputDevice  DD  ?             ; Handle for Output Device (example=display)
;--- Files names  from command string ---
SrcFilePath   DB  200 DUP (?)   ; Buffer for extract input file name
              DB  ?             ; Reserved space if algorithm overflows
DstFilePath   DB  200 DUP (?)   ; Buffer for extract output file name
              DB  ?             ; Reserved space if algorithm overflows
;--- Buffer for file I/O ---
align 16
FileBuffer    DB  32768 DUP (?) ; Buffer for file read-write
              DB  ?             ; Reserved space if algorithm overflows

;---------- Import data section -----------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'
include 'api\kernel32.inc'
include 'api\user32.inc'
