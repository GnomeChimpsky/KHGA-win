# Frame rate and game timing (M8)

The goal of M8: no gameplay change between 60 fps and the highest frame rate the port supports.

## How the engine keeps time

- The game advances once per drawn frame by the real time the frame took, like retail. There is no
  frame cap unless vsync or `[KHGWin] FrameRateLimit` sets one.
- `ULevel::Tick` clamps each actor tick to 1 ms or more (and 0.4 s or less). Above 1000 fps the game
  would run ahead of real time. The desktop main loop (`FKHGFramePacer`, `Unreal/Src/KHGTiming.cpp`)
  waits until at least 1 ms has passed, so 1000 fps is the highest supported rate.
- Key and mouse axes are scaled by `20/DeltaTime` in `UInput::ReadInput` and turned back into
  movement with `DeltaTime` in script, so they don't depend on frame rate. The one exception is the
  retail mouse smoothing in `PlayerPawn.PlayerInput` (`SmoothTime = FMin(0.2, 3*DeltaTime)`), which
  blends less at high frame rates. With a fast mouse the total turn is the same within a few percent.
- `Timer()` fires at most once per tick, even if several periods have passed. That only matters below
  the timer rate, not at high frame rates.
- Physics (walking, falling, swimming) integrates per tick. Jump height drifts by about 2.5% between
  60 and 500+ Hz (see the results below). Walking, running and strafing stay within 0.5%.

## Retail script that counts frames

Found by reading every `Tick()` in the retail `Klingons.u` and `Engine.u` (the script source is stored
in the packages). These functions change something by a fixed step every frame, or add one frame's
`DeltaTime` only every 0.05 s, so they run faster or slower with the frame rate.

| Class (Tick owner) | What it does per frame | Effect at high fps | Maps |
|---|---|---|---|
| HoloBox (states GoingVis2, GoingInvisible) | adds one frame's DeltaTime/TimeIn to ScaleGlow every 0.05 s | fades much slower; the fade out keeps the player's controls locked (5.9 s at 60 fps, 49 s at 500 fps) | M02, M20A, M20B, M20C |
| PushActor (PushFrequency 0) | `Velocity += PushSpeed` on everything inside it | pushes harder | M02, M04A, M07A, M07B, M08, M15, M20A, M20B |
| ParticleAttractor | adds the pull velocity and spin every frame | stronger pull, faster spin | (weapon effect) |
| KlingonPawn, AttackDroid | AmbientGlow and LightBrightness -5; SpecialDeath 3 spin | hit glow fades faster, death spin faster | all |
| KlingonCarcass | spin, LightBrightness -2, BloodSpawnRate +0.0003 | faster spin and fades, shorter blood spurts | all |
| BatlethProjectile, DaktaghProjectile | `SetRotation(Rotation + RotationRate)` | thrown weapons spin faster | all |
| Destructable, KlingonAmmo | damage smoke if FRand() > 0.7 | more smoke | many |
| KlingonProjectiles, RocketProjectile | trail if FRand() < TrailPercentage | more trail particles | all |
| HotSteam, WhiteSteam1, SmokeBreath | one particle per frame; velocity x0.75 or x0.5 per frame | denser, shorter plumes | see list |
| LightningPulse | next texture frame every frame | faster animation | |

Timing-correct examples, for comparison: DroidCarcass, HumanGibs and SentryTopCarcass spawn smoke on an
accumulated time; VideoMonitor and SpinClaw animate on accumulated time; Humanoids fades by `delta/2`.

## The fix

Above 60 fps, the `Tick()` of the classes in the table runs at 60 Hz with a 1/60 s DeltaTime, which is
exactly what it did at 60 fps (`KHGEventTick` in `Engine/Src/UnLevTic.cpp`). Only a Tick function owned
by one of these classes is affected; a subclass with its own Tick (Lethian, RPBeast...) runs every frame.
Physics, AI state code, timers, animation and every other Tick still run every frame.

`[KHGWin] FrameRateFixes`:
- `0`: off, retail behaviour
- `1`: gameplay only (HoloBox, PushActor, ParticleAttractor)
- `2`: gameplay and the visual effects (default)

## Settings

```
[KHGWin]
FrameRateLimit=0        ; fps cap, 0 = none (vsync also caps)
OriginalTiming=False    ; True: fixed 1/OriginalTimingRate s steps, one per frame, paced to real time
OriginalTimingRate=60
FrameRateFixes=2
```

Per run: `-khgframecap=N`, `-khgoriginaltiming=0|1|N`, `-khgframeratefixes=0|1|2`.

## The timing test

`KHGWin.exe -khgtiming[=dir]` (`builds\tools\TimingTest.bat`) runs the same scripted input at fixed
60, 144, 240, 500 and 1000 Hz ticks, plus 500 Hz with the fixes off, and writes
`Timing-<map>-<arch>.html` and `.csv`. Every run starts from a fresh map load with `rand()` seeded.

- Player tests (the map's other pawns removed, player can't be hurt): standing and running jump,
  run, walk, back, strafe, crouch, turn and look keys, mouse at 1000/125/60 Hz event rates, every
  HoloBox (fade in, fade out, controls lock), every PushActor, and every retail weapon (all ten are
  given through the normal pickup code) with fire and alt fire held for 3 s, each from a fresh load.
- World test: the map left running for 20 s with the player standing still: Timer() calls per class,
  actors spawned per class, other pawns alive and their health, mover key changes.
- Main loop pacing: 3 s of real time at frame caps 60/144/240/500/none and original timing, giving the
  frame rate reached and game time per real second.

Options: `-khgmap=X.unr`, `-khgmaps=A.unr,B.unr`, `-khgrates=60,240,500r` (`r` = fixes off),
`-khgworld=N`, `-khgtests=move,turn,holo,push,weapons,world,pacing`, `-khgnodraw` (no drawing, for slow
renderers; marks the player's zone as seen so bStasis actors near the player keep ticking),
`-khgtimingdebug`.

## Results

M02, 64-bit. The cloud test machine (`-khgnodraw`, software Vulkan) and a real PC (M02 and M04A, 64-bit,
drawing every frame, 2026-11-04) gave the same values.

| Test | 60 Hz | 500 Hz | 1000 Hz | 500 Hz, fixes off |
|---|---|---|---|---|
| HoloBox fade out (controls locked) | 5.9 s | 5.9 s | 5.9 s | 49.05 s |
| HoloBox fade in | 8.05 s | 8.05 s | 8.05 s | 52.05 s |
| PushActor peak speed | ~1050 | ~1050 | ~1050 | ~2500 |
| Grenade launcher effect actors | ~200-250 | ~200-250 | ~200-250 | 958 |
| Run, walk, back, strafe, crouch | | within 0.5% | within 0.5% | |
| Turn key (3 s) | 23040 | 23000 | 23000 | |
| Weapons: ammo used, projectiles | | same | same | |

Differences that are left, all from the retail engine and script, not from the frame-counting script:
- Jump height 60.8 at 60 Hz, 59.2 at 500 and 1000 Hz (-2.6%): the per-tick falling integration.
- Look key 3840 vs 3750: rotation is stored as integers, so a smaller step per tick loses more to
  truncation.
- Mouse at 1000 Hz events varies by about 3%, and at 125 Hz events it is 5% higher at 60 fps only:
  the retail mouse smoothing (`SmoothTime`), see above.
- World test: one Tarchop loses 3 health at 60-240 Hz and none at 500 Hz and above, every run (it is
  the only change in the 20 s). Not traced to the end; the most likely cause is a landing that falls
  just on the other side of the retail fall damage threshold, like the jump height drift.
- M04A pusher 3 (the one that throws the player furthest) moves the player 671 units at 60 Hz, 508 at
  120 Hz, 582 at 144 Hz and 503-524 at 240-1000 Hz (1039 at 500 Hz with the fixes off). At 120 Hz the
  pushes land at exactly the 60 Hz times, so the difference comes from the per-tick walking and
  collision, not from the push. M04A's other four pushers and M02's stay within 5%.
- Effect density with `FrameRateFixes=2` stays within about 25% of 60 Hz (M04A: 352-449 actors spawned in
  20 s against 361 at 60 Hz; 3477 at 500 Hz with the fixes off). Effects whose Tick is on the list then
  fire on a 1/60 s accumulator, which lines up differently with 144 Hz frames.
- Grenade launcher alt fire: the projectile count jumps between runs (3, 12 or 39), because the
  grenades bounce off the scenery differently.
- DisruptorPistol ammo (10-12) and rocket projectiles (4-5) in 3 s: whether the last shot falls just
  inside or outside the 3 s.
