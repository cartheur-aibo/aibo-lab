//---------------------------------------------
//  The dog which hail.
//---------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

:100
IF:<:Touch_head_time:200:100  //The time when it touches it.

//When it is touched by more than 200, banzai!
:200
PLAY:AIBO:Banzai_sit_C        //When it is stroked, banzai.
PLAY:SOUND:banz1ttp:50
WAIT

SET:Touch_head_time:0
GO:100
