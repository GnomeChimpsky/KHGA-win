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
