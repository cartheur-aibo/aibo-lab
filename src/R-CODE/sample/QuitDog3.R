//------------------------------------------
//  QUIT Dog 3
//------------------------------------------

SET:Power:1

:100
SET:stat:Gsensor_status
AND:stat:1

IF:=:stat:1:200 // Fall plump
MOVE:LEGS:WALK:SLOW:FORWARD:0
GO:100

:200
QUIT:AIBO
