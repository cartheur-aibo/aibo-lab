//-------------------------------------------
//  Start the circle 1
//-------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

// Normal mode - the right bend.
:100
MOVE:LEGS:WALK:0:FORWARD:0 // Advance

:110
IF:>:Distance:200:100      // The front obstacle check.
SET:label:110

// When there was an obstacle in front.
:120
POSE:AIBO:oStanding        // Stop
WAIT
MOVE:HEAD:ABS:30:-90:-90:500  // It turns to the right.
WAIT
IF:>:Distance:200:130:140  // The right obstacle check.

// When there was no obstacle in the right.
:130
SET:label:130
POSE:AIBO:oStanding        // Stop
WAIT
MOVE:LEGS:STEP:RIGHT_TURN:0:15 // The right for the right.
MOVE:HEAD:ABS:0:0:0:1000   // It faces the front.
GO:100

// When there was an obstacle in the right.
:140
SET:label:140
POSE:AIBO:oStanding        // Stop
WAIT
MOVE:HEAD:ABS:30:90:90:500 // It turns to the left.
WAIT
IF:>:Distance:200:160:170  // The left obstacle check.

// When there was no obstacle in the left.
:160
SET:label:160
POSE:AIBO:oStanding        // Stop
WAIT
MOVE:LEGS:STEP:LEFT_TURN:0:15 // The left for the left.
MOVE:HEAD:ABS:0:0:0:1000   // It faces the front.
GO:100

// In case of a dead end.
:170
SET:label:170
POSE:AIBO:oStanding        // A system is prepared.
WAIT
MOVE:LEGS:STEP:RIGHT_TURN:0:20 // U-turn
GO:100
