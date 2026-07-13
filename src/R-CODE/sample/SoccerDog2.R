//------------------------------------------
//  SoccerDog2
//------------------------------------------

SET:Power:1

SET:mode:0 // 0:search mode 1:It is run after, the mode.
SET:head:0 // counter to turn a head in the search mode.
SET:lost:0 // The number of times that it lost sight of a ball.

:100 // MainLoop
IF:&:Gsensor_status:1:9000 // Fall plump
IF:=:mode:0:1000 // search mode
IF:=:mode:1:2000 // It is run after, to the mode.
GO:100

:1000 // mode:0 search mode
SET:mode:0
MOVE:LEGS:STEP:RIGHT_TURN:0:4 // A ball is looked for with the right turn doing.
SWITCH:head //The management to turn a head.
CASE:0:MOVE:HEAD:ABS:-15:0:0:500
CASE:1:MOVE:HEAD:ABS:-15:-40:0:500
CASE:2:MOVE:HEAD:ABS:-15:-80:0:500
CASE:3:MOVE:HEAD:ABS:-45:-40:0:500
CASE:4:MOVE:HEAD:ABS:-45:0:0:500
CASE:5:MOVE:HEAD:ABS:-45:40:0:500
CASE:6:MOVE:HEAD:ABS:-45:80:0:500
CASE:7:MOVE:HEAD:ABS:-15:40:0:500
CASE:8:MOVE:HEAD:ABS:-15:0:0:500
ADD:head:1 // The increment of head.
MOD:head:9
WAIT
IF:<:Cdt_npixel:32:100 // If a ball can be seen, it is run after, to the mode

:2000 // It is run after, the mode.
SET:mode:1
IF:<:Cdt_npixel:32:1000    // Can you see a ball?

MOVE:HEAD:C-TRACKING:100   // Color chase start.
IF:>:Head_tilt:-58:2300    // In the angle of the head, distance estimation.

IF:>:Head_pan:0:2210:2220  // If it is near, kick!
:2210
MOVE:HEAD:HOME // Not to kick the jaw.
MOVE:LEGS:KICK:LEFT_KICK:0
MOVE:LEGS:STEP:SLOW:FORWARD:1
GO:2900
:2220
MOVE:HEAD:HOME // Not to kick the jaw.
MOVE:LEGS:KICK:RIGHT_KICK:0
MOVE:LEGS:STEP:SLOW:FORWARD:1
GO:2900

:2300 // It approaches a ball by the angle of the head.
CSET:>:Head_pan:60:1
CSET:>:Head_pan:45:2
CSET:>:Head_pan:15:3
CSET:<:Head_pan:-60:4
CSET:<:Head_pan:-45:5
CSET:<:Head_pan:-15:6
CSET:=:0:0:0
CASE:0:MOVE:LEGS:STEP:SLOW:FORWARD:4
CASE:1:MOVE:LEGS:STEP:SLOW:FORWARD:4
CASE:2:MOVE:LEGS:STEP:LEFT_TURN:0:4
CASE:3:MOVE:LEGS:STEP:SLOW:LEFT:4
CASE:4:MOVE:LEGS:STEP:SLOW:LEFTFORWARD:4
CASE:5:MOVE:LEGS:STEP:RIGHT_TURN:0:4
CASE:6:MOVE:LEGS:STEP:SLOW:RIGHT:4
CASE:7:MOVE:LEGS:STEP:SLOW:RIGHTFORWARD:4

:2900
WAIT
GO:100

:9000 // Turnover recovery
QUIT:AIBO
MOVE:AIBO:ReactiveGU
WAIT
GO:100
