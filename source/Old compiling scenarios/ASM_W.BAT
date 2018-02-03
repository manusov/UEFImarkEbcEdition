:START
cls
del uefimark.efi, uefimark.tmp
d:\fasm\fasm asm\uefimark.asm uefimark.tmp
ebcpatch uefimark.tmp uefimark.efi
copy uefimark.efi H:\UEFI\MyWorkspace_NT32_only\Build\NT32\DEBUG_VS2008x86\IA32\uefimark.efi
REM comp uefimark.tmp uefimark.efi
pause
REM goto START
