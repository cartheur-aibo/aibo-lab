//------------------------------------------
// Sound is taken out, and it moves forward.
// The sample that sound is changed and it 
// withdraws when an obstacle is detected.
//------------------------------------------

SET:Power:1		// Power supply On.

POSE:AIBO:std_std:::1	// Start posture

DO

  // Advance
  MOVE:LEGS:WALK:SLOW:FORWARD:0
  WHILE:Distance:>:500
    PLAY:SOUND:joy1_xxa:100:1
    PLAY:LIGHT:joy1_eye:1:1
  WEND
  STOP:SOUND

  // Recession
  MOVE:LEGS:WALK:SLOW:BACKWARD:0
  WHILE:Distance:<:500
    PLAY:SOUND:iya1_xxx:100:1
    PLAY:LIGHT:ang1_eye:1:1
  WEND
  STOP:SOUND

LOOP // An unlimited loop ¨ DO
