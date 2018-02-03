;===========================================================================;
;=                                                                         =;
;=              X86-EXECUTABLE HANDLERS CALLED FROM EBC CODE.              =;
;=       MEMORY RANGES STATUS CONTROL BY DIRECT READ AND WRITE MTRR.       =;
;=                                                                         =;
;===========================================================================;

; Use this control method recommended only if platform not support
; UEFI protocols with same functionality.

;---------- Comments -------------------------------------------------------
;
; IA32_MTRRCAP = MSR#000000FEh, Read Only.
; D00-D07 = Number of Variable Range MTRRs
; D08     = Fixed MTRR presence, 0=absent, 1=present
; D09     = Reserved = 0
; D10     = Write Combining Memory Type support: 0=not supported, 1=supported
; D11-D63 = Reserved = 0
; Typically contain 0000000000000508h, means:
; WC supported, Fixed MTRR supported, Number of Variable MTRR = 8
;---------------------------------------------------------------------------
;
; IA32_MTRR_DEF_TYPE = MSR#000002FFh, Read/Write.
; D00-D07 = Default Memory Type, used for regions, not described by MTRR
; D08-D09 = Reserved = 0
; D10     = Fixed Range MTRR enable: 0=disabled, 1=enabled
; D11     = MTRR capability enable: 0=disabled, all memory=UC, 1=enabled
; D12-D63 = Reserved = 0
; Memory types encoding (8-bit Memory Type used for all MTRRs):
; 00 = UC = Uncacheable
; 01 = WC = Write Combining
; 02 = Reserved
; 03 = Reserved
; 04 = WT = Write Through
; 05 = WP = Write Protected
; 06 = WB = Write Back
; 07-FF = Reserved
;---------------------------------------------------------------------------
;
; Region Base Address and Size is variable
; Name                     MSR# (hex)
; -------------------------------------------
; IA32_MTRR_PHYSBASE0      00000200
; IA32_MTRR_PHYSMASK0      00000201
; IA32_MTRR_PHYSBASE1      00000202
; IA32_MTRR_PHYSMASK1      00000203
; IA32_MTRR_PHYSBASE2      00000204
; IA32_MTRR_PHYSMASK2      00000205
; IA32_MTRR_PHYSBASE3      00000206
; IA32_MTRR_PHYSMASK3      00000207
; IA32_MTRR_PHYSBASE4      00000208
; IA32_MTRR_PHYSMASK4      00000209
; IA32_MTRR_PHYSBASE5      0000020A
; IA32_MTRR_PHYSMASK5      0000020B
; IA32_MTRR_PHYSBASE6      0000020C
; IA32_MTRR_PHYSMASK6      0000020D
; IA32_MTRR_PHYSBASE7      0000020E
; IA32_MTRR_PHYSMASK7      0000020F
;
; Variable Range MTRR. PHYSBASE(i) Registers:
; -------------------------------------------
; D00-D07 = Memory type code
; D08-D11 = Reserved = 0
; D12-D35 = Memory Region Base Address, 0-64GB (36-bit), 4K Granularity
; D36-D63 = Reserved = 0 (future use for address maximum 64-bit)
;
; Variable Range MTRR. PHYSMASK(i) Registers:
; -------------------------------------------
; D00-D10 = Reserved = 0
; D11     = Validity, 0=this MTRR not used, 1=used
; D12-D35 = Memory Region Address Mask, 0-64GB (36-bit), 4K Granularity
; D36-D63 = Reserved = 0 (future use for address maximum 64-bit)
;
; Fixed Range MTRR not listed here because not used for this subroutine
; See Intel/AMD documents.
;---------------------------------------------------------------------------

;---------- Equations for call subroutines ---------------------------------;

_IA32_Check_MTRR		= IA32_Check_MTRR - Global_Variables_Pool
_x64_Check_MTRR 		= x64_Check_MTRR - Global_Variables_Pool
_IA32_Setup_MTRR		= IA32_Setup_MTRR - Global_Variables_Pool
_x64_Setup_MTRR 		= x64_Setup_MTRR - Global_Variables_Pool
_IA32_Restore_MTRR		= IA32_Restore_MTRR - Global_Variables_Pool
_x64_Restore_MTRR		= x64_Restore_MTRR - Global_Variables_Pool
_IA32_Read_MSR			= IA32_Read_MSR - Global_Variables_Pool
_x64_Read_MSR			= x64_Read_MSR - Global_Variables_Pool

;---------- Subroutines ----------------------------------------------------;

;--- Check MTRR present for use fast write methods of video RAM ---------;
; Caller must check support CPUID function 1 before call this            ;
; subroutine.                                                            ;
;                                                                        ;
; INPUT:   Parm#1 = Pointer to output parameters block                   ;
; OUTPUT:  Output parameters block (MTRR_Area) updated                   ;
;          [00-03] DWORD  MTRR_Used = MSR address or 0=No, 1=No vacant   ;
;          [04-07] DWORD  Reserved                                       ;
;          [08-15] QWORD  Save Base of MTRR, selected for use            ;
;          [16-23] QWORD  Save Mask of MTRR, selected for use            ;
;          Mask/Base selected for restore when exit application          ;
;         R7 = Reserved for status, not used yet, see DWORD MTRR_Used    ;
;------------------------------------------------------------------------;
IA32_Check_MTRR:			; Entry point for IA32
		push	rdi		; Assembled same as PUSH EDI, can use for IA32
		mov	edi,[rsp+08]	; EDI = Parm#1 = Pointer, assembled same as [esp+8], can use for IA32
		jmp	Entry_C_MTRR
x64_Check_MTRR: 			; Entry point for x64
		push	rdi
		mov	rdi,rcx 	; RDI = Parm#1
Entry_C_MTRR:
;--- Save registers, it can be important for EBC/x86 gate ---
		push	rbx rcx rdx
;--- Pre-blank MSR address ---
; Address=0 means capability not detected or not used
		mov	dword [rdi+00],0	; Assembled same as [edi+00], can use for IA32 (field MTRR_Used)
;--- Detect MTRR capability ---
; Support CPUID function 1 already verified by previous operations
		mov	eax,1
		cpuid			; Get ECX:EDX=CPU features list
		test	dh,00010000b
		jz	None_MTRR	; Absence is not error here
;--- Check required features ---
		mov	ecx,000000FEh	; IA32_MTRRCAP
		rdmsr
		test	al,al
		jz	None_MTRR	; Go if no Variable MTRR
		test	ah,00000100b
		jz	None_MTRR	; Go if no WC Memory Mode
		xchg	eax,ebx 	; BL = Number of Variable MTRR
		mov	cx,02FFh	; IA32_MTRR_DEF_TYPE, ECX.[31-16]=0
		rdmsr
		test	ah,00001000b
		jz	Error_MTRR	; MTRR Capability Disabled ?
		test	al,al
		jnz	Error_MTRR	; Default must be UC
;--- Scan Variable MTRR ---
		mov	cx,0201h	; IA32_MTRR_PHYSMASK0, ECX.[31-16]=0
Scan_Var_MTRR:	rdmsr
		test	ah,00001000b
		jz	Found_MTRR	; This MTRR free ?
Next_Var_MTRR:	inc	ecx		; Try next MTRR
		inc	ecx
		dec	bl		; BL = Number of Variable MTRR
		jnz	Scan_Var_MTRR	; Cycle for find vacant MTRR
;--- Exit without errors if vacant MTRR not found, CF=0(NC) ---
		mov	dword [rdi+00],1	; Assembled same as [edi+00], can use for IA32 (field MTRR_Used)
;--- Exit if errors or not found (additional points equal because status not used) ---
Error_MTRR:
None_MTRR:
Exit_MTRR:
;--- Restore registers, it can be important for EBC/x86 gate ---
		pop	rdx rcx rbx rdi
		ret
;--- Available MTRR found ---
; Access [rdi] assembled same as [edi], can use for IA32
;---
Found_MTRR:	mov	[rdi+16],eax	; Save Mask [31-00]
		mov	[rdi+20],edx	; Save Mask [63-32]
		dec	ecx		; Modify MSR address
		rdmsr			; Read Base [63-00]
		mov	[rdi+08],eax	; Save Base [31-00]
		mov	[rdi+12],edx	; Save Base [63-32]
		mov	[rdi+00],ecx	; Save MSR address
;--- Exit if no errors ---
		jmp	Exit_MTRR


;--- Setup MTRR when application starts ---------------------------------;
; Caller must check MTRR really supported and old values saved.          ;
; New status is write combining.                                         ;
;                                                                        ;
; INPUT:   Parm#1 = MSR address for programmed MTRR pair, dword          ;
;          Parm#2 = CPU physical address size [bits], dword              ;
;          Parm#3 = Region Base, qword                                   ;
;          Parm#4 = Region Size, qword                                   ;
; OUTPUT:  R7 = UEFI status                                              ;
;------------------------------------------------------------------------;
IA32_Setup_MTRR:				; Entry point for IA32
		push	rbx rcx rdx rsi rdi rbp ; Assembled same as PUSH Exx, can use for IA32
		mov	ecx,[rsp+28]		; ECX = Parm#1  = MSR address for MTRR, assembled same as [esp+8], can use for IA32
		mov	ebx,[rsp+32]		; EBX = Parm#2  = CPU physical address size
		mov	eax,[rsp+36]		; EAX = Parm#3L = Base Low 32
		mov	edx,[rsp+40]		; EDX = Parm#3H = Base High 32
		mov	esi,[rsp+44]		; ESI = Parm#4L = Size Low 32
		mov	edi,[rsp+48]		; EDI = Parm#4H = Size High 32
		jmp	Entry_S_MTRR
x64_Setup_MTRR: 				; Entry point for x64
		push	rbx rcx rdx rsi rdi rbp ; ECX = Parm#1  = MSR address for MTRR
		mov	ebx,edx 		; EBX = Parm#2  = CPU physical address size
		mov	eax,r8d 		; EAX = Parm#3L = Base Low 32
		mov	rdx,r8
		shr	rdx,32			; EDX = Parm#3H = Base High 32                  
		mov	esi,r9d 		; ESI = Parm#4L = Size Low 32
		mov	rdi,r9
		shr	rdi,32			; EDI = Parm#4H = Size High 32                  
Entry_S_MTRR:
;--- Align address mask = EDI:ESI ---
; Use two 32-bit registers instead one 64-bit register, 
; because IA32 RDMSR/WRMSR compatibility required
;---
;--- Align address mask = EDI:ESI ---
Set_MTRR:	mov	ebp,01000000h		; 16MB
		cmp	esi,ebp
		jae	Aligned_MTRR
		mov	esi,ebp
;--- Calculate address mask = EDI:ESI ---
Aligned_MTRR:	sub	esi,1			; Need set CF, cannot DEC
		jnc	Not_Above_4
		dec	edi
Not_Above_4:	not	esi
		not	edi
		and	si,0F000h		; Clear not used bits
		or	si,00800h		; Set D11=Enable
		push	rcx
		sub	bl,32			; BL now contain Address Size
		mov	cl,32
		sub	cl,bl
		mov	ebx,0FFFFFFFFh
		shr	ebx,cl
		pop	rcx
		and	edi,ebx
;--- Prepare control fields ---
		and	ax,0F000h		; Clear not used bits
		mov	al,01h			; 01h=Write Combining
;--- Setup MTRR, ECX now contain Used MTRR address ---
		cli
		wrmsr				; Write Base(i) MTRR, EDX:EAX
		inc	ecx			; MSR address+1
		xchg	eax,esi
		mov	edx,edi
		wrmsr				; Write Mask(i) MTRR, EDX:EAX
		sti
;--- Restore registers, go to clear status and exit ---
		pop	rbp rdi rsi rdx rcx rbx
		jmp	Status_MTRR


;--- Restore MTRR settings before application exit ----------------------;
; Caller must check MTRR really supported and saved values valid         ;
;                                                                        ;
; INPUT:   Parm#1 = Pointer to output parameters block                   ;
;          Input parameters block (MTRR_Area) updated                    ;
;          [00-03] DWORD  MTRR_Used = MSR address or 0=No, 1=No vacant   ;
;          [04-07] DWORD  Reserved                                       ;
;          [08-15] QWORD  Save Base of MTRR, selected for use            ;
;          [16-23] QWORD  Save Mask of MTRR, selected for use            ;
;          Mask/Base selected for restore when exit application          ;
; OUTPUT:  R7 = UEFI status                                              ;
;------------------------------------------------------------------------;
IA32_Restore_MTRR:			; Entry point for IA32
		push	rdi		; Assembled same as PUSH EDI, can use for IA32
		mov	edi,[rsp+08]	; EDI = Parm#1 = Pointer, assembled same as [esp+8], can use for IA32
		jmp	Entry_R_MTRR
x64_Restore_MTRR:			; Entry point for x64
		push	rdi
		mov	rdi,rcx 	; RDI = Parm#1
Entry_R_MTRR:
;--- Save registers, it can be important for EBC/x86 gate ---
		push	rcx rdx
;--- Restore base and mask ---
; Access [rdi] assembled same as [edi], can use for IA32
;---
		cli
		mov	ecx,[rdi+00]	; ECX = MSR address
		mov	eax,[rdi+08]
		mov	edx,[rdi+12]
		wrmsr			; Restore Base MTRR
		inc	ecx		; MSR address+1
		mov	eax,[rdi+16]
		mov	edx,[rdi+20]
		wrmsr			; Restore Mask MTRR
		sti
;--- Restore registers, it can be important for EBC/x86 gate ---
		pop	rdx rcx rdi
;--- Clear status and exit ---
Status_MTRR:	xor	eax,eax 	; EAX/RAX copied to R7 by x86/EBC gate
		ret


;--- Read Model-Specific Register, selected by input index --------------;
; INPUT:   Parm#1 = MSR address (ECX before RDMSR), dword                ;
;          Parm#2 = Address for write output data, qword                 ;
; OUTPUT:  R7 = Reserved for UEFI status                                 ;
;          QWORD at Address [Parm#2] = MSR data (EDX:EAX after RDMSR)    ; 
;------------------------------------------------------------------------;
IA32_Read_MSR:				; Entry point for IA32
		push	rbx rcx rdx
		mov	ecx,[rsp+16]	; ECX = Parm#1 = MSR address, assembled same as [esp+16], can use for IA32
		mov	ebx,[rsp+20]	; EBX = Parm#2 = Output address, assembled same as [esp+20], can use for IA32
		jmp	Entry_R_MSR
x64_Read_MSR:				; Entry point for x64
		push	rbx rcx rdx
		mov	rbx,rdx 	; RBX=Output address (p#2), RCX=MSR address (p#1)
Entry_R_MSR:
		rdmsr			; RCX=Input, EDX:EAX=Output
		mov	[rbx+00],eax
		mov	[rbx+04],edx
		pop	rdx rcx rbx
		ret

