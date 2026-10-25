/*=============================================================================
	PlatformSurface.h: native window + VkSurfaceKHR creation for the KHG Vulkan device.

	The render device works with an opaque native window handle (ANativeWindow* on Android,
	HWND on Windows) and creates its VkSurfaceKHR through these helpers, so the device itself
	has no platform-specific code. Implemented in PlatformSurface.cpp.
=============================================================================*/

#pragma once

#include <zvulkan/vulkaninstance.h>

struct SDL_Window;

// Instance extension that provides the platform surface (VK_KHR_android_surface / VK_KHR_win32_surface).
const char* KHGVkPlatformSurfaceExtension();

// The native window behind an SDL window, or NULL if there is none right now
// (Android: the app is backgrounded and has no SurfaceView surface).
void* KHGVkGetNativeWindow( SDL_Window* Window );

// Current size of the native window in pixels.
void KHGVkGetNativeWindowSize( void* NativeWindow, int* OutWidth, int* OutHeight );

// Creates a surface for the native window. Returns VK_NULL_HANDLE on failure, with the result in OutResult.
VkSurfaceKHR KHGVkCreatePlatformSurface( VkInstance Instance, void* NativeWindow, VkResult* OutResult );
