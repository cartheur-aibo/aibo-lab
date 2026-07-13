//------------------------------------------
//  Movement AIBO
//------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

:100
PLAY:SOUND:trk4_xxx:50    	//Start sound
MOVE:LEGS:WALK:SLOW:FORWARD:10
WAIT

PLAY:SOUND:trk4_xxx:50    	//Start sound
MOVE:LEGS:WALK:SLOW:FORWARD:10
WAIT

PLAY:SOUND:trk4_xxx:50    	//Start sound
MOVE:LEGS:WALK:SLOW:FORWARD:10
WAIT

PLAY:SOUND:trk4_xxx:50    	//Start sound
MOVE:LEGS:WALK:SLOW:FORWARD:10
WAIT

PLAY:SOUND:trk4_xxx:50    	//Start sound
MOVE:LEGS:WALK:SLOW:FORWARD:10
WAIT

PLAY:SOUND:trk4_xxx:50    	//Start sound

:200
SET:stat:Gsensor_status
AND:stat:1
IF:=:stat:1:9000 // Fall plump
GO:200

:9000
MOVE:AIBO:ReactiveGU
WAIT
GO:200
