//------------------------------------------
//  Maze 2
//------------------------------------------

SET:Power:1
SET:loop_cnt:0
SET:pan_sum:0

ONCALL:&:Gsensor_status:1:9000
POSE:AIBO:oStanding
WAIT

// Normal mode
:100
    POSE:AIBO:oStanding
    WAIT
    MOVE:LEGS:WALK:0:FORWARD:0 // Advance

    SET:t:0

:110
    IF:<:Distance:300:120      // The front obstacle check.
    WAIT:1
    ADD:t:1
    IF:>:t:100:120
GO:110

:120 // When there was an obstacle in front.
    PLAY:LEGS:WalkToWS
    POSE:AIBO:oStanding        // Stop
    WAIT

:130
    MOVE:HEAD:ABS:0:-90:0:1000
    MOVE:HEAD:ABS:0:0:0:1000
    MOVE:HEAD:ABS:0:90:0:1000
    MOVE:HEAD:ABS:0:0:0:1000

:1010
    IF:=:Wait:0:1100
    // During the management
    :1011
        SET:count:0
        :1020
            IF:<:Distance:300:1040  // Is whether Distance less than 300?
                ADD:pan_sum:Head_pan
                ADD:count:1
                IF:=:Wait:0:100
            GO:1020

            :1040  // When Distance became less than 300.
            IF:=:count:0:1010
        :1041
            ADD:loop_cnt:1
            IF:=:loop_cnt:2:1043
                :1042   // At the time of loop_cnt=1
                    DIV:pan_sum:count
                    LET:ave1:pan_sum
                GO:1010
                :1043  // At the time of loop_cnt=2
                    DIV:pan_sum:count
                    LET:ave2:pan_sum
                GO:1010

:1100  // Wait loop end
    IF:=:loop_cnt:1:1110
        // When ave's are two. (2, the left, 1, the right)
        LET:temp_ave:ave1
        MUL:temp_ave:-1
        IF:<=:ave2:temp_ave:1110
            LET:pan:ave2
        GO:1200
    :1110
        LET:pan:ave1
    GO:1200
:1200
    LET:loop_cnt:0

:2000
    IF:>:pan:50:3010
    IF:>:pan:20:3020
    IF:>:pan:0:3030
    IF:>:pan:-20:2030
    IF:>:pan:-50:2020
    IF:>:pan:-90:2010
    IF:<:Distance:300:133
GO:100

:133
    PLAY:SOUND:ang1_xxa:100
    MOVE:LEGS:STEP:11:0:10
    WAIT
GO:130

:2010
    MOVE:HEAD:ABS:0:0:0:1000
    WAIT
:2011
    MOVE:LEGS:STEP:12:0:6
    WAIT
    IF:>:Distance:300:100
GO:2030

:2020
    MOVE:HEAD:ABS:0:0:0:1000
    WAIT
:2021
    MOVE:LEGS:STEP:12:0:4
    WAIT
    IF:>:Distance:300:100
GO:2030

:2030
    MOVE:HEAD:ABS:0:0:0:1000
    WAIT
:2031
    MOVE:LEGS:STEP:12:0:2
    WAIT
    IF:>:Distance:300:100
GO:2030

:3010
    MOVE:HEAD:ABS:0:0:0:1000
    WAIT
:3011
    MOVE:LEGS:STEP:13:0:6
    WAIT
    IF:>:Distance:300:100
GO:3030

:3020
    MOVE:HEAD:ABS:0:0:0:1000
    WAIT
:3021
    MOVE:LEGS:STEP:13:0:4
    WAIT
    IF:>:Distance:300:100
GO:3030

:3030
    MOVE:HEAD:ABS:0:0:0:1000
    WAIT
:3031
    MOVE:LEGS:STEP:13:0:2
    WAIT
    IF:>:Distance:300:100
GO:3030


:9000
    QUIT:AIBO
    MOVE:AIBO:ReactiveGU
    WAIT
RESUME
