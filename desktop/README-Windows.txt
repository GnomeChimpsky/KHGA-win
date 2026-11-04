Klingon Honor Guard - Windows desktop port (KHGWin)
===================================================

This is an unofficial engine port. It contains no game data: you need your own
retail copy of Star Trek: Klingon Honor Guard installed.

Install
-------
1. Copy everything in this folder into the System folder of your retail install
   (the folder that contains Khg.exe). Nothing in it replaces a retail file.
   Installs under Program Files work too.
2. Run KHGWin.exe.

Requirements: Windows 10 or 11 and a GPU with a Vulkan driver.

Your settings and saves
-----------------------
KHGWin keeps everything it writes in your Saved Games folder:

  C:\Users\<you>\Saved Games\Klingon Honor Guard\
    KHGWin.ini   settings for this port
    KHGWin.log   engine log. Lines tagged "[KHG] MissingNative" list engine
                 functions the game uses that the port does not implement yet
    Save\        saved games (Save0.usa ... Save8.usa, like the retail game)
    Shot0000.bmp screenshots (F9 / "shot")

so the install folder is never written to (it can stay in Program Files).

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
(or start it with -portable) and KHGWin keeps KHGWin.ini, KHGWin.log and
screenshots in the System folder and saves in the install's Save folder,
shared with the retail game, the way the retail game does it.
-userdir=<folder> uses another folder instead of Saved Games.

Movement is set to W/S (forward/back) and A/D (strafe) with mouse look,
and the controller layout below replaces the retail joystick bindings;
this is applied once to an existing KHGWin.ini too. Change keys in the
game's controls menu as usual. Delete KHGWin.ini to start fresh.

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
Ten levels have music events that start a track from the game CD, which the
retail game plays when UseCDMusic is on (track 2 in M07B, M12, M13, M15, M18C
and M19, track 3 in M06B, 4 in M20A, 6 in M07A, 11 in M09A).
KHGWin plays these from ripped audio files instead of the disc. Put them in
the Music folder of your install (next to System), named by track number:

  Music\Track02.ogg   Music\Track03.flac   Music\Track11.mp3   ...

.ogg, .flac, .mp3, .wav and .opus work; "Track 02", "02" or "02 - Title"
are also found. Like the CD, a track plays once from its start and is not
repeated, at the in-game music volume. Without the files those levels sound
as the retail game does without the CD. KHGWin.log says which file it plays
("[KHG] CD track"). Settings in KHGWin.ini:

  [NOpenALDrv.NOpenALAudioSubsystem]
  UseCDMusic=True            False = never play CD tracks
  CDMusicPath=..\Music       folder with the track files (relative to System)

The console command "cdtrack 2" plays a track, like in retail.

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
