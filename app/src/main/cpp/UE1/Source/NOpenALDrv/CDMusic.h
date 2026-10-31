/*------------------------------------------------------------------------------------
	[KHG] Ripped CD-audio tracks (desktop build only, KHG_CDMUSIC). See CDMusic.cpp.
------------------------------------------------------------------------------------*/
#pragma once

#include <string>

struct FCDMusicStream;

// Path of the audio file for CD track Track in folder Dir (UTF-8), or "" if there is none.
std::string CDMusicFindTrack( const char* Dir, int Track );

// Opens Path for decoding to interleaved 16-bit stereo at Rate Hz. Null on failure (reason in Error).
FCDMusicStream* CDMusicOpen( const char* Path, int Rate, char* Error, int ErrorSize );

// Decodes up to Frames stereo frames into Out. Returns the number written; 0 at the end of the track.
int CDMusicRead( FCDMusicStream* Stream, short* Out, int Frames );

void CDMusicClose( FCDMusicStream* Stream );
