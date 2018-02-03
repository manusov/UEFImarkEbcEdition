;===========================================================================;
;=                                                                         =;
;=                 MACRO FOR DEBUG CHECKPOINTS ASSEMBLING                  =;
;=                                                                         =;
;===========================================================================;

MACRO CHK op1			; Send checkpoint = op1, corrupt R7
{
MOVIBW R7,op1
CALL32 Send_Checkpoint_R7
}

MACRO CHKR7 op1			; Send checkpoint = R7
{
CALL32 Send_Checkpoint_R7
}
