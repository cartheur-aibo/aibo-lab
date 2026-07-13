//----------------------------------
// Turnover recovery
//----------------------------------

SET:Power:1
ONCALL:&:Gsensor_status:1:200:2  // When it fell down.

:100
WAIT:1000
GO:100

:200
QUIT:AIBO
MOVE:AIBO:ReactiveGU    // Turnover recovery
WAIT
RESUME
