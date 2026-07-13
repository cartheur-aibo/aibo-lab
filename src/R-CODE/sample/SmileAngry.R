//-----------------------------------
//  Change in the expression
//  PLAY,GO
//-----------------------------------

SET:Power:1
POSE:AIBO:slp_slp

:100
WAIT
//Joy
PLAY:HEAD:Yes_sta
PLAY:LIGHT:joy1_eye:17
PLAY:SOUND:joy3_xxa:40
WAIT
//It gets angry.
PLAY:LIGHT:ang1_eye:17
PLAY:HEAD:No_sta
PLAY:SOUND:ang4_ttc:40
WAIT

GO:100
