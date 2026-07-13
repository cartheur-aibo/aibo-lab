//-------------------------------
//  A dog MOVE to go around, LEGS
//-------------------------------

SET:Power:1       // The power is turned on.
POSE:AIBO:slp_slp

:100

//It walks about square.
MOVE:LEGS:STEP:SLOW:FORWARD:20     //It proceeds in the front.
MOVE:LEGS:STEP:RIGHT_TURN:0:13     //The right 90 degrees turn.
WAIT
GO:100  //Repetition

