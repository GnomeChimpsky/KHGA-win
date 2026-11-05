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

`[NOpenALDrv.NOpenALAudioSubsystem] MusicMode` (desktop, `KHG_CDMUSIC`) picks the music per level:
0 (default) = the level's CdTrack when `FindCDTrack` finds a file for it, otherwise its Song, never
both; 1 = CD tracks only; 2 = songs only; 3 = Galaxy's switches: `UseDigitalMusic` and `UseCDMusic`,
both may play. Retail maps have a CdTrack and no Song; the Collection Chamber maps have a Song
(trackN.umx) and no CdTrack, so mode 0 plays the right one on both. `KHGDesktopConfig.cpp` still copies
the two switches once from the retail Khg.ini (`[KHGWin] AudioDefaults=1`) for mode 3. Each level
logs `[KHG] Music: song ..., CD track ...` with what plays.

## Known differences

- 64 voices instead of 16; 44100 Hz mixing instead of 22050 Hz.
- Doppler: `DopplerFactor=0.01` is far weaker than Galaxy's `DopplerSpeed=6500` (not yet
  confirmed that Galaxy applies it).
- `UseHRTF` in the real context is passed with the wrong attribute name (`ALC_SOFT_HRTF`), so
  OpenAL Soft decides HRTF on its own (on for headphones).
- Default volumes (Music 63, Sound 127) are not taken from Khg.ini.

## Music from the game CD

Plain retail maps import no music packages: every mission's LevelInfo (and the menu, klingon.unr)
sets a CdTrack, and some MusicEvents change it, so the retail music is CD audio from disc 2
(tracks 2 to 8). Installs with the fan music pack have maps re-saved with `Song=trackN` modules
instead. `KHGDiscImport.cpp` rips the audio tracks of disc 2 (a drive through
`IOCTL_CDROM_RAW_READ`, or a .cue/.bin image) to `<CDMusicPath>\TrackNN.wav`, and copies its AVI
folder into System; when the install can't be written to, both go to the user folder
(`Music\`, `Movies\`), where NOpenALDrv and the FMV player look as well. Offered at startup when
`Brief0102.avi` or a file for track 2 is missing (`[KHGWin] DiscPrompt=False` turns it off), and
from the launcher's "Game CD..." button. `-khgdiscimport=<drive|.cue|.iso|auto>` copies without a
window and writes `KHGDiscImport.log` to the user folder.


Installing from disc 1 (same file): when KHGWin finds no install, the "Install Klingon Honor Guard"
window replaces the folder picker. It reads disc 1 (a drive, or a .cue/.bin or .iso image), unpacks
its InstallShield 5 `DATA1.CAB` with the vendored unshield library into the chosen folder (file
groups Maps, Textures and Sounds into their folders, everything else into System; "Editor Shared
DLLs", the editor's copies of Windows DLLs, is left out), copies the root `INTRO.AVI` into System
and saves the folder as GameDir. The result matches an unshield extraction byte for byte, and all
retail maps load from it. `-khginstall=<drive|.cue|.iso> [-khginstalldir=<folder>]` installs without
a window and writes `KHGInstall.log` to the folder.
