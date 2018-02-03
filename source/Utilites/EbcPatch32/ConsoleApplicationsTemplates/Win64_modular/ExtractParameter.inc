;--- Copy parameter from the command line -------;
; INPUT:   RSI = Source pointer, scanned string  ;
;          RDI = Destination pointer, buffer     ;
; OUTPUT:  RSI = Updated pointer by string copy  ;
;          RDI = Updated pointer by string copy  ;
;          AL  = Last detected ASCII char        ;
;                examples: 20h=SPACE=Next parm.  ;
;                          00h=End of string     ;
;------------------------------------------------;
ExtractParameter:
mov ecx,256-6
@@:
lodsb
cmp al,0
je @f
cmp al,' '
je @f
cmp al,'"'
je @b
stosb
jmp @b
@@:
push rax
mov al,0
stosb
pop rax
ret

