;--- Skip spaces in the command line ------------;
; INPUT:   RSI = Source pointer, scanned string  ;
;          RCX = Size limit for cycle counter    ;
; OUTPUT:  RSI = Updated pointer by string scan  ;
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
dec rsi
ret

