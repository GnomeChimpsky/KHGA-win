/*=============================================================================
	KHGSplash.h: reads the retail MicroProse launcher splash from the user's Khg.exe.

	The splash is RT_BITMAP resource 102 in the retail Khg.exe. Rather than shipping a copy of
	it, the launcher pulls it out of the user's own install at runtime. The PE parsing is
	portable (no Win32 resource API), so Android and Windows share it.
=============================================================================*/

#pragma once

// Resource id of the MicroProse splash bitmap inside the retail Khg.exe.
#define KHG_SPLASH_RESOURCE_ID 102

// Loads RT_BITMAP <ResourceId> from the PE file at ExePath and expands it to top-down BGRA
// (alpha 255). Supports uncompressed 8-bit palettized, 24-bit and 32-bit DIBs.
// On success returns true and a malloc'd buffer the caller frees with free().
bool KHGLoadPEBitmapBGRA( const char* ExePath, int ResourceId, unsigned char** OutBGRA, int* OutWidth, int* OutHeight );
