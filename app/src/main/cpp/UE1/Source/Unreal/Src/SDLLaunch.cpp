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
UEngine* InitEngine()
{
	guard(InitEngine);

	// Platform init.
	appInit();
#ifdef PLATFORM_WIN32
	KHGDesktopLogConfigStatus();   // [KHG] report how KHGWin.ini was set up (KHGDesktopConfig.cpp)
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
static void KHGRunMapTest( UEngine* Engine )
{
	guard(KHGRunMapTest);
	UGameEngine* GameEngine = Cast<UGameEngine>( Engine );
	if( !GameEngine )
		return;
	TArray<FString> Maps = appFindFiles( "..\\Maps\\*.unr" );
	debugf( NAME_Log, "[KHG] MapTest: %i maps", Maps.Num() );
	INT Loaded = 0;
	for( INT i = 0; i < Maps.Num() && !GIsRequestingExit; i++ )
	{
		if( appStricmp( *Maps(i), "Entry.unr" )==0 )
			continue;
		char URL[256], Error[256] = "";
		appSprintf( URL, "..\\Maps\\%s", *Maps(i) );
		debugf( NAME_Log, "[KHG] MapTest %i/%i: %s", i+1, Maps.Num(), *Maps(i) );
		if( !GameEngine->Browse( FURL( &GameEngine->LastURL, URL, TRAVEL_Absolute ), Error ) )
		{
			debugf( NAME_Log, "[KHG] MapTest: %s FAILED: %s", *Maps(i), Error );
			continue;
		}
		Loaded++;
		DOUBLE Start = appSeconds(), OldTime = Start;
		while( !GIsRequestingExit && appSeconds() - Start < 5.0 )
		{
			DOUBLE NewTime = appSeconds();
			Engine->Tick( NewTime - OldTime );
			OldTime = NewTime;
			appSleep( 0.01 );
		}
	}
	debugf( NAME_Log, "[KHG] MapTest: done, %i loaded", Loaded );
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
	if( PlayIntroAtStartup )
		KHGPlayIntroFMVOnce( Engine );
	if( ParseParam( appCmdLine(), "KHGMAPTEST" ) )
	{
		KHGRunMapTest( Engine );
		GIsRunning = 0;
		return;
	}
#else
	KHGPlayIntroFMVOnce( Engine );   // [KHG] intro FMV before the menu (UNREAL_ANDROID_INTRO_FMV_V1)
	KHGShowMicroProseSplashOnce( Engine );   // [KHG] MicroProse splash after the intro (UNREAL_ANDROID_MICROPROSE_SPLASH_V1)
#endif
	DOUBLE OldTime = appSeconds();
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
