//-------------------------------------------------
//  The dog which has a hand when a hand approaches
//  MOVE&PLAY& obstacle sensor
//-------------------------------------------------

SET:Power:1          // The power is turned on.
POSE:AIBO:slp_slp
SET:head:0   // Counter to turn a head
SET:ote:0

:100            //MainLoop
//IF:>:Sound_busy:0:110
IF:>:Light_busy:0:120
IF:=:ote:0:1000      //Round mode
IF:=:ote:1:2000      //Hand mode
GO:100

//:110
//STOP:SOUND
//GO:100
:120
STOP:LIGHT
GO:100

//HEAD is turned, and an obstacle is looked for.
:1000
SET:ote:0
SWITCH:head
CASE:0:MOVE:HEAD:ABS:-30:0:0:500
CASE:1:MOVE:HEAD:ABS:-30:-30:0:500
CASE:2:MOVE:HEAD:ABS:-30:-60:0:500
CASE:3:MOVE:HEAD:ABS:-30:-90:0:500
CASE:4:MOVE:HEAD:ABS:-30:-60:0:500
CASE:5:MOVE:HEAD:ABS:-30:-30:0:500
CASE:6:MOVE:HEAD:ABS:-30:0:0:500
CASE:7:MOVE:HEAD:ABS:-30:30:0:500
CASE:8:MOVE:HEAD:ABS:-30:60:0:500
CASE:9:MOVE:HEAD:ABS:-30:90:0:500
CASE:10:MOVE:HEAD:ABS:-30:60:0:500
CASE:11:MOVE:HEAD:ABS:-30:30:0:500
ADD:head:1               //The increment of Head
MOD:head:12
WAIT
IF:>:Distance:500:100    //If there is no obstacle near here, round mode.

SET:ote:1                //To the hand mode

//Hand processing.
:2000

MOVE:HEAD:ABS:0:0:0:100
PLAY:LEGS:LtouchL_sit      //With the left hand, hand.
WAIT
PLAY:SOUND:joy7_ttb:30     //Happy music.
PLAY:LIGHT:joy1_eye:15     //Happy
WAIT
SET:ote:0                  //ote is returned to 0.
GO:100	                   //loop
