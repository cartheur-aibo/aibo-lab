//------------------------------------------
//  QUIT Dog 2
//------------------------------------------

SET:Power:1
POSE:AIBO:std_std
WAIT

:100
LET:x:Gsensor_status  // status of Gsensor is brought.
AND:x:3               // 3bit (It is taken up in its arms.) is brought down.

MOVE:LEGS:WALK:SLOW:FORWARD:0
IF:=:x:2:200         // Does 3bit stand?
GO:100

:200
QUIT:AIBO                 // It is taken up and detected in its arms!
