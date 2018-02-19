;===========================================================================;
;=                                                                         =;
;=             MACRO FOR EFI BYTE CODE INSTRUCTIONS ASSEMBLING             =;
;=                      (in alphabetical order)                            =;
;=                     (C)2014-2018 IC Book Labs.                          =;
;=                                                                         =;
;===========================================================================;


;--- EBC Instruction: ADD32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Adds two signed operands and stores the result to Operand 1, for 32-bit.  ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 + Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO ADD32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Ch
else
DB 08Ch
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: ADD64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Adds two signed operands and stores the result to Operand 1, for 64-bit.  ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 + Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO ADD64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 04Ch
else
DB 0CCh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: AND32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a Logical AND on two operands and stores the result to           ;
; Operand 1, for 32-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 AND Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO AND32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 014h
else
DB 094h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: AND64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a Logical AND on two operands and stores the result to           ;
; Operand 1, for 64-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 AND Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO AND64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 054h
else
DB 0D4h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: ASHR32 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Performs an arithmetic right-shift of a signed 32-bit operand and stores  ;
; the result back to Operand 1.                                             ;
;                                                                           ;
; Operand 1 <= Operand 1 SHIFT-RIGHT Operand 2                              ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO ASHR32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 019h
else
DB 099h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: ASHR64 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Performs an arithmetic right-shift of a signed 64-bit operand and stores  ;
; the result back to Operand 1.                                             ;
;                                                                           ;
; Operand 1 <= Operand 1 SHIFT-RIGHT Operand 2                              ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO ASHR64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 059h
else
DB 0D9h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: BREAK break_code -------------------------------------;
;                                                                           ;
; BREAK 0 = Exception when program likely executed from cleared memory      ;
; BREAK 1 = Get virtual machine version in the R7                           ;
; BREAK 2 = Reserved                                                        ;
; BREAK 3 = Debug breakpoint                                                ;
; BREAK 4 = Reserved for system call                                        ;
; BREAK 5 = Create thunk                                                    ;
; BREAK 6 = Set compiler version                                            ;
; BREAK 7-255 = Reserved                                                    ;
;                                                                           ;
; op1 = Break code                                                          ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO BREAK op1
{
;--- Byte 0 = Operation code ---
DB 000h
;--- Byte 1 = Break code ---
DB op1
}


;--- EBC Instruction: CALL32 {@}R1, {Immed32|Index32} ----------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL32 is used to jump directly to EBC code within a given application    ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= IP + SizeOfThisInstruction + Operand 1 (target instruction addr.) ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL32 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 003h
else
DB 083h
end if
;--- Byte 1 = Operand addressing, mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 010h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1-@f
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
@@:
}


;--- EBC Instruction: CALL32A {@}R1, {Immed32|Index32} ---------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL32 is used to jump directly to EBC code within a given application    ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= Operand 1 (target instruction addr.)                              ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL32A op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 003h
else
DB 083h
end if
;--- Byte 1 = Operand addressing, mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 000h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: CALL32EX {@}R1, {Immed32|Index32} --------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL32EX is used to jump to external code (either native or EBC), which   ;
; requires thunking.                                                        ;
; Yet macro supported for native only.                                      ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= IP + SizeOfThisInstruction + Operand 1 (target instruction addr.) ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL32EX op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 003h
else
DB 083h
end if
;--- Byte 1 = Operand addressing, mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 030h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1-@f
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
@@:
}


;--- EBC Instruction: CALL32EXA {@}R1, {Immed32|Index32} -------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL32EX is used to jump to external code (either native or EBC), which   ;
; requires thunking.                                                        ;
; Yet macro supported for native only.                                      ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= Operand 1 (target instruction addr.)                              ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL32EXA op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 003h
else
DB 083h
end if
;--- Byte 1 = Operand addressing, mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 020h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
}


; CALL64 Reserved


;--- EBC Instruction: CALL64A Immed64 --------------------------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL64A is used to jump directly to EBC code within a given application   ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= Operand 1 (target instruction addr.)                              ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL64A op1
{
;--- Byte 0 = Operation code ---
DB 0C3h
;--- Byte 1 = Mode select ---
DB 000h
;--- Bytes 2-9 = 64-bit immediate ---
DQ op1
}


; CALL64EX Reserved


;--- EBC Instruction: CALL64EXA {@}R1, {Immed32|Index32} -------------------;
;                                                                           ;
; The CALL instruction pushes the address of the following instruction on   ;
; the stack and jumps to a subroutine.                                      ;
; CALL64EXA is used to jump to external code (either native or EBC), which  ;
; requires thunking.                                                        ;
; Yet macro supported for native only.                                      ;
;                                                                           ;
; R0   <= R0 - 16                                                           ;
; [R0] <= IP + SizeOfThisInstruction (instruction after CALL address)       ;
; IP   <= Operand 1 (target instruction addr.)                              ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CALL64EXA op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 043h
else
DB 0C3h
end if
;--- Byte 1 = Operand addressing, mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 020h + x2 SHL 3 + x1
;--- Conditional bytes 2-9 = 64-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DQ op1
else
end if
end if

; Reserved place for Constant+Native addressing

}


;--- EBC Instruction: CMP32EQ R1, {@}R2, {Index16|Immed16} -----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP32EQ is signed compare 32-bit values for EQUAL condition.              ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareEqual Operand 2                         ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP32EQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 005h
else
DB 085h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP32GTE R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP32GTE is signed compare 32-bit values for GREAT OR EQUAL condition.    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareGreatOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP32GTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 007h
else
DB 087h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP32LTE R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP32LTE is signed compare 32-bit values for LEAST OR EQUAL condition.    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP32LTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 006h
else
DB 086h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP32UGTE R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP32UGTE is unsigned compare 32-bit values for GREAT OR EQUAL condition. ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareGreatOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP32UGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 009h
else
DB 089h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP32ULTE R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP32ULTE is unsigned compare 32-bit values for LEAST OR EQUAL condition. ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareLeastOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP32ULTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 008h
else
DB 088h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP64EQ R1, {@}R2, {Index16|Immed16} -----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP64EQ is signed compare 64-bit values for EQUAL condition.              ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareEqual Operand 2                         ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP64EQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 045h
else
DB 0C5h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP64GTE R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP64GTE is signed compare 64-bit values for GREAT OR EQUAL condition.    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareGreatOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP64GTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 047h
else
DB 0C7h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP64LTE R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP64LTE is signed compare 64-bit values for LEAST OR EQUAL condition.    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP64LTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 046h
else
DB 0C6h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP64UGTE R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP64UGTE is unsigned compare 64-bit values for GREAT OR EQUAL condition. ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareGreatOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP64UGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 049h
else
DB 0C9h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMP64ULTE R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; The CMP instruction is used to compare Operand 1 to Operand 2,            ;
; CMP64ULTE is unsigned compare 64-bit values for LEAST OR EQUAL condition. ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; Only register direct is supported for Operand 1.                          ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareLeastOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Second operand for comparision                                      ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMP64ULTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 048h
else
DB 0C8h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op2 in < R0,@R0 >
x2=0
end if
if op2 in < R1,@R1 >
x2=1
end if
if op2 in < R2,@R2 >
x2=2
end if
if op2 in < R3,@R3 >
x2=3
end if
if op2 in < R4,@R4 >
x2=4
end if
if op2 in < R5,@R5 >
x2=5
end if
if op2 in < R6,@R6 >
x2=6
end if
if op2 in < R7,@R7 >
x2=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x3 SHL 7 + x2 SHL 4 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
}


;--- EBC Instruction: CMPI32DEQ {@}R1, {Index16}, Immed32 ------------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DEQ is signed compare 32-bit values for EQUAL condition,            ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareEqual Operand 2                         ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32DEQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0ADh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI32DGTE {@}R1, {Index16}, Immed32 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DGTE is signed compare 32-bit values for GREAT OR EQUAL condition,  ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareGreatOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32DGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0AFh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI32DLTE {@}R1, {Index16}, Immed32 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DLTE is signed compare 32-bit values for LEAST OR EQUAL condition,  ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32DLTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0AEh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI32DUGTE {@}R1, {Index16}, Immed32 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DUGTE is unsigned compare 32-bit values for GREAT OR EQUAL          ;
; condition, second operand is 32-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareGreatOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32DUGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B1h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI32DULTE {@}R1, {Index16}, Immed32 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DULTE is unsigned compare 32-bit values for LEAST OR EQUAL          ;
; condition, second operand is 32-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareLeastOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32DULTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B0h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


; CMPI32EQ Reserved
; CMPI32GTE Reserved
; CMPI32LTE Reserved
; CMPI32UGTE Reserved
; CMPI32ULTE Reserved


;--- EBC Instruction: CMPI32WEQ {@}R1, {Index16}, Immed16 ------------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32WEQ is signed compare 32-bit values for EQUAL condition,            ;
; second operand is 16-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareEqual Operand 2                         ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32WEQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 02Dh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI32WGTE {@}R1, {Index16}, Immed16 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32WGTE is signed compare 32-bit values for GREAT OR EQUAL condition,  ;
; second operand is 16-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareGreatOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32WGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 02Fh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI32WLTE {@}R1, {Index16}, Immed16 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DLTE is signed compare 32-bit values for LEAST OR EQUAL condition,  ;
; second operand is 16-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32WLTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 02Eh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI32WUGTE {@}R1, {Index16}, Immed16 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32WUGTE is unsigned compare 32-bit values for GREAT OR EQUAL          ;
; condition, second operand is 16-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareGreatOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32WUGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 031h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI32WULTE {@}R1, {Index16}, Immed16 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32WULTE is unsigned compare 16-bit values for LEAST OR EQUAL          ;
; condition, second operand is 32-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareLeastOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI32WULTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 030h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI64DEQ {@}R1, {Index16}, Immed32 ------------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64DEQ is signed compare 64-bit values for EQUAL condition,            ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareEqual Operand 2                         ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64DEQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0EDh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI64DGTE {@}R1, {Index16}, Immed32 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64DGTE is signed compare 64-bit values for GREAT OR EQUAL condition,  ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareGreatOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64DGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0EFh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI64DLTE {@}R1, {Index16}, Immed32 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI32DLTE is signed compare 64-bit values for LEAST OR EQUAL condition,  ;
; second operand is 32-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64DLTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0EEh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI64DUGTE {@}R1, {Index16}, Immed32 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64DUGTE is unsigned compare 32-bit values for GREAT OR EQUAL          ;
; condition, second operand is 32-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareGreatOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64DUGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F1h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: CMPI64DULTE {@}R1, {Index16}, Immed32 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64DULTE is unsigned compare 64-bit values for LEAST OR EQUAL          ;
; condition, second operand is 32-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareLeastOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64DULTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F0h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DD op2
else
DD op4
end if
}


; CMPI64EQ Reserved
; CMPI64LTE Reserved
; CMPI64UGTE Reserved
; CMPI64ULTE Reserved


;--- EBC Instruction: CMPI64WEQ {@}R1, {Index16}, Immed16 ------------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64WEQ is signed compare 64-bit values for EQUAL condition,            ;
; second operand is 16-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareEqual Operand 2                         ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64WEQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 06Dh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI64WGTE {@}R1, {Index16}, Immed16 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64WGTE is signed compare 64-bit values for GREAT OR EQUAL condition,  ;
; second operand is 16-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareGreatOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64WGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 06Fh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI64WLTE {@}R1, {Index16}, Immed16 -----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64DLTE is signed compare 64-bit values for LEAST OR EQUAL condition,  ;
; second operand is 16-bit immediate.                                       ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 SignedCompareLeastOrEqual Operand 2                  ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64WLTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 06Eh
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI64WUGTE {@}R1, {Index16}, Immed16 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64WUGTE is unsigned compare 64-bit values for GREAT OR EQUAL          ;
; condition, second operand is 16-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareGreatOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64WUGTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 071h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: CMPI64WULTE {@}R1, {Index16}, Immed16 ----------------;
;                                                                           ;
; Compares two operands, one of which is an immediate value,                ;
; CMPI64WULTE is unsigned compare 16-bit values for LEAST OR EQUAL          ;
; condition, second operand is 32-bit immediate.                            ;
; The immediate data is fetched as a signed value.                          ;
; If the immediate data is smaller than the comparision size, then the      ;
; immediate data is sign-extended appropriately.                            ;
;                                                                           ;
; Flags.C <= Operand 1 UnsignedCompareLeastOrEqual Operand 2                ;
;                                                                           ;
; op1 = First operand for comparision                                       ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Second operand for comparision                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO CMPI64WULTE op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 070h
;--- Byte 1 = Operand 1 addressing ---
if op4 eq
x1=0
else
x1=1
end if
if op1 in < R0,@R0 >
x2=0
end if
if op1 in < R1,@R1 >
x2=1
end if
if op1 in < R2,@R2 >
x2=2
end if
if op1 in < R3,@R3 >
x2=3
end if
if op1 in < R4,@R4 >
x2=4
end if
if op1 in < R5,@R5 > 
x2=5
end if
if op1 in < R6,@R6 >
x2=6
end if
if op1 in < R7,@R7 >
x2=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x3=0
else
x3=1
end if
DB x1 SHL 4 + x3 SHL 3 + x2
;--- Conditional bytes 2,3 = 16-bit index ---
if op4 eq
else
if op3 < 0
x4=-op3
else
x4=op3
end if
if op4 < 0
x5=-op4
else
x5=op4
end if
if op3 = 0
x6=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x6=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x6=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x6=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x6=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x6=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x6=6
end if
end if
if op3<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate 32 ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: DIV32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a divide operation on two signed operands and stores the         ;
; result to Operand 1. For 32-bit.                                          ;
; Perform a modulus on two unsigned 32-bit operands and store the result    ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit: if Operand 1 is direct, then the upper 32 bits of result are  ;
; set to 0 before storing to the Operand 1 register.                        ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 / Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO DIV32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 010h
else
DB 090h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: DIV64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a divide operation on two signed operands and stores the         ;
; result to Operand 1. For 64-bit.                                          ;
; Perform a modulus on two unsigned 32-bit operands and store the result    ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 / Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO DIV64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 050h
else
DB 0D0h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: DIVU32 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Performs a divide operation on two unsigned operands and stores the       ;
; result to Operand 1. For 32-bit.                                          ;
; Perform a modulus on two unsigned 32-bit operands and store the result    ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit: if Operand 1 is direct, then the upper 32 bits of result are  ;
; set to 0 before storing to the Operand 1 register.                        ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 / Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO DIVU32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 011h
else
DB 091h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: DIVU64 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Performs a divide operation on two unsigned operands and stores the       ;
; result to Operand 1. For 64-bit.                                          ;
; Perform a modulus on two unsigned 32-bit operands and store the result    ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 / Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO DIVU64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 051h
else
DB 0D1h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: EXTNDB32 {@}R1, {@}R2, {Index16|Immed16} -------------;
;                                                                           ;
; Sign-extend a byte value and store the result to Operand 1.               ;
; The byte is sign-extended to 32 bits.                                     ;
;                                                                           ;
; Operand 1 <= Operand 1 SignExtendedByte Operand 2                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO EXTNDB32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 01Ah
else
DB 09Ah
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: EXTNDB64 {@}R1, {@}R2, {Index16|Immed16} -------------;
;                                                                           ;
; Sign-extend a byte value and store the result to Operand 1.               ;
; The byte is sign-extended to 64 bits.                                     ;
;                                                                           ;
; Operand 1 <= Operand 1 SignExtendedByte Operand 2                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO EXTNDB64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 05Ah
else
DB 0DAh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: EXTNDD32 {@}R1, {@}R2, {Index16|Immed16} -------------;
;                                                                           ;
; Sign-extend a dword value and store the result to Operand 1.              ;
; The dword is sign-extended to 32 bits,                                    ;
; because dword=32 bits, this functionality equal dword copy.               ;
;                                                                           ;
; Operand 1 <= Operand 1 SignExtendedByte Operand 2                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO EXTNDD32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 01Ch
else
DB 09Ch
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: EXTNDD64 {@}R1, {@}R2, {Index16|Immed16} -------------;
;                                                                           ;
; Sign-extend a dword value and store the result to Operand 1.              ;
; The dword is sign-extended to 64 bits.                                    ;
;                                                                           ;
; Operand 1 <= Operand 1 SignExtendedByte Operand 2                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO EXTNDD64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 05Ch
else
DB 0DCh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: EXTNDW32 {@}R1, {@}R2, {Index16|Immed16} -------------;
;                                                                           ;
; Sign-extend a word value and store the result to Operand 1.               ;
; The word is sign-extended to 32 bits.                                     ;
;                                                                           ;
; Operand 1 <= Operand 1 SignExtendedByte Operand 2                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO EXTNDW32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 01Bh
else
DB 09Bh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: EXTNDW64 {@}R1, {@}R2, {Index16|Immed16} -------------;
;                                                                           ;
; Sign-extend a word value and store the result to Operand 1.               ;
; The word is sign-extended to 64 bits.                                     ;
;                                                                           ;
; Operand 1 <= Operand 1 SignExtendedByte Operand 2                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO EXTNDW64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 05Bh
else
DB 0DBh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: JMP32 {@}R1 , {Immed32|Index32} ----------------------;
;                                                                           ;
; The JMP32 instruction used to unconditionally jump to a relative          ;
; address and continue executing EBC instructions.                          ;
; If Operand 1 register is R0 (Stack Pointer), then the register contents   ;
; are assumed to be 0, immediate operand used only.                         ; 
;                                                                           ;
; IP <= IP + SizeOfThisInstruction + Operand 1 (target instr. addr.)        ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP32 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 001h
else
DB 081h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 010h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1-@f
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
@@:
}


;--- EBC Instruction: JMP32A {@}R1 , {Immed32|Index32} ---------------------;
;                                                                           ;
; The JMP32 instruction used to unconditionally jump to a absolute          ;
; address and continue executing EBC instructions.                          ;
; If Operand 1 register is R0 (Stack Pointer), then the register contents   ;
; are assumed to be 0, immediate operand used only.                         ; 
;                                                                           ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP32A op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 001h
else
DB 081h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 000h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: JMP32ACC {@}R1 , {Immed32|Index32} -------------------;
;                                                                           ;
; The JMP32CC instruction used to conditionally jump to a absolute          ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=0.                                                   ;
; If Operand 1 register is R0 (Stack Pointer), then the register contents   ;
; are assumed to be 0, immediate operand used only.                         ; 
;                                                                           ;
; If Flags.C=0                                                              ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP32ACC op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 001h
else
DB 081h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 080h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: JMP32ACS {@}R1 , {Immed32|Index32} -------------------;
;                                                                           ;
; The JMP32CC instruction used to conditionally jump to a absolute          ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=1.                                                   ;
; If Operand 1 register is R0 (Stack Pointer), then the register contents   ;
; are assumed to be 0, immediate operand used only.                         ; 
;                                                                           ;
; If Flags.C=1                                                              ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP32ACC op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 001h
else
DB 081h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 0C0h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: JMP32CC {@}R1 , {Immed32|Index32} --------------------;
;                                                                           ;
; The JMP32CC instruction used to conditionally jump to a relative          ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=0.                                                   ;
; If Operand 1 register is R0 (Stack Pointer), then the register contents   ;
; are assumed to be 0, immediate operand used only.                         ; 
;                                                                           ;
; If Flags.C=0                                                              ;
; IP <= IP + SizeOfThisInstruction + Operand 1 (target instr. addr.)        ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP32CC op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 001h
else
DB 081h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 090h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1-@f
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
@@:
}


;--- EBC Instruction: JMP32CS {@}R1 , {Immed32|Index32} --------------------;
;                                                                           ;
; The JMP32CS instruction used to conditionally jump to a relative          ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=1.                                                   ;
; If Operand 1 register is R0 (Stack Pointer), then the register contents   ;
; are assumed to be 0, immediate operand used only.                         ; 
;                                                                           ;
; If Flags.C=1                                                              ;
; IP <= IP + SizeOfThisInstruction + Operand 1 (target instr. addr.)        ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP32CC op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 001h
else
DB 081h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 0D0h + x2 SHL 3 + x1
;--- Conditional bytes 2-5 = 32-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DD op1-@f
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op3 = 0
x5=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x5=1
end if
if op3>15 & op2<256 | op2<-15 & op2>-256
x5=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x5=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x5=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x5=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x5=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x5=7
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*4
DD x6 SHL 31 + x5 SHL 28 + x4 SHL x7 + x3
end if
@@:
}


; JMP64 Reserved


;--- EBC Instruction: JMP64A {@}R1, Immed64 --------------------------------;
;                                                                           ;
; The JMP64A instruction used to unconditionally jump to a absolute         ;
; address and continue executing EBC instructions.                          ;
;                                                                           ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP64A op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 041h
else
DB 0C1h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 000h + x2 SHL 3 + x1
;--- Conditional bytes 2-9 = 64-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DQ op1
else
end if
end if
; Reserved for N+C index.
}


;--- EBC Instruction: JMP64A {@}R1, Immed64 --------------------------------;
;                                                                           ;
; The JMP64A instruction used to conditionally jump to a absolute           ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=0.                                                   ;
;                                                                           ;
; If Flags.C=0                                                              ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP64ACC op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 041h
else
DB 0C1h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 080h + x2 SHL 3 + x1
;--- Conditional bytes 2-9 = 64-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DQ op1
else
end if
end if
; Reserved for N+C index.
}


;--- EBC Instruction: JMP64A {@}R1, Immed64 --------------------------------;
;                                                                           ;
; The JMP64A instruction used to conditionally jump to a absolute           ;
; address and continue executing EBC instructions,                          ;
; condition is Flags.C=1.                                                   ;
;                                                                           ;
; If Flags.C=1                                                              ;
; IP <= Operand 1 (target instr. addr.)                                     ;
;                                                                           ;
; op1 = Value for IP for target instruction address                         ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP64ACS op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq & op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
DB 041h
else
DB 0C1h
end if
;--- Byte 1 = Operand addressing and mode select ---
x1=0
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
DB 0C0h + x2 SHL 3 + x1
;--- Conditional bytes 2-9 = 64-bit index or immediate ---
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 > & op2 eq
else
if op3 eq
DQ op1
else
end if
end if
; Reserved for N+C index.
}


; JMP64CC Reserved
; JMP64CS Reserved


;--- EBC Instruction: JMP8 Immed8 ------------------------------------------;
;                                                                           ;
; Unconditionally jump to a relative offset and continue execution.         ;
; The offset is a signed one-byte offset specified in the number of words.  ;
; The offset is relative to the start of the following instruction.         ;
;                                                                           ;
; IP <= IP + SizeOfThisInstruction + Operand 1 * 2 (target instr. addr.)    ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP8 op1
{
;--- Byte 0 = Operation code ---
DB 002h
;--- Byte 1 = Scaled offset ---
x1=(op1-@f)/2
DB x1
@@:
}


;--- EBC Instruction: JMP8CC Immed8 ----------------------------------------;
;                                                                           ;
; Conditionally jump to a relative offset and continue execution,           ;
; condition is Flags.C=0.                                                   ;
; The offset is a signed one-byte offset specified in the number of words.  ;
; The offset is relative to the start of the following instruction.         ;
;                                                                           ;
; If Flags.C=0                                                              ;
; IP <= IP + SizeOfThisInstruction + Operand 1 * 2 (target instr. addr.)    ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP8CC op1
{
;--- Byte 0 = Operation code ---
DB 082h
;--- Byte 1 = Scaled offset ---
x1=(op1-@f)/2
DB x1
@@:
}


;--- EBC Instruction: JMP8CS Immed8 ----------------------------------------;
;                                                                           ;
; Conditionally jump to a relative offset and continue execution,           ;
; condition is Flags.C=1.                                                   ;
; The offset is a signed one-byte offset specified in the number of words.  ;
; The offset is relative to the start of the following instruction.         ;
;                                                                           ;
; If Flags.C=1                                                              ;
; IP <= IP + SizeOfThisInstruction + Operand 1 * 2 (target instr. addr.)    ;
;                                                                           ;
; op1 = Addend to IP for calculate target instruction address               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO JMP8CS op1
{
;--- Byte 0 = Operation code ---
DB 0C2h
;--- Byte 1 = Scaled offset ---
x1=(op1-@f)/2
DB x1
@@:
}


;--- EBC Instruction: LOADSP [special reg] , R2 ----------------------------;
;                                                                           ;
; This instruction loads a VM dedicated register with the contents of a VM  ;
; general-purpose register R0-R7. The dedicated register is specified by    ;
; the index: 0=Flags, 1=IP, 2-7=Reserved.                                   ;
;                                                                           ;
; Special Reg <= Operand 2                                                  ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO LOADSP op1, op2
{
;--- Byte 0 = Operation code ---
DB 029h
;--- Byte 1 = Two operands addressing ---
if op1 eq Flags
x1=0
else
x1=1
end if
if op2 in < R0 >
x2=0
end if
if op2 in < R1 >
x2=1
end if
if op2 in < R2 >
x2=2
end if
if op2 in < R3 >
x2=3
end if
if op2 in < R4 >
x2=4
end if
if op2 in < R5 > 
x2=5
end if
if op2 in < R6 >
x2=6
end if
if op2 in < R7 >
x2=7
end if
DB x2 SHL 4 + x1
}


;--- EBC Instruction: MOD32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Perform a modulus on two signed 32-bit operands and store the result      ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 MOD Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOD32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 012h
else
DB 092h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MOD64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Perform a modulus on two signed 64-bit operands and store the result      ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 MOD Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOD64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 052h
else
DB 0D2h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MODU32 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Perform a modulus on two unsigned 32-bit operands and store the result    ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 MODU Operand 2                                     ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MODU32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 013h
else
DB 093h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MODU64 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Perform a modulus on two unsigned 64-bit operands and store the result    ;
; to Operand 1.                                                             ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; If Operand 2 = 0, then a divide-by-zero exception is generated.           ;
;                                                                           ;
; Operand 1 <= Operand 1 MOD Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MODU64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 053h
else
DB 0D3h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MOVB {@}R1, {@}R2 ------------------------------------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVB = Move bytes without indexing.                                       ;
;                                                                           ;
; Operand 1 bits [07-00] <= Operand 2 bits [07-00]                          ;
; Operand 1 bits [63-08] <= 0  , if Operand 1 is register                   ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVB op1, op2
{
;--- Byte 0 = Operation code ---
DB 01Dh
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
}


;--- EBC Instruction: MOVBD {@}R1 , {Index32} , {@}R2 , {Index32} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVBD = Move bytes with 32-bit indexing.                                  ;
;                                                                           ;
; Operand 1 bits [07-00] <= Operand 2 bits [07-00]                          ;
; Operand 1 bits [63-08] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVBD op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 021h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x9=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x9=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x9=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x9=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x9=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x9=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*4
DD x10 SHL 31 + x9 SHL 28 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<16 | op3<0 & op3>-16
x14=1
end if
if op3>15 & op3<256 | op3<-15 & op3>-256
x14=2
end if
if op3>255 & op3<4096 | op3<-255 & op3>-4096
x14=3
end if
if op3>4095 & op3<65536 | op3<-4095 & op3>-65536
x14=4
end if
if op3>65535 & op3<1048576 | op3<-65535 & op3>-1048576
x14=5
end if
if op3>1048575 & op3<16777216 | op3<-1048575 & op3>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<16 | op5<0 & op5>-16
x14=1
end if
if op5>15 & op5<256 | op5<-15 & op5>-256
x14=2
end if
if op5>255 & op5<4096 | op5<-255 & op5>-4096
x14=3
end if
if op5>4095 & op5<65536 | op5<-4095 & op5>-65536
x14=4
end if
if op5>65535 & op5<1048576 | op5<-65535 & op5>-1048576
x14=5
end if
if op5>1048575 & op5<16777216 | op5<-1048575 & op5>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVBW {@}R1 , {Index16} , {@}R2 , {Index16} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVBW = Move bytes with 16-bit indexing.                                  ;
;                                                                           ;
; Operand 1 bits [07-00] <= Operand 2 bits [07-00]                          ;
; Operand 1 bits [63-08] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVBW op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 01Dh + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x9=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x9=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x9=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x9=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x9=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x9=6
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*2
DW x10 SHL 15 + x9 SHL 12 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x14=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x14=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x14=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x14=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x14=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x14=6
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 4-5 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<4 | op5<0 & op5>-4
x14=1
end if
if op5>3 & op5<16 | op5<-3 & op5>-16
x14=2
end if
if op5>15 & op5<64 | op5<-15 & op5>-64
x14=3
end if
if op5>63 & op5<256 | op5<-63 & op5>-256
x14=4
end if
if op5>255 & op5<1024 | op5<-255 & op5>-1024
x14=5
end if
if op5>1023 & op5<4096 | op5<-1023 & op5>-4096
x14=6
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVD {@}R1, {@}R2 ------------------------------------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVD = Move dwords without indexing.                                      ;
;                                                                           ;
; Operand 1 bits [31-00] <= Operand 2 bits [15-00]                          ;
; Operand 1 bits [63-32] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVD op1, op2
{
;--- Byte 0 = Operation code ---
DB 01Fh
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
}


;--- EBC Instruction: MOVDD {@}R1 , {Index32} , {@}R2 , {Index32} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVWD = Move dwords with 32-bit indexing.                                 ;
;                                                                           ;
; Operand 1 bits [31-00] <= Operand 2 bits [31-00]                          ;
; Operand 1 bits [63-32] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVDD op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 023h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x9=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x9=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x9=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x9=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x9=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x9=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*4
DD x10 SHL 31 + x9 SHL 28 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<16 | op3<0 & op3>-16
x14=1
end if
if op3>15 & op3<256 | op3<-15 & op3>-256
x14=2
end if
if op3>255 & op3<4096 | op3<-255 & op3>-4096
x14=3
end if
if op3>4095 & op3<65536 | op3<-4095 & op3>-65536
x14=4
end if
if op3>65535 & op3<1048576 | op3<-65535 & op3>-1048576
x14=5
end if
if op3>1048575 & op3<16777216 | op3<-1048575 & op3>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<16 | op5<0 & op5>-16
x14=1
end if
if op5>15 & op5<256 | op5<-15 & op5>-256
x14=2
end if
if op5>255 & op5<4096 | op5<-255 & op5>-4096
x14=3
end if
if op5>4095 & op5<65536 | op5<-4095 & op5>-65536
x14=4
end if
if op5>65535 & op5<1048576 | op5<-65535 & op5>-1048576
x14=5
end if
if op5>1048575 & op5<16777216 | op5<-1048575 & op5>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVDW {@}R1 , {Index16} , {@}R2 , {Index16} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVWW = Move dwords with 16-bit indexing.                                 ;
;                                                                           ;
; Operand 1 bits [31-00] <= Operand 2 bits [31-00]                          ;
; Operand 1 bits [63-32] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVDW op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 01Fh + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x9=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x9=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x9=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x9=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x9=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x9=6
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*2
DW x10 SHL 15 + x9 SHL 12 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x14=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x14=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x14=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x14=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x14=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x14=6
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 4-5 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<4 | op5<0 & op5>-4
x14=1
end if
if op5>3 & op5<16 | op5<-3 & op5>-16
x14=2
end if
if op5>15 & op5<64 | op5<-15 & op5>-64
x14=3
end if
if op5>63 & op5<256 | op5<-63 & op5>-256
x14=4
end if
if op5>255 & op5<1024 | op5<-255 & op5>-1024
x14=5
end if
if op5>1023 & op5<4096 | op5<-1023 & op5>-4096
x14=6
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVIBD {@}R1 {Index16}, Immed32 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 8 bits, Source width = 32 bits,         ;
; form not used, because destination width < source width.                  ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIBD op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: MOVIBQ {@}R1 {Index16}, Immed64 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 8 bits, Source width = 64 bits,         ;
; form not used, because destination width < source width.                  ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIBQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DQ op2
else
DQ op4
end if
}


;--- EBC Instruction: MOVIBW {@}R1 {Index16}, Immed16 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 8 bits, Source width = 16 bits,         ;
; but this 16-bit immediate form used, 8-bit immediate not supported,       ;
; because word alignment required.                                          ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIBW op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 077h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: MOVIDD {@}R1 {Index16}, Immed32 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 32 bits, Source width = 32 bits,        ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIDD op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 020h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: MOVIDQ {@}R1 {Index16}, Immed64 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 32 bits, Source width = 64 bits,        ;
; form not used, because destination width < source width.                  ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIDQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 020h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DQ op2
else
DQ op4
end if
}


;--- EBC Instruction: MOVIDW {@}R1 {Index16}, Immed16 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 32 bits, Source width = 16 bits,        ;
; use sign-extension.                                                       ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIDW op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 077h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 020h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: MOVIND {@}R1 {Index16}, Index32 ----------------------;
;                                                                           ;
; This instruction moves an indexed value of form (+n,+c) to Operand 1.     ;
; The index value is converted from (+n,+c) format to a signed offset.      ;
; This form use 32-bit Operand 2 index data.                                ;
;                                                                           ;
; Operand 1 <= Operand 2 Index value                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIND op1, op2, op3, op4, op5
{

;--- Byte 0 = Operation code ---
DB 0B8h

;--- Byte 1 = First operand addressing and index presence ---
if op5 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index for first operand addressing ---
if op5 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if

;--- Conditional bytes 2-5 = 32-bit index for first operand addressing ---
;--- Conditional bytes 4-7 = 32-bit index for second operand loading ---
if op5 eq

if op2 < 0
x9=-op2
else
x9=op2
end if
if op3 < 0
x10=-op3
else
x10=op3
end if
if op2 = 0
x11=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x11=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x11=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x11=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x11=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x11=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x11=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x11=7
end if
end if
if op2<0
x12=1
else
x12=0
end if
x13=x11*4
DD x12 SHL 31 + x11 SHL 28 + x10 SHL x13 + x9

else

if op4 < 0
x9=-op4
else
x9=op4
end if
if op5 < 0
x10=-op5
else
x10=op5
end if
if op4 = 0
x11=0
else
if op4>0 & op4<16 | op4<0 & op4>-16
x11=1
end if
if op4>15 & op4<256 | op4<-15 & op4>-256
x11=2
end if
if op4>255 & op4<4096 | op4<-255 & op4>-4096
x11=3
end if
if op4>4095 & op4<65536 | op4<-4095 & op4>-65536
x11=4
end if
if op4>65535 & op4<1048576 | op4<-65535 & op4>-1048576
x11=5
end if
if op4>1048575 & op4<16777216 | op4<-1048575 & op4>-16777216
x11=6
end if
if op4>16777215 & op4<268435456 | op4<-16777216 & op4>-268435456
x11=7
end if
end if
if op4<0
x12=1
else
x12=0
end if
x13=x11*4
DD x12 SHL 31 + x11 SHL 28 + x10 SHL x13 + x9

end if

}


;--- EBC Instruction: MOVINQ {@}R1 {Index16}, Index64 ----------------------;
;                                                                           ;
; This instruction moves an indexed value of form (+n,+c) to Operand 1.     ;
; The index value is converted from (+n,+c) format to a signed offset.      ;
; This form use 64-bit Operand 2 index data.                                ;
;                                                                           ;
; Operand 1 <= Operand 2 Index value                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVINQ op1, op2, op3, op4, op5
{

;--- Byte 0 = Operation code ---
DB 0F8h

;--- Byte 1 = First operand addressing and index presence ---
if op5 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index for first operand addressing ---
if op5 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if

;--- Conditional bytes 2-5 = 32-bit index for first operand addressing ---
;--- Conditional bytes 4-7 = 32-bit index for second operand loading ---
if op5 eq

if op2 < 0
x9=-op2
else
x9=op2
end if
if op3 < 0
x10=-op3
else
x10=op3
end if
if op2 = 0
x11=0
else
if op2>0 & op2<100h | op2<0 & op2>-100h
x11=1
end if
if op2>0FFh & op2<10000h | op2<-0FFh & op2>-10000h
x11=2
end if
if op2>0FFFFh & op2<1000000h | op2<-0FFFFh & op2>-1000000h
x11=3
end if
if op2>0FFFFFFh & op2<100000000h | op2<-0FFFFFFh & op2>-100000000h
x11=4
end if
if op2>0FFFFFFFFh & op2<10000000000h | op2<-0FFFFFFFFh & op2>-10000000000h
x11=5
end if
if op2>0FFFFFFFFFFh & op2<1000000000000h | op2<-0FFFFFFFFFFh & op2>-1000000000000h
x11=6
end if
if op2>0FFFFFFFFFFFFh & op2<100000000000000h | op2<-0FFFFFFFFFFFFh & op2>-100000000000000h
x11=7
end if
end if
if op2<0
x12=1
else
x12=0
end if
x13=x11*8
DQ x12 SHL 63 + x11 SHL 60 + x10 SHL x13 + x9

else

if op4 < 0
x9=-op4
else
x9=op4
end if
if op5 < 0
x10=-op5
else
x10=op5
end if
if op4 = 0
x11=0
else
if op4>0 & op4<100h | op4<0 & op4>-100h
x11=1
end if
if op4>0FFh & op4<10000h | op4<-0FFh & op4>-10000h
x11=2
end if
if op4>0FFFFh & op4<1000000h | op4<-0FFFFh & op4>-1000000h
x11=3
end if
if op4>0FFFFFFh & op4<100000000h | op4<-0FFFFFFh & op4>-100000000h
x11=4
end if
if op4>0FFFFFFFFh & op4<10000000000h | op4<-0FFFFFFFFh & op4>-10000000000h
x11=5
end if
if op4>0FFFFFFFFFFh & op4<1000000000000h | op4<-0FFFFFFFFFFh & op4>-1000000000000h
x11=6
end if
if op4>0FFFFFFFFFFFFh & op4<100000000000000h | op4<-0FFFFFFFFFFFFh & op4>-100000000000000h
x11=7
end if
end if
if op4<0
x12=1
else
x12=0
end if
x13=x11*8
DQ x12 SHL 63 + x11 SHL 60 + x10 SHL x13 + x9

end if

}


;--- EBC Instruction: MOVINW {@}R1 {Index16}, Index16 ----------------------;
;                                                                           ;
; This instruction moves an indexed value of form (+n,+c) to Operand 1.     ;
; The index value is converted from (+n,+c) format to a signed offset.      ;
; This form use 16-bit Operand 2 index data.                                ;
;                                                                           ;
; Operand 1 <= Operand 2 Index value                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVINW op1, op2, op3, op4, op5
{

;--- Byte 0 = Operation code ---
DB 078h

;--- Byte 1 = First operand addressing and index presence ---
if op5 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index for first operand addressing ---
;--- Conditional bytes 2-3 = 16-bit index for second operand loading ---
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4

;--- Conditional bytes 4-5 = 16-bit index for second operand loading ---
if op5 eq
else
if op4 < 0
x9=-op4
else
x9=op4
end if
if op5 < 0
x10=-op5
else
x10=op5
end if
if op4 = 0
x11=0
else
if op4>0 & op4<4 | op4<0 & op4>-4
x11=1
end if
if op4>2 & op4<16 | op4<-3 & op4>-16
x11=2
end if
if op4>15 & op4<64 | op4<-15 & op4>-64
x11=3
end if
if op4>63 & op4<256 | op4<-63 & op4>-256
x11=4
end if
if op4>255 & op4<1024 | op4<-255 & op4>-1024
x11=5
end if
if op4>1023 & op4<4096 | op4<-1023 & op4>-4096
x11=6
end if
end if
if op4<0
x12=1
else
x12=0
end if
x13=x11*2
DW x12 SHL 15 + x11 SHL 12 + x10 SHL x13 + x9
end if

}


;--- EBC Instruction: MOVIQD {@}R1 {Index16}, Immed32 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 64 bits, Source width = 32 bits,        ;
; use sign-extension.                                                       ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIQD op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 030h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: MOVIQQ {@}R1 {Index16}, Immed64 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 64 bits, Source width = 64 bits.        ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIQQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 030h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DQ op2
else
DQ op4
end if
}


;--- EBC Instruction: MOVIQW {@}R1 {Index16}, Immed16 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 64 bits, Source width = 16 bits,        ;
; use sign-extension.                                                       ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIQW op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 077h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 030h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DW op2
else
DW op4
end if
}


;--- EBC Instruction: MOVIWD {@}R1 {Index16}, Immed32 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 16 bits, Source width = 32 bits,        ;
; form not used, because destination width < source width.                  ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIWD op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 010h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: MOVIWQ {@}R1 {Index16}, Immed64 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 16 bits, Source width = 64 bits,        ;
; form not used, because destination width < source width.                  ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIWQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F7h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 010h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DQ op2
else
DQ op4
end if
}


;--- EBC Instruction: MOVIWW {@}R1 {Index16}, Immed16 ----------------------;
;                                                                           ;
; This instruction moves a signed immediate value to Operand 1. In the      ;
; instruction syntax, the first variable character specifies the width of   ;
; the move, which may be 8 (b), 16 (w), 32 (d), 64 (q) bits.  The second    ;
; variable character specifies the width of the immediate data, which may   ;
; be 16 (b), 32 (d), 64 (q) bits.                                           ;
; This form use destination width = 16 bits, Source width = 16 bits.        ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVIWW op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 077h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 010h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2,3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x7*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op4 eq
DW op2
else
DW op4
end if
}


; MOVN Reserved


;--- EBC Instruction: MOVND {@}R1 , {Index32} , {@}R2 , {Index32} ----------;
;                                                                           ;
; This instruction loads an unsigned natural value from Operand 2 and       ;
; stores the value to Operand 1. Both operands can be indexed, though       ;
; both operand indexes are the same size.                                   ;
; MOVND = Move natural with 32-bit indexing.                                ;
;                                                                           ;
; Operand 1 <= (UINTN) Operand 2                                            ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVND op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 033h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x9=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x9=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x9=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x9=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x9=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x9=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*4
DD x10 SHL 31 + x9 SHL 28 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<16 | op3<0 & op3>-16
x14=1
end if
if op3>15 & op3<256 | op3<-15 & op3>-256
x14=2
end if
if op3>255 & op3<4096 | op3<-255 & op3>-4096
x14=3
end if
if op3>4095 & op3<65536 | op3<-4095 & op3>-65536
x14=4
end if
if op3>65535 & op3<1048576 | op3<-65535 & op3>-1048576
x14=5
end if
if op3>1048575 & op3<16777216 | op3<-1048575 & op3>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<16 | op5<0 & op5>-16
x14=1
end if
if op5>15 & op5<256 | op5<-15 & op5>-256
x14=2
end if
if op5>255 & op5<4096 | op5<-255 & op5>-4096
x14=3
end if
if op5>4095 & op5<65536 | op5<-4095 & op5>-65536
x14=4
end if
if op5>65535 & op5<1048576 | op5<-65535 & op5>-1048576
x14=5
end if
if op5>1048575 & op5<16777216 | op5<-1048575 & op5>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVNW {@}R1 , {Index16} , {@}R2 , {Index16} ----------;
;                                                                           ;
; This instruction loads an unsigned natural value from Operand 2 and       ;
; stores the value to Operand 1. Both operands can be indexed, though       ;
; both operand indexes are the same size.                                   ;
; MOVNW = Move natural with 16-bit indexing.                                ;
;                                                                           ;
; Operand 1 <= (UINTN) Operand 2                                            ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVNW op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---


;if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x1=1
;if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x2=1
;else
;x2=0
;end if
;else
;x1=0
;if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x2=1
;else
;x2=0
;end if
;end if


if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=0
if op3 eq
x2=0
else
x2=1
end if
else
x1=1
if op5 eq
x2=0
else
x2=1
end if
end if


DB 032h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x9=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x9=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x9=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x9=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x9=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x9=6
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*2
DW x10 SHL 15 + x9 SHL 12 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x14=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x14=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x14=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x14=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x14=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x14=6
end if
end if
if op3<0 | op4<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 4-5 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<4 | op5<0 & op5>-4
x14=1
end if
if op5>3 & op5<16 | op5<-3 & op5>-16
x14=2
end if
if op5>15 & op5<64 | op5<-15 & op5>-64
x14=3
end if
if op5>63 & op5<256 | op5<-63 & op5>-256
x14=4
end if
if op5>255 & op5<1024 | op5<-255 & op5>-1024
x14=5
end if
if op5>1023 & op5<4096 | op5<-1023 & op5>-4096
x14=6
end if
end if
if op5<0 | op6<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVQ {@}R1, {@}R2 ------------------------------------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVD = Move qwords without indexing.                                      ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVQ op1, op2
{
;--- Byte 0 = Operation code ---
DB 020h
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
}


;--- EBC Instruction: MOVQD {@}R1 , {Index32} , {@}R2 , {Index32} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVQD = Move qwords with 32-bit indexing.                                 ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVQD op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 024h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x9=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x9=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x9=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x9=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x9=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x9=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*4
DD x10 SHL 31 + x9 SHL 28 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<16 | op3<0 & op3>-16
x14=1
end if
if op3>15 & op3<256 | op3<-15 & op3>-256
x14=2
end if
if op3>255 & op3<4096 | op3<-255 & op3>-4096
x14=3
end if
if op3>4095 & op3<65536 | op3<-4095 & op3>-65536
x14=4
end if
if op3>65535 & op3<1048576 | op3<-65535 & op3>-1048576
x14=5
end if
if op3>1048575 & op3<16777216 | op3<-1048575 & op3>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<16 | op5<0 & op5>-16
x14=1
end if
if op5>15 & op5<256 | op5<-15 & op5>-256
x14=2
end if
if op5>255 & op5<4096 | op5<-255 & op5>-4096
x14=3
end if
if op5>4095 & op5<65536 | op5<-4095 & op5>-65536
x14=4
end if
if op5>65535 & op5<1048576 | op5<-65535 & op5>-1048576
x14=5
end if
if op5>1048575 & op5<16777216 | op5<-1048575 & op5>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVQQ {@}R1 , {Index64} , {@}R2 , {Index64} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVQQ = Move qwords with 64-bit indexing.                                 ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVQQ op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 028h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<100h | op2<0 & op2>-100h
x9=1
end if
if op2>0FFh & op2<10000h | op2<-0FFh & op2>-10000h
x9=2
end if
if op2>0FFFFh & op2<1000000h | op2<-0FFFFh & op2>-1000000h
x9=3
end if
if op2>0FFFFFFh & op2<100000000h | op2<-0FFFFFFh & op2>-100000000h
x9=4
end if
if op2>0FFFFFFFFh & op2<10000000000h | op2<-0FFFFFFFFh & op2>-10000000000h
x9=5
end if
if op2>0FFFFFFFFFFh & op2<1000000000000h | op2<-0FFFFFFFFFFh & op2>-1000000000000h
x9=6
end if
if op2>0FFFFFFFFFFFFh & op2<100000000000000h | op2<-0FFFFFFFFFFFFh & op2>-100000000000000h
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*8
DQ x10 SHL 63 + x9 SHL 60 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<100h | op3<0 & op3>-100h
x14=1
end if
if op3>0FFh & op3<10000h | op3<-0FFh & op3>-10000h
x14=2
end if
if op3>0FFFFh & op3<1000000h | op3<-0FFFFh & op3>-1000000h
x14=3
end if
if op3>0FFFFFFh & op3<100000000h | op3<-0FFFFFFh & op3>-100000000h
x14=4
end if
if op3>0FFFFFFFFh & op3<10000000000h | op3<-0FFFFFFFFh & op3>-10000000000h
x14=5
end if
if op3>0FFFFFFFFFFh & op3<1000000000000h | op3<-0FFFFFFFFFFh & op3>-1000000000000h
x14=6
end if
if op3>0FFFFFFFFFFFFh & op3<100000000000000h | op3<-0FFFFFFFFFFFFh & op3>-100000000000000h
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*8
DQ x15 SHL 63 + x14 SHL 60 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<100h | op5<0 & op5>-100h
x14=1
end if
if op5>0FFh & op5<10000h | op5<-0FFh & op5>-10000h
x14=2
end if
if op5>0FFFFh & op5<1000000h | op5<-0FFFFh & op5>-1000000h
x14=3
end if
if op5>0FFFFFFh & op5<100000000h | op5<-0FFFFFFh & op5>-100000000h
x14=4
end if
if op5>0FFFFFFFFh & op5<10000000000h | op5<-0FFFFFFFFh & op5>-10000000000h
x14=5
end if
if op5>0FFFFFFFFFFh & op5<1000000000000h | op5<-0FFFFFFFFFFh & op5>-1000000000000h
x14=6
end if
if op5>0FFFFFFFFFFFFh & op5<100000000000000h | op5<-0FFFFFFFFFFFFh & op5>-100000000000000h
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*8
DQ x15 SHL 63 + x14 SHL 60 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVQW {@}R1 , {Index16} , {@}R2 , {Index16} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVQW = Move qwords with 16-bit indexing.                                 ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVQW op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 020h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x9=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x9=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x9=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x9=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x9=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x9=6
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*2
DW x10 SHL 15 + x9 SHL 12 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x14=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x14=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x14=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x14=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x14=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x14=6
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 4-5 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<4 | op5<0 & op5>-4
x14=1
end if
if op5>3 & op5<16 | op5<-3 & op5>-16
x14=2
end if
if op5>15 & op5<64 | op5<-15 & op5>-64
x14=3
end if
if op5>63 & op5<256 | op5<-63 & op5>-256
x14=4
end if
if op5>255 & op5<1024 | op5<-255 & op5>-1024
x14=5
end if
if op5>1023 & op5<4096 | op5<-1023 & op5>-4096
x14=6
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVRELD {@}R1 {Index16}, Immed16 ---------------------;
;                                                                           ;
; This instruction calculates an IP-relative immediate offset for Operand 2 ;
; and stores the result to Operand 1. The offset is a signed offset         ;
; relative to the following instruction.                                    ;
; This form adds 32-bit offset to the next instruction address.             ;
;                                                                           ;
; Operand 1 <= IP + ThisInstructionSize + Immediate                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVRELD op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0B9h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2-3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-5 or 4-7 = Immediate data ---
if op3 eq
DD op2
else
DD op4
end if
}


;--- EBC Instruction: MOVRELQ {@}R1 {Index16}, Immed16 ---------------------;
;                                                                           ;
; This instruction calculates an IP-relative immediate offset for Operand 2 ;
; and stores the result to Operand 1. The offset is a signed offset         ;
; relative to the following instruction.                                    ;
; This form adds 64-bit offset to the next instruction address.             ;
;                                                                           ;
; Operand 1 <= IP + ThisInstructionSize + Immediate                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVRELQ op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 0F9h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2-3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-9 or 4-11 = Immediate data ---
if op3 eq
DQ op2
else
DQ op4
end if
}


;--- EBC Instruction: MOVRELW {@}R1 {Index16}, Immed16 ---------------------;
;                                                                           ;
; This instruction calculates an IP-relative immediate offset for Operand 2 ;
; and stores the result to Operand 1. The offset is a signed offset         ;
; relative to the following instruction.                                    ;
; This form adds 16-bit offset to the next instruction address.             ;
;                                                                           ;
; Operand 1 <= IP + ThisInstructionSize + Immediate                         ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Natural offset, optional                                            ;
; op3 = Constant offset, optional                                           ;
; op4 = Immediate data                                                      ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVRELW op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
DB 079h
;--- Byte 1 = First operand addressing and size ---
if op3 eq
x1=0
else
x1=1
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 >
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
DB 000h + x1 SHL 6 + x2 SHL 3 + x3
;--- Conditional bytes 2-3 = 16-bit index ---
if op3 eq
else
if op2 < 0
x4=-op2
else
x4=op2
end if
if op3 < 0
x5=-op3
else
x5=op3
end if
if op2 = 0
x6=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x6=1
end if
if op2>2 & op2<16 | op2<-3 & op2>-16
x6=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x6=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x6=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x6=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x6=6
end if
end if
if op2<0
x7=1
else
x7=0
end if
x8=x6*2
DW x7 SHL 15 + x6 SHL 12 + x5 SHL x8 + x4
end if
;--- Bytes 2-3 or 4-5 = Immediate data ---
if op3 eq
DW op2
else
DW op4
end if
}


; MOVSN Reserved


;--- EBC Instruction: MOVSND {@}R1, {Index32}, {@}R2, {Index32|Immed32} ----;
;                                                                           ;
; Moves a signed natural value from Operand 2 to Operand 1.                 ;
; Both operands can be indexed, though the indexes are the same size.       ;
; This form use 32-bit indexes.                                             ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset or Index, optional                            ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVSND op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---


;if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x1=1
;if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x2=1
;else
;x2=0
;end if
;else
;x1=0
;if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x2=1
;else
;x2=0
;end if
;end if


if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=0
if op3 eq
x2=0
else
x2=1
end if
else
x1=1
if op5 eq
x2=0
else
x2=1
end if
end if


DB 026h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x9=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x9=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x9=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x9=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x9=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x9=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*4
DD x10 SHL 31 + x9 SHL 28 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op4 eq

if op3 eq
else
DD op3
end if

else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<16 | op3<0 & op3>-16
x14=1
end if
if op3>15 & op3<256 | op3<-15 & op3>-256
x14=2
end if
if op3>255 & op3<4096 | op3<-255 & op3>-4096
x14=3
end if
if op3>4095 & op3<65536 | op3<-4095 & op3>-65536
x14=4
end if
if op3>65535 & op3<1048576 | op3<-65535 & op3>-1048576
x14=5
end if
if op3>1048575 & op3<16777216 | op3<-1048575 & op3>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq

if op5 eq
else
DW op5
end if

else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<16 | op5<0 & op5>-16
x14=1
end if
if op5>15 & op5<256 | op5<-15 & op5>-256
x14=2
end if
if op5>255 & op5<4096 | op5<-255 & op5>-4096
x14=3
end if
if op5>4095 & op5<65536 | op5<-4095 & op5>-65536
x14=4
end if
if op5>65535 & op5<1048576 | op5<-65535 & op5>-1048576
x14=5
end if
if op5>1048575 & op5<16777216 | op5<-1048575 & op5>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVSNW {@}R1, {Index16}, {@}R2, {Index16|Immed16} ----;
;                                                                           ;
; Moves a signed natural value from Operand 2 to Operand 1.                 ;
; Both operands can be indexed, though the indexes are the same size.       ;
; This form use 16-bit indexes.                                             ;
;                                                                           ;
; Operand 1 <= Operand 2                                                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset or Index, optional                            ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVSNW op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---


;if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x1=1
;if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x2=1
;else
;x2=0
;end if
;else
;x1=0
;if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
;x2=1
;else
;x2=0
;end if
;end if


if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=0
if op3 eq
x2=0
else
x2=1
end if
else
x1=1
if op5 eq
x2=0
else
x2=1
end if
end if


DB 025h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x9=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x9=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x9=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x9=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x9=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x9=6
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*2
DW x10 SHL 15 + x9 SHL 12 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op4 eq

if op3 eq
else
DW op3
end if

else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x14=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x14=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x14=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x14=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x14=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x14=6
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 4-5 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq

if op5 eq
else
DW op5
end if

else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<4 | op5<0 & op5>-4
x14=1
end if
if op5>3 & op5<16 | op5<-3 & op5>-16
x14=2
end if
if op5>15 & op5<64 | op5<-15 & op5>-64
x14=3
end if
if op5>63 & op5<256 | op5<-63 & op5>-256
x14=4
end if
if op5>255 & op5<1024 | op5<-255 & op5>-1024
x14=5
end if
if op5>1023 & op5<4096 | op5<-1023 & op5>-4096
x14=6
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVW {@}R1, {@}R2 ------------------------------------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVW = Move words without indexing.                                       ;
;                                                                           ;
; Operand 1 bits [15-00] <= Operand 2 bits [15-00]                          ;
; Operand 1 bits [63-16] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVW op1, op2
{
;--- Byte 0 = Operation code ---
DB 01Eh
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
}


;--- EBC Instruction: MOVWD {@}R1 , {Index32} , {@}R2 , {Index32} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVWD = Move words with 32-bit indexing.                                  ;
;                                                                           ;
; Operand 1 bits [15-00] <= Operand 2 bits [15-00]                          ;
; Operand 1 bits [63-16] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVWD op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 022h + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<16 | op2<0 & op2>-16
x9=1
end if
if op2>15 & op2<256 | op2<-15 & op2>-256
x9=2
end if
if op2>255 & op2<4096 | op2<-255 & op2>-4096
x9=3
end if
if op2>4095 & op2<65536 | op2<-4095 & op2>-65536
x9=4
end if
if op2>65535 & op2<1048576 | op2<-65535 & op2>-1048576
x9=5
end if
if op2>1048575 & op2<16777216 | op2<-1048575 & op2>-16777216
x9=6
end if
if op2>16777215 & op2<268435456 | op2<-16777216 & op2>-268435456
x9=7
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*4
DD x10 SHL 31 + x9 SHL 28 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-5 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<16 | op3<0 & op3>-16
x14=1
end if
if op3>15 & op3<256 | op3<-15 & op3>-256
x14=2
end if
if op3>255 & op3<4096 | op3<-255 & op3>-4096
x14=3
end if
if op3>4095 & op3<65536 | op3<-4095 & op3>-65536
x14=4
end if
if op3>65535 & op3<1048576 | op3<-65535 & op3>-1048576
x14=5
end if
if op3>1048575 & op3<16777216 | op3<-1048575 & op3>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 6-9 = 32-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<16 | op5<0 & op5>-16
x14=1
end if
if op5>15 & op5<256 | op5<-15 & op5>-256
x14=2
end if
if op5>255 & op5<4096 | op5<-255 & op5>-4096
x14=3
end if
if op5>4095 & op5<65536 | op5<-4095 & op5>-65536
x14=4
end if
if op5>65535 & op5<1048576 | op5<-65535 & op5>-1048576
x14=5
end if
if op5>1048575 & op5<16777216 | op5<-1048575 & op5>-16777216
x14=6
end if
if op3>16777215 & op3<268435456 | op3<-16777216 & op3>-268435456
x14=7
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*4
DD x15 SHL 31 + x14 SHL 28 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MOVWW {@}R1 , {Index16} , {@}R2 , {Index16} ----------;
;                                                                           ;
; This instruction moves data from Operand 2 to Operand 1. Both operands    ;
; can be indexed, though both indexes are the same size. In the instruction ;
; syntax for the first form, the first variable character indicates the     ;
; size of the data move. which can be 8 bits (b), 16 bits (w), 32 bits (d), ;
; or 64 bits (q). The optional character indicates the presence and size of ;
; the index value(s), which may be 16 bits (w) or 32 bits (d). The MOVQQ    ;
; instruction adds support for 64-bit indexes.                              ;
; MOVWW = Move words with 16-bit indexing.                                  ;
;                                                                           ;
; Operand 1 bits [15-00] <= Operand 2 bits [15-00]                          ;
; Operand 1 bits [63-16] <= 0 , if Operand 1 is register                    ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Destination natural offset, optional                                ;
; op3 = Destination constant offset, optional                               ;
; op4 = Source                                                              ;
; op5 = Source natural offset, optional                                     ;
; op6 = Source constant offset, optional                                    ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MOVWW op1, op2, op3, op4, op5, op6
{

;--- Byte 0 = Operation code ---
if op1 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x1=1
if op4 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
else
x1=0
if op2 in < @R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
x2=1
else
x2=0
end if
end if
DB 01Eh + x1 SHL 7 + x2 SHL 6

;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x3=0
end if
if op1 in < R1,@R1 >
x3=1
end if
if op1 in < R2,@R2 >
x3=2
end if
if op1 in < R3,@R3 >
x3=3
end if
if op1 in < R4,@R4 >
x3=4
end if
if op1 in < R5,@R5 > 
x3=5
end if
if op1 in < R6,@R6 >
x3=6
end if
if op1 in < R7,@R7 >
x3=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op2 in < R0,@R0 >
x5=0
end if
if op2 in < R1,@R1 >
x5=1
end if
if op2 in < R2,@R2 >
x5=2
end if
if op2 in < R3,@R3 >
x5=3
end if
if op2 in < R4,@R4 >
x5=4
end if
if op2 in < R5,@R5 >
x5=5
end if
if op2 in < R6,@R6 >
x5=6
end if
if op2 in < R7,@R7 >
x5=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
else
if op4 in < R0,@R0 >
x5=0
end if
if op4 in < R1,@R1 >
x5=1
end if
if op4 in < R2,@R2 >
x5=2
end if
if op4 in < R3,@R3 >
x5=3
end if
if op4 in < R4,@R4 >
x5=4
end if
if op4 in < R5,@R5 >
x5=5
end if
if op4 in < R6,@R6 >
x5=6
end if
if op4 in < R7,@R7 >
x5=7
end if
if op4 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x6=0
else
x6=1
end if
end if
DB x6 SHL 7 + x5 SHL 4 + x4 SHL 3 + x3

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 1 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op3 eq
else
if op2 < 0
x7=-op2
else
x7=op2
end if
if op3 < 0
x8=-op3
else
x8=op3
end if
if op2 = 0
x9=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x9=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x9=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x9=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x9=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x9=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x9=6
end if
end if
if op2<0
x10=1
else
x10=0
end if
x11=x9*2
DW x10 SHL 15 + x9 SHL 12 + x8 SHL x11 + x7
end if
end if

;--- Conditional bytes 2-3 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
if op3 eq
else
if op3 < 0
x12=-op3
else
x12=op3
end if
if op4 < 0
x13=-op4
else
x13=op4
end if
if op3 = 0
x14=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x14=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x14=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x14=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x14=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x14=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x14=6
end if
end if
if op3<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

;--- Conditional bytes 4-5 = 16-bit index or immediate for Operand 2 ---
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7,@R0,@R1,@R2,@R3,@R4,@R5,@R6,@R7 >
else
if op6 eq
else
if op5 < 0
x12=-op5
else
x12=op5
end if
if op6 < 0
x13=-op6
else
x13=op6
end if
if op5 = 0
x14=0
else
if op5>0 & op5<4 | op5<0 & op5>-4
x14=1
end if
if op5>3 & op5<16 | op5<-3 & op5>-16
x14=2
end if
if op5>15 & op5<64 | op5<-15 & op5>-64
x14=3
end if
if op5>63 & op5<256 | op5<-63 & op5>-256
x14=4
end if
if op5>255 & op5<1024 | op5<-255 & op5>-1024
x14=5
end if
if op5>1023 & op5<4096 | op5<-1023 & op5>-4096
x14=6
end if
end if
if op5<0
x15=1
else
x15=0
end if
x16=x14*2
DW x15 SHL 15 + x14 SHL 12 + x13 SHL x16 + x12
end if
end if

}


;--- EBC Instruction: MUL32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Perform a signed multiply of two operands and store the result back to    ;
; Operand 1. For 32-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 * Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MUL32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Eh
else
DB 08Eh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MUL64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Perform a signed multiply of two operands and store the result back to    ;
; Operand 1. For 64-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 * Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MUL64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 04Eh
else
DB 0CEh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MULU32 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Perform a unsigned multiply of two operands and store the result back to  ;
; Operand 1. For 32-bit.
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 * Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MULU32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Fh
else
DB 08Fh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: MULU64 {@}R1, {@}R2, {Index16|Immed16} ---------------;
;                                                                           ;
; Perform a unsigned multiply of two operands and store the result back to  ;
; Operand 1. For 64-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 * Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO MULU64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 04Fh
else
DB 0CFh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: NEG32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Multiply Operand 2 by negative 1, and store the result back to Operand 1. ;
; Operand 2 is a signed value. Operation is 32-bit.                         ;
;                                                                           ;
; Operand 1 <= -1 * Operand 2                                               ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO NEG32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Bh
else
DB 08Bh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: NEG64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Multiply Operand 2 by negative 1, and store the result back to Operand 1. ;
; Operand 2 is a signed value. Operation is 64-bit.                         ;
;                                                                           ;
; Operand 1 <= -1 * Operand 2                                               ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO NEG64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 04Bh
else
DB 0CBh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: NOT32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a logical NOT operation on Operand 2, an unsigned value, and     ;
; stores the result back to Operand 1. Operation is 32-bit.                 ;
;                                                                           ;
; Operand 1 <= NOT Operand 2                                                ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO NOT32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Ah
else
DB 08Ah
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: NOT64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a logical NOT operation on Operand 2, an unsigned value, and     ;
; stores the result back to Operand 1. Operation is 64-bit.                 ;
;                                                                           ;
; Operand 1 <= NOT Operand 2                                                ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO NOT64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 04Ah
else
DB 0CAh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: OR32 {@}R1, {@}R2, {Index16|Immed16} -----------------;
;                                                                           ;
; Performs a Logical OR on two operands and stores the result to            ;
; Operand 1, for 32-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 OR Operand 2                                       ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO OR32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 015h
else
DB 095h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: OR64 {@}R1, {@}R2, {Index16|Immed16} -----------------;
;                                                                           ;
; Performs a Logical OR on two operands and stores the result to            ;
; Operand 1, for 64-bit.                                                    ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 AND Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO OR64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 055h
else
DB 0D5h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: POP32 {@}R1, {Index16|Immed16} -----------------------;
;                                                                           ;
; Read an unsigned 32-bit value from memory pointed to by stack pointer     ;
; R0, adjust the stack pointer accordingly, and store the value back to     ;
; Operand 1.                                                                ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the value popped  ;
; from the stack and the result is stored back to the Operand 1 register.   ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the value popped ;
; from the stack is stored at [R1+Index16].                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored to     ;
; the Operand 1 register with the upper 32 bits cleared.                    ;
;                                                                           ;
; Operand 1 <= [R0]                                                         ;
; R0   <= R0 + 4                                                            ;
;                                                                           ;
; op1 = Operand to store popped value                                       ;
; op2 = Natural offset or addend to popped value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO POP32 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 02Ch
else
DB 0ACh
end if
;--- Byte 1 = Operand addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: POP64 {@}R1, {Index16|Immed16} -----------------------;
;                                                                           ;
; Read an unsigned 32-bit value from memory pointed to by stack pointer     ;
; R0, adjust the stack pointer accordingly, and store the value back to     ;
; Operand 1.                                                                ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the value popped  ;
; from the stack and the result is stored back to the Operand 1 register.   ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the value popped ;
; from the stack is stored at [R1+Index16].                                 ;
;                                                                           ;
; Operand 1 <= [R0]                                                         ;
; R0   <= R0 + 8                                                            ;
;                                                                           ;
; op1 = Operand to store popped value                                       ;
; op2 = Natural offset or addend to popped value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO POP64 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 06Ch
else
DB 0ECh
end if
;--- Byte 1 = Operand addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: POPN {@}R1, {Index16|Immed16} ------------------------;
;                                                                           ;
; Read an unsigned natural value from memory pointed to by stack pointer    ;
; R0, adjust the stack pointer accordingly, and store the value back to     ;
; Operand 1.                                                                ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the value popped  ;
; from the stack and the result is stored back to the Operand 1 register.   ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the value popped ;
; from the stack is stored at [R1+Index16].                                 ;
; If Operand 1 is direct, and the instruction is executed on a 32-bit       ;
; machine, then the result is stored to the Operand 1 register with the     ;
; upper 32 bits cleared.                                                    ;
;                                                                           ;
; Operand 1 <= NaturalSize [R0]                                             ;
; R0   <= R0 + NaturalSize                                                  ;
;                                                                           ;
; op1 = Operand to store popped value                                       ;
; op2 = Natural offset or addend to popped value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO POPN op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 036h
else
DB 0B6h
end if
;--- Byte 1 = Operand addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: PUSH32 {@}R1, {Index16|Immed16} ----------------------;
;                                                                           ;
; Adjust the stack pointer R0 and store a 32-bit value on the stack.        ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the Operand 1     ;
; register contents such that Operand 1 = R1+Immed16.                       ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the Operand 1    ;
; value pushed is fetched from [R1+Index16].                                ;
;                                                                           ;
; R0   <= R0 - 4                                                            ;
; [R0] <= Operand 1                                                         ;
;                                                                           ;
; op1 = Operand to push                                                     ;
; op2 = Natural offset or addend to pushed value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO PUSH32 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 02Bh
else
DB 0ABh
end if
;--- Byte 1 = Operand addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: PUSH64 {@}R1, {Index16|Immed16} ----------------------;
;                                                                           ;
; Adjust the stack pointer R0 and store a 64-bit value on the stack.        ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the Operand 1     ;
; register contents such that Operand 1 = R1+Immed16.                       ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the Operand 1    ;
; value pushed is fetched from [R1+Index16].                                ;
;                                                                           ;
; R0   <= R0 - 8                                                            ;
; [R0] <= Operand 1                                                         ;
;                                                                           ;
; op1 = Operand to push                                                     ;
; op2 = Natural offset or addend to pushed value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO PUSH64 op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 06Bh
else
DB 0EBh
end if
;--- Byte 1 = Operand addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: PUSHN {@}R1, {Index16|Immed16} -----------------------;
;                                                                           ;
; Adjust the stack pointer R0 and store a natural value on the stack.       ;
; If Operand 1 is direct and an index/immediate data is specified, then the ;
; immediate data is fetched as a signed value is added to the Operand 1     ;
; register contents such that Operand 1 = R1+Immed16.                       ;
; If Operand 1 is indirect and an index/immediate data is specified, then   ;
; the immediate data is interpreted as a natural index and the Operand 1    ;
; value pushed is fetched from [R1+Index16].                                ;
;                                                                           ;
; R0   <= R0 - NaturalSize                                                  ;
; [R0] <= Operand 1                                                         ;
;                                                                           ;
; op1 = Operand to push                                                     ;
; op2 = Natural offset or addend to pushed value, optional                  ;
; op3 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO PUSHN op1, op2, op3
{
;--- Byte 0 = Operation code ---
if op2 eq
DB 035h
else
DB 0B5h
end if
;--- Byte 1 = Operand addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
DB x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op2 eq
else
if op3 eq
DW op2
else
end if
end if
if op3 eq
else
if op2 < 0
x3=-op2
else
x3=op2
end if
if op3 < 0
x4=-op3
else
x4=op3
end if
if op2 = 0
x5=0
else
if op2>0 & op2<4 | op2<0 & op2>-4
x5=1
end if
if op2>3 & op2<16 | op2<-3 & op2>-16
x5=2
end if
if op2>15 & op2<64 | op2<-15 & op2>-64
x5=3
end if
if op2>63 & op2<256 | op2<-63 & op2>-256
x5=4
end if
if op2>255 & op2<1024 | op2<-255 & op2>-1024
x5=5
end if
if op2>1023 & op2<4096 | op2<-1023 & op2>-4096
x5=6
end if
end if
if op2<0
x6=1
else
x6=0
end if
x7=x5*2
DW x6 SHL 15 + x5 SHL 12 + x4 SHL x7 + x3
end if
}


;--- EBC Instruction: RET --------------------------------------------------;
;                                                                           ;
; Fetches the return address from stack, sets the IP to the value, adjust   ;
; the stack pointer register R0 and continues execution at the return       ;
; address. If the RET is a final return from the EBC driver (application)   ;
; then execution control returns to the caller, which may be EBC or native  ;
; code.                                                                     ;
;                                                                           ;
; IP <= [R0]                                                                ;
; R0 <= R0+16                                                               ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO RET
{
DW 00004h
}


;--- EBC Instruction: SHL32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Left-shifts Operand 1 by Operand 2 bit positions and stores the result    ;
; back to Operand 1. Operand size is 32 bits.                               ;
;                                                                           ;
; Operand 1 <= Operand 1 << Operand 2                                       ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO SHL32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 017h
else
DB 097h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: SHL64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Left-shifts Operand 1 by Operand 2 bit positions and stores the result    ;
; back to Operand 1. Operand size is 64 bits.                               ;
;                                                                           ;
; Operand 1 <= Operand 1 << Operand 2                                       ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO SHL64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 057h
else
DB 0D7h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: SHR32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Right-shifts Operand 1 by Operand 2 bit positions and stores the result   ;
; back to Operand 1. Operand size is 32 bits.                               ;
;                                                                           ;
; Operand 1 <= Operand 1 >> Operand 2                                       ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO SHR32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 018h
else
DB 098h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: SHR64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Right-shifts Operand 1 by Operand 2 bit positions and stores the result   ;
; back to Operand 1. Operand size is 64 bits.                               ;
;                                                                           ;
; Operand 1 <= Operand 1 >> Operand 2                                       ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO SHR64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 058h
else
DB 0D8h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: STORESP R1 , [IP|Flags] ------------------------------;
;                                                                           ;
; This instruction transfers the contents of a dedicated register to a      ;
; general-purpose register. The dedicated register is specified by the      ;
; the index: 0=Flags, 1=IP, 2-7=Reserved.                                   ;
;                                                                           ;
; Operand 1 <= Special Reg                                                  ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO STORESP op1, op2
{
;--- Byte 0 = Operation code ---
DB 02Ah
;--- Byte 1 = Two operands addressing ---
if op2 eq Flags
x1=0
else
x1=1
end if
if op1 in < R0 >
x2=0
end if
if op1 in < R1 >
x2=1
end if
if op1 in < R2 >
x2=2
end if
if op1 in < R3 >
x2=3
end if
if op1 in < R4 >
x2=4
end if
if op1 in < R5 > 
x2=5
end if
if op1 in < R6 >
x2=6
end if
if op1 in < R7 >
x2=7
end if
DB x1 SHL 4 + x2
}


;--- EBC Instruction: SUB32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Subtracts a 32-bit signed operand 2 value from signed Operand 1 value and ;
; stores the result to Operand 1.                                           ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 + Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO SUB32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 00Dh
else
DB 08Dh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: SUB64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Subtracts a 64-bit signed operand 2 value from signed Operand 1 value and ;
; stores the result to Operand 1.                                           ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a signed value at ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 + Operand 2                                        ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO SUB64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 04Dh
else
DB 0CDh
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: XOR32 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a bit-wise exclusive OR of two 32-bit operands and stores the    ;
; result to Operand 1.                                                      ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a value at        ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
; For 32-bit only: if Operand 1 is direct, then the result is stored back   ;
; to the Operand 1 register with the upper 32 bits cleared.                 ;
;                                                                           ;
; Operand 1 <= Operand 1 XOR Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO XOR32 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 016h
else
DB 096h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


;--- EBC Instruction: XOR64 {@}R1, {@}R2, {Index16|Immed16} ----------------;
;                                                                           ;
; Performs a bit-wise exclusive OR of two 64-bit operands and stores the    ;
; result to Operand 1.                                                      ;
; If Operand 2 is indirect, then the immediate data is interpreted as an    ;
; index and the Operand 2 value is fetched from memory as a value at        ;
; address [R2+Index16], index has Constant and Natural addends.             ;
; If Operand 2 is direct, then the immediate data is considered a signed    ;
; immediate value and is added to the R2 register contents such that        ;
; Operand 2 = R2 + Immed16.                                                 ;
;                                                                           ;
; Operand 1 <= Operand 1 XOR Operand 2                                      ;
;                                                                           ;
; op1 = Destination                                                         ;
; op2 = Source                                                              ;
; op3 = Natural offset, optional                                            ;
; op4 = Constant offset, optional                                           ;
;                                                                           ;
;---------------------------------------------------------------------------;

MACRO XOR64 op1, op2, op3, op4
{
;--- Byte 0 = Operation code ---
if op3 eq
DB 056h
else
DB 0D6h
end if
;--- Byte 1 = Two operands addressing ---
if op1 in < R0,@R0 >
x1=0
end if
if op1 in < R1,@R1 >
x1=1
end if
if op1 in < R2,@R2 >
x1=2
end if
if op1 in < R3,@R3 >
x1=3
end if
if op1 in < R4,@R4 >
x1=4
end if
if op1 in < R5,@R5 > 
x1=5
end if
if op1 in < R6,@R6 >
x1=6
end if
if op1 in < R7,@R7 >
x1=7
end if
if op1 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x2=0
else
x2=1
end if
if op2 in < R0,@R0 >
x3=0
end if
if op2 in < R1,@R1 >
x3=1
end if
if op2 in < R2,@R2 >
x3=2
end if
if op2 in < R3,@R3 >
x3=3
end if
if op2 in < R4,@R4 >
x3=4
end if
if op2 in < R5,@R5 >
x3=5
end if
if op2 in < R6,@R6 >
x3=6
end if
if op2 in < R7,@R7 >
x3=7
end if
if op2 in < R0,R1,R2,R3,R4,R5,R6,R7 >
x4=0
else
x4=1
end if
DB x4 SHL 7 + x3 SHL 4 + x2 SHL 3 + x1
;--- Conditional bytes 2,3 = 16-bit index or immediate ---
if op3 eq
else
if op4 eq
DW op3
else
end if
end if
if op4 eq
else
if op3 < 0
x5=-op3
else
x5=op3
end if
if op4 < 0
x6=-op4
else
x6=op4
end if
if op3 = 0
x7=0
else
if op3>0 & op3<4 | op3<0 & op3>-4
x7=1
end if
if op3>3 & op3<16 | op3<-3 & op3>-16
x7=2
end if
if op3>15 & op3<64 | op3<-15 & op3>-64
x7=3
end if
if op3>63 & op3<256 | op3<-63 & op3>-256
x7=4
end if
if op3>255 & op3<1024 | op3<-255 & op3>-1024
x7=5
end if
if op3>1023 & op3<4096 | op3<-1023 & op3>-4096
x7=6
end if
end if
if op3<0
x8=1
else
x8=0
end if
x9=x7*2
DW x8 SHL 15 + x7 SHL 12 + x6 SHL x9 + x5
end if
}


