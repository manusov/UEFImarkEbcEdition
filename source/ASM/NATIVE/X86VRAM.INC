;===========================================================================;
;=                                                                         =;
;=              X86-EXECUTABLE HANDLERS CALLED FROM EBC CODE.              =;
;=  DIRECT WRITE TO VIDEO RAM WITH BENCHMARKS, HARDWARE SIZING VIDEO RAM.  =;
;=                                                                         =;
;===========================================================================;

;---------- Equations for call subroutines ---------------------------------;

_IA32_Sizing_VRAM	= IA32_Sizing_VRAM - Global_Variables_Pool
_x64_Sizing_VRAM	= x64_Sizing_VRAM - Global_Variables_Pool

;---------- Subroutines ----------------------------------------------------;

;--- Detect Video Memory size by Write/Read sequence --------------------;
; Detect only region, accessible by PCI PnP mechanism (Prefetchable BAR) ;
; without GPU-specific operations.                                       ;
; Caller must verify WBINVD instruction supported before call this.      ;
;                                                                        ;
; INPUT:   Parm#1 = Video memory region base address, 64-bit, qword      ;
;          Parm#2 = Video memory region size, 64-bit, qword              ;
;                   (this is parameters of VRAM PCI PnP BAR)             ;
; OUTPUT:  R7     = Video memory size, bytes, dword                      ;
;                   valid bits R7[31-00] only                            ;
;                   0 means detection error or size unknown              ;
;------------------------------------------------------------------------;

IA32_Sizing_VRAM:			; Entry point for IA32
		push	rbx rcx rdx
		mov	ebx,[rsp+16]	; EBX = Parm#1 = Region base, assembled same as [esp+16], can use for IA32
		mov	edx,[rsp+20]	; EDX = Parm#2 = Region size, assembled same as [esp+20], can use for IA32
		jmp	Entry_S_VRAM
x64_Sizing_VRAM: 			; Entry point for x64
		push	rbx rcx rdx
		mov	rbx,rcx		; RBX=Base, RDX=Size limit
Entry_S_VRAM:
		push	rsi rdi		; Assembled same as 32-bit PUSH, can use for IA32
;--- Detect video memory by physical scan ---
		mov	rcx,00080000h	; RCX = Size Approximator, 512KB
;--- Prepare for cycle 1 ---		; RBX = Base,  RDX = Maximum size
Check_Exclus:	push	qword [rbx]
		push	qword [rbx+rcx]
		mov	eax,055555555h
		mov	edi,0AAAAAAAAh
;--- Cycle 2, required for WC memory fence, buffers flush ---
		mov	si,256
Fence_Cyc:	mov	[rbx],eax
		mov	[rbx+rcx],edi
		wbinvd			; Writeback and Invalidate cache
		dec	si
		jnz	Fence_Cyc
;--- Check results ---
		cmp	[rbx+rcx],edi
		jne	Mem_Failure	; Go if video memory failure
		cmp	[rbx],eax
		stc			; CF=1(C) means continue
		je	End_Pass	; Go if try this size OK
		cmp	[rbx],edi
		je	End_Pass	; Go if not failure, but stop, CF=0(NC)
Mem_Failure:	
	;!	xor	al,al		; AL=0, also CF=0(NC)

End_Pass:	pop	qword [rbx+rcx]
		pop	qword [rbx]
		jnc	End_Scan	; Go if size detected or error
		shl	rcx,1
		cmp	rcx,rdx		; RCX = Size
		jb	Check_Exclus
;--- End of cycle 1 ---
End_Scan:	
;--- Prepare at EAX output parameter: R7=Size, kilobytes ---
; Note EAX transferred to R7[31-00] by x86/EBC gate
; R7[63-00] can be unpredictable for IA32, contain RAX[63-00] for x64
; for compatibility accept this bits as unpredictable
;---
		cmp	al,055h
		je	Mem_Good	; Go if no failures
		xor	eax,eax		; ECX=0 if failure
Mem_Good:	xchg	eax,ecx		; EAX=Size or 0, XCHG for compact
	;*	shr	eax,10		; Units Bytes -> Kilobytes
;--- Restore registers and exit ---
		pop	rdi rsi rdx rcx rbx
		ret

