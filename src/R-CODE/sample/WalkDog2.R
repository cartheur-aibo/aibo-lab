//------------------------------------------
//  Walk Dog 2
//------------------------------------------

SET:Power:1

:100
SET:stat:Gsensor_status
AND:stat:1

IF:=:stat:1:200 // Fall plump
MOVE:LEGS:WALK:STABLE:FORWARD:10

MOVE:LEGS:WALK:LIZARD:FORWARD:10

MOVE:LEGS:WALK:CREEP:FORWARD:10

MOVE:LEGS:WALK:TROT:FORWARD:10

MOVE:LEGS:WALK:SLOW:BACKWARD:40
GO:100

:200
QUIT:AIBO
MOVE:AIBO:ReactiveGU    // Turnover recovery
WAIT
GO:100
