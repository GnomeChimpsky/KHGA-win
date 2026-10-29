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

Original game files are never modified. Khg.exe is only read, to show the
MicroProse splash screen.

Third-party licenses are in the KHGWin-Licenses folder.
