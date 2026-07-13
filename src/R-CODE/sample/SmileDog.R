//------------------------------------
//  It is pleasant.
//  Touch_head_press
//------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

SET:cont:1

:100
IF:>:Sounc_busy:0:110
STOP:SOUND
:110
IF:>:Light_busy:0:120
STOP:LIGHT
WAIT

:120
IF:<:Touch_head_time:200:100 //If it is stroked.

:200
ADD:cont:1
IF:>:cont:5:300            //If it is done five times, end
PLAY:LIGHT:joy3_eye:17     //joy
PLAY:TAIL:oSittingJoy2     //The tail is wagged.
PLAY:SOUND:joy1rxxy:30     //Joy sound
WAIT
GO:100

:300
QUIT:AIBO
SET:Power:0

