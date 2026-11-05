Klingon Honor Guard - Windows desktop port (KHGWin)
===================================================

This is an unofficial engine port. It contains no game data: you need your own
retail copy of Star Trek: Klingon Honor Guard installed.

Install
-------
1. Unzip the KHGWin folder anywhere, for example next to the game's folder
   or in your Documents. It doesn't need to be installed.
2. Run KHGWin.exe.

The first time, KHGWin looks for the game: in its own folder, then in the
folders around it (so a KHGWin folder inside or next to the game's folder is
found). If it can't find it, it asks you for the folder you installed the game
in (the one with the System, Maps and Textures folders) and remembers it, as
GameDir in KHGWin.ini. To choose another one later, start KHGWin.exe with
-pickgamedir, or use -gamedir=<folder> for one run.

You can also copy the files into the System folder of your retail install
(the folder that contains Khg.exe), as earlier builds were installed; nothing
in it replaces a retail file. Installs under Program Files work either way.

Requirements: Windows 10 or 11 and a GPU with a Vulkan driver.

Which version of the game: the retail CD, version 1.0 or patched to 1.1, and
installs prepared by others (such as the Collection Chamber release). Patch
1.1 is not needed; install it for mods made for 1.1 (KHG Restored) and the
MicroProse splash. No Indeo codec, no-CD exe, dgVoodoo or edited Khg.ini is
needed either.

Movies and music from the game CD
---------------------------------
The retail game plays its movies and its music from game disc 2, and its
installer copies neither. The first time KHGWin starts on an install without
them, it offers to copy them into the game folder once:

- the movies: disc 2's AVI folder goes into System (about 450 MB);
- the music: the CD audio tracks are read from the disc and saved as
  Music\Track02.wav and so on (about 320 MB; reading a disc takes a few
  minutes, depending on the drive).

Insert disc 2 (or mount its image) and click Copy, or click "Disc image..." to
use a .cue/.bin image (movies and music) or an .iso (movies only). Skip starts
the game without them, as the retail game runs without its disc; "Don't ask
again at startup" stops the question. The launcher's "Game CD..." button opens
the same window at any time. If the game folder can't be written to (Program
Files), the files go to Movies\ and Music\ in your Saved Games folder instead,
where KHGWin finds them too. KHGWin.log lists what was copied ("[KHG] Game
CD:"). Installs that already have the movies in System and the music (ripped
tracks, or the fan music pack) are not asked.

If it crashes
-------------
KHGWin saves a crash report in the Crashes folder next to your settings
(Saved Games\Klingon Honor Guard\Crashes) and tells you where it is:
KHGWin-<date>-<time>.txt (what happened, readable), .dmp (a crash dump for a
debugger) and .log (the game log up to the crash). Send all three along with
a bug report. KHGWin-version.txt says which build you have.

Launcher
--------
When you start KHGWin.exe, a settings window opens first, with the settings
the game's own menus can't change (display mode, any resolution, vsync,
widescreen, lighting, raw mouse, controller, audio device, music volume and
more). Hover over a setting for a short description. Play saves them to
KHGWin.ini and starts the game; Quit closes without starting it. Only the
settings you change are written. "Reset settings" puts everything back to
the defaults (the old file is kept as KHGWin.ini.bak; saves are kept).

Settings that only the launcher adds (all off or retail by default):

  [NSDLDrv.NSDLClient]
  DefaultDisplay=0           monitor the game opens on (0 = the main one)
  [Engine.PlayerPawn]
  MouseSensitivity=3.0       finer steps than the Options menu's 1-10
  [VulkanDrv.VulkanRenderDevice]
  Antialiasing=0             MSAA samples: 0 = off, 2, 4 or 8
  Anisotropy=8               anisotropic filtering: 1 = off, up to 16
  [KHGWin]
  FrameRateLimit=0           highest frame rate, 0 = no limit
  FOV=90                     field of view across a 4:3 view; 90 = retail.
                             The game resets its view to this value, and
                             mouse speed grows with it as in retail.

Untick "Show this window at startup" to start the game straight away. Hold
Shift while starting KHGWin.exe (or start it with -launcher) to bring the
window back; -nolauncher skips it for one start. The test tools (-khg...
switches) never show it.

Your settings and saves
-----------------------
KHGWin keeps everything it writes in your Saved Games folder:

  C:\Users\<you>\Saved Games\Klingon Honor Guard\
    KHGWin.ini   settings for this port
    KHGWin.log   engine log. Lines tagged "[KHG] MissingNative" list engine
                 functions the game uses that the port does not implement yet
    Save\        saved games (Save0.usa ... Save8.usa, like the retail game)
    Shot0000.bmp screenshots (F9 / "shot")

so the install folder is never written to (it can stay in Program Files),
except when you copy the movies and music from the game CD into it.

The first time it starts, KHGWin sets this folder up from your install:
- KHGWin.ini is copied from the System folder if an older KHGWin made one
  there; otherwise it is created from your retail Default.ini (or Khg.ini)
  with only the video/audio driver lines changed, so your retail key
  bindings and options carry over. Your Khg.ini is never touched.
- Your retail saves are copied from the install's Save folder (and from
  Windows' VirtualStore copy of it, where the retail game's saves end up
  when it is installed under Program Files), with their names from Khg.ini,
  so the load menu shows them. The originals stay where they are; saves
  you make in KHGWin from then on are separate from the retail game's.
KHGWin.log lists what was copied ("[KHG] Migration"). To redo it, delete
the Klingon Honor Guard folder in Saved Games.

Save compatibility (64-bit and 32-bit KHGWin, the retail game) is listed in
docs/Saves.md in the source.

Portable mode: put an empty file named portable.txt next to KHGWin.exe
(or start it with -portable) and KHGWin keeps its files next to KHGWin.exe
instead of in Saved Games. When KHGWin.exe is in the install's System folder
that means KHGWin.ini, KHGWin.log and screenshots in the System folder and
saves in the install's Save folder, shared with the retail game, the way the
retail game does it. When it runs from a folder of its own, everything
(including Save\ and Crashes\) goes in that folder, set up from the install
the same way as the Saved Games folder.
-userdir=<folder> uses another folder instead of Saved Games.

Movement is set to W/S (forward/back) and A/D (strafe) with mouse look,
and the controller layout below replaces the retail joystick bindings;
this is applied once to an existing KHGWin.ini too. Change keys in the
game's controls menu as usual. Delete KHGWin.ini to start fresh.

Mods
----
Mods made for the retail game (KHG Restored, for one) install the usual
way: run the mod's own installer, or copy its files into the System folder
and edit Khg.ini as its readme says. You can install the mod before or
after KHGWin. On every start KHGWin brings the keys such mods change in
Khg.ini over into KHGWin.ini: the game types (DefaultGame,
DefaultServerGame and the server menu's game list), ServerPackages, Paths,
the player model menus, and the mod's own settings sections, for example
[KHGRestored.KHGRestoreHelper]. Run the mod's uninstaller and it is turned
off in KHGWin too. Key binds, bots and video/audio settings are left alone,
and so is any of these keys you change in KHGWin.ini yourself, until Khg.ini
changes it again. KHGWin.log lists every key it copied ("[KHG] Mods").
To turn this off, set ModSync=False under [KHGWin] in KHGWin.ini.

Startup
-------
Like the retail game, the MicroProse splash shows while the game loads, then
the title screen. The intro movie is not played at startup (retail only offers
it in the video log menu). To play it before the menu every time, add this to
KHGWin.ini:

  [KHGWin]
  PlayIntroAtStartup=True

The title splash and legal screen are shown at 4:3 with black bars on a
wider screen, like at the retail game's 4:3 resolutions. A mouse click moves
on to the next one.

Widescreen
----------
On a screen wider than 4:3 the game uses Hor+: you see the same from top to
bottom as at 4:3 and more at the sides. The HUD panels keep their 4:3 shape and
sit in the screen corners, with the weapon bar centred, and the menus are shown
at 4:3 with black bars. Both can be switched back to the retail look (the view
angle spans the screen width, and the HUD and menus are stretched) in KHGWin.ini:

  [KHGWin]
  HorPlusFOV=False
  WidescreenHUD=False

or in the console: HorPlus and WideHUD toggle them (and save the setting);
HorPlus on / off and WideHUD on / off also work. Gameplay is not affected: the
game's own FOV value, zoom and mouse speed stay the same. 4:3 and narrower
screens look the same either way.

Frame rate
----------
Like retail, the game advances once per drawn frame, with no frame cap (vsync
caps it at the screen's refresh rate). Two retail limits are handled for fast
PCs:
- The engine never advances less than 1 ms per frame, so above 1000 fps the
  game would run ahead of real time. KHGWin waits for that 1 ms instead.
- Some of the game's scripts count frames instead of time. The holodeck box
  fade (M02, M20A-C) locks your controls until it ends, which takes about
  4 s at 60 fps and much longer at high frame rates, and pushers and effects
  run faster. Above 60 fps those scripts run at 60 Hz, as they did at 60 fps.
The settings, in KHGWin.ini:

  [KHGWin]
  FrameRateLimit=0        ; cap in fps, 0 = none
  OriginalTiming=False    ; True = fixed 1/60 s steps, one per frame, paced
  OriginalTimingRate=60   ;        to real time (the game slows down if the
                          ;        PC can't keep up)
  FrameRateFixes=2        ; 0 = off (retail), 1 = gameplay only (holodeck
                          ; boxes, pushers, particle attractor), 2 = also
                          ; the visual effects

docs/Timing.md in the source lists what was measured and which scripts are
affected. builds\tools\TimingTest.bat runs the measurements.

Movies
------
Movies play the way the retail game plays them: on a 640x480 screen (shown 4:3
in the window, with black bars on wider screens), with the Klingon console frame
built up and broken down around the briefings. Esc, Space, Enter or a mouse
click skips a movie; the console frame animations cannot be skipped, as in
retail. Movie sound goes to the default Windows audio device (WASAPI), like
the game sound. KHGWin.log has one "[KHG] FMV:" line per movie.

Music from the game CD
----------------------
The retail game's music is CD audio on game disc 2 (tracks 2 to 8): every
mission and the main menu start a track, and some levels change it with a
music event. The retail game plays them when UseCDMusic is on.
KHGWin plays these from ripped audio files instead of the disc ("Movies and
music from the game CD" above makes them). They live in the Music folder of
your install (next to System), named by track number:

  Music\Track02.ogg   Music\Track03.flac   Music\Track11.mp3   ...

.ogg, .flac, .mp3, .wav and .opus work; "Track 02", "02" or "02 - Title"
are also found. Like the CD, a track plays once from its start and is not
repeated, at the in-game music volume. Without the files the game has no
music, as the retail game has none without the CD. KHGWin.log says which file it plays
("[KHG] CD track"). Settings in KHGWin.ini:

  [NOpenALDrv.NOpenALAudioSubsystem]
  UseCDMusic=True            False = never play CD tracks
  UseDigitalMusic=True       False = never play the module music (.umx)
  CDMusicPath=..\Music       folder with the track files (relative to System)

Like the retail Galaxy audio, the level's module music plays only with
UseDigitalMusic and the CD track only with UseCDMusic. KHGWin takes both from
your retail Khg.ini the first time it runs (the retail game's "CD music" and
"digital music" choice), so it plays the same music as Khg.exe. Some installs
have a fan patch that turns the CD tracks into .umx files (track2.umx and so
on, Khg.ini with UseCDMusic=False); turning both on there plays the same piece
twice.

The console command "cdtrack 2" plays a track, like in retail.

Room reverb
-----------
Areas the level designers marked as reverb zones (caves, halls, corridors) echo
the way they do in the retail game: KHGWin rebuilds the retail Galaxy audio's
own echo (six echo taps per zone with feedback and treble damping, from the
zone's Delay, Gain, MasterGain and CutoffHz), on the sound effects only, not on
the music. KHGWin.log has a "[KHG] Reverb:" line whenever it changes.
UseReverb=False in [NOpenALDrv.NOpenALAudioSubsystem] turns it off. (Retail can
also use EAX 1.0 or Aureal A3D hardware with Use3dHardware=True; that needs a
1998 sound card and Windows 98, so the software echo is what retail sounds like
today.)

Keyboard, mouse and controller
------------------------------
Keys are read as Windows key codes, like the retail game, so your retail key
bindings mean the same keys (also on non-US keyboard layouts). The mouse is
read raw (no Windows pointer speed or acceleration). A controller works as soon
as it is plugged in, with a twin-stick layout:

  Left stick  move/strafe        Right stick  turn/look
  RT  fire          LT  alt fire       A  jump        B  duck
  X   grab          Y   reload         LB/RB  previous/next weapon
  Start  menu       Back  change HUD   D-pad  inventory (up uses the item)
  Left stick click  walk               Right stick click  centre view

Rebind controller buttons in the game's controls menu like keys (the menu
shows them as Joy1, Joy2, ...); stick bindings are JoyX/JoyY (left) and
JoyU/JoyV (right) in [Engine.Input]. Settings in KHGWin.ini:

  [NSDLDrv.NSDLClient]
  WindowsKeyCodes=True       False = physical key positions of a US keyboard
  RawMouseInput=True         False = move the cursor like the retail game, so
                             Windows pointer speed and acceleration apply
  UseJoystick=True           False = ignore controllers (the retail default)
  ScaleRUV=100.0             right stick speed
  DeadZoneXYZ=0.2            left stick dead zone (0-1)
  DeadZoneRUV=0.15           right stick dead zone (0-1)
  InvertV=False              True = push the right stick up to look down

Display
-------
Alt+Enter switches between a window and fullscreen. Settings in KHGWin.ini:

  [NSDLDrv.NSDLClient]
  StartupFullscreen=True     start fullscreen (False = in a window)
  ViewportX=1024             fullscreen resolution; any size works, also
  ViewportY=768              ones the in-game menu does not list
  BorderlessFullscreen=False False = switch the display to that resolution,
                             like the retail game; True = a borderless window
                             over the desktop, the picture scaled to fit with
                             its shape kept (black bars if needed)
  WindowedViewportX=0        window size; 0 = the fullscreen resolution,
  WindowedViewportY=0        shrunk to fit the desktop. The window can be
                             resized by dragging and remembers its size.

  [VulkanDrv.VulkanRenderDevice]
  UseVSync=True              wait for the monitor refresh like the retail
                             Glide driver; False = no frame cap
  LightMode=3                world lighting level: 3 = the retail Glide
                             level, 0 = the brighter OpenGL/Android level,
                             1 = darker
  MirrorReflection=0.25      reflection strength on shiny see-through floors
                             (1 = the OpenGL/Android strength)

In the console, "setres 1600x1200" changes the resolution and "vsync on" or
"vsync off" changes vsync.

Original game files are never modified. Khg.exe is only read, to show the
MicroProse splash screen.

Third-party licenses are in the KHGWin-Licenses folder.
