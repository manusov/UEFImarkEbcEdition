\TASM\TASM   efipatch.asm /zi /m /jjumps /z
\TASM\TLINK  efipatch.obj /3 /v /x
DEL          *.obj
\TASM\TD     efipatch.exe 1.dll 1.efi
DEL          efipatch.exe