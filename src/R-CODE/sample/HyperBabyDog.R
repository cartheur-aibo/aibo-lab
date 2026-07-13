//-----------------------------------------------------------
//  Hyper-baby DOG
//  It is run after when a ball is seen.
//  It cries when it is struck.
//  It is glad to stroke it.
//-----------------------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

SET:ball:0         // It is run after, the mode
SET:bang:0         // Were you struck?
SET:nade:0         // Were you stroked?
SET:amae:0         // It takes it for granted, the mode.
SET:head:0         // The value of Prev_Touch_head_press is held.
SET:cntb:0         // The number of times that it was struck.
SET:cntr:0         // Counter of the war for several student.
SET:Touch_head_press:0 // For debugging?
//-----------------------------------------------------------
//  Main loop
:100
SET:fall:Gsensor_status
AND:fall:1         //turnover check

SET:start:100

IF:=:fall:1:9000   // turnover
IF:=:ball:1:2000   // The ball follow-up mode
IF:=:bang:1:3000   // Were you struck?
IF:=:amae:1:4000   // It takes it for granted, the mode
IF:=:nade:1:5000   // pad mode.
IF:=:ball:0:1000   // Each flag detection mode.
GO:100

//-----------------------------------------------------------
//  Each flag detection mode.
:1000
WAIT:5000
SET:ball:0   // The initialization of ball.

:1100
IF:=:head:Touch_head_press:1300 // At the time of the value of the last loop.
SET:head:Touch_head_press
IF:<:Touch_head_press:300:1200 //Were you struck?
SET:bang:1
GO:100
:1200
IF:<:Touch_head_press:30:1300  //Were you stroked?
SET:nade:1
GO:100

:1300
IF:>:Distance:200:1400   //When a person is near here.
SET:amae:1               //The amae mode is set.
GO:100

:1400
IF:<:Cdt_npixel:1000:1500  //If a ball can be seen near here.
SET:ball:1               //A ball is kicked.
GO:100

//When what is specially never done, too, by the number of wars, for the behavior, change
:1500
ADD:cntr:1
IF:>:cntr:10:1510  //When it was too active, to the fatigue mode
RND:rand:1:3       //The occurrence of the number of wars is done. (1 - 3)
IF:=:rand:1:1530
IF:=:rand:2:1540
IF:=:rand:3:1550
GO:100

//AIBO fatigue mode 1 sleep is done.
:1510
SET:cntr:0    //Counter initialization
PLAY:AIBO:Akubi_slp_D     //Yawn
POSE:AIBO:oSleeping5      //The pose of sleep.
WAIT

:1520  //It sleeps until it is set up.
IF:>:Touch_head_time:80:100
GO:1520

//The first.
:1530
PLAY:AIBO:Think_slp_C  //It seems to be boring.
GO:100

//The second.
:1540
MOVE:LEGS:STEP:BABY:FORWARD:10  //swaying walk (back)
GO:100

//The third.
:1550
MOVE:LEGS:STEP:BABY:BACKWARD:10 //swaying walk (the front)
GO:100

//-----------------------------------------------------------
//The ball kick mode
:2000
SET:ball:1

IF:<:Cdt_npixel:1000:100   //Is a ball near?
MOVE:HEAD:C-TRACKING:100   //Color chase start.
IF:>:Head_pan:0:2100:2200  //A foot to kick by the angle of the neck is decided.

// The left foot kick.
:2100
MOVE:HEAD:HOME
MOVE:LEGS:KICK:LEFT_KICK:0
MOVE:LEGS:STEP:BABY:FORWARD:1
SET:ball:2100
SET:ball:0   // The initialization of ball.
GO:100

// The right foot kick.
:2200
MOVE:HEAD:HOME
MOVE:LEGS:KICK:RIGHT_KICK:0
MOVE:LEGS:STEP:BABY:FORWARD:1
SET:ball:2200
SET:ball:0   // The initialization of ball.
GO:100


//-----------------------------------------------------------
//Were you struck? mode
:3000
SET:bang:3000
SET:bang:0
ADD:cntb:1
IF:>:cntb:3:3100       // It gets angry if it is struck more than five times!
POSE:AIBO:Reset3
PLAY:AIBO:Cry_slp_C    // A sad feeling
PLAY:SOUND:sad1_xxa:50 // It cries.
WAIT
SET:Touch_head_press:0 // Initialization
WAIT:5000
GO:100


//-----------------------------------------------------------
//It gets angry when it is struck too much!
:3100
PLAY:AIBO:Tail3_sta     //anger
WAIT
PLAY:SOUND:ang5_dda:30  //anger sound
PLAY:LIGHT:ang1_eye:4   //anger eye
WAIT:5000
SET:cntb:0
GO:100


//-----------------------------------------------------------
//It takes it for granted, the mode.
:4000
SET:amae:0                //amae is initialized.
PLAY:AIBO:Please_sit3     
WAIT:5000
GO:100


//-----------------------------------------------------------
//nade mode
:5000
ADD:cntb:-1     //I do the number of times that it was struck to a little.
SET:nade:0      //The initialization of the flag.
PLAY:LIGHT:joy3_eye:10 //grin happily
PLAY:AIBO:Tail2_sit
PLAY:SOUND:joy1rxxy:50 //Joy sound
SET:Touch_head_press:0 //Initialization
WAIT:1000
GO:100

//-----------------------------------------------------------
//It fell down!
:9000
QUIT:AIBO
MOVE:AIBO:ReactiveGU
WAIT
GO:100
//==========================END==============================
