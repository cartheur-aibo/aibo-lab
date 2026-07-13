//---------------------------------------------
//  The dog which gets angry when it is struck.
//---------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp
WAIT
SET:Touch_head_press:0

:100
IF:<:Touch_head_press:200:100

:200
PLAY:AIBO:Tail3_sta     //anger
WAIT
PLAY:SOUND:ang5_dda:30  //anger sound
PLAY:LIGHT:ang1_eye:10  //anger eye
WAIT
