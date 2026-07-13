//------------------------------------------
//  The movement of the tail
//------------------------------------------

SET:Power:1

:100

MOVE:TAIL:HOME			//A former position
WAIT

MOVE:TAIL:ABS:30:0:1000		//A vertical movement
WAIT

MOVE:TAIL:ABS:-30:0:1000	//A vertical movement
WAIT

MOVE:TAIL:ABS:0:30:1000		//A side movement
WAIT

MOVE:TAIL:ABS:0:-30:1000	//A side movement
WAIT

MOVE:TAIL:SWING:30:10:1000	//rotation
WAIT
