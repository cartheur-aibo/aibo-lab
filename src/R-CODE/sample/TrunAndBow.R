//---------------------------------------
//  It turns three times, bow-wows ÅI
//  The medley of PLAY
//-------------------------------------

SET:Power:1          // The power is turned on.
POSE:AIBO:slp_slp

:100

PLAY:AIBO:std_sit          //It sits down.
WAIT
PLAY:AIBO:StouchL_sit      //Hand
WAIT
PLAY:AIBO:StouchR_sit      //Substitute
WAIT
PLAY:AIBO:sit_slp          //Lie down.
WAIT
POSE:AIBO:std_std          //It stands up.
WAIT
MOVE:LEGS:WALK:RIGHT_TURN:0:100   // It turns three times.
WAIT


:200

WAIT
PLAY:SOUND:bow1_ddp:30     //bow-wows
WAIT
GO:100

