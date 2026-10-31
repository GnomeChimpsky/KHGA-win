# FMV / Cutscene System

*Klingon Honor Guard* plays its cutscenes — the intro, mission briefings, and in-game Klingon
"comm console" cutscenes — as AVI movies. On the original Windows release these were decoded by
Indeo 5 + DirectShow inside the game's Windows video code, which does not exist on Android (and
no longer works on modern Windows either). This port decodes the movies on-device with FFmpeg
and presents them through the render device.

## Decoder

The movies are **Indeo 5** (`IV50`) video with **MS-ADPCM** (`adpcm_ms`, 22050 Hz stereo) audio
in an AVI container. They are decoded by a **minimal, LGPL-only FFmpeg** build for armeabi-v7a:

- Components enabled: the AVI demuxer, the Indeo 5 decoder, the MS-ADPCM decoder, swscale, and
  the file protocol — nothing else.
- Built **without** `--enable-gpl` and **without** `--enable-nonfree`, so the engine takes on no
  GPL copyleft. Obligation: include the FFmpeg license text and attribution; the project ships
  the *decoder*, never the user's video.

Playback is **audio-master synced** (the audio clock drives frame scheduling, the standard
ffplay/ScummVM model). The player exposes the current decoded frame as a BGRA buffer, which the
engine wraps as a transient texture and draws through the render device — so presentation is
**device-agnostic** (the same path works under Vulkan and OpenGL ES) rather than tied to a GL
blit.

## The `playavi` handler

KHG's UnrealScript triggers movies with `ConsoleCommand("playavi <file>.avi <tokens...>")`. A
native `playavi` Exec handler in the SDL viewport parses the filename and the trailing flag
tokens, pauses the game, runs the decode/present loop, allows skipping (any button / touch /
key), then resumes. The flag tokens select playback mode, including whether a clip is a plain
fullscreen movie or part of the decorated comm-frame chain.

Fullscreen clips (the intro, ship cinematics, briefings on the plain path) are letterboxed to
their native aspect and centered. The intro movie and the MicroProse launcher splash are played
once at startup, before the menu, and are skippable (and no-op if the files are absent).

## Windows desktop build: the retail sequence (M4)

On the desktop build the `playavi` handler reproduces the retail PC game exactly. The retail
handler is `UWindowsViewport::PlayAVI` in **WinDrv.dll** (not Window.dll); it switches to
640x480x16, opens each clip with DirectShow (`IAMMultiMediaStream`) and DirectDraw-blits every
frame, centred at its native size, onto the primary surface, which keeps whatever was drawn
last. `UE1FMVPlayAVIRetail` (FMVPlayer.cpp) follows it step for step on a 640x480 canvas that
is presented 4:3 with black bars:

| Flag | Before the clip | The clip | After the clip |
|---|---|---|---|
| `Y` | if no console frame is up: `buffer.avi`, `buildup.avi` | colour-keyed | `breakdn.avi`, clear |
| `C` | if no console frame is up: `buffer.avi` only | plain copy | nothing |
| `N` | clear to black | plain copy | nothing |
| `T` | clear to black | colour-keyed | nothing |

- The flag is the first letter of its token; up to five file/flag pairs; `None` skips the file.
- *Colour-keyed*: the clip's first frame is not shown; its top-left pixel, compared as RGB565,
  is the colour key, and later frames are drawn only where they differ from it. Nothing is
  cleared between frames.
- `buffer.avi` (one second of black), `buildup.avi` and `Brief03a.avi` (the `C` clip before
  `Brief03bcd.avi`, which builds its own frame) cannot be skipped; everything else stops on
  Esc, Space, Enter or a mouse click. A skip ends only the current clip.
- Once the frame is up, every clip whose flag is not `C` is followed by `breakdn.avi` (retail
  never clears that state within one command).

### Indeo 5 transparency

The briefing clips (`Brief*.avi` except `Brief03a.avi`, and `kcs4-1.avi`) use Indeo 5
**transparency**: GOP flag 8, a fill colour of 00ff00, and after the colour planes of each
frame a run-length-coded mask (layout in `desktop/patches/ffmpeg-indeo5-transparency.patch`;
rows are XOR-coded against the row above). FFmpeg skips it, so the character's grey surround
was opaque. The desktop build patches FFmpeg to decode the mask and return it as
`AV_FRAME_DATA_SEI_UNREGISTERED` side data (id `KHG-IVI5-TRANSP`). Transparent pixels become
black, which is also the key colour the first (fully transparent) frame gives the briefings,
so the character appears inside the console frame's hexagon exactly where the clip says.
Cinematics played with `Y` (`f_ss`, `f_s2kv`, `victory`) have no mask; their key is black, so
the console frame shows through their dark areas, as in retail.

Android keeps the approach below (it uses prebuilt FFmpeg libraries without the patch).

## The decorated Klingon comm-frame (Android)

The in-game comm/briefing cutscenes are the interesting case. The character video itself is just
**a figure composited into a hexagonal window on a flat dark surround** — there is **no ornate
frame baked into the clip**. The decorated Klingon console frame (riveted bronze/red border,
rank/emblem corners, gold-edged central hex window) is a **separate video chain**, not in the
clip and not drawn by any of the original game's code in a way the port could reuse.

The chain is three movies driven by the `playavi` flags:

| Clip | Role |
|---|---|
| `buildup.avi` | assembles the ornate console frame |
| *(content clip)* | the character video composited into the frame's hex window |
| `breakdn.avi`   | tears the frame back down |

The port reproduces this by:

1. Playing `buildup.avi` and **retaining its final frame** as an overlay (the fully-assembled
   ornate frame with its central hexagonal window).
2. Compositing the content clip into the hex window, then
3. Playing `breakdn.avi`.

### Compositing approach

The central window in the assembled frame is an irregular flat-topped hexagon (widest below
center), bounded by a bright gold ring. Two early approaches were rejected:

- An analytic SDF hexagon mask — the window is irregular, so guessed constants never fit.
- Color-keying the content clip's dark surround — fails, because the character's black hair is
  as dark as the surround, so distance-keying punches holes in the character.

The working method **masks the window, not the character**: the overlay (buildup's last frame)
has its central black window flood-filled (bounded by the bright gold ring) and baked into the
overlay texture's **alpha channel** (0 = window, 255 = border), with a measured-hexagon polygon
fallback if the flood-fill result is implausible. Compositing is then a trivial
`mix(content, frame, frame.alpha)`. Because the frame and content are pre-aligned at the same
resolution, this reproduces the exact window shape with no guessing and preserves the character's
dark hair. The decorated frame composites correctly on-device.

## Known remaining items

- Audio sync/skip handling across the `buildup → content → breakdn` clip transitions can still
  be refined.
- Fine centering of the content inside the hex window is being re-verified.
