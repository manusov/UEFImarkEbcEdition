;===========================================================================;
;                                                                           ;
;              TABLES AND DATA FOR SAVE REPORT AS TEXT FILE.                ;
;                                                                           ;
;===========================================================================;

;---------- Equations for data access --------------------------------------;

_Report_Title		= Report_Title - Global_Variables_Pool
_Report_Middle_2	= Report_Middle_2 - Global_Variables_Pool
_Report_Middle		= Report_Middle - Global_Variables_Pool
_Name_TXT		= Name_TXT - Global_Variables_Pool

;---------- Data -----------------------------------------------------------;

;--- Strings for text report ---

Report_Title		DB  'System information.',0Dh,0Ah,0
Report_Middle_2		DB  0Dh,0Ah
Report_Middle		DB  0Dh,0Ah,0

;--- File name pattern ---

Name_TXT:
DB  'text____.txt',0

