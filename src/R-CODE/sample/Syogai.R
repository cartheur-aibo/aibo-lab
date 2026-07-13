//------------------------------------------
//  Difficulty
//------------------------------------------

SET:Power:1

POSE:AIBO:std_std
WAIT

:100
MOVE:LEGS:WALK:SLOW:FORWARD:0 	// Advance

:200
IF:<:Distance:500:300:200	//Is it smaller than a distance 500mm of the obstacle?

:300
PLAY:LIGHT:sup1_eye:0		//The eye which it was surprised at.
PLAY:AIBO:Biku_sit		//It is surprised, and it sits down.
WAIT
QUIT:LIGHT
MOVE:LEGS:STEP:SLOW:BACKWARD:10	//Recession
GO:100
