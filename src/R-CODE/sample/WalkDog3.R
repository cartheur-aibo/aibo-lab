//------------------------------------------
//  Walk Dog 3
//------------------------------------------

SET:Power:1

:100
SET:stat:Gsensor_status
AND:stat:1

IF:=:stat:1:200 // Fall plump
MOVE:LEGS:WALK:ADAPTIVE:FORWARD:10

MOVE:LEGS:WALK:CUSTOM:FORWARD:10

MOVE:LEGS:WALK:SKIP_BACK:FORWARD:10

MOVE:LEGS:WALK:RIGHT_TURN:0:13

MOVE:LEGS:WALK:LEFT_TURN:0:13

MOVE:LEGS:WALK:SLOW:BACKWARD:30

GO:100

:200
QUIT:AIBO
MOVE:AIBO:ReactiveGU    // Turnover recovery
WAIT
GO:100
