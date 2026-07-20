\ Expectation-Maximization clustering demo for gforth.
\ This is a small 1D Gaussian mixture example intended as a
\ runnable bridge from the AIBO histogram and EM notes into real Forth code.
\ Run on this machine with:
\   XDG_CACHE_HOME=/tmp gforth-fast --no-0rc scripts/aibo-em.fs

decimal

12 constant n-samples
2 constant n-clusters
50 constant max-iterations
256 constant histogram-bins

2e0 pi f* fsqrt fconstant SQRT-2PI
1e-6 fconstant SIGMA-FLOOR
1e-6 fconstant LL-THRESHOLD

create samples
  1.0e0 f, 1.2e0 f, 0.8e0 f, 1.1e0 f, 0.9e0 f, 1.3e0 f,
  8.8e0 f, 9.1e0 f, 9.4e0 f, 8.9e0 f, 9.2e0 f, 9.0e0 f,

create means n-clusters floats allot
create sigmas n-clusters floats allot
create probs n-samples n-clusters * floats allot

create hist-query histogram-bins floats allot
create hist-view-a histogram-bins floats allot
create hist-view-b histogram-bins floats allot

fvariable last-ll
fvariable current-ll
fvariable best-score
fvariable hist-a-bin
fvariable hist-b-bin
fvariable hist-denom

: sample-addr ( i -- addr )
  floats samples + ;

: mean-addr ( k -- addr )
  floats means + ;

: sigma-addr ( k -- addr )
  floats sigmas + ;

: prob-addr ( i k -- addr )
  locals| k i |
  i n-clusters * k + floats probs + ;

: sample@ ( i -- r )
  sample-addr f@ ;

: mean@ ( k -- r )
  mean-addr f@ ;

: sigma@ ( k -- r )
  sigma-addr f@ ;

: prob@ ( i k -- r )
  prob-addr f@ ;

: mean! ( r k -- )
  mean-addr f! ;

: sigma! ( r k -- )
  sigma-addr f! ;

: prob! ( r i k -- )
  prob-addr f! ;

: hist-bin-addr ( hist i -- addr )
  locals| i hist |
  i floats hist + ;

: hist@ ( hist i -- r )
  hist-bin-addr f@ ;

: hist! ( r hist i -- )
  hist-bin-addr f! ;

: clear-hist ( hist -- )
  locals| hist |
  histogram-bins 0 do
    0e0 hist i hist!
  loop ;

: hist-divergence ( hist-a hist-b -- r )
  locals| hist-b hist-a |
  0e0
  histogram-bins 0 do
    hist-a i hist@ hist-a-bin f!
    hist-b i hist@ hist-b-bin f!
    hist-a-bin f@ hist-b-bin f@ f+ hist-denom f!
    hist-denom f@ SIGMA-FLOOR f<
    if
      \ zero denominator contributes nothing
    else
      hist-a-bin f@ hist-b-bin f@ f-
      fdup f*
      hist-denom f@ f/
      f+
    then
  loop ;

: init-histograms ( -- )
  hist-query clear-hist
  hist-view-a clear-hist
  hist-view-b clear-hist
  8e0 hist-query 10 hist!
  7e0 hist-query 11 hist!
  3e0 hist-query 50 hist!
  7e0 hist-view-a 10 hist!
  8e0 hist-view-a 11 hist!
  3e0 hist-view-a 50 hist!
  1e0 hist-view-b 180 hist!
  8e0 hist-view-b 181 hist!
  6e0 hist-view-b 220 hist! ;

: nearest-view ( -- n )
  hist-query hist-view-a hist-divergence
  fdup best-score f!
  hist-query hist-view-b hist-divergence
  best-score f@ f<
  if
    1
  else
    0
  then ;

: gaussian-pdf ( i k -- r )
  locals| k i |
  i sample@ k mean@ f-
  fdup f*
  2e0 k sigma@ fdup f* f* f/
  fnegate fexp
  SQRT-2PI k sigma@ f* f/ ;

: sample-likelihood ( i -- r )
  locals| sample-index |
  0e0
  n-clusters 0 do
    sample-index i gaussian-pdf f+
  loop
;

: log-likelihood ( -- r )
  0e0
  n-samples 0 do
    i sample-likelihood fln f+
  loop ;

: expectation-step ( -- )
  n-samples 0 do
    0e0
    n-clusters 0 do
      j i gaussian-pdf f+
    loop
    fdup SIGMA-FLOOR f<
    if
      fdrop
      n-clusters s>f
      1e0 fswap f/
      n-clusters 0 do
        fdup j i prob!
      loop
      fdrop
    else
      n-clusters 0 do
        j i gaussian-pdf fover f/ j i prob!
      loop
      fdrop
    then
  loop ;

: prob-sum ( k -- r )
  locals| k |
  0e0
  n-samples 0 do
    i k prob@ f+
  loop
;

: weighted-sample-sum ( k -- r )
  locals| k |
  0e0
  n-samples 0 do
    i k prob@
    i sample@
    f* f+
  loop
;

: weighted-variance-sum ( k -- r )
  locals| k |
  0e0
  n-samples 0 do
    i k prob@
    i sample@ k mean@ f-
    fdup f*
    f* f+
  loop
;

: update-mean ( k -- )
  locals| k |
  k prob-sum
  fdup SIGMA-FLOOR f<
  if
    fdrop
    0e0 k mean!
  else
    k weighted-sample-sum fswap f/ k mean!
  then ;

: update-sigma ( k -- )
  locals| k |
  k prob-sum
  fdup SIGMA-FLOOR f<
  if
    fdrop
    SIGMA-FLOOR k sigma!
  else
    k weighted-variance-sum fswap f/ fsqrt
    fdup SIGMA-FLOOR f<
    if
      fdrop SIGMA-FLOOR
    then
    k sigma!
  then ;

: maximization-step ( -- )
  n-clusters 0 do
    i update-mean
    i update-sigma
  loop ;

: converged? ( -- flag )
  current-ll f@ last-ll f@ f- fabs LL-THRESHOLD f< ;

: init-params ( -- )
  1.0e0 0 mean!
  9.0e0 1 mean!
  1.0e0 0 sigma!
  1.0e0 1 sigma!
  0e0 last-ll f!
  0e0 current-ll f! ;

: .params ( -- )
  cr ." Final parameters:" cr
  n-clusters 0 do
    ."   cluster " i . ." : mean=" i mean@ f. ."  sigma=" i sigma@ f. cr
  loop ;

: .memberships ( -- )
  cr ." Sample memberships:" cr
  n-samples 0 do
    ."   x=" i sample@ f.
    n-clusters 0 do
      ."  p[" i . ." ]=" j i prob@ f.
    loop
    cr
  loop ;

: .histogram-demo ( -- )
  cr ." Histogram divergence demo (page 17 / eq. 1):" cr
  init-histograms
  ."   D(query, view-a) = " hist-query hist-view-a hist-divergence f. cr
  ."   D(query, view-b) = " hist-query hist-view-b hist-divergence f. cr
  ."   nearest view index = " nearest-view . cr ;

: run-em ( -- iterations )
  init-params
  max-iterations 0 do
    current-ll f@ last-ll f!
    expectation-step
    maximization-step
    log-likelihood current-ll f!
    converged? if i 1+ unloop exit then
  loop
  max-iterations ;

: main ( -- )
  cr ." Running EM clustering demo in gforth" cr
  .histogram-demo
  cr ." EM clustering demo:" cr
  run-em
  ." Iterations: " . cr
  ." Log-likelihood: " current-ll f@ f. cr
  .params
  .memberships
  bye ;

main
