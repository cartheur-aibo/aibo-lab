//------------------------------------------
//  Maze 3
//------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp

SET:Back:0

//main
DO
  IF:Back:=:0:CALL:100    // Advance

  CALL:200          //A head is shaken at the side.

  SET:dd:0

  CALL:300          //The settlement of the value

  IF:pan:<:-15:THEN
    CALL:400        //The right turn
  ELSE
    IF:pan:>:15:THEN
      CALL:500      //The left turn
    ELSE
      IF:dd:<:300:THEN
        SET:Back:1
        CALL:600    //Stepping backward
      ELSE
        SET:Back:0
      ENDIF
    ENDIF
  ENDIF
LOOP

:100
POSE:AIBO:oStanding
WAIT
MOVE:HEAD:ABS:0:0:0:1000
WAIT
MOVE:LEGS:WALK:0:FORWARD:0 // Advance

FOR:t:0:100:1
  IF:Distance:<:300:BREAK
  WAIT:1
NEXT

// When there was an obstacle in front.
PLAY:LEGS:WalkToWS
//POSE:AIBO:oStanding        // Stop
WAIT
RETURN

:200
MOVE:HEAD:ABS:0:0:0:1000
MOVE:HEAD:ABS:0:-90:0:1000
MOVE:HEAD:ABS:0:90:0:2000
MOVE:HEAD:ABS:0:0:0:1000
RETURN

:300
DO:WHILE:<>:Wait:0
  SET:d:Distance
  SET:p:Head_pan
  IF:d:<:dd:300
  SET:dd:d
  SET:pan:p
LOOP
RETURN

:400
MOVE:HEAD:ABS:0:0:0:1000
WAIT
DO
  MOVE:LEGS:STEP:12:0:1
  WAIT
  WAIT:500
LOOP:UNTIL:Distance:>:500
RETURN

:500
MOVE:HEAD:ABS:0:0:0:1000
WAIT
DO
  MOVE:LEGS:STEP:13:0:1
  WAIT
  WAIT:500
LOOP:UNTIL:Distance:>:500
RETURN

:600
PLAY:SOUND:ang1_xxa:100
MOVE:LEGS:STEP:11:0:10
WAIT
WAIT:1000
RETURN
