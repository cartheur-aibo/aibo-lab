//------------------------------------------
//  QUIT Dog 1
//------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

SET:motion:0

:100 //main
IF:>:Cdt_npixel:32:1000     //When a ball is seen, emergency shut down.

SWITCH:motion
CASE:0:PLAY:AIBO:Yes_sit
CASE:1:MOVE:LEGS:STEP:SLOW:1:10
CASE:2:MOVE:HEAD:ABS:90:0:0:1000
CASE:3:MOVE:TAIL:SWING:20:0:1000
ADD:motion:1 //The increment of motion.
MOD:motion:5
WAIT
GO:100

:1000
SWITCH:motion
CASE:0:QUIT:AIBO
CASE:1:QUIT:LEGS
CASE:2:QUIT:HEAD
CASE:3:QUIT:TAIL
