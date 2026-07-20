\ Expectation-Maximization clustering demo for gforth.
\ This is a small 1D Gaussian mixture example intended as a
\ runnable bridge from the AIBO EM appendix into real Forth code.
\ Run on this machine with:
\   XDG_CACHE_HOME=/tmp gforth-fast --no-0rc scripts/aibo-em.fs

decimal

12 constant n-samples
2 constant n-clusters
50 constant max-iterations

2e0 pi f* fsqrt fconstant SQRT-2PI
1e-6 fconstant SIGMA-FLOOR
1e-6 fconstant LL-THRESHOLD

create samples
  1.0e0 f, 1.2e0 f, 0.8e0 f, 1.1e0 f, 0.9e0 f, 1.3e0 f,
  8.8e0 f, 9.1e0 f, 9.4e0 f, 8.9e0 f, 9.2e0 f, 9.0e0 f,

create means n-clusters floats allot
create sigmas n-clusters floats allot
create probs n-samples n-clusters * floats allot

fvariable last-ll
fvariable current-ll

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

: gaussian-pdf ( i k -- r )
  locals| k i |
  i sample@ k mean@ f-
  fdup f*
  2e0 k sigma@ fdup f* f* f/
  fnegate fexp
  SQRT-2PI k sigma@ f* f/ ;

: sample-likelihood ( i -- r )
  0e0
  n-clusters 0 do
    over i gaussian-pdf f+
  loop
  nip ;

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
  run-em
  ." Iterations: " . cr
  ." Log-likelihood: " current-ll f@ f. cr
  .params
  .memberships
  bye ;

main
