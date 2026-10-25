/*=============================================================================
	PlatformSurface.cpp: native window + VkSurfaceKHR creation (Android, Windows).
	See PlatformSurface.h.
=============================================================================*/

#include "PlatformSurface.h"

// [KHG] Surface via the platform WSI call (not SDL_Vulkan_*): SDL doesn't treat this port's window as a Vulkan window, so build the surface straight from the native window.
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#if defined(VK_USE_PLATFORM_ANDROID_KHR)

#include <android/native_window.h>

const char* KHGVkPlatformSurfaceExtension()
{
	return VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
}

void* KHGVkGetNativeWindow( SDL_Window* Window )
{
	if( !Window )
		return nullptr;
	SDL_SysWMinfo wm; SDL_VERSION( &wm.version );
	if( !SDL_GetWindowWMInfo( Window, &wm ) )
		return nullptr;
	return wm.info.android.window;
}

void KHGVkGetNativeWindowSize( void* NativeWindow, int* OutWidth, int* OutHeight )
{
	ANativeWindow* awin = (ANativeWindow*)NativeWindow;
	*OutWidth = awin ? ANativeWindow_getWidth( awin ) : 0;
	*OutHeight = awin ? ANativeWindow_getHeight( awin ) : 0;
}

VkSurfaceKHR KHGVkCreatePlatformSurface( VkInstance Instance, void* NativeWindow, VkResult* OutResult )
{
	VkAndroidSurfaceCreateInfoKHR sci = {};
	sci.sType  = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	sci.window = (ANativeWindow*)NativeWindow;
	VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
	*OutResult = vkCreateAndroidSurfaceKHR( Instance, &sci, NULL, &surfaceHandle );
	return *OutResult == VK_SUCCESS ? surfaceHandle : VK_NULL_HANDLE;
}

#elif defined(VK_USE_PLATFORM_WIN32_KHR)

const char* KHGVkPlatformSurfaceExtension()
{
	return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}

void* KHGVkGetNativeWindow( SDL_Window* Window )
{
	if( !Window )
		return nullptr;
	SDL_SysWMinfo wm; SDL_VERSION( &wm.version );
	if( !SDL_GetWindowWMInfo( Window, &wm ) )
		return nullptr;
	return wm.info.win.window;
}

void KHGVkGetNativeWindowSize( void* NativeWindow, int* OutWidth, int* OutHeight )
{
	RECT rc = {};
	if( NativeWindow && GetClientRect( (HWND)NativeWindow, &rc ) )
	{
		*OutWidth = rc.right - rc.left;
		*OutHeight = rc.bottom - rc.top;
	}
	else
	{
		*OutWidth = *OutHeight = 0;
	}
}

VkSurfaceKHR KHGVkCreatePlatformSurface( VkInstance Instance, void* NativeWindow, VkResult* OutResult )
{
	VkWin32SurfaceCreateInfoKHR sci = {};
	sci.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	sci.hwnd      = (HWND)NativeWindow;
	sci.hinstance = GetModuleHandle( nullptr );
	VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
	*OutResult = vkCreateWin32SurfaceKHR( Instance, &sci, NULL, &surfaceHandle );
	return *OutResult == VK_SUCCESS ? surfaceHandle : VK_NULL_HANDLE;
}

#else
#error "VulkanDrv: no platform surface for this target (define VK_USE_PLATFORM_ANDROID_KHR or VK_USE_PLATFORM_WIN32_KHR)."
#endif
