REM cls
REM del uefimark.efi, uefimark.tmp
REM C:\Work\Fasm_Editor_Debugger\WinFasm171\fasm.exe asm\uefimark.asm uefimark.tmp
REM ebcpatch uefimark.tmp uefimark.efi
REM copy uefimark.efi e:\ebc_2\uefimark.efi

cls
comp 1.tmp 1.efi
REM pause