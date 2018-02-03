D:\TASM\TASM   ebcpatch.asm /zi /m /jjumps /z
D:\TASM\TLINK  ebcpatch.obj /3 /v /x
DEL            *.obj
D:\TASM\TD     ebcpatch.exe sample.tmp sample.efi
DEL            ebcpatch.exe