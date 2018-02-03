;--- Scan for space in the command line ---------;
; INPUT:   RSI = Source pointer, scanned string  ;
;          RCX = Size limit for cycle counter    ;
; OUTPUT:  RSI = Updated pointer by string scan  ;
;          AL  = Last detected ASCII char        ;  
;------------------------------------------------;
ScanForSpace:
@@:
lodsb
cmp al,' '
loopne @b
ret

