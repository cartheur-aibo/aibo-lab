//-----------------------------------------------------
//  A dog to wee-wee
//  Head touch sensor,,PLAY,MOVE,STOP
//  A number of times limitation is added to SosouDog.
//  When it is struck beyond the necessity, AIBO seems 
//  to be lonely, and to pass.
//-----------------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp
SET:bang:0                  //It is struck, the flag of the business
SET:cont:1                  //A struck number of times counter

:100
IF:>:Sound_busy:0:110       //It is stopped if sound is ringing.
STOP:SOUND
:110
IF:>:Light_busy:0:120       //It is stopped if the light is on.
STOP:LIGHT

:120
PLAY:LIGHT:joy4_eye:17      //joy
PLAY:AIBO:Ran_slpb4
WAIT

:130
SET:Touch_head_time:0       //The initialization of Touch_head_time
//IF:<:Touch_head_time:10:100
IF:<:Touch_head_press:300:100 //It was struck!

//Were you struck suddenly?
:200
ADD:cont:1       //Increase in a counter
IF:>:cont:2:300  //If it is being struck more than three times.

//It gets angry, and wee-wees.
WAIT
PLAY:LIGHT:ang1_eye:17        //Anger resides in the eyes.
WAIT
POSE:AIBO:oStanding
PLAY:AIBO:Angry_std             //It gets angry.
WAIT
PLAY:LIGHT:ang5_eye:17
PLAY:AIBO:Hang_std_D            //The pose that it wee-wees?
WAIT:1000
GO:100        //loop

//AIBO which it passes when it is struck too much
:300
STOP:LIGHT
WAIT
MOVE:HEAD:ABS:60:0:0:5000   //It faces the top.
MOVE:HEAD:ABS:-90:0:0:5000  //It faces the bottom slowly.
WAIT

POSE:AIBO:oStanding
PLAY:SOUND:sad5_ttc:40
MOVE:HEAD:ABS:-60:0:0:1000      //It faces the bottom.
MOVE:LEGS:STEP:RIGHT_TURN:0:20  //Turn
MOVE:LEGS:STEP:SLOW:FORWARD:50  //It walks and passes.

