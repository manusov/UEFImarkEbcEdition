;--- Copy parameter from the command line -------;
; INPUT:   ESI = Source pointer, scanned string  ;
;          EDI = Destination pointer, buffer     ;
; OUTPUT:  ESI = Updated pointer by string copy  ;
;          EDI = Updated pointer by string copy  ;
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
push eax
mov al,0
stosb
pop eax
ret

