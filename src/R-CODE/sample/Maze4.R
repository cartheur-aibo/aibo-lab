//------------------------------------------
//  Maze 4
//------------------------------------------

SET:Power:1

//----- Main Loop
:100
  POSE:AIBO:oStanding
  WAIT:AIBO
  MOVE:HEAD:ABS:0:0:0:1000
  WAIT
  MOVE:LEGS:WALK:0:FORWARD:0 // Advance

FOR:i:0:100
  CALL:110
NEXT
EXIT
//------ Main Loop end

// Obstacle check
:110
  SET:t:0
  WHILE:t:<:100  // WHILE Loop
    IF:Distance:<:300:THEN  // When there was an obstacle in front.
      CALL:120
      IF:dd:<>:0:BREAK
    ELSE
      ADD:t:1
    ENDIF
  WEND           // End of While loop
  IF:t:>=:100:CALL:120
RETURN


// A posture is prepared.
:120
  PLAY:LEGS:WalkToWS
  WAIT:LEGS
  CALL:130
RETURN


// The direction check which can progress.
:130
  SET:Wait:0
  MOVE:HEAD:ABS:0:-90:0:1000
  MOVE:HEAD:ABS:0:90:0:2000
  MOVE:HEAD:ABS:0:0:0:1000
  SET:dd:0

  WHILE:Wait:<>:0
    SET:d:Distance
    SET:p:Head_pan
    IF:d:>:dd:THEN
      SET:dd:d
      SET:pan:p
    ELSE
      SET:dd:dd
    ENDIF
  WEND

  IF:pan:<:-15:THEN    // At the time of pan<-15, turning right.
    CALL:200
    IF:Distance:>:500:BREAK
  ELSE
    IF:pan:>:15:THEN   // At the time of pan>15, turning left.
      CALL:300
      IF:Distance:>:500:BREAK
    ELSE
      IF:dd:<:300:CALL:133     // At the time of dd<300, recession
      IF:Distance:>:500:BREAK
    ENDIF
  ENDIF
RETURN


// Back
:133
  PLAY:SOUND:ang1_xxa:100
  MOVE:LEGS:STEP:11:0:10
  WAIT
  WAIT:1000
RETURN


// Turning right
:200
  MOVE:HEAD:ABS:0:0:0:1000
  WAIT
  WHILE:Distance:<:500
    MOVE:LEGS:STEP:12:0:1
    WAIT
    WAIT:500
  WEND
RETURN


// Turning left
:300
  MOVE:HEAD:ABS:0:0:0:1000
  WAIT
  WHILE:Distance:<:500
    MOVE:LEGS:STEP:13:0:1
    WAIT
    WAIT:500
  WEND
RETURN

