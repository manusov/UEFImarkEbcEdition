:START
cls
del uefimark.efi
del uefimark.tmp
c:\fasm\fasm asm\uefimark.asm uefimark.tmp
ebcpatch uefimark.tmp uefimark.efi
REM comp uefimark.tmp uefimark.efi
pause
REM goto START
