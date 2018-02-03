;===========================================================================;
;                                                                           ;
;       UEFI API RELEVANT MESSAGES STRINGS FOR ERRORS AND WARNINGS.         ;
;                                                                           ;
;===========================================================================;

;---------- Equations for data access --------------------------------------;
; For compact coding, use 16-bit offsets relative Global_Variables_Pool
; instead full 64-bit flat addresses. This method also eliminate relocation
; elements with some bugs in the old IA32 EFI implementations.

_EFI_Errors_Names	= EFI_Errors_Names - Global_Variables_Pool
_EFI_Warnings_Names	= EFI_Warnings_Names - Global_Variables_Pool
_Operations_Names	= Operations_Names - Global_Variables_Pool
_Save_File_Status	= Save_File_Status - Global_Variables_Pool
_Str_Operation		= Str_Operation - Global_Variables_Pool
_Str_Status		= Str_Status - Global_Variables_Pool

;---------- Equations for errors and warnings codes ------------------------;

;--- Success status ---

EFI_SUCCESS			= 0

;--- EFI error codes, status high bit=1 ---

NUMBER_OF_EFI_ERRORS		= 32
EFIERR				= 8000000000000000h	; This value for 64-bit interpreting
EFI_LOAD_ERROR			= EFIERR or 1
EFI_INVALID_PARAMETER		= EFIERR or 2
EFI_UNSUPPORTED 		= EFIERR or 3
EFI_BAD_BUFFER_SIZE		= EFIERR or 4
EFI_BUFFER_TOO_SMALL		= EFIERR or 5
EFI_NOT_READY			= EFIERR or 6
EFI_DEVICE_ERROR		= EFIERR or 7
EFI_WRITE_PROTECTED		= EFIERR or 8
EFI_OUT_OF_RESOURCES		= EFIERR or 9
EFI_VOLUME_CORRUPTED		= EFIERR or 10
EFI_VOLUME_FULL 		= EFIERR or 11
EFI_NO_MEDIA			= EFIERR or 12
EFI_MEDIA_CHANGED		= EFIERR or 13
EFI_NOT_FOUND			= EFIERR or 14
EFI_ACCESS_DENIED		= EFIERR or 15
EFI_NO_RESPONSE 		= EFIERR or 16
EFI_NO_MAPPING			= EFIERR or 17
EFI_TIMEOUT			= EFIERR or 18
EFI_NOT_STARTED 		= EFIERR or 19
EFI_ALREADY_STARTED		= EFIERR or 20
EFI_ABORTED			= EFIERR or 21
EFI_ICMP_ERROR			= EFIERR or 22
EFI_TFTP_ERROR			= EFIERR or 23
EFI_PROTOCOL_ERROR		= EFIERR or 24
EFI_INCOMPATIBLE_VERSION	= EFIERR or 25
EFI_SECURITY_VIOLATION		= EFIERR or 26
EFI_CRC_ERROR			= EFIERR or 27
EFI_END_OF_MEDIA		= EFIERR or 28
EFI_END_OF_FILE			= EFIERR or 31
EFI_INVALID_LANGUAGE		= EFIERR or 32

;--- EFI warning codes, status high bit=0 ---

NUMBER_OF_EFI_WARNINGS		= 4
EFI_WARN_UNKNOWN_GLYPH		= 1
EFI_WARN_DELETE_FAILURE		= 2
EFI_WARN_WRITE_FAILURE		= 3
EFI_WARN_BUFFER_TOO_SMALL	= 4

;---------- Strings --------------------------------------------------------;

;--- EFI error codes, status high bit=1 ---

EFI_Errors_Names:
DB  '?'				, 0	; 0=Invalid error code
DB  'EFI_LOAD_ERROR'		, 0     ; 1 
DB  'EFI_INVALID_PARAMETER'	, 0     ; 2 
DB  'EFI_UNSUPPORTED'		, 0     ; 3 
DB  'EFI_BAD_BUFFER_SIZE'	, 0     ; 4 
DB  'EFI_BUFFER_TOO_SMALL'	, 0     ; 5 
DB  'EFI_NOT_READY'		, 0     ; 6 
DB  'EFI_DEVICE_ERROR'		, 0     ; 7 
DB  'EFI_WRITE_PROTECTED'	, 0     ; 8 
DB  'EFI_OUT_OF_RESOURCES'	, 0     ; 9 
DB  'EFI_VOLUME_CORRUPTED'	, 0     ; 10
DB  'EFI_VOLUME_FULL'		, 0     ; 11
DB  'EFI_NO_MEDIA'		, 0     ; 12
DB  'EFI_MEDIA_CHANGED'		, 0     ; 13
DB  'EFI_NOT_FOUND'		, 0     ; 14
DB  'EFI_ACCESS_DENIED'		, 0     ; 15
DB  'EFI_NO_RESPONSE'		, 0     ; 16
DB  'EFI_NO_MAPPING'		, 0     ; 17
DB  'EFI_TIMEOUT'		, 0     ; 18
DB  'EFI_NOT_STARTED'		, 0     ; 19
DB  'EFI_ALREADY_STARTED'	, 0     ; 20
DB  'EFI_ABORTED'		, 0     ; 21
DB  'EFI_ICMP_ERROR'		, 0     ; 22
DB  'EFI_TFTP_ERROR'		, 0     ; 23
DB  'EFI_PROTOCOL_ERROR'	, 0     ; 24
DB  'EFI_INCOMPATIBLE_VERSION'	, 0     ; 25
DB  'EFI_SECURITY_VIOLATION'	, 0     ; 26
DB  'EFI_CRC_ERROR'		, 0     ; 27
DB  'EFI_END_OF_MEDIA'		, 0     ; 28
DB  '?'				, 0	; 29
DB  '?'				, 0	; 30
DB  'EFI_END_OF_FILE'		, 0     ; 31
DB  'EFI_INVALID_LANGUAGE'	, 0     ; 32

;--- EFI warning codes, status high bit=0 ---

EFI_Warnings_Names:
DB  '?'				, 0	; 0=Invalid warning code
DB  'EFI_WARN_UNKNOWN_GLYPH'	, 0     ; 1
DB  'EFI_WARN_DELETE_FAILURE'	, 0	; 2
DB  'EFI_WARN_WRITE_FAILURE'	, 0     ; 3
DB  'EFI_WARN_BUFFER_TOO_SMALL'	, 0     ; 4

;--- Program operations names ---

Operations_Names:
DB  '?'				, 0     ; 0=Invalid operation code
DB  'Output message'		, 0     ; 1
DB  'Initialize keyboard'	, 0     ; 2
DB  'Initialize video'		, 0     ; 3
DB  'Time stall'		, 0     ; 4
DB  'Check keys press'		, 0     ; 5
DB  'Locate handle'		, 0     ; 6
DB  'Handle protocol'		, 0     ; 7
DB  'Get video mode'		, 0     ; 8
DB  'Set video mode'		, 0     ; 9
DB  'Locate protocol'		, 0     ; 10
DB  'Open or create file'	, 0     ; 11
DB  'Close file'		, 0     ; 12
DB  'Read file'			, 0     ; 13
DB  'Write file'		, 0     ; 14

;--- Save text report and screen image status ---

Save_File_Status:
DB  '?'					, 0	; 0=No file operations exec.
DB  'Text report saved OK, '		, 0	; 1
DB  'Graphics screen saved OK, '	, 0	; 2
DB  'Error saving text report'		, 0	; 3
DB  'Error saving graphics screen'	, 0	; 4

;--- Additional strings ---

Str_Operation	DB  'Operation = ' , 0
Str_Status	DB  'Status    = ' , 0

