;--- Scan for space in the command line ---------;
; INPUT:   ESI = Source pointer, scanned string  ;
;          ECX = Size limit for cycle counter    ;
; OUTPUT:  ESI = Updated pointer by string scan  ;
;          AL  = Last detected ASCII char        ;  
;------------------------------------------------;
ScanForSpace:
@@:
lodsb
cmp al,' '
loopne @b
ret

