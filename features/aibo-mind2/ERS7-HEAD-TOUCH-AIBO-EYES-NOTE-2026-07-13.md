# ERS-7 Head Touch / AIBO Eyes Note

Date: 2026-07-13

## Working conclusion

The statement below is only partly supported by the preserved ERS-7 MIND 2
payload in this repo:

> On ERS-7, the head touch usually triggers a command or mode change, not a
> Navigator setting. The quickest way to stop it is to disable the
> touch-command behavior in the robot's own settings on the Memory Stick side,
> then restart AIBO. If you're only seeing this after petting its head, also
> check whether the head touch is mapped to "AIBO Eyes" or a camera/record
> action and turn that mapping off.

What is supported:

- The relevant behavior appears to live on the MIND 2 / Memory Stick side, not
  in any obvious `Navigator` setting preserved here.
- The MIND 2 web UI exposes AIBO Eyes and photo behavior controls.
- Remote control mode is separate from normal MIND 2 behavior.

What is not yet directly confirmed:

- A documented ERS-7 MIND 2 setting that explicitly says `head touch`,
  `touch-command`, or a direct head-touch-to-photo mapping.

## Preserved evidence

- Top-level MIND 2 web UI:
  [features/aibo-mind2/build/stick/OPEN-R/APP/PC/INDEX.HTM](./build/stick/OPEN-R/APP/PC/INDEX.HTM)
- AIBO Eyes mail setup:
  [features/aibo-mind2/build/stick/OPEN-R/APP/PC/AIBOMAIL/AMOWNER.FRM](./build/stick/OPEN-R/APP/PC/AIBOMAIL/AMOWNER.FRM)
- AIBO Eyes private mode setup:
  [features/aibo-mind2/build/stick/OPEN-R/APP/PC/AIBOMAIL/AMUSER.FRM](./build/stick/OPEN-R/APP/PC/AIBOMAIL/AMUSER.FRM)
- AIBO Eyes connection/status note:
  [features/aibo-mind2/build/stick/OPEN-R/APP/PC/AIBOMAIL/AMNWSTAT.FRM](./build/stick/OPEN-R/APP/PC/AIBOMAIL/AMNWSTAT.FRM)

Specific confirmed UI elements:

- `AIBO EYES function setup`
- `Sending pictures by Voice Command and in House Sitting Mode`
- `VOICE_SEND_COMMAND` options `PHOTO` and `PHOTOL`
- `Periodic photography (WATCH command)`

Specific confirmed status note:

- `When AIBO is in remote control mode, AIBO does not connect to e-mail
  servers.`

## Practical guidance for later

If head petting appears to trigger photo, mail, or AIBO Eyes behavior on an
ERS-7 running MIND 2, treat the first fix as a MIND 2 Memory Stick settings
issue rather than an Entertainment Player issue.

Suggested steps:

1. Back up the Memory Stick.
2. Open the MIND 2 web UI from the stick.
3. Go to `AIBO EYES` -> `Mail Setup`.
4. In the section for sending pictures by voice command / house sitting mode,
   set the destination to no message sending if possible.
5. Avoid active `PHOTO` / `PHOTOL` behavior unless intentionally wanted.
6. Go to `AIBO EYES` -> `Private Mode Setup`.
7. Check whether any `WATCH` periodic photography settings are active and
   neutralize them.
8. Save changes and fully restart AIBO.

## Safe wording

Preferred wording for future discussion:

> On ERS-7 MIND 2, if head petting is causing photo or AIBO Eyes behavior, the
> fix is likely on the MIND 2 Memory Stick side rather than in Entertainment
> Player itself. The preserved MIND 2 UI confirms AIBO Eyes, voice-photo, and
> house-sitting photo controls, but we have not yet confirmed a preserved
> screen that explicitly labels a head-touch mapping.
