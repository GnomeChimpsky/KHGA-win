#include "SDL2/SDL.h"
#ifdef PLATFORM_WIN32
#include <windows.h>
#endif
#ifdef PLATFORM_PSVITA
#include <vitasdk.h>
#include <vitaGL.h>
#include <unistd.h>
#endif

#include "Engine.h"
#include "KHGSplash.h"   // [KHG] retail splash read from the user's Khg.exe
#include "UnRender.h"   // [KHG] URenderDevice::ReadPixels for -khgshots
#include <math.h>
#ifdef PLATFORM_WIN32
#include "KHGDesktopConfig.h"   // [KHG] first-run KHGWin.ini (desktop build)
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

extern CORE_API FGlobalPlatform GTempPlatform;
extern DLL_IMPORT UBOOL GTickDue;
extern "C" {HINSTANCE hInstance;}
extern "C" {char GCC_HIDDEN THIS_PACKAGE[64]="Launch";}

// FExecHook.
class FExecHook : public FExec
{
	UBOOL Exec( const char* Cmd, FOutputDevice* Out )
	{
		return 0;
	}
};

FExecHook GLocalHook;
DLL_EXPORT FExec* GThisExecHook = &GLocalHook;

#ifdef PLATFORM_PSVITA

//
// PSVita-specific globals.
//

#define MAX_PATH 1024
#define SYSTEM_PATH "data/unreal/System/"

// 200MB libc heap, 512K main thread stack, 16MB for loading game DLLs
// the rest goes to vitaGL
extern "C" { SceUInt32 sceUserMainThreadStackSize = 512 * 1024; }
extern "C" { unsigned int _pthread_stack_default_user = 512 * 1024; }
extern "C" { unsigned int _newlib_heap_size_user = 200 * 1024 * 1024; }
#define VGL_MEM_THRESHOLD ( 16 * 1024 * 1024 )

static char GRootPath[MAX_PATH] = "app0:/";

//
// PSVita-specific functions.
//

static bool FindRootPath( char* Out, int OutLen )
{
	static const char *Drives[] = { "uma0", "imc0", "ux0" };

	// check if an unreal folder exists on one of the drives
	// default to the last one (ux0)
	for ( unsigned int i = 0; i < sizeof(Drives) / sizeof(*Drives); ++i )
	{
		snprintf( Out, OutLen, "%s:/" SYSTEM_PATH, Drives[i] );
		SceUID Dir = sceIoDopen( Out );
		if ( Dir >= 0 )
		{
			sceIoDclose( Dir );
			return true;
		}
	}

	// not found
	return false;
}

static INT PowerCallback( INT NotifyID, INT NotifyCnt, INT PowerInfo, void* Common )
{
	if ( PowerInfo & ( SCE_POWER_CB_APP_RESUME | SCE_POWER_CB_APP_RESUMING ) )
	{
		debugf( "PowerCallback: resuming..." );
		appHandleSuspendResume( false );
	}
	else if ( PowerInfo & ( SCE_POWER_CB_BUTTON_PS_PRESS | SCE_POWER_CB_APP_SUSPEND | SCE_POWER_CB_SYSTEM_SUSPEND ) )
	{
		debugf( "PowerCallback: suspending..." );
		appHandleSuspendResume( true );
	}

	return 0;
}

static INT CallbackThread( DWORD Argc, void* Argv )
{
	const INT CbID = sceKernelCreateCallback( "Power Callback", 0, PowerCallback, nullptr );
	scePowerRegisterCallback( CbID );
	while( true )
		sceKernelDelayThreadCB( 10000000 );
	return 0;
}

[[noreturn]] static void EarlyError( const char* Msg )
{
	fprintf( stderr, "FATAL ERROR: %s\n", Msg );
	SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Fatal Error", Msg, nullptr );
	sceKernelExitProcess( 0 );
	abort();
}

static void PlatformPreInit()
{
	sceTouchSetSamplingState( SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_STOP );
	scePowerSetArmClockFrequency( 444 );
	scePowerSetBusClockFrequency( 222 );
	scePowerSetGpuClockFrequency( 222 );
	scePowerSetGpuXbarClockFrequency( 166 );
	sceSysmoduleLoadModule( SCE_SYSMODULE_NET );

	if ( !FindRootPath( GRootPath, sizeof(GRootPath) ) )
		EarlyError( "Could not find Unreal directory" );

	if ( chdir( GRootPath ) < 0 )
		EarlyError( "Could not chdir to Unreal directory" );

	SceUID Th = sceKernelCreateThread( "CallbackThread", CallbackThread, 0x10000100, 0x10000, 0, 0, nullptr );
	if( Th >= 0 )
		sceKernelStartThread( Th, 0, nullptr );

	vglInitWithCustomThreshold( 0, 960, 544, VGL_MEM_THRESHOLD, 0, 0, 0, SCE_GXM_MULTISAMPLE_2X );
	vglSetSemanticBindingMode( VGL_MODE_POSTPONED );
}

#elif defined(PLATFORM_ANDROID)

#define MAX_PATH 1024
static char GAndroidRootPath[MAX_PATH] = "";
static volatile UBOOL GAndroidAppSuspended = false;
static Uint32 GAndroidIgnoreQuitUntilTicks = 0;
static INT GAndroidRunSerial = 0;

extern "C" void UE1AndroidSetAudioSuspended( int Suspended ) __attribute__((weak));

extern "C" int UE1AndroidShouldIgnoreEarlyQuit()
{
	if( !GAndroidIgnoreQuitUntilTicks )
		return 0;

	const Uint32 Now = SDL_GetTicks();
	return SDL_TICKS_PASSED( Now, GAndroidIgnoreQuitUntilTicks ) ? 0 : 1;
}

static void AndroidResetRuntimeStateForStart()
{
	GAndroidRunSerial++;
	GIsRequestingExit = 0;
	GIsCriticalError  = 0;
	GIsRunning        = 0;
	GIsGuarded        = 0;
	GAndroidAppSuspended = false;
	GAndroidIgnoreQuitUntilTicks = SDL_GetTicks() + 2500;

	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "SDL_main start #%d: runtime flags reset", GAndroidRunSerial );
}

static void AndroidFlushStaleStartupEvents()
{
	SDL_PumpEvents();
	SDL_FlushEvent( SDL_QUIT );
	SDL_FlushEvent( SDL_APP_TERMINATING );
	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "SDL_main start #%d: stale startup events flushed", GAndroidRunSerial );
}

static bool AndroidDirExists( const char* Path )
{
	struct stat St;
	return stat( Path, &St ) == 0 && S_ISDIR( St.st_mode );
}

[[noreturn]] static void EarlyError( const char* Msg )
{
	__android_log_print( ANDROID_LOG_ERROR, "UE1Android", "%s", Msg );
	SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Unreal Android", Msg, nullptr );
	abort();
}

static void AndroidSetAppSuspended( UBOOL bSuspended )
{
	if( GAndroidAppSuspended == bSuspended )
		return;

	GAndroidAppSuspended = bSuspended;

	if( bSuspended )
		appFlushConfigFiles();

	appHandleSuspendResume( bSuspended );

	if( UE1AndroidSetAudioSuspended )
		UE1AndroidSetAudioSuspended( bSuspended ? 1 : 0 );

	// Harmless for SDL audio backends and useful as an additional safety net.
	SDL_PauseAudio( bSuspended ? 1 : 0 );

	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "Android lifecycle audio/game %s", bSuspended ? "suspended" : "resumed" );
}

static int AndroidEventWatch( void* UserData, SDL_Event* Event )
{
	switch( Event->type )
	{
		case SDL_APP_WILLENTERBACKGROUND:
		case SDL_APP_DIDENTERBACKGROUND:
			__android_log_print( ANDROID_LOG_INFO, "UE1Android", "SDL app background event: %u", Event->type );
			AndroidSetAppSuspended( true );
			break;

		case SDL_APP_WILLENTERFOREGROUND:
		case SDL_APP_DIDENTERFOREGROUND:
			__android_log_print( ANDROID_LOG_INFO, "UE1Android", "SDL app foreground event: %u", Event->type );
			AndroidSetAppSuspended( false );
			break;

		case SDL_APP_TERMINATING:
		case SDL_APP_LOWMEMORY:
			__android_log_print( ANDROID_LOG_WARN, "UE1Android", "SDL app terminating/lowmemory event: %u", Event->type );
			appFlushConfigFiles();
			AndroidSetAppSuspended( true );
			break;

		case SDL_QUIT:
			if( UE1AndroidShouldIgnoreEarlyQuit() )
			{
				__android_log_print( ANDROID_LOG_WARN, "UE1Android", "Ignoring early SDL_QUIT during startup guard" );
				break;
			}
			__android_log_print( ANDROID_LOG_WARN, "UE1Android", "SDL_QUIT accepted" );
			appFlushConfigFiles();
			AndroidSetAppSuspended( true );
			break;

		default:
			break;
	}

	return 1;
}

static void PlatformPreInit()
{
	// Keep all Unreal runtime data inside the app-specific external files dir:
	// Android/data/com.khg.android/files/Unreal/
	appAndroidInitFileSystem();

	snprintf( GAndroidRootPath, sizeof(GAndroidRootPath), "%s", appAndroidUnrealRootDir() );
	GAndroidRootPath[sizeof(GAndroidRootPath) - 1] = 0;

	const char* SystemPath = appAndroidUnrealSystemDir();

	if( !AndroidDirExists( SystemPath ) )
		EarlyError( "Unreal System directory could not be created." );

	setenv( "HOME", GAndroidRootPath, 1 );
	setenv( "UE1_ANDROID_ROOT", GAndroidRootPath, 1 );

	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "Android root: %s", GAndroidRootPath );
	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "Android system: %s", SystemPath );

	if( chdir( SystemPath ) < 0 )
	{
		char Err[MAX_PATH + 128];
		snprintf( Err, sizeof(Err), "Could not chdir to %s: errno=%d", SystemPath, errno );
		EarlyError( Err );
	}

	SDL_AddEventWatch( AndroidEventWatch, NULL );
}

#else

void PlatformPreInit()
{

}

#endif





























































































































































































































































































































































































































































































//
// Handle an error.
//
void HandleError()
{
	GIsGuarded=0;
	GIsCriticalError=1;
	debugf( NAME_Exit, "Shutting down after catching exception" );
	GObj.ShutdownAfterError();
	debugf( NAME_Exit, "Exiting due to exception" );
	GErrorHist[ARRAY_COUNT(GErrorHist)-1]=0;
	SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, LocalizeError("Critical"), GErrorHist, SDL_GetKeyboardFocus() );
}

//
// Initialize.
//
#ifdef PLATFORM_WIN32
// [KHG] Input record/replay, in NSDLViewport.cpp.
extern INT GKHGDemoMode;
extern DWORD GKHGDemoFrame, GKHGDemoEnd;
UBOOL KHGDemoOpen( const char* RecordFile, const char* ReplayFile );
void KHGDemoClose();
#endif

UEngine* InitEngine()
{
	guard(InitEngine);

	// Platform init.
	appInit();
#ifdef PLATFORM_WIN32
	KHGDesktopLogConfigStatus();   // [KHG] report how KHGWin.ini was set up (KHGDesktopConfig.cpp)
	KHGDesktopApplyUserPaths();    // [KHG] saves in the user folder unless the ini names its own Save folder
	{
		// [KHG] -khgrecord=<file> / -khgreplay=<file>: input record and replay (NSDLViewport.cpp). Seed rand()
		// before the first map's BeginPlay so the recording and every replay start from the same state.
		char Record[256] = "", Replay[256] = "";
		Parse( appCmdLine(), "KHGRECORD=", Record, ARRAY_COUNT(Record) );
		Parse( appCmdLine(), "KHGREPLAY=", Replay, ARRAY_COUNT(Replay) );
		if( *Record || *Replay )
		{
			if( KHGDemoOpen( Record, Replay ) )
			{
				srand( 1 );
				debugf( NAME_Log, "[KHG] Demo: %s %s", *Record ? "recording to" : "replaying", *Record ? Record : Replay );
			}
			else
				debugf( NAME_Warning, "[KHG] Demo: could not open %s", *Record ? Record : Replay );
		}
	}
#endif
	GDynMem.Init( 65536 );

	// Init subsystems.
	GSceneMem.Init( 32768 );

	// First-run menu.
	UBOOL FirstRun=0;
	GetConfigBool( "FirstRun", "FirstRun", FirstRun );

	// Create the global engine object.
	UClass* EngineClass;
	if( !GIsEditor )
	{
		// Create game engine.
		EngineClass = GObj.LoadClass( UGameEngine::StaticClass, NULL, "ini:Engine.Engine.GameEngine", NULL, LOAD_NoFail | LOAD_KeepImports, NULL );
	}
	else if( ParseParam( appCmdLine(),"MAKE" ) )
	{
		// Create editor engine.
		EngineClass = GObj.LoadClass( UEngine::StaticClass, NULL, "ini:Engine.Engine.EditorEngine", NULL, LOAD_NoFail | LOAD_DisallowFiles | LOAD_KeepImports, NULL );
	}
	else
	{
		// Editor.
		EngineClass = GObj.LoadClass( UEngine::StaticClass, NULL, "ini:Engine.Engine.EditorEngine", NULL, LOAD_NoFail | LOAD_KeepImports, NULL );
	}

	// Init engine.
	UEngine* Engine = ConstructClassObject<UEngine>( EngineClass );
	Engine->Init();

	return Engine;

	unguard;
}

// [KHG] Intro movie + MicroProse splash before the main menu, on every SDL platform (Android and the Windows desktop build).
// UNREAL_ANDROID_INTRO_FMV_V1: play the user's own INTRO.AVI once before the main menu via the native playavi handler (the original used Windows VFW, broken on modern Windows); skippable, no-op if absent.
extern "C" void UE1FMVBlackout( void );   // defined in NSDLDrv/Src/FMVPlayer.cpp
static void KHGPlayIntroFMVOnce( UEngine* Engine )
{
	static UBOOL bPlayed = 0;
	if( bPlayed )
		return;
	bPlayed = 1;
	if( !Engine || !Engine->Client || !Engine->Client->Viewports.Num() )
		return;
	UViewport* Viewport = Engine->Client->Viewports(0);
	if( !Viewport )
		return;
	// Black out first (via both UE1FMVBlackout and the render device, since the former's SDL_GL swap is a no-op under Vulkan) so the lingering 3D frame isn't shown while avformat opens the large intro file; playavi no-ops if the file is absent, so no existence check is needed.
	UE1FMVBlackout();
	if( Viewport->RenDev )
	{
		for( INT i = 0; i < 2; ++i )   // both swapchain images
			if( Viewport->Lock( FPlane(0.5,0.5,0.5,0), FPlane(0,0,0,0), FPlane(0,0,0,0), 1 /*LOCKR_ClearScreen*/, NULL, NULL ) )
				Viewport->Unlock( 1 );
	}
	debugf( NAME_Log, "[KHG] intro FMV: playing INTRO.AVI before main menu" );
	Viewport->Exec( "playavi intro.avi N None N None N None N None N" );
}

// [KHG] UNREAL_ANDROID_MICROPROSE_SPLASH_V1: after the intro movie and before the menu, show the retail MicroProse launcher splash for ~2.5s via the FMV still path, recreating the Windows hWndSplash the SDL launcher lacks.
// The bitmap is read at runtime from RT_BITMAP 102 of the user's own Khg.exe in the System folder (see KHGSplash.h); no copy of it ships with the port. Skipped if Khg.exe is absent.
extern "C" int  UE1FMVShowStill( const unsigned char* Bgra, int W, int H, double Seconds );

// Loads the MicroProse splash (RT_BITMAP 102) from the user's Khg.exe as top-down BGRA; the caller frees it.
static UBOOL KHGLoadRetailSplash( unsigned char** OutBGRA, INT* OutW, INT* OutH )
{
	// Retail installs spell the launcher Khg.exe; try the common casings for case-sensitive file systems.
	static const char* ExeNames[] = { "Khg.exe", "KHG.EXE", "khg.exe", "KHG.exe" };
	for( INT i = 0; i < ARRAY_COUNT(ExeNames); ++i )
	{
		char Path[1024];
		appSprintf( Path, "%s%s", appBaseDir(), ExeNames[i] );
		int W = 0, H = 0;
		if( KHGLoadPEBitmapBGRA( Path, KHG_SPLASH_RESOURCE_ID, OutBGRA, &W, &H ) )
		{
			*OutW = W;
			*OutH = H;
			return 1;
		}
	}
	return 0;
}
extern "C" int  UE1FMVAdvance( void );
extern "C" void UE1FMVClose( void );
static void KHGShowMicroProseSplashOnce( UEngine* Engine )
{
	static UBOOL bShown = 0;
	if( bShown )
		return;
	bShown = 1;
	if( !Engine || !Engine->Client || !Engine->Client->Viewports.Num() )
		return;
	UViewport* Viewport = Engine->Client->Viewports(0);
	if( !Viewport || !Viewport->RenDev )
		return;

	unsigned char* SplashBGRA = NULL;
	INT SplashW = 0, SplashH = 0;
	if( !KHGLoadRetailSplash( &SplashBGRA, &SplashW, &SplashH ) )
	{
		debugf( NAME_Log, "[KHG] MicroProse splash: Khg.exe (RT_BITMAP %i) not found in %s, skipping", KHG_SPLASH_RESOURCE_ID, appBaseDir() );
		return;
	}
	debugf( NAME_Log, "[KHG] MicroProse splash: showing after intro (%dx%d)", SplashW, SplashH );
	if( UE1FMVShowStill( SplashBGRA, SplashW, SplashH, 2.5 ) )
	{
		while( UE1FMVAdvance() )
		{
			Engine->Draw( Viewport );
			SDL_Delay( 2 );
		}
		UE1FMVClose();
	}
	free( SplashBGRA );
}

#ifdef PLATFORM_WIN32
// [KHG] Desktop: like the retail launcher's hWndSplash, show the MicroProse splash in its own centred, borderless window
// while the engine starts up, and close it once the engine (and the game window) is up. Skipped if Khg.exe is absent.
static SDL_Window*   GKHGSplashWindow   = NULL;
static SDL_Renderer* GKHGSplashRenderer = NULL;
static SDL_Texture*  GKHGSplashTexture  = NULL;
static void KHGOpenStartupSplash()
{
	unsigned char* BGRA = NULL;
	INT W = 0, H = 0;
	if( !KHGLoadRetailSplash( &BGRA, &W, &H ) )
		return;
	if( SDL_InitSubSystem( SDL_INIT_VIDEO ) == 0 )
	{
		GKHGSplashWindow = SDL_CreateWindow( "Klingon Honor Guard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H,
			SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR );
		if( GKHGSplashWindow )
			GKHGSplashRenderer = SDL_CreateRenderer( GKHGSplashWindow, -1, SDL_RENDERER_SOFTWARE );
		if( GKHGSplashRenderer )
			GKHGSplashTexture = SDL_CreateTexture( GKHGSplashRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, W, H );
		if( GKHGSplashTexture )
		{
			SDL_UpdateTexture( GKHGSplashTexture, NULL, BGRA, W * 4 );
			SDL_RenderCopy( GKHGSplashRenderer, GKHGSplashTexture, NULL, NULL );
			SDL_RenderPresent( GKHGSplashRenderer );
			SDL_PumpEvents();
		}
	}
	free( BGRA );
}
static void KHGCloseStartupSplash()
{
	if( !GKHGSplashWindow )
		return;
	debugf( NAME_Log, "[KHG] MicroProse splash: shown during startup" );
	if( GKHGSplashTexture )
		SDL_DestroyTexture( GKHGSplashTexture );
	if( GKHGSplashRenderer )
		SDL_DestroyRenderer( GKHGSplashRenderer );
	SDL_DestroyWindow( GKHGSplashWindow );
	GKHGSplashTexture  = NULL;
	GKHGSplashRenderer = NULL;
	GKHGSplashWindow   = NULL;
	SDL_QuitSubSystem( SDL_INIT_VIDEO );
}
#endif

#ifdef PLATFORM_WIN32
// [KHG] Desktop: KHGWin.exe -khgmaptest loads every map in ..\Maps in turn, runs each for a few seconds and quits,
// so one run shows whether each mission loads and which missing natives ([KHG] MissingNative) it hits. Each map's
// log lines follow its "[KHG] MapTest" line. Used for the M2 x86/x64 comparison; normal play never takes this path.
//   -khgmap=M07B.unr      test one map only
//   -khgticks=N           1/60 s game ticks per map (default 300)
//   -khgsavetest          after the ticks, save to slot 100+map number, load that save back and log every pawn
//                         again ("after load"), so a save/load round trip can be compared with "before save"
//   -khgloadtest          only load the saves in slots 100+map number (written by the other build) and log the pawns
//   -khgsavedir=DIR       folder for those saves instead of the game's Save folder
//   -khgloadsave=N        load save slot N once (e.g. a retail save), log the pawns, run the ticks, log them again
//   -khgloadsaves         the same for every Save<n>.usa in the save folder (slots 0-9 and 100-199)
static void KHGLogPawns( ULevel* Level, const char* Tag )
{
	// One line per pawn: the AI state, physics, health, zone and position it is in.
	for( INT a = 0; Level && a < Level->Num(); a++ )
	{
		APawn* P = Cast<APawn>( Level->Actors(a) );
		if( !P || P->bDeleteMe )
			continue;
		UState* State = P->GetMainFrame() ? P->GetMainFrame()->StateNode : NULL;
		debugf( NAME_Log, "[KHG] MapTest pawn%s %s state=%s phys=%i health=%i water=%i enemy=%s weapon=%s at=(%.0f,%.0f,%.0f)",
			Tag, P->GetName(), State ? State->GetName() : "None", (INT)P->Physics, P->Health,
			P->Region.Zone ? (INT)P->Region.Zone->bWaterZone : -1, P->Enemy ? P->Enemy->GetName() : "None",
			P->Weapon ? P->Weapon->GetName() : "None", P->Location.X, P->Location.Y, P->Location.Z );
	}
}
static void KHGRunMapTest( UEngine* Engine )
{
	guard(KHGRunMapTest);
	UGameEngine* GameEngine = Cast<UGameEngine>( Engine );
	if( !GameEngine )
		return;
	char Only[256] = "";
	Parse( appCmdLine(), "KHGMAP=", Only, ARRAY_COUNT(Only) );
	INT Ticks = 300;
	Parse( appCmdLine(), "KHGTICKS=", Ticks );
	UBOOL SaveTest = ParseParam( appCmdLine(), "KHGSAVETEST" );
	UBOOL LoadTest = ParseParam( appCmdLine(), "KHGLOADTEST" );
	char OldSavePath[ARRAY_COUNT(GSys->SavePath)], SaveDir[ARRAY_COUNT(GSys->SavePath)] = "";
	appStrcpy( OldSavePath, GSys->SavePath );
	if( Parse( appCmdLine(), "KHGSAVEDIR=", SaveDir, ARRAY_COUNT(SaveDir) ) )
		appStrcpy( GSys->SavePath, SaveDir );
	INT LoadSlot = -1;
	const UBOOL LoadAll = ParseParam( appCmdLine(), "KHGLOADSAVES" );
	if( LoadAll || ( Parse( appCmdLine(), "KHGLOADSAVE=", LoadSlot ) && LoadSlot >= 0 ) )
	{
		// One slot, or (-khgloadsaves) every Save<n>.usa in the save folder for slots 0-9 and 100-199.
		INT Tried = 0, Ok = 0;
		for( INT Slot = LoadAll ? 0 : LoadSlot; Slot <= ( LoadAll ? 199 : LoadSlot ) && !GIsRequestingExit; Slot++ )
		{
			char URL[64], File[256], Error[256] = "";
			if( LoadAll )
			{
				if( Slot == 10 )
					Slot = 100;
				appSprintf( File, "%s" PATH_SEPARATOR "Save%i.usa", PATH(GSys->SavePath), Slot );
				if( appFSize( File ) <= 0 )
					continue;
			}
			Tried++;
			appSprintf( URL, "?load=%i", Slot );
			debugf( NAME_Log, "[KHG] MapTest: loading save slot %i", Slot );
			srand( 1 );
			if( GameEngine->Browse( FURL( &GameEngine->LastURL, URL, TRAVEL_Absolute ), Error ) )
			{
				KHGLogPawns( GameEngine->GLevel, "-loaded" );
				for( INT t = 0; t < Ticks && !GIsRequestingExit; t++ )
					Engine->Tick( 1.0 / 60.0 );
				KHGLogPawns( GameEngine->GLevel, "" );
				debugf( NAME_Log, "[KHG] MapTest: save slot %i loaded (%s)", Slot, GameEngine->GLevel->GetFullName() );
				Ok++;
			}
			else debugf( NAME_Log, "[KHG] MapTest: save slot %i FAILED: %s", Slot, Error );
		}
		debugf( NAME_Log, "[KHG] MapTest: done, %i of %i saves loaded", Ok, Tried );
		appStrcpy( GSys->SavePath, OldSavePath );
		return;
	}
	TArray<FString> Maps = appFindFiles( "..\\Maps\\*.unr" );
	debugf( NAME_Log, "[KHG] MapTest: %i maps", Maps.Num() );
	INT Loaded = 0, Saved = 0, Restored = 0;
	for( INT i = 0; i < Maps.Num() && !GIsRequestingExit; i++ )
	{
		if( appStricmp( *Maps(i), "Entry.unr" )==0 )
			continue;
		if( *Only && appStricmp( *Maps(i), Only )!=0 )
			continue;
		char URL[256], Error[256] = "";
		debugf( NAME_Log, "[KHG] MapTest %i/%i: %s", i+1, Maps.Num(), *Maps(i) );
		INT Slot = 100 + i;
		if( LoadTest )
		{
			// Load the save the other build wrote for this map and log its pawns straight away.
			appSprintf( URL, "?load=%i", Slot );
			if( !GameEngine->Browse( FURL( &GameEngine->LastURL, URL, TRAVEL_Absolute ), Error ) )
			{
				debugf( NAME_Log, "[KHG] MapTest: %s load of slot %i FAILED: %s", *Maps(i), Slot, Error );
				continue;
			}
			Restored++;
			KHGLogPawns( GameEngine->GLevel, "-loaded" );
			continue;
		}
		appSprintf( URL, "..\\Maps\\%s", *Maps(i) );
		// Same random sequence and the same fixed 1/60 s ticks on every run, so a win64 and a win32 log of
		// one map can be compared line by line.
		srand( 1 );
		if( !GameEngine->Browse( FURL( &GameEngine->LastURL, URL, TRAVEL_Absolute ), Error ) )
		{
			debugf( NAME_Log, "[KHG] MapTest: %s FAILED: %s", *Maps(i), Error );
			continue;
		}
		Loaded++;
		for( INT t = 0; t < Ticks && !GIsRequestingExit; t++ )
			Engine->Tick( 1.0 / 60.0 );
		KHGLogPawns( GameEngine->GLevel, "" );
		if( SaveTest )
		{
			GameEngine->SaveGame( Slot );
			KHGLogPawns( GameEngine->GLevel, "-saved" );
			Saved++;
			appSprintf( URL, "?load=%i", Slot );
			if( !GameEngine->Browse( FURL( &GameEngine->LastURL, URL, TRAVEL_Absolute ), Error ) )
			{
				debugf( NAME_Log, "[KHG] MapTest: %s load of slot %i FAILED: %s", *Maps(i), Slot, Error );
				continue;
			}
			Restored++;
			KHGLogPawns( GameEngine->GLevel, "-loaded" );
		}
	}
	appStrcpy( GSys->SavePath, OldSavePath );
	debugf( NAME_Log, "[KHG] MapTest: done, %i loaded, %i saved, %i saves loaded", Loaded, Saved, Restored );
	unguard;
}
#endif

#ifdef PLATFORM_WIN32
// [KHG] Desktop: KHGWin.exe -khgshots=<dir> takes screenshots of fixed camera views in every map (M3 rendering
// parity), so they can be compared with the same views taken in the retail Khg.exe (builds\tools\RetailShots.bat).
//   -khgviews=<dir>   folder of view files, one <map>.txt per map (default <shots dir>\..\Views). A missing file
//                     is written first, with views chosen from the map: the player start in four directions, and
//                     a nearby path node looking at coronas, volumetric lights, translucent, modulated, masked,
//                     mirror and sky surfaces, plus a few spread through the level.
//   -khgmap=X.unr     one map only; -khgticks=N game ticks after loading (default 120)
//   -khgres=WxH       render size (default 1024x768, the retail Glide setting); not saved to KHGWin.ini
// Each view is a block of console commands under a "view <name>" line, run with the same "set" commands the
// retail game understands, then one frame is captured to <shots dir>\<map>_<name>.bmp.
static FString KHGStr( const char* Fmt, ... )
{
	char Buf[2048];
	va_list Args;
	va_start( Args, Fmt );
	vsnprintf( Buf, sizeof(Buf), Fmt, Args );
	va_end( Args );
	return FString( Buf );
}
struct FKHGShotView
{
	char Name[32];   // plain data: TArray copies its elements without constructing them
	FVector Eye;
	INT Pitch, Yaw;
};
static INT KHGRotUnits( DOUBLE Radians )
{
	return appRound( Radians * 32768.0 / PI ) & 65535;
}
static void KHGAddShotView( TArray<FKHGShotView>& Views, const char* Name, FVector Eye, FVector Target )
{
	FKHGShotView V;
	appStrncpy( V.Name, Name, ARRAY_COUNT(V.Name) );
	V.Eye = Eye;
	FVector D = Target - Eye;
	V.Yaw = KHGRotUnits( ::atan2( (DOUBLE)D.Y, (DOUBLE)D.X ) );
	V.Pitch = KHGRotUnits( ::atan2( (DOUBLE)D.Z, appSqrt( D.X*D.X + D.Y*D.Y ) ) );
	Views.AddItem( V );
}
// The path node that sees Target best: in clear view, in front of the surface if Normal is set, nearest to 400 units.
static UBOOL KHGFindShotEye( ULevel* Level, TArray<FVector>& Spots, FVector Target, const FVector* Normal, FVector& OutEye )
{
	FLOAT Best = 1e9;
	for( INT i = 0; i < Spots.Num(); i++ )
	{
		FVector Eye = Spots(i);
		FVector D = Target - Eye;
		FLOAT Dist = D.Size();
		if( Dist < 64.0f || Dist > 1500.0f )
			continue;
		if( Normal && ( (Eye - Target) | *Normal ) < 0.3f * Dist )
			continue;
		FCheckResult Hit;
		if( !Level->SingleLineCheck( Hit, NULL, Target, Eye, TRACE_VisBlocking ) )
			continue;
		FLOAT Score = Abs( Dist - 400.0f );
		if( Score < Best )
		{
			Best = Score;
			OutEye = Eye;
		}
	}
	return Best < 1e9;
}
static void KHGPlanShotViews( ULevel* Level, APlayerPawn* Player, TArray<FKHGShotView>& Views )
{
	const FLOAT EyeHeight = Player ? Player->BaseEyeHeight : 40.0f;
	TArray<FVector> Spots;
	APlayerStart* Start = NULL;
	for( INT a = 0; a < Level->Num(); a++ )
	{
		ANavigationPoint* Nav = Cast<ANavigationPoint>( Level->Actors(a) );
		if( !Nav )
			continue;
		Spots.AddItem( Nav->Location + FVector( 0, 0, EyeHeight ) );
		if( !Start )
			Start = Cast<APlayerStart>( Nav );
	}
	// The player start, looking ahead, right, back and left.
	FVector StartEye = Start ? Start->Location + FVector( 0, 0, EyeHeight ) : Player ? Player->Location + FVector( 0, 0, EyeHeight ) : FVector( 0, 0, 0 );
	INT StartYaw = Start ? Start->Rotation.Yaw : Player ? Player->Rotation.Yaw : 0;
	for( INT d = 0; d < 4; d++ )
	{
		FKHGShotView V;
		appSprintf( V.Name, "start%i", d );
		V.Eye = StartEye;
		V.Pitch = 0;
		V.Yaw = ( StartYaw + d * 16384 ) & 65535;
		Views.AddItem( V );
	}
	// Actor features.
	INT Coronas = 0, Volumes = 0, Translucent = 0;
	for( INT a = 0; a < Level->Num(); a++ )
	{
		AActor* A = Level->Actors(a);
		if( !A || A->bDeleteMe || A == Player )
			continue;
		const char* Kind = NULL;
		if( A->LightType != LT_None && A->bCorona && Coronas < 2 )
			Kind = "corona";
		else if( A->LightType != LT_None && A->VolumeRadius > 0 && Volumes < 2 )
			Kind = "volume";
		else if( !A->bHidden && A->DrawType != DT_None && A->Style == STY_Translucent && Translucent < 1 )
			Kind = "transactor";
		FVector Eye;
		if( !Kind || !KHGFindShotEye( Level, Spots, A->Location, NULL, Eye ) )
			continue;
		INT& Count = Kind[0]=='c' ? Coronas : Kind[0]=='v' ? Volumes : Translucent;
		KHGAddShotView( Views, *KHGStr( "%s%i", Kind, Count ), Eye, A->Location );
		Count++;
	}
	// BSP surfaces with special drawing, the biggest polygon of each kind first.
	struct { DWORD Flag; const char* Kind; INT Max; } Kinds[] =
	{
		{ PF_Translucent, "translucent", 2 }, { PF_Modulated, "modulated", 1 }, { PF_Masked, "masked", 1 },
		{ PF_Mirrored, "mirror", 1 }, { PF_FakeBackdrop, "sky", 1 },
	};
	UModel* Model = Level->Model;
	for( INT k = 0; k < ARRAY_COUNT(Kinds); k++ )
	{
		TArray<INT> Used;
		for( INT n = 0; n < Kinds[k].Max; n++ )
		{
			INT BestNode = INDEX_NONE;
			FLOAT BestArea = 0.0f;
			FVector BestCenter( 0, 0, 0 ), BestNormal( 0, 0, 1 );
			for( INT i = 0; i < Model->Nodes->Num(); i++ )
			{
				FBspNode& Node = Model->Nodes->Element(i);
				if( Node.NumVertices < 3 || Node.iSurf == INDEX_NONE )
					continue;
				FBspSurf& Surf = Model->Surfs->Element(Node.iSurf);
				INT UsedIndex;
				if( !(Surf.PolyFlags & Kinds[k].Flag) || (Surf.PolyFlags & (PF_Invisible|PF_Portal)) || Used.FindItem( Node.iSurf, UsedIndex ) )
					continue;
				FVector Center( 0, 0, 0 );
				FLOAT Area = 0.0f;
				FVector P0 = Model->Points->Element( Model->Verts->Element(Node.iVertPool).pVertex );
				for( INT v = 0; v < Node.NumVertices; v++ )
				{
					FVector P = Model->Points->Element( Model->Verts->Element(Node.iVertPool + v).pVertex );
					Center += P;
					if( v >= 2 )
						Area += ( ( Model->Points->Element( Model->Verts->Element(Node.iVertPool + v - 1).pVertex ) - P0 ) ^ ( P - P0 ) ).Size() * 0.5f;
				}
				Center /= Node.NumVertices;
				FVector Normal = Model->Vectors->Element( Surf.vNormal );
				FVector Eye;
				if( Area > BestArea && KHGFindShotEye( Level, Spots, Center + Normal * 2.0f, &Normal, Eye ) )
				{
					BestArea = Area;
					BestNode = i;
					BestCenter = Center;
					BestNormal = Normal;
				}
			}
			if( BestNode == INDEX_NONE )
				break;
			Used.AddItem( Model->Nodes->Element(BestNode).iSurf );
			FVector Eye;
			KHGFindShotEye( Level, Spots, BestCenter + BestNormal * 2.0f, &BestNormal, Eye );
			KHGAddShotView( Views, *KHGStr( "%s%i", Kinds[k].Kind, n ), Eye, BestCenter );
		}
	}
	// A few path nodes spread through the level, each looking the way with the most open space.
	for( INT n = 1; n <= 3 && Spots.Num() > 4; n++ )
	{
		FVector Eye = Spots( Spots.Num() * n / 4 );
		FLOAT BestDist = -1.0f;
		INT BestYaw = 0;
		for( INT d = 0; d < 8; d++ )
		{
			FLOAT Angle = d * PI / 4.0f;
			FVector End = Eye + FVector( appCos( Angle ), appSin( Angle ), 0 ) * 4000.0f;
			FCheckResult Hit;
			FLOAT Dist = Level->SingleLineCheck( Hit, NULL, End, Eye, TRACE_VisBlocking ) ? 4000.0f : Hit.Time * 4000.0f;
			if( Dist > BestDist )
			{
				BestDist = Dist;
				BestYaw = d * 8192;
			}
		}
		FKHGShotView V;
		appSprintf( V.Name, "spread%i", n );
		V.Eye = Eye;
		V.Pitch = 0;
		V.Yaw = BestYaw;
		Views.AddItem( V );
	}
}
static FString KHGShotViewCommands( const FKHGShotView& V, FLOAT EyeHeight )
{
	// The pawn's Location is its centre; the camera is EyeHeight above it.
	return KHGStr(
		"set LevelInfo bPlayersOnly True\r\n"
		"set PlayerPawn Physics PHYS_None\r\n"
		"set PlayerPawn Velocity (X=0.000000,Y=0.000000,Z=0.000000)\r\n"
		"set PlayerPawn Acceleration (X=0.000000,Y=0.000000,Z=0.000000)\r\n"
		"set PlayerPawn bBehindView False\r\n"
		"set PlayerPawn ViewTarget None\r\n"
		"set PlayerPawn Location (X=%f,Y=%f,Z=%f)\r\n"
		"set PlayerPawn ViewRotation (Pitch=%i,Yaw=%i,Roll=0)\r\n",
		V.Eye.X, V.Eye.Y, V.Eye.Z - EyeHeight, V.Pitch, V.Yaw );
}
static UBOOL KHGWriteShotBMP( const char* File, FColor* Buf, INT SizeX, INT SizeY )
{
	// The same 24-bit file the engine's SHOT command writes.
	FILE* F = appFopen( File, "wb" );
	if( !F )
		return 0;
	#pragma pack (push,1)
	struct { _WORD bfType; DWORD bfSize; _WORD bfReserved1, bfReserved2; DWORD bfOffBits; } FH;
	struct { DWORD biSize; INT biWidth, biHeight; _WORD biPlanes, biBitCount; DWORD biCompression, biSizeImage; INT biXPelsPerMeter, biYPelsPerMeter; DWORD biClrUsed, biClrImportant; } IH;
	#pragma pack (pop)
	FH.bfType = 'B' + 256*'M';
	FH.bfSize = sizeof(FH) + sizeof(IH) + 3 * SizeX * SizeY;
	FH.bfReserved1 = FH.bfReserved2 = 0;
	FH.bfOffBits = sizeof(FH) + sizeof(IH);
	appMemset( &IH, 0, sizeof(IH) );
	IH.biSize = sizeof(IH);
	IH.biWidth = SizeX;
	IH.biHeight = SizeY;
	IH.biPlanes = 1;
	IH.biBitCount = 24;
	IH.biSizeImage = SizeX * SizeY * 3;
	appFwrite( &FH, sizeof(FH), 1, F );
	appFwrite( &IH, sizeof(IH), 1, F );
	for( INT i = SizeY-1; i >= 0; i-- )
		for( INT j = 0; j < SizeX; j++ )
			appFwrite( &Buf[i*SizeX+j], 3, 1, F );
	appFclose( F );
	return 1;
}
static void KHGMakeDirs( const char* Path )
{
	// Create each folder along the path, like mkdir -p.
	char Dir[512];
	appStrncpy( Dir, Path, ARRAY_COUNT(Dir) );
	for( char* P = Dir; *P; P++ )
		if( ( *P == '\\' || *P == '/' ) && P > Dir && P[-1] != ':' && P[-1] != '.' )
		{
			char C = *P;
			*P = 0;
			CreateDirectoryA( Dir, NULL );
			*P = C;
		}
	CreateDirectoryA( Dir, NULL );
}
static void KHGRunShots( UEngine* Engine )
{
	guard(KHGRunShots);
	UGameEngine* GameEngine = Cast<UGameEngine>( Engine );
	if( !GameEngine || !GameEngine->Client || !GameEngine->Client->Viewports.Num() )
		return;
	UViewport* Viewport = GameEngine->Client->Viewports(0);
	char ShotDir[256] = "", ViewDir[256] = "", Only[256] = "";
	Parse( appCmdLine(), "KHGSHOTS=", ShotDir, ARRAY_COUNT(ShotDir) );
	if( !Parse( appCmdLine(), "KHGVIEWS=", ViewDir, ARRAY_COUNT(ViewDir) ) )
		appSprintf( ViewDir, "%s\\..\\Views", ShotDir );
	Parse( appCmdLine(), "KHGMAP=", Only, ARRAY_COUNT(Only) );
	INT Ticks = 120, ViewTicks = 60;
	Parse( appCmdLine(), "KHGTICKS=", Ticks );
	Parse( appCmdLine(), "KHGVIEWTICKS=", ViewTicks );   // frames after each view's commands: actor lighting (the weapon) takes ~1 s to settle
	KHGMakeDirs( ShotDir );
	KHGMakeDirs( ViewDir );
	// Render at the retail comparison size (Glide at 1024x768 by default), in whatever window mode is set.
	char Res[64] = "1024x768", Cmd[128];
	Parse( appCmdLine(), "KHGRES=", Res, ARRAY_COUNT(Res) );
	appSprintf( Cmd, "SetRes %s nosave", Res );
	Viewport->Exec( Cmd, GSystem );
	Engine->Tick( 1.0 / 60.0 );
	TArray<FString> Maps = appFindFiles( "..\\Maps\\*.unr" );
	debugf( NAME_Log, "[KHG] Shots: %i maps, %ix%i, views in %s, shots in %s", Maps.Num(), Viewport->SizeX, Viewport->SizeY, ViewDir, ShotDir );
	INT Taken = 0, Failed = 0;
	for( INT i = 0; i < Maps.Num() && !GIsRequestingExit; i++ )
	{
		if( appStricmp( *Maps(i), "Entry.unr" )==0 || ( *Only && appStricmp( *Maps(i), Only )!=0 ) )
			continue;
		char MapName[256], URL[256], Error[256] = "";
		appStrcpy( MapName, *Maps(i) );
		if( appStrchr( MapName, '.' ) )
			*appStrchr( MapName, '.' ) = 0;
		appSprintf( URL, "..\\Maps\\%s", *Maps(i) );
		srand( 1 );
		if( !GameEngine->Browse( FURL( &GameEngine->LastURL, URL, TRAVEL_Absolute ), Error ) )
		{
			debugf( NAME_Log, "[KHG] Shots: %s FAILED to load: %s", MapName, Error );
			Failed++;
			continue;
		}
		for( INT t = 0; t < Ticks && !GIsRequestingExit; t++ )
			Engine->Tick( 1.0 / 60.0 );
		Viewport = GameEngine->Client->Viewports(0);
		APlayerPawn* Player = Viewport->Actor;
		const FLOAT EyeHeight = Player ? Player->BaseEyeHeight : 40.0f;

		// The views: read the map's view file, or plan the views and write it.
		char ViewFile[512];
		appSprintf( ViewFile, "%s\\%s.txt", ViewDir, MapName );
		FString Text;
		if( !appLoadFileToString( Text, ViewFile ) )
		{
			TArray<FKHGShotView> Views;
			KHGPlanShotViews( GameEngine->GLevel, Player, Views );
			Text = KHGStr( "; Screenshot views for %s.unr, written by KHGWin.exe -khgshots. Each \"view <name>\" line\r\n"
				"; starts one screenshot; the console commands under it place the camera (also run by RetailShots in Khg.exe).\r\n", MapName );
			for( INT v = 0; v < Views.Num(); v++ )
							{
				Text += KHGStr( "view %s\r\n", Views(v).Name );
				Text += KHGShotViewCommands( Views(v), EyeHeight );
			}
			FILE* F = appFopen( ViewFile, "wb" );
			if( F )
			{
				appFwrite( *Text, Text.Length(), 1, F );
				appFclose( F );
			}
			else
				debugf( NAME_Warning, "[KHG] Shots: could not write %s", ViewFile );
			debugf( NAME_Log, "[KHG] Shots: %s planned %i views", MapName, Views.Num() );
		}

		// Run each view's commands, let a few frames render, capture.
		const char* Data = *Text;
		char Line[512], View[128] = "";
		UBOOL More = 1;
		while( More && !GIsRequestingExit )
		{
			More = ParseLine( &Data, Line, ARRAY_COUNT(Line) );
			if( More && appStrnicmp( Line, "view ", 5 ) != 0 )
			{
				if( *View && Line[0] && Line[0] != ';' )
					Viewport->Exec( Line, GSystem );
				continue;
			}
			if( *View )
			{
				for( INT t = 0; t < ViewTicks; t++ )
					Engine->Tick( 1.0 / 60.0 );
				char File[512];
				appSprintf( File, "%s\\%s_%s.bmp", ShotDir, MapName, View );
				FMemMark Mark( GMem );
				FColor* Buf = new( GMem, Viewport->SizeX * Viewport->SizeY )FColor;
				Viewport->RenDev->ReadPixels( Buf );
				if( KHGWriteShotBMP( File, Buf, Viewport->SizeX, Viewport->SizeY ) )
				{
					Taken++;
					APlayerPawn* P = Viewport->Actor;
					FPointRegion R = P->XLevel->Model->PointRegion( P->Level, P->Location + FVector(0,0,P->EyeHeight) );
					debugf( NAME_Log, "[KHG] Shots: %s_%s at (%.0f,%.0f,%.0f) eye %.0f zone %s (eye point %s leaf %i) fog (%.2f,%.2f,%.2f) scale %.2f",
						MapName, View, P->Location.X, P->Location.Y, P->Location.Z, P->EyeHeight,
						P->Region.Zone ? P->Region.Zone->GetName() : "None", R.Zone ? R.Zone->GetName() : "None", R.iLeaf,
						P->FlashFog.X, P->FlashFog.Y, P->FlashFog.Z, P->FlashScale.X );
					// Visible actors close to the camera (a patrolling pawn walking into the view point fills the screen).
					for( INT a=0; a<P->XLevel->Num(); a++ )
					{
						AActor* A = P->XLevel->Actors(a);
						if( A && A!=P && A->DrawType!=DT_None && !A->bHidden && (A->Location-P->Location).Size()<150.f )
							debugf( NAME_Log, "[KHG] Shots:   near %s (%s) dist %.0f mesh %s",
								A->GetName(), A->GetClass()->GetName(), (A->Location-P->Location).Size(), A->Mesh ? A->Mesh->GetName() : "None" );
					}
				}
				else
				{
					Failed++;
					debugf( NAME_Warning, "[KHG] Shots: could not write %s", File );
				}
				Mark.Pop();
			}
			if( More )
				appStrncpy( View, Line + 5, ARRAY_COUNT(View) );
		}
	}
	debugf( NAME_Log, "[KHG] Shots: done, %i screenshots, %i failed", Taken, Failed );
	unguard;
}
#endif

//
// Unreal's main message loop.  All windows in Unreal receive messages
// somewhere below this function on the stack.
//
void MainLoop( UEngine* Engine )
{
	guard(MainLoop);

	GIsRunning = 1;
#ifdef PLATFORM_WIN32
	// [KHG] Desktop: retail order. The MicroProse splash was already shown while the engine loaded (KHGOpenStartupSplash)
	// and retail plays no movie at startup: INTRO.AVI is only offered in the video log menu. [KHGWin]
	// PlayIntroAtStartup=True in KHGWin.ini brings back the Android port's intro before the menu.
	UBOOL PlayIntroAtStartup = 0;
	GetConfigBool( "KHGWin", "PlayIntroAtStartup", PlayIntroAtStartup );
	// [KHG] M7 widescreen: [KHGWin] HorPlusFOV=True keeps the 4:3 vertical view on wider screens (UnCamera.h).
	// -khghorplus=1|0 overrides it for one run (WideShots.bat) without touching the ini.
	UBOOL HorPlus = 1;   // default on since 2026-11-03 (Galvatron); False = retail vert- view
	GetConfigBool( "KHGWin", "HorPlusFOV", HorPlus );
	INT HorPlusArg = -1;
	if( Parse( appCmdLine(), "KHGHORPLUS=", HorPlusArg ) )
		HorPlus = HorPlusArg != 0;
	GKHGHorPlus = HorPlus;
	// [KHGWin] WidescreenHUD=True (default): HUD panels unstretched at the screen sides, menus on a 4:3 canvas
	// (UnCanvas.cpp); False = retail stretched HUD. -khgwidehud=1|0 overrides it for one run.
	UBOOL WideHUD = 1;
	GetConfigBool( "KHGWin", "WidescreenHUD", WideHUD );
	INT WideHUDArg = -1;
	if( Parse( appCmdLine(), "KHGWIDEHUD=", WideHUDArg ) )
		WideHUD = WideHUDArg != 0;
	GKHGWideHUD = WideHUD;
	debugf( NAME_Log, "[KHG] Widescreen: HorPlusFOV=%s WidescreenHUD=%s", GKHGHorPlus ? "True" : "False", GKHGWideHUD ? "True" : "False" );
	if( PlayIntroAtStartup )
		KHGPlayIntroFMVOnce( Engine );
	char KhgMapArg[256];
	if( Parse( appCmdLine(), "KHGSHOTS=", KhgMapArg, ARRAY_COUNT(KhgMapArg) ) )
	{
		KHGRunShots( Engine );
		GIsRunning = 0;
		return;
	}
	if( ParseParam( appCmdLine(), "KHGMAPTEST" ) || ParseParam( appCmdLine(), "KHGSAVETEST" ) || ParseParam( appCmdLine(), "KHGLOADTEST" ) || ParseParam( appCmdLine(), "KHGLOADSAVES" )
	 || Parse( appCmdLine(), "KHGLOADSAVE=", KhgMapArg, ARRAY_COUNT(KhgMapArg) )
	 || Parse( appCmdLine(), "KHGMAP=", KhgMapArg, ARRAY_COUNT(KhgMapArg) ) )
	{
		KHGRunMapTest( Engine );
		GIsRunning = 0;
		return;
	}
	// [KHG] KHGWin.exe <map>.unr -khgexec=<file>: once the map is up, run the console commands in <file>
	// (e.g. a "set PlayerPawn Location ..." line to drop the player at a spot to look at), then play normally.
	char KhgExec[256];
	if( Parse( appCmdLine(), "KHGEXEC=", KhgExec, ARRAY_COUNT(KhgExec) ) )
	{
		UGameEngine* GameEngine = Cast<UGameEngine>( Engine );
		FString Text;
		if( !appLoadFileToString( Text, KhgExec ) )
			debugf( NAME_Warning, "[KHG] Exec: could not read %s", KhgExec );
		else if( GameEngine && GameEngine->Client && GameEngine->Client->Viewports.Num() )
		{
			UViewport* Viewport = GameEngine->Client->Viewports(0);
			for( INT t = 0; t < 10; t++ )
				Engine->Tick( 1.0 / 60.0 );
			const char* Data = *Text;
			char Line[512];
			while( ParseLine( &Data, Line, ARRAY_COUNT(Line) ) )
				if( Line[0] && Line[0] != ';' )
				{
					debugf( NAME_Log, "[KHG] Exec: %s", Line );
					Viewport->Exec( Line, GSystem );
				}
		}
	}
#else
	KHGPlayIntroFMVOnce( Engine );   // [KHG] intro FMV before the menu (UNREAL_ANDROID_INTRO_FMV_V1)
	KHGShowMicroProseSplashOnce( Engine );   // [KHG] MicroProse splash after the intro (UNREAL_ANDROID_MICROPROSE_SPLASH_V1)
#endif
	DOUBLE OldTime = appSeconds();
#ifdef PLATFORM_WIN32
	const UBOOL KHGDemoFast = ParseParam( appCmdLine(), "KHGFAST" );
#endif
	while( GIsRunning && !GIsRequestingExit )
	{
#if defined(PLATFORM_ANDROID)
		if( GAndroidAppSuspended )
		{
			SDL_PumpEvents();
			appSleep( 0.05 );
			OldTime = appSeconds();
			continue;
		}
#endif
		// Update the world.
		DOUBLE NewTime = appSeconds();
#ifdef PLATFORM_WIN32
		if( GKHGDemoMode )
		{
			// [KHG] Recording/replay: fixed 1/60 s steps, paced to real time so both play at normal speed
			// (-khgfast runs a replay as fast as it can). Sleep most of the wait, then spin the last 2 ms:
			// Windows sleeps are too coarse to hit 1/60 s steps evenly.
			if( GKHGDemoMode == 2 && GKHGDemoFrame >= GKHGDemoEnd )
				break;
			Engine->Tick( 1.0 / 60.0 );
			GKHGDemoFrame++;
			if( GKHGDemoMode == 1 || !KHGDemoFast )
			{
				const DOUBLE Next = OldTime + 1.0/60.0;
				if( Next - appSeconds() > 0.002 )
					appSleep( Next - appSeconds() - 0.002 );
				while( appSeconds() < Next )
					;
				OldTime = Max( Next, appSeconds() - 0.1 );
			}
			continue;
		}
#endif
		Engine->Tick( NewTime - OldTime );
		OldTime = NewTime;

		// Enforce optional maximum tick rate.
		INT MaxTickRate = Engine->GetMaxTickRate();
		if( MaxTickRate )
		{
			DOUBLE Delta = (1.0/MaxTickRate) - (appSeconds()-OldTime);
			if( Delta > 0.0 )
				appSleep( Delta );
		}
	}
#ifdef PLATFORM_WIN32
	if( GKHGDemoMode )
	{
		// [KHG] End of a recording or replay: log where everything ended up, for comparing the runs.
		UGameEngine* GameEngine = Cast<UGameEngine>( Engine );
		debugf( NAME_Log, "[KHG] Demo: %s ended at frame %u", GKHGDemoMode==1 ? "recording" : "replay", (unsigned)GKHGDemoFrame );
		if( GameEngine && GameEngine->GLevel )
		{
			debugf( NAME_Log, "[KHG] Demo: level %s", GameEngine->GLevel->GetFullName() );
			KHGLogPawns( GameEngine->GLevel, "-demoend" );
		}
		KHGDemoClose();
	}
#endif
	GIsRunning = 0;
	unguard;
}

//
// Exit the engine.
//
void ExitEngine( UEngine* Engine )
{
	guard(ExitEngine);

	GObj.Exit();
	GMem.Exit();
	GDynMem.Exit();
	GSceneMem.Exit();
	GCache.Exit(1);
	appDumpAllocs( &GTempPlatform );

	unguard;
}

#ifdef PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* InCmdLine, INT nCmdShow )
#elif defined(PLATFORM_ANDROID)
extern "C" __attribute__((visibility("default"))) int SDL_main( int argc, char** argv )
#else
int main( int argc, const char** argv )
#endif
{
#ifdef PLATFORM_WIN32
	hInstance = hInInstance;
#elif defined(PLATFORM_ANDROID)
	hInstance = NULL;
	AndroidResetRuntimeStateForStart();
	// SDLActivity looks up a C symbol named SDL_main with dlsym().
	// Keep Unreal's command-line helper using const char** internally.
	appSetCmdLine( argc, (const char**)argv );
	PlatformPreInit();
	AndroidFlushStaleStartupEvents();
#else
	hInstance = NULL;
	// Remember arguments since we don't have GetCommandLine().
	appSetCmdLine( argc, argv );
	PlatformPreInit();
#endif

	GIsStarted = 1;

	// Set package name.
	appStrcpy( THIS_PACKAGE, appPackage() );

	// Init mode.
	GIsServer = 1;
	GIsClient = !ParseParam(appCmdLine(),"SERVER") && !ParseParam(appCmdLine(),"MAKE");
	GIsEditor = ParseParam(appCmdLine(),"EDITOR") || ParseParam(appCmdLine(),"MAKE");

	// Init windowing.
#ifdef PLATFORM_ANDROID // UNREAL_ANDROID_SKIP_APPBASEDIR_CHDIR
	// PlatformPreInit already chdir()s to ANDROIDROOT/System.
	// Do not call appBaseDir() here: SDL_GetBasePath() may be null on Android.
#else
	appChdir( appBaseDir() );
#endif
#ifdef PLATFORM_WIN32
	// [KHG] Desktop build: create KHGWin.ini from the retail defaults before appInit() loads it.
	// [KHG] Ini, log and saves go to Saved Games\Klingon Honor Guard unless portable (KHGDesktopConfig.cpp).
	KHGDesktopChooseUserDir( appBaseDir(), appCmdLine() );
	KHGDesktopPrepareConfig( appBaseDir(), appPackage() );
#endif

	// Init log.
	// TODO: GLog
	GExecHook = GThisExecHook;

	// Begin.
#ifndef _DEBUG
	try
	{
#endif
			// Start main loop.
			GIsGuarded=1;
			GSystem = &GTempPlatform;
#ifdef PLATFORM_ANDROID
			__android_log_print( ANDROID_LOG_INFO, "UE1Android", "InitEngine begin" );
#endif
#ifdef PLATFORM_WIN32
			KHGOpenStartupSplash();   // [KHG] retail splash window while the engine loads
#endif
			UEngine* Engine = InitEngine();
#ifdef PLATFORM_WIN32
			KHGCloseStartupSplash();
			if( Engine && Engine->Client && Engine->Client->Viewports.Num() && Engine->Client->Viewports(0)->GetWindow() )
				SDL_RaiseWindow( (SDL_Window*)Engine->Client->Viewports(0)->GetWindow() );
#endif
#ifdef PLATFORM_ANDROID
			__android_log_print( ANDROID_LOG_INFO, "UE1Android", "InitEngine end: Engine=%p GIsRequestingExit=%d GIsCriticalError=%d", Engine, GIsRequestingExit, GIsCriticalError );
#endif
			if( !GIsRequestingExit )
			{
#ifdef PLATFORM_ANDROID
				__android_log_print( ANDROID_LOG_INFO, "UE1Android", "MainLoop begin" );
#endif
				MainLoop( Engine );
#ifdef PLATFORM_ANDROID
				__android_log_print( ANDROID_LOG_INFO, "UE1Android", "MainLoop end: GIsRequestingExit=%d", GIsRequestingExit );
#endif
			}
#ifdef PLATFORM_ANDROID
			else
			{
				__android_log_print( ANDROID_LOG_ERROR, "UE1Android", "MainLoop skipped because GIsRequestingExit=%d GIsCriticalError=%d", GIsRequestingExit, GIsCriticalError );
			}
#endif
			appFlushConfigFiles();
			ExitEngine( Engine );
#ifdef PLATFORM_ANDROID
			__android_log_print( ANDROID_LOG_INFO, "UE1Android", "ExitEngine returned" );
#endif
			GIsGuarded=0;
#ifndef _DEBUG
	}
	catch( ... )
	{
		// Crashed.
		try {HandleError();} catch( ... ) {}
	}
#endif

	// Shut down.
	GExecHook=NULL;
#ifdef PLATFORM_ANDROID
	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "SDL_main shutting down via appExit" );
#endif
	appExit();
	GIsStarted = 0;
#ifdef PLATFORM_ANDROID // UNREAL_ANDROID_HARD_EXIT_AFTER_MENU_QUIT
	// When Unreal is closed through the in-game Beenden/Exit command, SDL_main
	// returns while the Android process can stay warm. SDLActivity and UE1 then
	// keep stale Java/native state and the next launcher tap may immediately
	// fall back to the Android launcher. Task-manager close works because it
	// kills the process. Do the same after a clean engine shutdown, but only
	// after config flushing, ExitEngine() and appExit() have completed.
	__android_log_print( ANDROID_LOG_INFO, "UE1Android", "SDL_main finished after in-game quit; terminating Android process for clean next launch" );
	SDL_Quit();
	fflush( NULL );
	_exit( 0 );
#endif
	return 0;
}
