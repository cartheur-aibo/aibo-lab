//------------------------------------------
//  SoccerDog1
//------------------------------------------

SET:Power:1

SET:mode:0 // 0:search mode 1:It is run after, the mode
SET:head:0 // Ccounter to turn a head in the search mode
SET:lost:0 // The number of times that it lost sight of a ball

:100
SET:stat:Gsensor_status
AND:stat:1
IF:=:stat:1:9000 // Fall plump.
IF:=:mode:0:1000 // To the search mode.
IF:=:mode:1:2000 // It is run after, to the mode.
GO:100

:1000
MOVE:LEGS:STEP:RIGHT_TURN:0:4  // A ball is looked for with the right turn doing.
IF:=:head:0:1101 // The management to turn a head
IF:=:head:1:1102
IF:=:head:2:1103
IF:=:head:3:1104
IF:=:head:4:1105
IF:=:head:5:1106
IF:=:head:6:1107
IF:=:head:7:1108
IF:=:head:8:1109
:1101
MOVE:HEAD:ABS:-15:0:0:500
GO:1200
:1102
MOVE:HEAD:ABS:-15:-40:0:500
GO:1200
:1103
MOVE:HEAD:ABS:-15:-80:0:500
GO:1200
:1104
MOVE:HEAD:ABS:-45:-40:0:500
GO:1200
:1105
MOVE:HEAD:ABS:-45:0:0:500
GO:1200
:1106
MOVE:HEAD:ABS:-45:40:0:500
GO:1200
:1107
MOVE:HEAD:ABS:-45:80:0:500
GO:1200
:1108
MOVE:HEAD:ABS:-15:40:0:500
GO:1200
:1109
MOVE:HEAD:ABS:-15:0:0:500
GO:1200
:1200
ADD:head:1  // The increment of head.
MOD:head:9
IF:<:Cdt_npixel:32:1210  // Can you see a ball?
SET:mode:1               // If it can be seen, it is run after, to the mode.
GO:100
:1210
WAIT
GO:100

:2000
IF:<:Cdt_npixel:32:2010:2020  // Can you see a ball?
:2010
ADD:lost:1 // lost, +1
GO:2100
:2020
SET:lost:0 // lost, 0 clearance
GO:2100
:2100
IF:>:lost:0:2110:2120  // As for lost>0, to the search mode
:2110
SET:mode:0
GO:100
:2120
SET:mode:1
MOVE:HEAD:C-TRACKING:1000  // It is run after, the mode start.

IF:>:Head_tilt:-58:2300    // In the angle of the head, distance estimation.
IF:>:Psd_range:9999:2300   // Can't you use an obstacle sensor?

IF:>:Head_pan:0:2210:2220  // If it is near, kick!
:2210
MOVE:LEGS:KICK:LEFT_KICK:0
MOVE:LEGS:STEP:SLOW:FORWARD:1
GO:2900
:2220
MOVE:LEGS:KICK:RIGHT_KICK:0
MOVE:LEGS:STEP:SLOW:FORWARD:1
GO:2900

:2300  // It approaches a ball by the angle of the head.
IF:>:Head_pan:60:2310
IF:>:Head_pan:45:2320
IF:>:Head_pan:15:2330
IF:<:Head_pan:-60:2340
IF:<:Head_pan:-45:2350
IF:<:Head_pan:-15:2360
MOVE:LEGS:STEP:SLOW:FORWARD:4
GO:2900
:2310
MOVE:LEGS:STEP:LEFT_TURN:0:4
GO:2900
:2320
MOVE:LEGS:STEP:SLOW:LEFT:4
GO:2900
:2330
MOVE:LEGS:STEP:SLOW:LEFTFORWARD:4
GO:2900
:2340
MOVE:LEGS:STEP:RIGHT_TURN:0:4
GO:2900
:2350
MOVE:LEGS:STEP:SLOW:RIGHT:4
GO:2900
:2360
MOVE:LEGS:STEP:SLOW:RIGHTFORWARD:4
GO:2900
:2900
WAIT
GO:100

:9000
QUIT:AIBO
MOVE:AIBO:ReactiveGU	// Turnover recovery
WAIT
GO:100
