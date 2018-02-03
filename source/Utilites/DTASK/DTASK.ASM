format pe64 dll efi
entry main

section '.text' code executable readable
main:
		xor	eax,eax			; Same for IA32 and x64
		ret				; Return

section '.data' data readable writeable
Dummy_Label	DQ  Dummy_Label			; This for prevent zero number of relocations

section '.reloc' fixups data discardable
