;---------------------------------------------------------------------------;
;									                                                          ;
;	              Get Command Line sample by WinMain convention.              ;
;									                                                          ;
;---------------------------------------------------------------------------;

WorkCycles = 1 

;---------- Header ---------------------------------------------------------;
format PE64 GUI 5.0
entry start
include 'win64a.inc'
;---------- Code section ---------------------------------------------------;
section '.text' code readable executable
start:

;--- Get WinMain parameter#3 = command line ---
; This useable for graphics (GUI) applications only ?
; mov r15,r8

;--- Prepare stack ---
sub	rsp,8*5 			   ; Reserve stack for API use and make stack dqword aligned
;--- Get start time (OS time and CPU Time Stamp Counter) ---
lea	rcx,[File_Time_Start]
call [GetSystemTimeAsFileTime]	; Save start time - File Time
rdtsc
shl	rdx,32
or rax,rdx
mov	[TSC_Start],rax 		        ; Save start time - TSC

;---------- Start target debug fragment ------------------------------------;

; INT3

call [GetCommandLineA]
;mov r15,rax
;mov rax,[r15+00]
;mov rbx,[r15+08]
;mov rcx,[r15+16]
;mov rdx,[r15+24]

mov rdx,rax           ; RDX = Parm #2 = Message
lea r8,[_Caption]     ; R8  = Parm #3 = Caption (upper message)
xor r9,r9             ; R9  = Parm #4 = Message flags
jmp MessageAndExit


;---------- Start visual data ----------------------------------------------;
;--- Reserve stack frame ---
sub	rsp,128+64+512
;--- Save CPU state for visual ---
mov [rsp+000+000],rax
mov [rsp+008+000],rbx
mov [rsp+016+000],rcx
mov [rsp+024+000],rdx
lea rax,[rsp-32-128-64-512]
mov [rsp+032+000],rax
mov [rsp+040+000],rbp
mov [rsp+048+000],rsi
mov [rsp+056+000],rdi
mov [rsp+064+000],r8
mov [rsp+072+000],r9
mov [rsp+080+000],r10
mov [rsp+088+000],r11
mov [rsp+096+000],r12
mov [rsp+104+000],r13
mov [rsp+112+000],r14
mov [rsp+120+000],r15
fstp qword [rsp+00+128]
fstp qword [rsp+08+128]
fstp qword [rsp+16+128]
fstp qword [rsp+24+128]
fstp qword [rsp+32+128]
fstp qword [rsp+40+128]
fstp qword [rsp+48+128]
fstp qword [rsp+56+128]
vmovupd [rsp+32*00+192],ymm0
vmovupd [rsp+32*01+192],ymm1
vmovupd [rsp+32*02+192],ymm2
vmovupd [rsp+32*03+192],ymm3
vmovupd [rsp+32*04+192],ymm4
vmovupd [rsp+32*05+192],ymm5
vmovupd [rsp+32*06+192],ymm6
vmovupd [rsp+32*07+192],ymm7
vmovupd [rsp+32*08+192],ymm8
vmovupd [rsp+32*09+192],ymm9
vmovupd [rsp+32*10+192],ymm10
vmovupd [rsp+32*11+192],ymm11
vmovupd [rsp+32*12+192],ymm12
vmovupd [rsp+32*13+192],ymm13
vmovupd [rsp+32*14+192],ymm14
vmovupd [rsp+32*15+192],ymm15
;--- Get stop time ---
rdtsc
shl rdx,32
or rax,rdx
mov [TSC_Current],rax            ; Save stop time - TSC
sub rsp,32		                   ; Otherwise corrupt QWORD [RSP]
lea	rcx,[File_Time_Current]
call	[GetSystemTimeAsFileTime]  ; Save stop time - File Time
add	rsp,32
;--- Additional fragment for T1, T2 ---
; Non-optimal because separate module
mov rax,[File_Time_Start]
xor edx,edx
mov rcx,10000000*100000000h
div rcx
xchg rax,rdx       ; RAX = Start time - OverConstant, units=100ns
mov rbp,10000000
xor edx,edx
div rbp            ; RAX = Start time - OverConstant, units=1sec
mov bl,0
lea rdi,[_T1]
call Decimal_Print_32
mov	rax,[File_Time_Current]
xor	edx,edx
div	rcx
xchg	rax,rdx      ; RAX = Stop time - OverConstant, units=100ns
xor	edx,edx
div	rbp            ; RAX = Stop time - OverConstant, units=1sec
lea	rdi,[_T2]
call Decimal_Print_32
;--- Analysing and dump timings results ---
mov rax,[File_Time_Current]
sub rax,[File_Time_Start]
mov rbx,10000
xor rdx,rdx
div rbx			; RAX = Miliseconds
mov rsi,rax
mov bl,0
mov rdi,_DTMS
call Decimal_Print_32
mov rax,[TSC_Current]
sub rax,[TSC_Start]
test rsi,rsi
jz M1
xor rdx,rdx
div rsi			; RAX = dTSC per ms
mov rbx,1000
xor rdx,rdx
div rbx			; RAX = MHz
mov bl,0
mov rdi,_DMHZ
call Decimal_Print_32
mov rax,[TSC_Current]
sub rax,[TSC_Start]
;---------- Duplicate clock measurement, result = ST0 ----------------------;
push rax			         ; Store DeltaTSC in the stack frame
mov rax,WorkCycles     ; Old = BlockX*BlockY*LinesA
push rax			         ; Store constant in the stack frame
fild qword [rsp+08]
fild qword [rsp+00]	   ; ST0 = Number of instructions, ST1 = Number of clocks
fdivp				           ; ST0 = Number of clocks / Number of instructions
pop rax rax 		       ; Clear stack
fstp qword [rsp+00+128]	 ; Update ST0 in the stack frame
;----------Make patch for visual clocks with precision 1/100 ---------------;
mov rbx,WorkCycles/100   ; Old = BlockX*BlockY*LinesA / 100
test rbx,rbx
jz M1
xor edx,edx
div rbx
mov ebx,100
xor edx,edx
div ebx
lea rdi,[_DCLK]
mov bl,0
call Decimal_Print_32
mov al,'.'
stosb
mov bl,2
xchg eax,edx
call Decimal_Print_32		
M1:
;---------------------------------------------------------------------------;
;--- Visual CPU x86 general purpose registers state ---
lea	rdi,[_RAX]
mov rax,[rsp+000]
call Hex_Print_64
lea	rdi,[_RBX]
mov	rax,[rsp+008]
call Hex_Print_64
lea rdi,[_RCX]
mov	rax,[rsp+016]
call Hex_Print_64
lea	rdi,[_RDX]
mov rax,[rsp+024]
call Hex_Print_64
lea	rdi,[_RSP]
mov rax,[rsp+032]
call Hex_Print_64
lea	rdi,[_RBP]
mov rax,[rsp+040]
call Hex_Print_64
lea	rdi,[_RSI]
mov rax,[rsp+048]
call Hex_Print_64
lea	rdi,[_RDI]
mov	rax,[rsp+056]
call Hex_Print_64
lea	rdi,[_R8]
mov rax,[rsp+064]
call Hex_Print_64
lea	rdi,[_R9]
mov rax,[rsp+072]
call Hex_Print_64
lea	rdi,[_R10]
mov rax,[rsp+080]
call Hex_Print_64
lea	rdi,[_R11]
mov rax,[rsp+088]
call Hex_Print_64
lea rdi,[_R12]
mov rax,[rsp+096]
call Hex_Print_64
lea	rdi,[_R13]
mov rax,[rsp+104]
call Hex_Print_64
lea	rdi,[_R14]
mov rax,[rsp+112]
call Hex_Print_64
lea	rdi,[_R15]
mov rax,[rsp+120]
call Hex_Print_64
;--- Visual FPU x87 floating point registers state ---
lea rdi,[_ST0]
mov rax,[rsp+00+128]
call Floating_Print_64
lea rdi,[_ST1]
mov rax,[rsp+08+128]
call Floating_Print_64
lea rdi,[_ST2]
mov rax,[rsp+16+128]
call Floating_Print_64
lea rdi,[_ST3]
mov rax,[rsp+24+128]
call Floating_Print_64
lea rdi,[_ST4]
mov rax,[rsp+32+128]
call Floating_Print_64
lea rdi,[_ST5]
mov rax,[rsp+40+128]
call Floating_Print_64
lea rdi,[_ST6]
mov rax,[rsp+48+128]
call Floating_Print_64
lea rdi,[_ST7]
mov rax,[rsp+56+128]
call Floating_Print_64
;--- Visual AVX registers state (as floating point double precision) ---
lea rdi,[_YMM0]
lea rbp,[rsp+32*00+192]
call AVX_Floating_Print_64
lea rdi,[_YMM1]
lea rbp,[rsp+32*01+192]
call AVX_Floating_Print_64
lea rdi,[_YMM2]
lea rbp,[rsp+32*02+192]
call AVX_Floating_Print_64
lea rdi,[_YMM3]
lea rbp,[rsp+32*03+192]
call AVX_Floating_Print_64
lea rdi,[_YMM4]
lea rbp,[rsp+32*04+192]
call AVX_Floating_Print_64
lea rdi,[_YMM5]
lea rbp,[rsp+32*05+192]
call AVX_Floating_Print_64
lea rdi,[_YMM6]
lea rbp,[rsp+32*06+192]
call AVX_Floating_Print_64
lea rdi,[_YMM7]
lea rbp,[rsp+32*07+192]
call AVX_Floating_Print_64
lea rdi,[_YMM8]
lea rbp,[rsp+32*08+192]
call AVX_Floating_Print_64
lea rdi,[_YMM9]
lea rbp,[rsp+32*09+192]
call AVX_Floating_Print_64
lea rdi,[_YMM10]
lea rbp,[rsp+32*10+192]
call AVX_Floating_Print_64
lea rdi,[_YMM11]
lea rbp,[rsp+32*11+192]
call AVX_Floating_Print_64
lea rdi,[_YMM12]
lea rbp,[rsp+32*12+192]
call AVX_Floating_Print_64
lea rdi,[_YMM13]
lea rbp,[rsp+32*13+192]
call AVX_Floating_Print_64
lea rdi,[_YMM14]
lea rbp,[rsp+32*14+192]
call AVX_Floating_Print_64
lea  rdi,[_YMM15]
lea rbp,[rsp+32*15+192]
call AVX_Floating_Print_64
;--- Remove stack frame ---
add rsp,128+64+512
;--- Visualize text strings in the window ---
lea rdx,[_Message]    ; RDX = Parm #2 = Message
lea r8,[_Caption]     ; R8  = Parm #3 = Caption (upper message)
xor r9,r9             ; R9  = Parm #4 = Message flags
MessageAndExit:
xor rcx,rcx 		      ; RCX = Parm #1 = Parent window
call [MessageBoxA]
;--- Exit program ---
mov ecx,eax           ; ECX = Parm #1
call [ExitProcess]
;--- Output message if memory allocation error ---
MemoryAllocationError:
lea rdx,[_ErrorMessage]
xor r8d,r8d
mov r9d,MB_ICONERROR
jmp MessageAndExit
;---------- Service subroutines --------------------------------------------;
;--- Print 256-bit vector as 4 FP DP numbers ------------------;
; Print 4 x 64-bit Floating point double precision numbers     ;
; Note. FISTTP instruction manipulate with x87,                ;
; but must be validated by SSE3.                               ;
;	                                                             ;
; INPUT:   RBP = Pointer to number values                      ;
;          RDI = Destination Pointer (flat)                    ;
; OUTPUT:  RDI = New Destination Pointer (flat)                ;
;                modified because string write                 ;
;--------------------------------------------------------------;
AVX_Floating_Print_64:
push rax rcx rdi rbp
mov ecx,4
AFP64_00:
mov rax,[rbp]
push rdi
call Floating_Print_64
pop rdi
add rbp,8
add rdi,14
loop AFP64_00
pop rbp rdi rcx rax
ret
;--- Print 64-bit Floating point double precision number ---------;
; Version 2 for correct visual big numbers,                       ;
; required next modification for small numbers                    ;
; Notes.                                                          ;
; FISTTP instruction manipulate with x87,                         ;
; but must be validated by CPUID.SSE3.                            ;
; FCOMI instruction must be validated by                          ;
; CPUID.CMOV and CPUID.x87.                                       ;
;                                                                 ;
; INPUT:   RAX = Number value (Floation Point Double Precision)   ;
;          RDI = Destination Pointer (flat)                       ;
; OUTPUT:  RDI = New Destination Pointer (flat)                   ;
;		             modified because string write                    ;
;-----------------------------------------------------------------;
Floating_Print_64:
;--- Save parameters, built stack frame ---
push rax rbx rdx rax rax  ; Save input and built scratch pad (rax)
pushq 999                 ; [rsp+24] = Integer value, used as big number new limit
pushq 10                  ; [rsp+16] = Integer value, used as float divisor
pushq 9999                ; [rsp+08] = Integer value, used as big number limit
pushq 10000000            ; [rsp+00] = Integer value, used as float multiplier
;--- Check and write sign "+" or "-" ---
; No STOSB, can't corrupt RAX
btr rax,63                ; SF flag = bit RAX.63 (sign) , clear RAX.63=0
mov byte [rdi],' '        ; Write " " if sign = plus
jnc FP64_00               ; Go if CF flag = 0
mov byte [rdi],'-'        ; Write "-" if sign = minus
FP64_00:
inc rdi                   ; Output string pointer +1
;--- Detect special cases ---
test rax,rax
jz FP64_Zero              ; Go if special case = 0.0
mov rbx,07FF8000000000000h
cmp rax,rbx
je FP64_QNAN              ; Go if special case = QNAN
mov rbx,07FF0000000000000h
cmp rax,rbx
je FP64_INF              ; Go if special case = INF
ja FP64_NAN              ; Go if special case = NAN
;--- Load input number(x), get abs(x) ---
fld qword [rsp+32]       ; Load input number to ST0
fabs                     ; Make positive number (absolute)
;--- Check for big number ---
fild qword [rsp+08]      ; Load big number limit
fcomip st1
jc FP64_Big
;--- Number calculations ---
fld st0                  ; Duplicate input number
fisttp qword [rsp+32]    ; Store integer part
fild qword [rsp+32]      ; Load integer part
fxch st1                 ; Exchange, st0=full, st1=integer
fsub st0,st1             ; Subtract integer, st0=float
fimul dword [rsp+00]     ; Multiply, st0=float*10000000
fisttp qword [rsp+40]    ; Store float part as integer 0-10000000
fstp st                  ; Free ST0
;--- Write number integer.float ---
mov eax,[rsp+32]		     ; Load EAX=Integer , high 32 bits = 0
mov bl,0                 ; BL=0, no chars template
call Decimal_Print_32	   ; Built ASCII string for integer
mov al,'.'
stosb                    ; Write decimal point
mov eax,[rsp+40]         ; Load EAX=Float
mov bl,7                 ; BL=7, template=7 chars
call Decimal_Print_32    ; Built ASCII string for float
;--- Remove stack frame and return ---
FP64_01:
add	rsp,48               ; Remove scratch pads
pop	rdx rbx rax          ; Restore registers
ret                      ; Return to caller
;--- Handling big numbers ---
FP64_Big:
mov dword [rsp+00],2
FP64_03:
inc dword [rsp+00]
fidiv word [rsp+16]
fild qword [rsp+24]      ; Load show big number limit
fcomip st1
jc FP64_03
fistp dword [rsp+32]     ; Store integer part
mov eax,[rsp+32]         ; Load integer part
xor edx,edx              ; Dividend high 32 bits = 0
mov ebx,100
div ebx
mov bl,0                 ; BL=0, no chars template
call Decimal_Print_32    ; Built ASCII string for integer
mov al,'.'
stosb                    ; Write decimal point
mov eax,edx
mov bl,2
call Decimal_Print_32    ; Built ASCII string for float
mov al,'E'
stosb                    ; Write "E"
mov eax,[rsp+00]         ; Get decimal exponent
call Decimal_Print_32    ; Built ASCII string for exponent
jmp FP64_01
;--- Handling special cases ---
FP64_Zero:					; Zero
mov eax,'0.0 '
jmp FP64_02
FP64_INF:                ; "INF" = Infinity
mov eax,'INF '
jmp FP64_02
FP64_NAN:
mov eax,'NAN '           ; "NAN" = Not a number
jmp FP64_02
FP64_QNAN:
mov eax,'QNAN'           ; "QNAN" = Quiet not a number
jmp FP64_02
FP64_02:
stosd
jmp FP64_01
;---------- Print 32-bit Decimal Number -----------------------;
; INPUT:   EAX = Number value                                  ;
;          BL  = Template size, chars. 0=No template           ;
;          RDI = Destination Pointer (flat)                    ;
; OUTPUT:  RDI = New Destination Pointer (flat)                ;
;                modified because string write                 ;
;--------------------------------------------------------------;
Decimal_Print_32:
cld
push rax rbx rcx rdx
mov bh,80h-10
add bh,bl
mov ecx,1000000000
.MainCycle:
xor edx,edx
div ecx         ; Produce current digit
and al,0Fh
test bh,bh
js .FirstZero
cmp ecx,1
je .FirstZero
cmp al,0        ; Not actual left zero ?
jz .SkipZero
.FirstZero:
mov bh,80h      ; Flag = 1
or al,30h
stosb           ; Store char
.SkipZero:
push rdx
xor edx,edx
mov eax,ecx
mov ecx,10
div ecx
mov ecx,eax
pop rax
inc bh
test ecx,ecx
jnz .MainCycle
pop rdx rcx rbx rax
ret
;---------- Print 64-bit Hex Number ---------------------------;
; INPUT:  RAX = Number                                         ;
;         RDI = Destination Pointer                            ;
; OUTPUT: RDI = Modify                                         ;
;--------------------------------------------------------------;
Hex_Print_64:
push rax
ror rax,32
call Hex_Print_32
pop rax
; no RET, continue at next subroutine
;---------- Print 32-bit Hex Number ---------------------------;
; INPUT:  EAX = Number                                         ;
;         RDI = Destination Pointer                            ;
; OUTPUT: RDI = Modify                                         ;
;--------------------------------------------------------------;
Hex_Print_32:
push rax
ror eax,16
call Hex_Print_16
pop rax
; no RET, continue at next subroutine
;---------- Print 16-bit Hex Number ---------------------------;
; INPUT:  AX  = Number                                         ;
;         RDI = Destination Pointer                            ;
; OUTPUT: RDI = Modify                                         ;
;--------------------------------------------------------------;
Hex_Print_16:
push rax
xchg al,ah
call Hex_Print_8
pop rax
; no RET, continue at next subroutine
;---------- Print 8-bit Hex Number ----------------------------;
; INPUT:  AL  = Number                                         ;
;         RDI = Destination Pointer                            ;
; OUTPUT: RDI = Modify	                                       ;
;--------------------------------------------------------------;
Hex_Print_8:
push rax
ror al,4
call Hex_Print_4
pop rax
; no RET, continue at next subroutine
;---------- Print 4-bit Hex Number ----------------------------;
; INPUT:  AL  = Number (bits 0-3)                              ;
;         RDI = Destination Pointer                            ;
; OUTPUT: RDI = Modify                                         ;
;--------------------------------------------------------------;
Hex_Print_4:
cld
push rax
and al,0Fh
cmp al,9
ja .HP4_AF
add al,'0'
jmp .HP4_Store
.HP4_AF:
add al,'A'-10
.HP4_Store:
stosb
pop rax
ret
;---------- Data section ---------------------------------------------------;
section '.data' data readable writeable
;--- Visualizer data ---
_Caption       DB '  Debug dump',0
_ErrorMessage  DB 'Memory allocation error',0
;--- Registers dump window ---
_Message DB 'RAX   =    '
_RAX     DB 18 DUP (' '),0Ah,0Dh
         DB 'RBX   =    '
_RBX     DB 18 DUP (' '),0Ah,0Dh
         DB 'RCX   =    '
_RCX     DB 18 DUP (' '),0Ah,0Dh
         DB 'RDX   =    '
_RDX     DB 18 DUP (' '),0Ah,0Dh
         DB 'RSP   =    '
_RSP     DB 18 DUP (' '),0Ah,0Dh
         DB 'RBP   =    '
_RBP     DB 18 DUP (' '),0Ah,0Dh
         DB 'RSI   =    '
_RSI     DB 18 DUP (' '),0Ah,0Dh
         DB 'RDI   =    '
_RDI     DB 18 DUP (' '),0Ah,0Dh
         DB 'R8    =    '
_R8      DB 18 DUP (' '),0Ah,0Dh
         DB 'R9    =    '
_R9      DB 18 DUP (' '),0Ah,0Dh
         DB 'R10   =    '
_R10     DB 18 DUP (' '),0Ah,0Dh
         DB 'R11   =    '
_R11     DB 18 DUP (' '),0Ah,0Dh
         DB 'R12   =    '
_R12     DB 18 DUP (' '),0Ah,0Dh
         DB 'R13   =    '
_R13     DB 18 DUP (' '),0Ah,0Dh
         DB 'R14   =    '
_R14     DB 18 DUP (' '),0Ah,0Dh
         DB 'R15   =    '
_R15     DB 18 DUP (' '),0Ah,0Ah,0Dh
         DB 'ST0   =    '
_ST0     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST1   =    '
_ST1     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST2   =    '
_ST2     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST3   =    '
_ST3     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST4   =    '
_ST4     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST5   =    '
_ST5     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST6   =    '
_ST6     DB 30 DUP (' '),0Ah,0Dh
         DB 'ST7   =    '
_ST7     DB 30 DUP (' '),0Ah,0Ah,0Dh
         DB 'YMM0  =     '
_YMM0    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM1  =     '
_YMM1    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM2  =     '
_YMM2    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM3  =     '
_YMM3    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM4  =     '
_YMM4    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM5  =     '
_YMM5    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM6  =     '
_YMM6    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM7  =     '
_YMM7    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM8  =     '
_YMM8    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM9  =     '
_YMM9    DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM10 =     '
_YMM10   DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM11 =     '
_YMM11   DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM12 =     '
_YMM12   DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM13 =     '
_YMM13   DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM14 =     '
_YMM14   DB 60 DUP (' '),0Ah,0Dh
         DB 'YMM15 =     '
_YMM15   DB 60 DUP (' '),0Ah,0Ah,0Dh
         DB 'dT(ms) =  '
_DTMS    DB 20 DUP (' '),0Ah,0Dh
;--- Add for T1, T2 ------------------;
         DB 'T1(s) =  '
_T1      DB 20 DUP (' '),0Ah,0Dh
         DB 'T2(s) =  '
_T2      DB 20 DUP (' '),0Ah,0Dh
;-------------------------------------;
         DB 'dTSC/Sec (MHz) =  '
_DMHZ    DB 20 DUP (' '),0Ah,0Dh
         DB 'dTSC/Pass (Clks) =  '
_DCLK    DB 20 DUP (' '),0
;--- Time variables ---
File_Time_Start     DQ  0
File_Time_Current   DQ  0
TSC_Start           DQ  0
TSC_Current         DQ  0

;--- Data for matrix multiply routine debug ---

align 32
Counter  DQ  0


;---------- Import data section --------------------------------------------;
section '.idata' import data readable writeable
library kernel32 , 'KERNEL32.DLL' , user32 , 'USER32.DLL'

;include 'api\kernel32.inc'
include 'api\user32.inc'
; This file not used because NUMA API declaration missing
; include 'api\kernel32.inc'
import kernel32, \
ExitProcess, 'ExitProcess',\
GetSystemInfo, 'GetSystemInfo',\
HeapCreate, 'HeapCreate',\
HeapAlloc, 'HeapAlloc',\
HeapFree, 'HeapFree',\
HeapDestroy, 'HeapDestroy',\
GetSystemTimeAsFileTime, 'GetSystemTimeAsFileTime',\
CreateEvent, 'CreateEventA',\
CloseHandle, 'CloseHandle',\
WaitForMultipleObjects, 'WaitForMultipleObjects',\
CreateThread, 'CreateThread',\
SetEvent, 'SetEvent',\
ResetEvent, 'ResetEvent',\
GetNumaHighestNodeNumber, 'GetNumaHighestNodeNumber',\
GetNumaNodeProcessorMask, 'GetNumaNodeProcessorMask',\
VirtualAllocExNuma, 'VirtualAllocExNuma',\
VirtualFreeEx, 'VirtualFreeEx',\
SetThreadAffinityMask, 'SetThreadAffinityMask',\
GetCurrentProcessId, 'GetCurrentProcessId',\
GetCurrentProcess, 'GetCurrentProcess',\
GetLastError, 'GetLastError',\
GetModuleHandle, 'GetModuleHandleA',\
GlobalMemoryStatusEx, 'GlobalMemoryStatusEx',\
GetLogicalProcessorInformation, 'GetLogicalProcessorInformation',\
GetSystemFirmwareTable, 'GetSystemFirmwareTable',\
VirtualAlloc, 'VirtualAlloc',\
VirtualFree, 'VirtualFree',\
GetCurrentThread, 'GetCurrentThread',\
GetLargePageMinimum, 'GetLargePageMinimum',\
GetCommandLineA, 'GetCommandLineA'

