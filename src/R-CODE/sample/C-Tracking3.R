//-------------------------------------------
//  Tracking 3
//-------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

:100
MOVE:LEGS:WALK:0:1:0     // walk start
MOVE:HEAD:C-TRACKING:100 // tracking start
:200
IF:>:Cdt_npixel:4000:300 // Is a ball terribly near?
GO:200
:300
MOVE:HEAD:HOME           // Suspension of tracking.
WAIT
PLAY:AIBO:Joy1_std       // The pose of joy.
WAIT
GO:100                   // loop
