//------------------------------------------
//  The dog that it hug
//------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

// MainLoop
:100
SET:stat:Gsensor_status
AND:stat:2          // 2bit is brought down.
IF:<>:stat:0:9000   // When it hug, to 9000
GO:100

:9000
PLAY:SOUND:joy2_xxt:50  // Happy sound
PLAY:LIGHT:joy4_eye:16  // Happy light
