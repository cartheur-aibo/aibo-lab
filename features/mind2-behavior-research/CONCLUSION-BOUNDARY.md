# Conclusion Boundary

This note defines what counts as enough evidence to make a meaningful first
conclusion in the current behavior-forensics phase.

## We Can Reach A First Conclusion If

At least one of these becomes true:

1. The identity-neutral transplant reliably transfers the distinctive behavior.
2. The identity-sensitive transplant reliably transfers the distinctive
   behavior while the identity-neutral transplant does not.
3. Neither transplant transfers the behavior across repeated trials, strongly
   suggesting the current candidate cluster is incomplete or context-bound.

## We Should Not Claim More Than The Evidence Supports

For example:

- one successful weird boot is not enough
- one failed boot is not enough
- one single shutdown anecdote is not enough

We want repeated observations under logged conditions.

## Minimum Standard

Before claiming a first conclusion, aim for:

- at least one neutral test on sacrificial media
- at least one sensitive test on sacrificial media
- clear observation notes for both
- comparison against the preserved specimen behavior

## After The First Conclusion

Once the neutral-versus-sensitive split is understood, the next boundary is:

- file-group reduction

That means shrinking the candidate set from the current transplant cluster down
to smaller subsets such as:

- `STTLOG` plus `AWAKING.CFG`
- `STTLOG` plus `IEG.CFG`
- `FVAR`/`GVAR` only
- app-state cluster without `PAT.LOG`
