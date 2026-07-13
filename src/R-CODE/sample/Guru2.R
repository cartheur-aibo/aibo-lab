//-------------------------------------------
//  Start the circle 2
//-------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp
WAIT

// Normal mode - the right bend.
:100
MOVE:HEAD:ABS:-30:0:0:1000
WAIT
MOVE:LEGS:WALK:0:FORWARD:0 // Advance

:110
IF:<:Distance:300:120      // The front obstacle check.
WAIT:1
GO:110

// When there was an obstacle in front.
:120
PLAY:LEGS:WalkToWS
POSE:AIBO:oStanding        // Stop
WAIT

:130
MOVE:HEAD:ABS:-30:-90:0:1000  // It turns to the right.
WAIT
WAIT:500
IF:>:Distance:300:200  // The right turn.
MOVE:HEAD:ABS:-30:90:0:2000  // It turns to the left.
WAIT
WAIT:500
IF:>:Distance:300:300  // The left turn.

MOVE:HEAD:ABS:-30:0:0:1000

PLAY:SOUND:ang1_xxa:100
MOVE:LEGS:STEP:11:0:10
WAIT
GO:130

:200
MOVE:HEAD:ABS:-30:0:0:1000
WAIT
:210
MOVE:LEGS:STEP:12:0:4
WAIT
IF:>:Distance:300:100
GO:210

:300
MOVE:HEAD:ABS:-30:0:0:1000
WAIT
:310
MOVE:LEGS:STEP:13:0:4
WAIT
IF:>:Distance:300:100
GO:310
