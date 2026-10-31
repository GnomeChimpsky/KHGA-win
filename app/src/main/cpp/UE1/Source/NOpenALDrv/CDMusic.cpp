/*------------------------------------------------------------------------------------
	[KHG] Ripped CD-audio tracks for the Windows desktop build.

	The retail Galaxy.dll plays PlayerPawn.CdTrack from the game CD through MCI
	(MCI_PLAY from the track's start to its end, once). The desktop build plays the
	same track from an audio file instead: <CDMusicPath>\Track02.ogg and so on.
	This file finds that file and decodes it with FFmpeg to 16-bit stereo at the
	OpenAL output rate. Only built when KHG_CDMUSIC is defined (desktop CMake).
------------------------------------------------------------------------------------*/

#include "CDMusic.h"

#include <ctype.h>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
}

namespace fs = std::filesystem;

struct FCDMusicStream
{
	AVFormatContext* Fmt = nullptr;
	AVCodecContext* Dec = nullptr;
	SwrContext* Swr = nullptr;
	AVPacket* Pkt = nullptr;
	AVFrame* Frm = nullptr;
	int Stream = -1;
	int Rate = 44100;
	bool Eof = false;          // no more packets
	bool Drained = false;      // decoder and resampler fully flushed
	std::vector<short> Pending; // converted stereo samples not yet handed out
	size_t PendingPos = 0;
};

static const char* const GExts[] = { ".ogg", ".flac", ".mp3", ".wav", ".opus" };

static std::string Lower( std::string S )
{
	for( char& C : S ) C = (char)tolower( (unsigned char)C );
	return S;
}

static bool IsAudioExt( const std::string& Ext )
{
	const std::string L = Lower( Ext );
	for( const char* E : GExts )
		if( L == E )
			return true;
	return false;
}

// First run of digits in the name (without extension), or -1.
static int FirstNumber( const std::string& Stem )
{
	for( size_t i = 0; i < Stem.size(); ++i )
	{
		if( isdigit( (unsigned char)Stem[i] ) )
		{
			int N = 0;
			while( i < Stem.size() && isdigit( (unsigned char)Stem[i] ) && N < 1000 )
				N = N * 10 + ( Stem[i++] - '0' );
			return N;
		}
	}
	return -1;
}

std::string CDMusicFindTrack( const char* Dir, int Track )
{
	std::error_code Ec;
	fs::path Folder = fs::u8path( Dir ? Dir : "" );
	if( !fs::is_directory( Folder, Ec ) )
		return std::string();

	// Exact names first: Track02, Track 02, Track2, 02, 2.
	char Names[5][32];
	snprintf( Names[0], sizeof( Names[0] ), "track%02d", Track );
	snprintf( Names[1], sizeof( Names[1] ), "track %02d", Track );
	snprintf( Names[2], sizeof( Names[2] ), "track%d", Track );
	snprintf( Names[3], sizeof( Names[3] ), "%02d", Track );
	snprintf( Names[4], sizeof( Names[4] ), "%d", Track );

	std::vector<fs::path> Audio;
	for( fs::directory_iterator It( Folder, Ec ), End; !Ec && It != End; It.increment( Ec ) )
	{
		if( !It->is_regular_file( Ec ) )
			continue;
		const fs::path& P = It->path();
		if( IsAudioExt( P.extension().u8string() ) )
			Audio.push_back( P );
	}
	for( const char* Want : Names )
		for( const fs::path& P : Audio )
			if( Lower( P.stem().u8string() ) == Want )
				return P.u8string();

	// Otherwise the first file whose name starts its numbering with the track number,
	// such as "02 - Title.flac" or "Klingon Honor Guard (Track 02).mp3".
	for( const fs::path& P : Audio )
	{
		const std::string Stem = P.stem().u8string();
		if( FirstNumber( Stem ) == Track )
			return P.u8string();
	}
	return std::string();
}

void CDMusicClose( FCDMusicStream* S )
{
	if( !S )
		return;
	if( S->Swr ) swr_free( &S->Swr );
	if( S->Dec ) avcodec_free_context( &S->Dec );
	if( S->Fmt ) avformat_close_input( &S->Fmt );
	if( S->Pkt ) av_packet_free( &S->Pkt );
	if( S->Frm ) av_frame_free( &S->Frm );
	delete S;
}

FCDMusicStream* CDMusicOpen( const char* Path, int Rate, char* Error, int ErrorSize )
{
	auto Fail = [&]( FCDMusicStream* S, const char* Msg ) -> FCDMusicStream*
	{
		if( Error && ErrorSize > 0 )
			snprintf( Error, ErrorSize, "%s", Msg );
		CDMusicClose( S );
		return nullptr;
	};

	FCDMusicStream* S = new FCDMusicStream;
	S->Rate = Rate > 0 ? Rate : 44100;
	if( avformat_open_input( &S->Fmt, Path, nullptr, nullptr ) != 0 )
		return Fail( S, "cannot open file" );
	if( avformat_find_stream_info( S->Fmt, nullptr ) < 0 )
		return Fail( S, "cannot read stream info" );
	const AVCodec* Codec = nullptr;
	S->Stream = av_find_best_stream( S->Fmt, AVMEDIA_TYPE_AUDIO, -1, -1, &Codec, 0 );
	if( S->Stream < 0 || !Codec )
		return Fail( S, "no audio stream this build can decode (use .ogg, .flac, .mp3, .wav or .opus)" );
	S->Dec = avcodec_alloc_context3( Codec );
	if( !S->Dec || avcodec_parameters_to_context( S->Dec, S->Fmt->streams[S->Stream]->codecpar ) < 0
		|| avcodec_open2( S->Dec, Codec, nullptr ) < 0 )
		return Fail( S, "cannot open decoder" );
	if( S->Dec->ch_layout.order == AV_CHANNEL_ORDER_UNSPEC )
		av_channel_layout_default( &S->Dec->ch_layout, S->Dec->ch_layout.nb_channels );

	AVChannelLayout Stereo = AV_CHANNEL_LAYOUT_STEREO;
	if( swr_alloc_set_opts2( &S->Swr, &Stereo, AV_SAMPLE_FMT_S16, S->Rate,
			&S->Dec->ch_layout, S->Dec->sample_fmt, S->Dec->sample_rate, 0, nullptr ) < 0
		|| swr_init( S->Swr ) < 0 )
		return Fail( S, "cannot set up resampler" );
	S->Pkt = av_packet_alloc();
	S->Frm = av_frame_alloc();
	if( !S->Pkt || !S->Frm )
		return Fail( S, "out of memory" );
	return S;
}

// Converts one decoded frame (or flushes the resampler when Frame is null) into Pending.
static void Convert( FCDMusicStream* S, const AVFrame* Frame )
{
	const int InSamples = Frame ? Frame->nb_samples : 0;
	const int OutMax = swr_get_out_samples( S->Swr, InSamples );
	if( OutMax <= 0 )
		return;
	const size_t Old = S->Pending.size();
	S->Pending.resize( Old + (size_t)OutMax * 2 );
	uint8_t* Out = (uint8_t*)( S->Pending.data() + Old );
	const int Got = swr_convert( S->Swr, &Out, OutMax,
		Frame ? (const uint8_t**)Frame->extended_data : nullptr, InSamples );
	S->Pending.resize( Old + (size_t)( Got > 0 ? Got : 0 ) * 2 );
}

// Decodes until Pending holds at least Want stereo frames or the file ends.
static void Fill( FCDMusicStream* S, size_t Want )
{
	while( !S->Drained && S->Pending.size() - S->PendingPos < Want * 2 )
	{
		if( S->PendingPos > 0 )
		{
			S->Pending.erase( S->Pending.begin(), S->Pending.begin() + S->PendingPos );
			S->PendingPos = 0;
		}
		int R = avcodec_receive_frame( S->Dec, S->Frm );
		if( R == 0 )
		{
			Convert( S, S->Frm );
			av_frame_unref( S->Frm );
			continue;
		}
		if( R == AVERROR_EOF )
		{
			Convert( S, nullptr );
			S->Drained = true;
			break;
		}
		if( R != AVERROR( EAGAIN ) )
		{
			S->Drained = true; // decode error: end the track here
			break;
		}
		if( S->Eof )
		{
			avcodec_send_packet( S->Dec, nullptr );
			continue;
		}
		R = av_read_frame( S->Fmt, S->Pkt );
		if( R < 0 )
		{
			S->Eof = true;
			avcodec_send_packet( S->Dec, nullptr );
			continue;
		}
		if( S->Pkt->stream_index == S->Stream )
			avcodec_send_packet( S->Dec, S->Pkt );
		av_packet_unref( S->Pkt );
	}
}

int CDMusicRead( FCDMusicStream* S, short* Out, int Frames )
{
	if( !S || Frames <= 0 )
		return 0;
	Fill( S, (size_t)Frames );
	const size_t Avail = ( S->Pending.size() - S->PendingPos ) / 2;
	const size_t N = Avail < (size_t)Frames ? Avail : (size_t)Frames;
	if( N )
	{
		memcpy( Out, S->Pending.data() + S->PendingPos, N * 2 * sizeof( short ) );
		S->PendingPos += N * 2;
	}
	return (int)N;
}
