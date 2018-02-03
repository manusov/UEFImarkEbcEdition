;--- Skip spaces in the command line ------------;
; INPUT:   ESI = Source pointer, scanned string  ;
;          ECX = Size limit for cycle counter    ;
; OUTPUT:  ESI = Updated pointer by string scan  ;
;          AL  = Last detected ASCII char        ;  
;------------------------------------------------;
SkipExtraSpaces:
@@:
lodsb
cmp al,0
je @f 
cmp al,' '
loope @b
@@:
dec esi
ret

