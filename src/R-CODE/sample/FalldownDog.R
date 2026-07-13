//---------------------------------------
//  A dog MOVE to go around, LEGS
//  With the turnover recovery function
//---------------------------------------

SET:Power:1       // The power is turned on.
POSE:AIBO:slp_slp

:100
SET:stat:Gsensor_status // It prepares for the flag for the turnover time.
AND:stat:1
SET:round:0             
IF:=:round:0:1000       // When what is entailed.

// It walks about square.
:1000
MOVE:LEGS:STEP:SLOW:FORWARD:20     // It proceeds in the front.
IF:=:stat:1:9000        // fall plump 
MOVE:LEGS:STEP:RIGHT_TURN:0:13     // The right 90 degrees turn
IF:=:stat:1:9000        // fall plump 
GO:100  //Repetition

//Turnover recovery management.
:9000
QUIT:AIBO
MOVE:AIBO:ReactiveGU	// Turnover recovery.
WAIT
GO:100
