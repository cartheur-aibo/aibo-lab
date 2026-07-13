//------------------------------------------
//  Step Dog 3
//------------------------------------------

SET:Power:1

:100
SET:stat:Gsensor_status //It prepares for the flag for the turnover time.
AND:stat:1

MOVE:LEGS:STEP:ADAPTIVE:FORWARD:10

MOVE:LEGS:STEP:CUSTOM:FORWARD:10

MOVE:LEGS:STEP:SKIP_BACK:FORWARD:10

MOVE:LEGS:STEP:RIGHT_TURN:0:13

MOVE:LEGS:STEP:LEFT_TURN:0:13

MOVE:LEGS:STEP:SLOW:BACKWARD:30

IF:=:stat:1:9000        // Fall plump
GO:100  //Repetition

//Turnover recovery management
:9000
QUIT:AIBO
MOVE:AIBO:ReactiveGU	// Turnover recovery
WAIT
GO:100
