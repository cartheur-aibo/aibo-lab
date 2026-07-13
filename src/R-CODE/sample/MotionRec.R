//-------------------------------------------
//  MOTION REC
//
//  00  Motion videotaping
//  01  Videotaping start
//  02  Regeneration
//  03  Cancellation of videotaping
//  11 LOAD POSE
//  12 SAVE_POSE
//  31 Two times regeneration
//  32 1/2 times
//
//  How to use
//  After AIBO transits in the Sleep posture, gain of the motor is cut off.
//  It becomes a videotaping start with 01Å®SEND of the sound commander after that.
//  The condition of the joint of AIBO is memorized with 00Å®SEND.
//  It becomes possible to regenerate a motion it regenerates with 02Å®SEND.
//  
//-------------------------------------------

DEF:POSE:pose:20

DEF:MOVE:move
  1000:pose11
  1000:pose12
  1000:pose13
  1000:pose14
  1000:pose15
  1000:pose16
  1000:pose17
  1000:pose18
  1000:pose19
  1000:pose20
END

SET:Power:1
SET:Melody_id:0

ONCALL:Melody_id:=:9:9000:1:100    // 01 videotaping start
ONCALL:Melody_id:=:17:9000:1:3000  // 02 regeneration
ONCALL:Melody_id:=:25:9000:1:2000  // 03 Cancellation of videotaping
ONCALL:Melody_id:=:163:9000:1:4000 // 11 LOAD_POSE
ONCALL:Melody_id:=:168:9000:1:5000 // 12 SAVE_POSE
ONCALL:Melody_id:=:193:9000:1:3100 // 31 Two times regeneration
ONCALL:Melody_id:=:200:9000:1:3200 // 32 1/2 times


POSE:AIBO:oSleeping
POSE:AIBO:PSToStation
WAIT

:start
DO
:100
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:11
  ELSE
    GO:100
  ENDIF
:200
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:12
  ELSE
    GO:200
  ENDIF
:300
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:13
  ELSE
    GO:300
  ENDIF
:400
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:14
  ELSE
    GO:400
  ENDIF
:500
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:15
  ELSE
    GO:500
  ENDIF
:600
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:16
  ELSE
    GO:600
  ENDIF
:700
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:17
  ELSE
    GO:700
  ENDIF
:800
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:18
  ELSE
    GO:800
  ENDIF
:900
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:19
  ELSE
    GO:900
  ENDIF
:1000
  IF:Melody_id:=:1:THEN
    SET:Melody_id:0
    REC:POSE:pose:20
  ELSE
    GO:1000
  ENDIF

:2000
  IF:Melody_id:<>:9:2000

:3000
PLAY:APK:move
WAIT:1000
GO:start
:3100
PLAY:APK:move:1:200
WAIT:1000
GO:start
:3200
PLAY:APK:move:1:50
WAIT:1000
GO:start

LOOP

:4000
LOAD_POSE:pose11
LOAD_POSE:pose12
LOAD_POSE:pose13
LOAD_POSE:pose14
LOAD_POSE:pose15
LOAD_POSE:pose16
LOAD_POSE:pose17
LOAD_POSE:pose18
LOAD_POSE:pose19
LOAD_POSE:pose20
GO:2000

:5000
SAVE_POSE:pose11
SAVE_POSE:pose12
SAVE_POSE:pose13
SAVE_POSE:pose14
SAVE_POSE:pose15
SAVE_POSE:pose16
SAVE_POSE:pose17
SAVE_POSE:pose18
SAVE_POSE:pose19
SAVE_POSE:pose20
GO:2000

EXIT

:9000
  SET:Melody_id:0
RESUME

