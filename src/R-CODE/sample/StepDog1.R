//------------------------------------------
//  Step Dog 1
//------------------------------------------

SET:Power:1

:100
SET:stat:Gsensor_status //It prepares for the flag for the turnover time.
AND:stat:1

MOVE:LEGS:STEP:SLOW:FORWARD:10

MOVE:LEGS:STEP:FAST:FORWARD:10

MOVE:LEGS:STEP:OFFLOAD:FORWARD:10

MOVE:LEGS:STEP:BABY:FORWARD:10

MOVE:LEGS:STEP:SLOW:BACKWARD:40

IF:=:stat:1:9000 // Fall plump

GO:100  //Repetition

//Turnover recovery management
:9000
QUIT:AIBO
MOVE:AIBO:ReactiveGU	// Turnover recovery
WAIT
GO:100

