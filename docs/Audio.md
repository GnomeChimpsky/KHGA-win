# Audio

KHGWin plays sound through OpenAL Soft (`NOpenALDrv`), module music through libxmp and movie
sound through SDL2. This page lists what the retail Galaxy audio (`Galaxy.dll`, build 219)
does and how the port matches it.

## Retail Galaxy (from Galaxy.dll and the retail inis)

- Settings: `UseReverb=True`, `UseSpatial=False`, `Use3dHardware=False`, `UseSurround=False`,
  `OutputRate=22050Hz`, `EffectsChannels=16`, `DopplerSpeed=6500`, `AmbientFactor=0.6`.
- EAX 1.0 (reverb property sets) and Aureal A3D 1.0 are used only with `Use3dHardware=True`
  on DirectSound3D hardware. The EAX path picks an EAX 1.0 environment from the zone's echo.
- Music (`UGalaxyAudioSubsystem::Update`, 0x10604110): when a transition is done the song,
  section and CD track are taken from the PlayerPawn; the module plays only if
  `UseDigitalMusic` (property offset 0x40), the CD track only if `UseCDMusic` (0x3c).

## Zone reverb (desktop: `KHG_GALAXYREVERB`, `GalaxyReverb.cpp`)

`UpdateReverb` (0x10604576) fills a reverb block from the viewport actor's zone when
`UseReverb` (0x48) is on and the zone has `bReverbZone`, else zeros:
`MasterGain/255`, `CutoffHz` clamped to 0..44100, `Delay[i]*0.002` s, `Gain[i]/255`.
`SpeedOfSound` and `bRaytraceReverb` are not read. `glxSetSampleReverb2` (0x106090c0) turns the
echo off when MasterGain is 0, refuses a cutoff above the mix rate, and otherwise restarts it
with an empty delay line (0x10608ba0):

- ring buffer of 32768 16-bit mono samples;
- six taps at `-rate*delay` samples, gains in Q12 (`gain*4095`), summed and saturated;
- one-pole low-pass, `x = 1 - cos(2*pi*cutoff/rate)`, `a = sqrt(x*(x+2)) - x`, Q15;
- per sample (MMX loop at 0x1060e49d): `y = lowpass(taps)`, output `L+y`, `R+y`,
  ring input `(y + L + R) * MasterGain` (Q15).

Galaxy mixes the reverb-enabled effects before the echo and the music after it.
KHGWin mixes the effect sources on an OpenAL Soft loopback device (same speaker layout as the
real device; HRTF when the real device uses it), adds the echo in `PumpFxStream` and streams the
result to the real device (8 buffers of 256 frames, about 46 ms at 44100 Hz). Music and CD
sources stay on the real device. The log has `[KHG] Effects:` at startup and `[KHG] Reverb:`
on every change. Android keeps the EFX generic reverb.

## Music switches

`[NOpenALDrv.NOpenALAudioSubsystem] UseDigitalMusic` and `UseCDMusic` work like Galaxy's.
`KHGDesktopConfig.cpp` copies them once from the retail Khg.ini (else Default.ini) into
KHGWin.ini (`[KHGWin] AudioDefaults=1`).

## Known differences

- 64 voices instead of 16; 44100 Hz mixing instead of 22050 Hz.
- Doppler: `DopplerFactor=0.01` is far weaker than Galaxy's `DopplerSpeed=6500` (not yet
  confirmed that Galaxy applies it).
- `UseHRTF` in the real context is passed with the wrong attribute name (`ALC_SOFT_HRTF`), so
  OpenAL Soft decides HRTF on its own (on for headphones).
- Default volumes (Music 63, Sound 127) are not taken from Khg.ini.
