//------------------------------------------
//  Maze 1
//------------------------------------------

SET:Power:1

:100
POSE:AIBO:oStanding
WAIT
MOVE:HEAD:ABS:0:0:0:1000
WAIT
MOVE:LEGS:WALK:0:FORWARD:0 // Advance

SET:t:0
:110
IF:<:Distance:300:120
WAIT:1
ADD:t:1
IF:>:t:100:120
GO:110

// When there was an obstacle in front.
:120
PLAY:LEGS:WalkToWS
//POSE:AIBO:oStanding        // Stop
WAIT

:130
MOVE:HEAD:ABS:0:0:0:1000
MOVE:HEAD:ABS:0:-90:0:1000
MOVE:HEAD:ABS:0:90:0:2000
MOVE:HEAD:ABS:0:0:0:1000
SET:dd:0
:131
IF:=:Wait:0:132
SET:d:Distance
SET:p:Head_pan
IF:<:d:dd:131
SET:dd:d
SET:pan:p
GO:131
:132
IF:<:pan:-15:200
IF:>:pan:15:300
IF:<:dd:300:133
GO:100

:133
PLAY:SOUND:ang1_xxa:100
MOVE:LEGS:STEP:11:0:10
WAIT
WAIT:1000
GO:130

:200
MOVE:HEAD:ABS:0:0:0:1000
WAIT
:210
MOVE:LEGS:STEP:12:0:1
WAIT
WAIT:500
IF:>:Distance:500:100
GO:210

:300
MOVE:HEAD:ABS:0:0:0:1000
WAIT
:310
MOVE:LEGS:STEP:13:0:1
WAIT
WAIT:500
IF:>:Distance:500:100
GO:310

