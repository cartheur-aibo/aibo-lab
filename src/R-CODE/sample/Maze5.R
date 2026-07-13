//------------------------------------------
//  Maze 4
//------------------------------------------

SET:Power:1

ONCALL:&:Gsensor_status:1:9000:2  // When it fell down.

DO // An unlimited loop

  POSE:AIBO:oStanding
  WAIT
  MOVE:HEAD:ABS:0:0:0:1000
  WAIT

  MOVE:LEGS:WALK:0:FORWARD:0 // Start advance.

  FOR:t:1:10000
    IF:Distance:<:300:BREAK  // Whether to discover an obstacle, with t=100, Break
    WAIT:1
  NEXT

  PLAY:LEGS:WalkToWS         // Suspension of a walk.
  WAIT

  MOVE:HEAD:ABS:0:0:0:1000   // Neck vibration work start.
  MOVE:HEAD:ABS:0:-80:0:1000
  MOVE:HEAD:ABS:0:80:0:2000
  MOVE:HEAD:ABS:0:0:0:1000

  SET:dd:0         // The angle that a distance to the obstacle is the biggest is looked for.
  WHILE:Wait:>:0
    SET:d:Distance
    SET:p:Head_pan
    IF:d:>:dd:THEN
      SET:dd:d
      SET:pan:p
    ENDIF
  WEND

  IF:pan:<:-15:THEN          // The right turn.
    REPEAT
      MOVE:LEGS:STEP:12:0:1  // A turn is repeated.
      WAIT
      WAIT:500
    UNTIL:Distance:>:500     // Until a distance to the obstacle becomes more than 500.
  ELSE
  IF:pan:>:15:THEN           // The left turn
    REPEAT
      MOVE:LEGS:STEP:13:0:1  // A turn is repeated.
      WAIT
      WAIT:500
    UNTIL:Distance:>:500     // Until a distance to the obstacle becomes more than 500.
  ENDIF
  ENDIF

  IF:dd:<:300:THEN           // When all directions are occupied. (recession)
    PLAY:SOUND:ang1_xxa:100
    MOVE:LEGS:STEP:11:0:10
    WAIT
    WAIT:3000
  ENDIF

LOOP
EXIT

:9000                        // Turnover recovery
QUIT:AIBO
MOVE:AIBO:ReactiveGU
WAIT
RESUME
