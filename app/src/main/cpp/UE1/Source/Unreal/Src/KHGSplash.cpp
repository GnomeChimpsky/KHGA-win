/*=============================================================================
	KHGSplash.cpp: portable PE RT_BITMAP reader for the retail splash. See KHGSplash.h.
=============================================================================*/

#include "KHGSplash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>

namespace
{

static const uint32_t RT_BITMAP_ID = 2;

struct FPEImage
{
	std::vector<unsigned char> Data;

	struct FSection { uint32_t VirtualAddress, VirtualSize, RawOffset, RawSize; };
	std::vector<FSection> Sections;
	uint32_t ResourceRVA = 0;

	bool Has( size_t Offset, size_t Count ) const
	{
		return Offset <= Data.size() && Count <= Data.size() - Offset;
	}
	uint16_t U16( size_t Offset ) const { return (uint16_t)( Data[Offset] | ( Data[Offset+1] << 8 ) ); }
	uint32_t U32( size_t Offset ) const
	{
		return (uint32_t)Data[Offset] | ( (uint32_t)Data[Offset+1] << 8 ) | ( (uint32_t)Data[Offset+2] << 16 ) | ( (uint32_t)Data[Offset+3] << 24 );
	}

	// Maps an RVA to a file offset; returns false if it isn't backed by file data.
	bool RVAToOffset( uint32_t RVA, uint32_t Size, size_t* OutOffset ) const
	{
		for( const FSection& S : Sections )
		{
			const uint32_t Span = S.VirtualSize ? S.VirtualSize : S.RawSize;
			if( RVA >= S.VirtualAddress && RVA - S.VirtualAddress < Span )
			{
				const uint32_t Delta = RVA - S.VirtualAddress;
				if( Delta > S.RawSize || Size > S.RawSize - Delta )
					return false;
				*OutOffset = (size_t)S.RawOffset + Delta;
				return Has( *OutOffset, Size );
			}
		}
		return false;
	}

	bool Parse()
	{
		if( !Has( 0, 0x40 ) || Data[0] != 'M' || Data[1] != 'Z' )
			return false;
		const size_t PE = U32( 0x3C );
		if( !Has( PE, 24 ) || memcmp( &Data[PE], "PE\0\0", 4 ) != 0 )
			return false;
		const uint16_t NumSections = U16( PE + 6 );
		const uint16_t OptSize = U16( PE + 20 );
		const size_t Opt = PE + 24;
		if( !Has( Opt, OptSize ) || OptSize < 2 )
			return false;

		// Data directory 2 (resources): PE32 has it at +96+16, PE32+ at +112+16.
		const uint16_t Magic = U16( Opt );
		const size_t DirBase = ( Magic == 0x20B ) ? Opt + 112 : ( Magic == 0x10B ) ? Opt + 96 : 0;
		if( !DirBase || DirBase + 3 * 8 > Opt + OptSize )
			return false;
		ResourceRVA = U32( DirBase + 2 * 8 );
		if( !ResourceRVA )
			return false;

		const size_t SecTable = Opt + OptSize;
		if( !Has( SecTable, (size_t)NumSections * 40 ) )
			return false;
		for( uint16_t i = 0; i < NumSections; i++ )
		{
			const size_t S = SecTable + (size_t)i * 40;
			Sections.push_back( { U32( S + 12 ), U32( S + 8 ), U32( S + 20 ), U32( S + 16 ) } );
		}
		return true;
	}

	// Finds the entry with integer id Id in the resource directory at DirOffset (relative to the
	// resource section). Id < 0 takes the first id entry. Returns the entry's OffsetToData field.
	bool FindEntry( uint32_t DirOffset, int Id, uint32_t* OutEntry ) const
	{
		size_t Dir = 0;
		if( !RVAToOffset( ResourceRVA + DirOffset, 16, &Dir ) )
			return false;
		const uint16_t NumNamed = U16( Dir + 12 );
		const uint16_t NumIds = U16( Dir + 14 );
		for( uint32_t i = 0; i < (uint32_t)NumNamed + NumIds; i++ )
		{
			const size_t E = Dir + 16 + (size_t)i * 8;
			if( !Has( E, 8 ) )
				return false;
			const uint32_t Name = U32( E );
			if( Name & 0x80000000u )
				continue;   // named entry
			if( Id < 0 || Name == (uint32_t)Id )
			{
				*OutEntry = U32( E + 4 );
				return true;
			}
		}
		return false;
	}

	// Locates the raw data of resource Type/Id (first language).
	bool FindResource( uint32_t Type, uint32_t Id, size_t* OutOffset, uint32_t* OutSize ) const
	{
		uint32_t TypeEntry, IdEntry, LangEntry;
		if( !FindEntry( 0, (int)Type, &TypeEntry ) || !( TypeEntry & 0x80000000u ) )
			return false;
		if( !FindEntry( TypeEntry & 0x7FFFFFFFu, (int)Id, &IdEntry ) || !( IdEntry & 0x80000000u ) )
			return false;
		if( !FindEntry( IdEntry & 0x7FFFFFFFu, -1, &LangEntry ) || ( LangEntry & 0x80000000u ) )
			return false;
		size_t DataEntry = 0;
		if( !RVAToOffset( ResourceRVA + LangEntry, 16, &DataEntry ) )
			return false;
		const uint32_t DataRVA = U32( DataEntry );
		const uint32_t DataSize = U32( DataEntry + 4 );
		if( !RVAToOffset( DataRVA, DataSize, OutOffset ) )
			return false;
		*OutSize = DataSize;
		return true;
	}
};

static bool ReadFile( const char* Path, std::vector<unsigned char>& Out )
{
	FILE* F = fopen( Path, "rb" );
	if( !F )
		return false;
	bool Ok = fseek( F, 0, SEEK_END ) == 0;
	const long Size = Ok ? ftell( F ) : -1;
	Ok = Ok && Size > 0 && Size < 64L * 1024 * 1024 && fseek( F, 0, SEEK_SET ) == 0;
	if( Ok )
	{
		Out.resize( (size_t)Size );
		Ok = fread( Out.data(), 1, Out.size(), F ) == Out.size();
	}
	fclose( F );
	return Ok;
}

// Expands a packed DIB (BITMAPINFOHEADER + palette + pixels, as stored in RT_BITMAP) to top-down BGRA.
static bool DIBToBGRA( const unsigned char* Dib, size_t Size, unsigned char** OutBGRA, int* OutWidth, int* OutHeight )
{
	auto U16 = [Dib]( size_t O ) { return (uint32_t)( Dib[O] | ( Dib[O+1] << 8 ) ); };
	auto U32 = [Dib]( size_t O ) { return (uint32_t)Dib[O] | ( (uint32_t)Dib[O+1] << 8 ) | ( (uint32_t)Dib[O+2] << 16 ) | ( (uint32_t)Dib[O+3] << 24 ); };
	if( Size < 40 )
		return false;
	const uint32_t HeaderSize = U32( 0 );
	const int32_t Width = (int32_t)U32( 4 );
	const int32_t RawHeight = (int32_t)U32( 8 );
	const uint32_t Bits = U16( 14 );
	const uint32_t Compression = U32( 16 );
	uint32_t ClrUsed = U32( 32 );
	if( HeaderSize < 40 || HeaderSize > Size || Width <= 0 || RawHeight == 0 || Compression != 0 /*BI_RGB*/ )
		return false;
	if( Bits != 8 && Bits != 24 && Bits != 32 )
		return false;
	const bool TopDown = RawHeight < 0;
	const int32_t Height = TopDown ? -RawHeight : RawHeight;
	if( Width > 8192 || Height > 8192 )
		return false;

	if( Bits == 8 && ( ClrUsed == 0 || ClrUsed > 256 ) )
		ClrUsed = 256;
	const size_t PaletteSize = ( Bits == 8 ) ? (size_t)ClrUsed * 4 : 0;
	const size_t Stride = ( ( (size_t)Width * Bits + 31 ) / 32 ) * 4;
	const size_t PixelOffset = HeaderSize + PaletteSize;
	if( PixelOffset > Size || Stride * Height > Size - PixelOffset )
		return false;

	unsigned char* BGRA = (unsigned char*)malloc( (size_t)Width * Height * 4 );
	if( !BGRA )
		return false;
	const unsigned char* Palette = Dib + HeaderSize;   // RGBQUAD = B, G, R, reserved
	for( int32_t y = 0; y < Height; y++ )
	{
		const unsigned char* Src = Dib + PixelOffset + Stride * ( TopDown ? y : Height - 1 - y );
		unsigned char* Dst = BGRA + (size_t)y * Width * 4;
		for( int32_t x = 0; x < Width; x++, Dst += 4 )
		{
			const unsigned char* C;
			if( Bits == 8 )
			{
				const uint32_t Index = Src[x];
				C = Index < ClrUsed ? Palette + Index * 4 : Palette;
			}
			else
			{
				C = Src + (size_t)x * ( Bits / 8 );
			}
			Dst[0] = C[0];
			Dst[1] = C[1];
			Dst[2] = C[2];
			Dst[3] = 255;
		}
	}
	*OutBGRA = BGRA;
	*OutWidth = Width;
	*OutHeight = Height;
	return true;
}

} // namespace

bool KHGLoadPEBitmapBGRA( const char* ExePath, int ResourceId, unsigned char** OutBGRA, int* OutWidth, int* OutHeight )
{
	FPEImage Image;
	if( !ExePath || !ReadFile( ExePath, Image.Data ) || !Image.Parse() )
		return false;
	size_t Offset = 0;
	uint32_t Size = 0;
	if( !Image.FindResource( RT_BITMAP_ID, (uint32_t)ResourceId, &Offset, &Size ) )
		return false;
	return DIBToBGRA( Image.Data.data() + Offset, Size, OutBGRA, OutWidth, OutHeight );
}
