;===========================================================================;
;=                                                                         =;
;=              X86-EXECUTABLE HANDLERS CALLED FROM EBC CODE.              =;
;=     GET PRIVELEGE, CPU DETECTION WITH CPUID, MEASURE CLOCK FREQUENCY.   =;
;=                                                                         =;
;===========================================================================;

;---------- Equations for call subroutines ---------------------------------;

_IA32_Get_CPL			= IA32_Get_CPL - Global_Variables_Pool
_x64_Get_CPL			= x64_Get_CPL - Global_Variables_Pool
_IA32_Check_CPUID		= IA32_Check_CPUID - Global_Variables_Pool
_x64_Check_CPUID		= x64_Check_CPUID - Global_Variables_Pool
_IA32_Get_CPUID			= IA32_Get_CPUID - Global_Variables_Pool
_x64_Get_CPUID			= x64_Get_CPUID - Global_Variables_Pool
_IA32_Measure_TSC		= IA32_Measure_TSC - Global_Variables_Pool
_x64_Measure_TSC		= x64_Measure_TSC - Global_Variables_Pool
_IA32_Checkpoint		= IA32_Checkpoint - Global_Variables_Pool
_x64_Checkpoint			= x64_Checkpoint - Global_Variables_Pool

;---------- Subroutines ----------------------------------------------------;

;--- Get current privelege level -----------------------------------;
; INPUT:   None                                                     ;
; OUTPUT:  R7 = Current privelege level: 0-3, 0 means highest       ;
;          Note EAX/RAX transferred to R7 during x86/EBC transition ;
;          Output R7 bits [63-32] undefined for IA32 EFI            ;
;-------------------------------------------------------------------;
IA32_Get_CPL:				; Entry point for IA32
x64_Get_CPL:				; Entry point for x64
		mov	ax,cs
		and	eax,3
		ret

;--- Check CPUID availability --------------------------------------;
; INPUT:   None                                                     ;
; OUTPUT:  R7 = Status: 0=Not available, 1=Available                ;
;          Note EAX/RAX transferred to R7 during x86/EBC transition ;
;          Output R7 bits [63-32] undefined for IA32 EFI            ;
;-------------------------------------------------------------------;
IA32_Check_CPUID:			; Entry point for IA32
x64_Check_CPUID:			; Entry point for x64
		push	rbx
;--- Verify CPU support CPUID instruction ---
; Check bit EFLAGS.21 toggleable, it is CPUID support indicator
; Note. Probably wrong result if trace this subroutine code
;--- Check for ID bit writeable for "1" ---
		mov	ebx,21
	;!	cli
		pushf			; If 32-bit mode, push EFLAGS
		pop	rax		; If 32-bit mode, pop EAX
		bts	eax,ebx		; Set EAX.21=1
		push	rax		; If 32-bit mode, push EAX
		popf			; If 32-bit mode, pop EFLAGS
		pushf			; If 32-bit mode, push EFLAGS
		pop	rax		; If 32-bit mode, pop EAX
	;!	sti
		btr	eax,ebx		; Check EAX.21=1, Set EAX.21=0
		jnc	Bad_CPUID	; Go if cannot set EFLAGS.21=1
;--- Check for ID bit writeable for "0" ---
	;!	cli
		push	rax		; If 32-bit mode, push EAX
		popf			; If 32-bit mode, pop EFLAGS
		pushf			; If 32-bit mode, push EFLAGS
		pop	rax		; If 32-bit mode, pop EAX
	;!	sti
		btr	eax,ebx		; Check EAX.21=0
		jc	Bad_CPUID	; Go if cannot set EFLAGS.21=0
;--- Exit points ---
		mov	eax,1
		pop	rbx
		ret
Bad_CPUID:	xor	eax,eax
		pop	rbx
		ret

;--- Execute selected CPUID function -------------------------------;
; INPUT:   Parm#1 = Function number, dword                          ;
;          Parm#2 = Output buffer base address, qword               ;
; OUTPUT:  R7 = EAX value after CPUID                               ;
;          Output R7 bits [63-32] undefined for IA32, 0 for x64     ;
;          Destination buffer contain 16 bytes:                     ;
;          EAX,EBX,ECX,EDX after CPUID execution                    ;
;-------------------------------------------------------------------;
x64_Get_CPUID:				; Entry point for x64
		push	rdi
		mov	eax,ecx		; EAX=Parm#1=Function
		mov	rdi,rdx		; RDI=Parm#2=Pointer
		jmp	Entry_CPUID
IA32_Get_CPUID:				; Entry point for IA32
		push	rdi
		mov	eax,[rsp+08]	; EAX=Parm#1=Function
		mov	edi,[rsp+12]	; EDI=Parm#2=Pointer
Entry_CPUID:
		push	rbx rcx rdx

		;--- v0.25 addition, yet subfunction 0 supported only, this for function 7 useable ---
		xor	ecx,ecx		; Subfunction 0, required valid for function 7
		;---

		cpuid

		stosd			; Store bytes [00-03]
		xchg	eax,ebx		; XCHG instead MOV, save size
		stosd			; Store bytes [04-07]
		xchg	eax,ecx
		stosd			; Store bytes [08-11]
		xchg	eax,edx
		stosd			; Store bytes [12-15]
		xchg	eax,ebx		; This for R7 output
		pop	rdx rcx rbx rdi
		ret

;--- Measure clock by TSC ------------------------------------------;
; INPUT:   Parm#1 = Output buffer base address, qword               ;
; OUTPUT:  Destination buffer contain 8 bytes:                      ;
;          Delta TSC per 1 second                                   ;
;-------------------------------------------------------------------;
x64_Measure_TSC:			; Entry point for x64
		push	rdi
		mov	rdi,rcx		; EDI=Parm#1=Output pointer
		jmp	Entry_TSC
IA32_Measure_TSC:			; Entry point for IA32
		push	rdi
		mov	edi,[rsp+08]	; EDI=Parm#1=Output pointer
Entry_TSC:
		push	rbx rdx rsi rbp
;--- Synchronization with current seconds change ---
		call	Read_RTC_Seconds
		mov	bl,al
Wait_RTC_2:	call	Read_RTC_Seconds
		cmp	bl,al
		je	Wait_RTC_2
;--- Measurement, wait 1 full second ---
		mov	bl,al
		rdtsc
		mov	esi,eax
		mov	ebp,edx
Wait_RTC_3:	call	Read_RTC_Seconds
		cmp	bl,al
		je	Wait_RTC_3
		rdtsc
		sub	eax,esi
		sbb	edx,ebp
;--- Update buffer ---
		stosd			; Use [edi] if UA32, use [rdi] if x64
		xchg	eax,edx
		stosd
;--- Exit ---
		pop	rbp rsi rdx rbx rdi
		ret

;--- Send debug code to port 80h -----------------------------------;
; INPUT:   Parm#1 = Debug Code, byte                                ;
; OUTPUT:  None                                                     ;
;-------------------------------------------------------------------;

IA32_Checkpoint:
		mov	al,[rsp+04]		; Parameter#1 = [esp+04], encoded for x64 same as [rsp+04] for IA32
		jmp	Entry_Checkpoint
x64_Checkpoint:
		mov	al,cl			; Parameter#1 = rcx
Entry_Checkpoint:
		out	80h,al
		ret


;---------- Local subroutines --------------------------------------;

;--- Read seconds from RTC, wait for UIP=0 -----;
; UIP=Update In Progress flag,                  ;
; it must be 0 for valid read time.             ;
; Procedure optimized for speed (precision),    ;
; instead minimize code size.                   ;
; INPUT:   None                                 ;
; OUTPUT:  AL = RTC seconds counter             ;
;-----------------------------------------------;
Read_RTC_Seconds:
		cli
;--- Wait for UIP=0 ---
Wait_RTC_1:	mov	al,0Ah		; Index=0Ah, control/status reg. 0Ah
		out	70h,al
		in	al,71h		; Read register 0Ah
		test	al,80h		; Index=0Ah, Bit=7, UIP bit
		jnz	Wait_RTC_1
;--- Read seconds ---
		mov	al,00h		; Index=00h, seconds register
		out	70h,al
		in	al,71h		; Read seconds register
;--- Exit ---
		sti
		ret



