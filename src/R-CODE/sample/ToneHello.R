//------------------------------------------
// ERS-111 tone-trigger hello demo
//
// Goal:
// - boot from the preserved R-CODE programming stick
// - wait for the melody ID produced by do-re-mi (CDE)
// - perform one unmistakable built-in motion
//
// This stays intentionally close to the Sony manual's Prog 1-2 example.
//------------------------------------------

SET:Power:1
POSE:AIBO:slp_slp
SET:Melody_id:0

:100
IF:=:Melody_id:1:200
WAIT:200
GO:100

:200
PLAY:AIBO:Akubi_sit
WAIT:2000
SET:Melody_id:0
GO:100
