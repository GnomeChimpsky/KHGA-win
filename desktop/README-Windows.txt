Klingon Honor Guard - Windows desktop port (KHGWin)
===================================================

This is an unofficial engine port. It contains no game data: you need your own
retail copy of Star Trek: Klingon Honor Guard installed.

Install
-------
1. Copy everything in this folder into the System folder of your retail install
   (the folder that contains Khg.exe). Nothing in it replaces a retail file.
2. Run KHGWin.exe.

Requirements: Windows 10 or 11 and a GPU with a Vulkan driver.

Files it creates (all in the System folder)
-------------------------------------------
KHGWin.ini   Settings for this port. Created on first run from your retail
             Default.ini (or Khg.ini) with only the video/audio driver lines
             changed, so your retail key bindings and options carry over.
             Delete it to start fresh. Your Khg.ini is never touched.
             Movement is set to W/S (forward/back) and A/D (strafe) with
             mouse look, and the controller layout below replaces the
             retail joystick bindings; this is applied once to an existing
             KHGWin.ini too.
             Change keys in the game's controls menu as usual.
KHGWin.log   Engine log. Lines tagged "[KHG] MissingNative" list engine
             functions the game uses that the port does not implement yet.

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
