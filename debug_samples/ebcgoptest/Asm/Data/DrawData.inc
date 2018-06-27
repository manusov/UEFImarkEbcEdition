;===========================================================================;
;=                                                                         =;
;=              CONSTANTS FOR TEXT AND GRAPHICS DRAWINGS.                  =;
;=                                                                         =;
;===========================================================================;

;--- Font 8x16 ---
Font_8x16:
;--- 00h-7Fh ---
DB  000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 000
DB  000h,000h,07Eh,081h,0A5h,081h,081h,0A5h,099h,081h,081h,07Eh,000h,000h,000h,000h  ; 001
DB  000h,000h,07Eh,0FFh,0DBh,0FFh,0FFh,0DBh,0E7h,0FFh,0FFh,07Eh,000h,000h,000h,000h  ; 002
DB  000h,000h,000h,000h,06Ch,0FEh,0FEh,0FEh,0FEh,07Ch,038h,010h,000h,000h,000h,000h  ; 003
DB  000h,000h,000h,000h,010h,038h,07Ch,0FEh,07Ch,038h,010h,000h,000h,000h,000h,000h  ; 004
DB  000h,000h,000h,018h,03Ch,03Ch,0E7h,0E7h,0E7h,018h,018h,03Ch,000h,000h,000h,000h  ; 005
DB  000h,000h,000h,018h,03Ch,07Eh,0FFh,0FFh,07Eh,018h,018h,03Ch,000h,000h,000h,000h  ; 006
DB  000h,000h,000h,000h,000h,000h,018h,03Ch,03Ch,018h,000h,000h,000h,000h,000h,000h  ; 007
DB  0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0E7h,0C3h,0C3h,0E7h,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh  ; 008
DB  000h,000h,000h,000h,000h,03Ch,066h,042h,042h,066h,03Ch,000h,000h,000h,000h,000h  ; 009
DB  0FFh,0FFh,0FFh,0FFh,0FFh,0C3h,099h,0BDh,0BDh,099h,0C3h,0FFh,0FFh,0FFh,0FFh,0FFh  ; 00A
DB  000h,000h,01Eh,006h,00Eh,01Ah,078h,0CCh,0CCh,0CCh,0CCh,078h,000h,000h,000h,000h  ; 00B
DB  000h,000h,03Ch,066h,066h,066h,066h,03Ch,018h,07Eh,018h,018h,000h,000h,000h,000h  ; 00C
DB  000h,000h,03Fh,033h,03Fh,030h,030h,030h,030h,070h,0F0h,0E0h,000h,000h,000h,000h  ; 00D
DB  000h,000h,07Fh,063h,07Fh,063h,063h,063h,063h,067h,0E7h,0E6h,0C0h,000h,000h,000h  ; 00E
DB  000h,000h,000h,018h,018h,0DBh,03Ch,0E7h,03Ch,0DBh,018h,018h,000h,000h,000h,000h  ; 00F
DB  000h,080h,0C0h,0E0h,0F0h,0F8h,0FEh,0F8h,0F0h,0E0h,0C0h,080h,000h,000h,000h,000h  ; 010
DB  000h,002h,006h,00Eh,01Eh,03Eh,0FEh,03Eh,01Eh,00Eh,006h,002h,000h,000h,000h,000h  ; 011
DB  000h,000h,018h,03Ch,07Eh,018h,018h,018h,07Eh,03Ch,018h,000h,000h,000h,000h,000h  ; 012
DB  000h,000h,066h,066h,066h,066h,066h,066h,066h,000h,066h,066h,000h,000h,000h,000h  ; 013
DB  000h,000h,07Fh,0DBh,0DBh,0DBh,07Bh,01Bh,01Bh,01Bh,01Bh,01Bh,000h,000h,000h,000h  ; 014
DB  000h,07Ch,0C6h,060h,038h,06Ch,0C6h,0C6h,06Ch,038h,00Ch,0C6h,07Ch,000h,000h,000h  ; 015
DB  000h,000h,000h,000h,000h,000h,000h,000h,0FEh,0FEh,0FEh,0FEh,000h,000h,000h,000h  ; 016
DB  000h,000h,018h,03Ch,07Eh,018h,018h,018h,07Eh,03Ch,018h,07Eh,000h,000h,000h,000h  ; 017
DB  000h,000h,018h,03Ch,07Eh,018h,018h,018h,018h,018h,018h,018h,000h,000h,000h,000h  ; 018
DB  000h,000h,018h,018h,018h,018h,018h,018h,018h,07Eh,03Ch,018h,000h,000h,000h,000h  ; 019
DB  000h,000h,000h,000h,000h,018h,00Ch,0FEh,00Ch,018h,000h,000h,000h,000h,000h,000h  ; 01A
DB  000h,000h,000h,000h,000h,030h,060h,0FEh,060h,030h,000h,000h,000h,000h,000h,000h  ; 01B
DB  000h,000h,000h,000h,000h,000h,0C0h,0C0h,0C0h,0FEh,000h,000h,000h,000h,000h,000h  ; 01C
DB  000h,000h,000h,000h,000h,028h,06Ch,0FEh,06Ch,028h,000h,000h,000h,000h,000h,000h  ; 01D
DB  000h,000h,000h,000h,010h,038h,038h,07Ch,07Ch,0FEh,0FEh,000h,000h,000h,000h,000h  ; 01E
DB  000h,000h,000h,000h,0FEh,0FEh,07Ch,07Ch,038h,038h,010h,000h,000h,000h,000h,000h  ; 01F
DB  000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 020
DB  000h,000h,018h,03Ch,03Ch,03Ch,018h,018h,018h,000h,018h,018h,000h,000h,000h,000h  ; 021
DB  000h,066h,066h,066h,024h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 022
DB  000h,000h,000h,06Ch,06Ch,0FEh,06Ch,06Ch,06Ch,0FEh,06Ch,06Ch,000h,000h,000h,000h  ; 023
DB  018h,018h,07Ch,0C6h,0C2h,0C0h,07Ch,006h,006h,086h,0C6h,07Ch,018h,018h,000h,000h  ; 024
DB  000h,000h,000h,000h,0C2h,0C6h,00Ch,018h,030h,060h,0C6h,086h,000h,000h,000h,000h  ; 025
DB  000h,000h,038h,06Ch,06Ch,038h,076h,0DCh,0CCh,0CCh,0CCh,076h,000h,000h,000h,000h  ; 026
DB  000h,030h,030h,030h,060h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 027
DB  000h,000h,00Ch,018h,030h,030h,030h,030h,030h,030h,018h,00Ch,000h,000h,000h,000h  ; 028
DB  000h,000h,030h,018h,00Ch,00Ch,00Ch,00Ch,00Ch,00Ch,018h,030h,000h,000h,000h,000h  ; 029
DB  000h,000h,000h,000h,000h,066h,03Ch,0FFh,03Ch,066h,000h,000h,000h,000h,000h,000h  ; 02A
DB  000h,000h,000h,000h,000h,018h,018h,07Eh,018h,018h,000h,000h,000h,000h,000h,000h  ; 02B
DB  000h,000h,000h,000h,000h,000h,000h,000h,000h,018h,018h,018h,030h,000h,000h,000h  ; 02C
DB  000h,000h,000h,000h,000h,000h,000h,0FEh,000h,000h,000h,000h,000h,000h,000h,000h  ; 02D
DB  000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,018h,018h,000h,000h,000h,000h  ; 02E
DB  000h,000h,000h,000h,002h,006h,00Ch,018h,030h,060h,0C0h,080h,000h,000h,000h,000h  ; 02F
DB  000h,000h,038h,06Ch,0C6h,0C6h,0D6h,0D6h,0C6h,0C6h,06Ch,038h,000h,000h,000h,000h  ; 030
DB  000h,000h,018h,038h,078h,018h,018h,018h,018h,018h,018h,07Eh,000h,000h,000h,000h  ; 031
DB  000h,000h,07Ch,0C6h,006h,00Ch,018h,030h,060h,0C0h,0C6h,0FEh,000h,000h,000h,000h  ; 032
DB  000h,000h,07Ch,0C6h,006h,006h,03Ch,006h,006h,006h,0C6h,07Ch,000h,000h,000h,000h  ; 033
DB  000h,000h,00Ch,01Ch,03Ch,06Ch,0CCh,0FEh,00Ch,00Ch,00Ch,01Eh,000h,000h,000h,000h  ; 034
DB  000h,000h,0FEh,0C0h,0C0h,0C0h,0FCh,006h,006h,006h,0C6h,07Ch,000h,000h,000h,000h  ; 035
DB  000h,000h,038h,060h,0C0h,0C0h,0FCh,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 036
DB  000h,000h,0FEh,0C6h,006h,006h,00Ch,018h,030h,030h,030h,030h,000h,000h,000h,000h  ; 037
DB  000h,000h,07Ch,0C6h,0C6h,0C6h,07Ch,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 038
DB  000h,000h,07Ch,0C6h,0C6h,0C6h,07Eh,006h,006h,006h,00Ch,078h,000h,000h,000h,000h  ; 039
DB  000h,000h,000h,000h,018h,018h,000h,000h,000h,018h,018h,000h,000h,000h,000h,000h  ; 03A
DB  000h,000h,000h,000h,018h,018h,000h,000h,000h,018h,018h,030h,000h,000h,000h,000h  ; 03B
DB  000h,000h,000h,006h,00Ch,018h,030h,060h,030h,018h,00Ch,006h,000h,000h,000h,000h  ; 03C
DB  000h,000h,000h,000h,000h,07Eh,000h,000h,07Eh,000h,000h,000h,000h,000h,000h,000h  ; 03D
DB  000h,000h,000h,060h,030h,018h,00Ch,006h,00Ch,018h,030h,060h,000h,000h,000h,000h  ; 03E
DB  000h,000h,07Ch,0C6h,0C6h,00Ch,018h,018h,018h,000h,018h,018h,000h,000h,000h,000h  ; 03F
DB  000h,000h,000h,07Ch,0C6h,0C6h,0DEh,0DEh,0DEh,0DCh,0C0h,07Ch,000h,000h,000h,000h  ; 040
DB  000h,000h,010h,038h,06Ch,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 041
DB  000h,000h,0FCh,066h,066h,066h,07Ch,066h,066h,066h,066h,0FCh,000h,000h,000h,000h  ; 042
DB  000h,000h,03Ch,066h,0C2h,0C0h,0C0h,0C0h,0C0h,0C2h,066h,03Ch,000h,000h,000h,000h  ; 043
DB  000h,000h,0F8h,06Ch,066h,066h,066h,066h,066h,066h,06Ch,0F8h,000h,000h,000h,000h  ; 044
DB  000h,000h,0FEh,066h,062h,068h,078h,068h,060h,062h,066h,0FEh,000h,000h,000h,000h  ; 045
DB  000h,000h,0FEh,066h,062h,068h,078h,068h,060h,060h,060h,0F0h,000h,000h,000h,000h  ; 046
DB  000h,000h,03Ch,066h,0C2h,0C0h,0C0h,0DEh,0C6h,0C6h,066h,03Ah,000h,000h,000h,000h  ; 047
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 048
DB  000h,000h,03Ch,018h,018h,018h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 049
DB  000h,000h,01Eh,00Ch,00Ch,00Ch,00Ch,00Ch,0CCh,0CCh,0CCh,078h,000h,000h,000h,000h  ; 04A
DB  000h,000h,0E6h,066h,066h,06Ch,078h,078h,06Ch,066h,066h,0E6h,000h,000h,000h,000h  ; 04B
DB  000h,000h,0F0h,060h,060h,060h,060h,060h,060h,062h,066h,0FEh,000h,000h,000h,000h  ; 04C
DB  000h,000h,0C6h,0EEh,0FEh,0FEh,0D6h,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 04D
DB  000h,000h,0C6h,0E6h,0F6h,0FEh,0DEh,0CEh,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 04E
DB  000h,000h,07Ch,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 04F
DB  000h,000h,0FCh,066h,066h,066h,07Ch,060h,060h,060h,060h,0F0h,000h,000h,000h,000h  ; 050
DB  000h,000h,07Ch,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0D6h,0DEh,07Ch,00Ch,00Eh,000h,000h  ; 051
DB  000h,000h,0FCh,066h,066h,066h,07Ch,06Ch,066h,066h,066h,0E6h,000h,000h,000h,000h  ; 052
DB  000h,000h,07Ch,0C6h,0C6h,060h,038h,00Ch,006h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 053
DB  000h,000h,07Eh,07Eh,05Ah,018h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 054
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 055
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,06Ch,038h,010h,000h,000h,000h,000h  ; 056
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0D6h,0D6h,0D6h,0FEh,0EEh,06Ch,000h,000h,000h,000h  ; 057
DB  000h,000h,0C6h,0C6h,06Ch,07Ch,038h,038h,07Ch,06Ch,0C6h,0C6h,000h,000h,000h,000h  ; 058
DB  000h,000h,066h,066h,066h,066h,03Ch,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 059
DB  000h,000h,0FEh,0C6h,086h,00Ch,018h,030h,060h,0C2h,0C6h,0FEh,000h,000h,000h,000h  ; 05A
DB  000h,000h,03Ch,030h,030h,030h,030h,030h,030h,030h,030h,03Ch,000h,000h,000h,000h  ; 05B
DB  000h,000h,000h,080h,0C0h,0E0h,070h,038h,01Ch,00Eh,006h,002h,000h,000h,000h,000h  ; 05C
DB  000h,000h,03Ch,00Ch,00Ch,00Ch,00Ch,00Ch,00Ch,00Ch,00Ch,03Ch,000h,000h,000h,000h  ; 05D
DB  010h,038h,06Ch,0C6h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 05E
DB  000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,0FFh,000h,000h  ; 05F
DB  030h,030h,018h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 060
DB  000h,000h,000h,000h,000h,078h,00Ch,07Ch,0CCh,0CCh,0CCh,076h,000h,000h,000h,000h  ; 061
DB  000h,000h,0E0h,060h,060h,078h,06Ch,066h,066h,066h,066h,07Ch,000h,000h,000h,000h  ; 062
DB  000h,000h,000h,000h,000h,07Ch,0C6h,0C0h,0C0h,0C0h,0C6h,07Ch,000h,000h,000h,000h  ; 063
DB  000h,000h,01Ch,00Ch,00Ch,03Ch,06Ch,0CCh,0CCh,0CCh,0CCh,076h,000h,000h,000h,000h  ; 064
DB  000h,000h,000h,000h,000h,07Ch,0C6h,0FEh,0C0h,0C0h,0C6h,07Ch,000h,000h,000h,000h  ; 065
DB  000h,000h,038h,06Ch,064h,060h,0F0h,060h,060h,060h,060h,0F0h,000h,000h,000h,000h  ; 066
DB  000h,000h,000h,000h,000h,076h,0CCh,0CCh,0CCh,0CCh,0CCh,07Ch,00Ch,0CCh,078h,000h  ; 067
DB  000h,000h,0E0h,060h,060h,06Ch,076h,066h,066h,066h,066h,0E6h,000h,000h,000h,000h  ; 068
DB  000h,000h,018h,018h,000h,038h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 069
DB  000h,000h,006h,006h,000h,00Eh,006h,006h,006h,006h,006h,006h,066h,066h,03Ch,000h  ; 06A
DB  000h,000h,0E0h,060h,060h,066h,06Ch,078h,078h,06Ch,066h,0E6h,000h,000h,000h,000h  ; 06B
DB  000h,000h,038h,018h,018h,018h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 06C
DB  000h,000h,000h,000h,000h,0ECh,0FEh,0D6h,0D6h,0D6h,0D6h,0C6h,000h,000h,000h,000h  ; 06D
DB  000h,000h,000h,000h,000h,0DCh,066h,066h,066h,066h,066h,066h,000h,000h,000h,000h  ; 06E
DB  000h,000h,000h,000h,000h,07Ch,0C6h,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 06F
DB  000h,000h,000h,000h,000h,0DCh,066h,066h,066h,066h,066h,07Ch,060h,060h,0F0h,000h  ; 070
DB  000h,000h,000h,000h,000h,076h,0CCh,0CCh,0CCh,0CCh,0CCh,07Ch,00Ch,00Ch,01Eh,000h  ; 071
DB  000h,000h,000h,000h,000h,0DCh,076h,066h,060h,060h,060h,0F0h,000h,000h,000h,000h  ; 072
DB  000h,000h,000h,000h,000h,07Ch,0C6h,060h,038h,00Ch,0C6h,07Ch,000h,000h,000h,000h  ; 073
DB  000h,000h,010h,030h,030h,0FCh,030h,030h,030h,030h,036h,01Ch,000h,000h,000h,000h  ; 074
DB  000h,000h,000h,000h,000h,0CCh,0CCh,0CCh,0CCh,0CCh,0CCh,076h,000h,000h,000h,000h  ; 075
DB  000h,000h,000h,000h,000h,066h,066h,066h,066h,066h,03Ch,018h,000h,000h,000h,000h  ; 076
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0D6h,0D6h,0D6h,0FEh,06Ch,000h,000h,000h,000h  ; 077
DB  000h,000h,000h,000h,000h,0C6h,06Ch,038h,038h,038h,06Ch,0C6h,000h,000h,000h,000h  ; 078
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,07Eh,006h,00Ch,0F8h,000h  ; 079
DB  000h,000h,000h,000h,000h,0FEh,0CCh,018h,030h,060h,0C6h,0FEh,000h,000h,000h,000h  ; 07A
DB  000h,000h,00Eh,018h,018h,018h,070h,018h,018h,018h,018h,00Eh,000h,000h,000h,000h  ; 07B
DB  000h,000h,018h,018h,018h,018h,000h,018h,018h,018h,018h,018h,000h,000h,000h,000h  ; 07C
DB  000h,000h,070h,018h,018h,018h,00Eh,018h,018h,018h,018h,070h,000h,000h,000h,000h  ; 07D
DB  000h,000h,076h,0DCh,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 07E
DB  000h,000h,000h,000h,010h,038h,06Ch,0C6h,0C6h,0C6h,0FEh,000h,000h,000h,000h,000h  ; 07F
;--- 80h-FFh ---
DB  000h,000h,01Eh,036h,066h,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 080
DB  000h,000h,0FEh,062h,062h,060h,07Ch,066h,066h,066h,066h,0FCh,000h,000h,000h,000h  ; 081
DB  000h,000h,0FCh,066h,066h,066h,07Ch,066h,066h,066h,066h,0FCh,000h,000h,000h,000h  ; 082
DB  000h,000h,0FEh,062h,062h,060h,060h,060h,060h,060h,060h,0F0h,000h,000h,000h,000h  ; 083
DB  000h,000h,01Eh,036h,066h,066h,066h,066h,066h,066h,066h,0FFh,0C3h,081h,000h,000h  ; 084
DB  000h,000h,0FEh,066h,062h,068h,078h,068h,060h,062h,066h,0FEh,000h,000h,000h,000h  ; 085
DB  000h,000h,0D6h,0D6h,054h,054h,07Ch,07Ch,054h,0D6h,0D6h,0D6h,000h,000h,000h,000h  ; 086
DB  000h,000h,07Ch,0C6h,006h,006h,03Ch,006h,006h,006h,0C6h,07Ch,000h,000h,000h,000h  ; 087
DB  000h,000h,0C6h,0C6h,0CEh,0CEh,0D6h,0E6h,0E6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 088
DB  038h,038h,0C6h,0C6h,0CEh,0CEh,0D6h,0E6h,0E6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 089
DB  000h,000h,0E6h,066h,06Ch,06Ch,078h,078h,06Ch,06Ch,066h,0E6h,000h,000h,000h,000h  ; 08A
DB  000h,000h,01Eh,036h,066h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 08B
DB  000h,000h,0C6h,0EEh,0FEh,0FEh,0D6h,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 08C
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 08D
DB  000h,000h,07Ch,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 08E
DB  000h,000h,0FEh,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 08F
DB  000h,000h,0FCh,066h,066h,066h,07Ch,060h,060h,060h,060h,0F0h,000h,000h,000h,000h  ; 090
DB  000h,000h,03Ch,066h,0C2h,0C0h,0C0h,0C0h,0C0h,0C2h,066h,03Ch,000h,000h,000h,000h  ; 091
DB  000h,000h,07Eh,05Ah,018h,018h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 092
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,07Eh,006h,006h,0C6h,07Ch,000h,000h,000h,000h  ; 093
DB  000h,03Ch,018h,07Eh,0DBh,0DBh,0DBh,0DBh,0DBh,07Eh,018h,03Ch,000h,000h,000h,000h  ; 094
DB  000h,000h,0C6h,0C6h,06Ch,07Ch,038h,038h,07Ch,06Ch,0C6h,0C6h,000h,000h,000h,000h  ; 095
DB  000h,000h,0CCh,0CCh,0CCh,0CCh,0CCh,0CCh,0CCh,0CCh,0CCh,0FEh,006h,006h,000h,000h  ; 096
DB  000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,07Eh,006h,006h,006h,006h,000h,000h,000h,000h  ; 097
DB  000h,000h,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0FFh,000h,000h,000h,000h  ; 098
DB  000h,000h,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0DBh,0FFh,003h,003h,000h,000h  ; 099
DB  000h,000h,0F8h,0B0h,030h,030h,03Eh,033h,033h,033h,033h,07Eh,000h,000h,000h,000h  ; 09A
DB  000h,000h,0C3h,0C3h,0C3h,0C3h,0F3h,0DBh,0DBh,0DBh,0DBh,0F3h,000h,000h,000h,000h  ; 09B
DB  000h,000h,0F0h,060h,060h,060h,07Ch,066h,066h,066h,066h,0FCh,000h,000h,000h,000h  ; 09C
DB  000h,000h,07Ch,0C6h,006h,026h,03Eh,026h,006h,006h,0C6h,07Ch,000h,000h,000h,000h  ; 09D
DB  000h,000h,0CEh,0DBh,0DBh,0DBh,0FBh,0DBh,0DBh,0DBh,0DBh,0CEh,000h,000h,000h,000h  ; 09E
DB  000h,000h,03Fh,066h,066h,066h,03Eh,03Eh,066h,066h,066h,0E7h,000h,000h,000h,000h  ; 09F
DB  000h,000h,000h,000h,000h,078h,00Ch,07Ch,0CCh,0CCh,0CCh,076h,000h,000h,000h,000h  ; 0A0
DB  000h,002h,006h,03Ch,060h,060h,07Ch,066h,066h,066h,066h,03Ch,000h,000h,000h,000h  ; 0A1
DB  000h,000h,000h,000h,000h,0FCh,066h,066h,07Ch,066h,066h,0FCh,000h,000h,000h,000h  ; 0A2
DB  000h,000h,000h,000h,000h,07Eh,032h,032h,030h,030h,030h,078h,000h,000h,000h,000h  ; 0A3
DB  000h,000h,000h,000h,000h,01Eh,036h,036h,066h,066h,066h,0FFh,0C3h,0C3h,000h,000h  ; 0A4
DB  000h,000h,000h,000h,000h,07Ch,0C6h,0FEh,0C0h,0C0h,0C6h,07Ch,000h,000h,000h,000h  ; 0A5
DB  000h,000h,000h,000h,000h,0D6h,0D6h,054h,07Ch,054h,0D6h,0D6h,000h,000h,000h,000h  ; 0A6
DB  000h,000h,000h,000h,000h,03Ch,066h,006h,00Ch,006h,066h,03Ch,000h,000h,000h,000h  ; 0A7
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0CEh,0D6h,0E6h,0C6h,0C6h,000h,000h,000h,000h  ; 0A8
DB  000h,000h,000h,038h,038h,0C6h,0C6h,0CEh,0D6h,0E6h,0C6h,0C6h,000h,000h,000h,000h  ; 0A9
DB  000h,000h,000h,000h,000h,0E6h,06Ch,078h,078h,06Ch,066h,0E6h,000h,000h,000h,000h  ; 0AA
DB  000h,000h,000h,000h,000h,01Eh,036h,066h,066h,066h,066h,066h,000h,000h,000h,000h  ; 0AB
DB  000h,000h,000h,000h,000h,0C6h,0EEh,0FEh,0FEh,0D6h,0D6h,0C6h,000h,000h,000h,000h  ; 0AC
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 0AD
DB  000h,000h,000h,000h,000h,07Ch,0C6h,0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h  ; 0AE
DB  000h,000h,000h,000h,000h,0FEh,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,000h,000h  ; 0AF
DB  011h,044h,011h,044h,011h,044h,011h,044h,011h,044h,011h,044h,011h,044h,011h,044h  ; 0B0
DB  055h,0AAh,055h,0AAh,055h,0AAh,055h,0AAh,055h,0AAh,055h,0AAh,055h,0AAh,055h,0AAh  ; 0B1
DB  0DDh,077h,0DDh,077h,0DDh,077h,0DDh,077h,0DDh,077h,0DDh,077h,0DDh,077h,0DDh,077h  ; 0B2
DB  018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h  ; 0B3
DB  018h,018h,018h,018h,018h,018h,018h,0F8h,018h,018h,018h,018h,018h,018h,018h,018h  ; 0B4
DB  018h,018h,018h,018h,018h,0F8h,018h,0F8h,018h,018h,018h,018h,018h,018h,018h,018h  ; 0B5
DB  036h,036h,036h,036h,036h,036h,036h,0F6h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0B6
DB  000h,000h,000h,000h,000h,000h,000h,0FEh,036h,036h,036h,036h,036h,036h,036h,036h  ; 0B7
DB  000h,000h,000h,000h,000h,0F8h,018h,0F8h,018h,018h,018h,018h,018h,018h,018h,018h  ; 0B8
DB  036h,036h,036h,036h,036h,0F6h,006h,0F6h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0B9
DB  036h,036h,036h,036h,036h,036h,036h,036h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0BA
DB  000h,000h,000h,000h,000h,0FEh,006h,0F6h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0BB
DB  036h,036h,036h,036h,036h,0F6h,006h,0FEh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0BC
DB  036h,036h,036h,036h,036h,036h,036h,0FEh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0BD
DB  018h,018h,018h,018h,018h,0F8h,018h,0F8h,000h,000h,000h,000h,000h,000h,000h,000h  ; 0BE
DB  000h,000h,000h,000h,000h,000h,000h,0F8h,018h,018h,018h,018h,018h,018h,018h,018h  ; 0BF
DB  018h,018h,018h,018h,018h,018h,018h,01Fh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0C0
DB  018h,018h,018h,018h,018h,018h,018h,0FFh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0C1
DB  000h,000h,000h,000h,000h,000h,000h,0FFh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0C2
DB  018h,018h,018h,018h,018h,018h,018h,01Fh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0C3
DB  000h,000h,000h,000h,000h,000h,000h,0FFh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0C4
DB  018h,018h,018h,018h,018h,018h,018h,0FFh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0C5
DB  018h,018h,018h,018h,018h,01Fh,018h,01Fh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0C6
DB  036h,036h,036h,036h,036h,036h,036h,037h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0C7
DB  036h,036h,036h,036h,036h,037h,030h,03Fh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0C8
DB  000h,000h,000h,000h,000h,03Fh,030h,037h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0C9
DB  036h,036h,036h,036h,036h,0F7h,000h,0FFh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0CA
DB  000h,000h,000h,000h,000h,0FFh,000h,0F7h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0CB
DB  036h,036h,036h,036h,036h,037h,030h,037h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0CC
DB  000h,000h,000h,000h,000h,0FFh,000h,0FFh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0CD
DB  036h,036h,036h,036h,036h,0F7h,000h,0F7h,036h,036h,036h,036h,036h,036h,036h,036h  ; 0CE
DB  018h,018h,018h,018h,018h,0FFh,000h,0FFh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0CF
DB  036h,036h,036h,036h,036h,036h,036h,0FFh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0D0
DB  000h,000h,000h,000h,000h,0FFh,000h,0FFh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0D1
DB  000h,000h,000h,000h,000h,000h,000h,0FFh,036h,036h,036h,036h,036h,036h,036h,036h  ; 0D2
DB  036h,036h,036h,036h,036h,036h,036h,03Fh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0D3
DB  018h,018h,018h,018h,018h,01Fh,018h,01Fh,000h,000h,000h,000h,000h,000h,000h,000h  ; 0D4
DB  000h,000h,000h,000h,000h,01Fh,018h,01Fh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0D5
DB  000h,000h,000h,000h,000h,000h,000h,03Fh,036h,036h,036h,036h,036h,036h,036h,036h  ; 0D6
DB  036h,036h,036h,036h,036h,036h,036h,0FFh,036h,036h,036h,036h,036h,036h,036h,036h  ; 0D7
DB  018h,018h,018h,018h,018h,0FFh,018h,0FFh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0D8
DB  018h,018h,018h,018h,018h,018h,018h,0F8h,000h,000h,000h,000h,000h,000h,000h,000h  ; 0D9
DB  000h,000h,000h,000h,000h,000h,000h,01Fh,018h,018h,018h,018h,018h,018h,018h,018h  ; 0DA
DB  0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh  ; 0DB
DB  000h,000h,000h,000h,000h,000h,000h,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh  ; 0DC
DB  0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h,0F0h  ; 0DD
DB  00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh,00Fh  ; 0DE
DB  0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 0DF
DB  000h,000h,000h,000h,000h,0DCh,066h,066h,066h,066h,066h,07Ch,060h,060h,0F0h,000h  ; 0E0
DB  000h,000h,000h,000h,000h,07Ch,0C6h,0C0h,0C0h,0C0h,0C6h,07Ch,000h,000h,000h,000h  ; 0E1
DB  000h,000h,000h,000h,000h,07Eh,05Ah,018h,018h,018h,018h,03Ch,000h,000h,000h,000h  ; 0E2
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,07Eh,006h,006h,0C6h,07Ch,000h  ; 0E3
DB  000h,000h,000h,000h,03Ch,018h,07Eh,0DBh,0DBh,0DBh,0DBh,07Eh,018h,018h,03Ch,000h  ; 0E4
DB  000h,000h,000h,000h,000h,0C6h,06Ch,038h,038h,038h,06Ch,0C6h,000h,000h,000h,000h  ; 0E5
DB  000h,000h,000h,000h,000h,0CCh,0CCh,0CCh,0CCh,0CCh,0CCh,0FEh,006h,006h,000h,000h  ; 0E6
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0C6h,0C6h,07Eh,006h,006h,000h,000h,000h,000h  ; 0E7
DB  000h,000h,000h,000h,000h,0D6h,0D6h,0D6h,0D6h,0D6h,0D6h,0FEh,000h,000h,000h,000h  ; 0E8
DB  000h,000h,000h,000h,000h,0D6h,0D6h,0D6h,0D6h,0D6h,0D6h,0FEh,003h,003h,000h,000h  ; 0E9
DB  000h,000h,000h,000h,000h,0F8h,0B0h,030h,03Eh,033h,033h,07Eh,000h,000h,000h,000h  ; 0EA
DB  000h,000h,000h,000h,000h,0C6h,0C6h,0C6h,0F6h,0DEh,0DEh,0F6h,000h,000h,000h,000h  ; 0EB
DB  000h,000h,000h,000h,000h,0F0h,060h,060h,07Ch,066h,066h,0FCh,000h,000h,000h,000h  ; 0EC
DB  000h,000h,000h,000h,000h,03Ch,066h,006h,01Eh,006h,066h,03Ch,000h,000h,000h,000h  ; 0ED
DB  000h,000h,000h,000h,000h,0CEh,0DBh,0DBh,0FBh,0DBh,0DBh,0CEh,000h,000h,000h,000h  ; 0EE
DB  000h,000h,000h,000h,000h,07Eh,0CCh,0CCh,0FCh,06Ch,0CCh,0CEh,000h,000h,000h,000h  ; 0EF
DB  000h,06Ch,000h,0FEh,062h,060h,068h,078h,068h,060h,062h,0FEh,000h,000h,000h,000h  ; 0F0
DB  000h,000h,000h,06Ch,000h,07Ch,0C6h,0FEh,0C0h,0C0h,0C6h,07Ch,000h,000h,000h,000h  ; 0F1
DB  000h,000h,000h,030h,018h,00Ch,006h,00Ch,018h,030h,000h,07Eh,000h,000h,000h,000h  ; 0F2
DB  000h,000h,000h,00Ch,018h,030h,060h,030h,018h,00Ch,000h,07Eh,000h,000h,000h,000h  ; 0F3
DB  000h,000h,00Eh,01Bh,01Bh,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h,018h  ; 0F4
DB  018h,018h,018h,018h,018h,018h,018h,018h,0D8h,0D8h,0D8h,070h,000h,000h,000h,000h  ; 0F5
DB  000h,000h,000h,000h,018h,018h,000h,07Eh,000h,018h,018h,000h,000h,000h,000h,000h  ; 0F6
DB  000h,000h,000h,000h,000h,076h,0DCh,000h,076h,0DCh,000h,000h,000h,000h,000h,000h  ; 0F7
DB  000h,038h,06Ch,06Ch,038h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 0F8
DB  000h,000h,000h,000h,000h,000h,000h,018h,018h,000h,000h,000h,000h,000h,000h,000h  ; 0F9
DB  000h,000h,000h,000h,000h,000h,000h,000h,018h,000h,000h,000h,000h,000h,000h,000h  ; 0FA
DB  000h,000h,000h,001h,003h,002h,006h,004h,0CCh,068h,038h,010h,000h,000h,000h,000h  ; 0FB
DB  000h,000h,000h,03Ch,042h,099h,0A5h,0A1h,0A5h,099h,042h,03Ch,000h,000h,000h,000h  ; 0FC
DB  000h,070h,0D8h,030h,060h,0C8h,0F8h,000h,000h,000h,000h,000h,000h,000h,000h,000h  ; 0FD
DB  000h,000h,000h,000h,07Ch,07Ch,07Ch,07Ch,07Ch,07Ch,07Ch,000h,000h,000h,000h,000h  ; 0FE
DB  000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,042h,07Eh,000h,000h,000h,000h  ; 0FF