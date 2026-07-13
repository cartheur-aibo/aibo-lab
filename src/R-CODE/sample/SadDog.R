//----------------------------------
//  A pitiful dog
//  Obstacle sensor
//----------------------------------

SET:Power:1
POSE:AIBO:slp_slp
WAIT
POSE:AIBO:std_std
WAIT

:100
MOVE:LEGS:WALK:SLOW:FORWARD:0  //It walks slowly.

:200
IF:<:Distance:500:300:200            //Obstacle discovery!

:300
STOP:AIBO
PLAY:AIBO:Wake_slpb             // help
WAIT

//end
