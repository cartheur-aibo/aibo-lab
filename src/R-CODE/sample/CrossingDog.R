//------------------------------------------------
//  The imitation to cross a pedestrian crossing.
//  The combination of mere PLAY
//------------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

:100       //A condition branches off only with a main loop.

POSE:AIBO:oStanding    //It stands up, and it is raised.

//It keeps still for a while, and the right and left are confirmed. 
//(the right and left of the right)
MOVE:HEAD:ABS:0:-90:0:3000
MOVE:HEAD:ABS:0:0:0:3000
MOVE:HEAD:ABS:0:90:0:3000
MOVE:HEAD:ABS:0:0:0:3000
MOVE:HEAD:ABS:0:-90:0:3000
MOVE:HEAD:ABS:0:0:0:3000
WAIT

POSE:LEGS:Oterrb_sit   // A hand is raised. (Hi!)
POSE:AIBO:oStanding    // It stands up, and it is raised.

MOVE:LEGS:STEP:DEFAULT:FORWARD:20   // It proceeds in the front.
 
GO:100
