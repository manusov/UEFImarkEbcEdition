cls
del uefimark.efi, uefimark.tmp, d:\uefimark.efi
C:\Fasm_Editor_Debugger\WinFasm17149\fasm asm\uefimark.asm uefimark.tmp
Utilites\ebcpatch32 uefimark.tmp uefimark.efi
comp uefimark.tmp uefimark.efi
copy uefimark.efi e:\
del uefimark.efi
del uefimark.tmp