//------------------------------------------
//  The movement of the head
//------------------------------------------

SET:Power:1

:100
// It is absolutely moved in the angle.
MOVE:HEAD:HOME			// A former position
WAIT

MOVE:HEAD:ABS:30:0:0:1000	//A vertical movement.
WAIT

MOVE:HEAD:ABS:-30:0:0:1000	//A vertical movement.
WAIT

MOVE:HEAD:ABS:0:30:0:1000	//A side movement.
WAIT

MOVE:HEAD:ABS:0:-30:0:1000	//A side movement.
WAIT

MOVE:HEAD:ABS:0:0:30:1000	//It is twisted.
WAIT

MOVE:HEAD:ABS:0:0:-30:1000	//It is twisted.
WAIT

// It is moved in the relative angle.
MOVE:HEAD:HOME			//A former position.
WAIT

MOVE:HEAD:REL:30:0:0:1000	//A vertical movement.
WAIT

MOVE:HEAD:REL:-30:0:0:1000	//A vertical movement.
WAIT

MOVE:HEAD:REL:0:30:0:1000	//A side movement.
WAIT

MOVE:HEAD:REL:0:-30:0:1000	//A side movement.
WAIT

MOVE:HEAD:REL:0:0:30:1000	//It is twisted.
WAIT

MOVE:HEAD:REL:0:0:-30:1000	//It is twisted.
WAIT
