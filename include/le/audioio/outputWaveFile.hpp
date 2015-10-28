////////////////////////////////////////////////////////////////////////////////
///
/// \file outputWaveFile.hpp
/// ------------------------
///
/// Copyright (c) 2012 - 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef outputWaveFile_hpp__20E1BC50_AD13_436D_B244_5D8FB26FD8EA
#define outputWaveFile_hpp__20E1BC50_AD13_436D_B244_5D8FB26FD8EA
#pragma once
//------------------------------------------------------------------------------
#include "le/utility/abi.hpp"
#ifndef _MSC_VER
#include "le/utility/filesystem.hpp"
#endif // _MSC_VER
#include "le/utility/pimpl.hpp"
//------------------------------------------------------------------------------
namespace LE
{
//------------------------------------------------------------------------------
#ifdef _MSC_VER
namespace Utility { enum SpecialLocations; }
#endif // _MSC_VER
//------------------------------------------------------------------------------
namespace AudioIO
{
//------------------------------------------------------------------------------

/// \addtogroup AudioIO
/// @{

typedef char const * error_msg_t;

////////////////////////////////////////////////////////////////////////////////
///
/// \class OutputWaveFile
///
/// \brief A lightweight portable class for creating and writing uncompressed
/// WAVE_FORMAT_PCM WAVE files.
///
////////////////////////////////////////////////////////////////////////////////

class OutputWaveFile
#ifndef DOXYGEN_ONLY
    :
    public Utility::StackPImpl<OutputWaveFile, 30 * sizeof( void * )>
#endif // DOXYGEN_ONLY
{
public:
    LE_NOTHROW  OutputWaveFile();
    LE_NOTHROW ~OutputWaveFile(); ///< \details Implicitly calls close().

    /// <B>Effect:</B> Creates a file at <VAR>pathToFile</VAR> for writing.<BR>
    /// The function can be called many times, i.e. one instance of OutputWaveFile can be used to create and write as many WAVE files as desired. Note that the function offers only the basic (error safety) guarantee.<BR>
    /// <B>Postconditions:</B> Unless an error is reported, the file was successfully created with preallocated space for the header/metadata. Implicitly closes any previously possibly open file.<BR>
    /// \tparam rootLocation The filesystem location at which to create the desired file. @see Utility::SpecialLocations
    template <Utility::SpecialLocations rootLocation>
    LE_NOTHROW error_msg_t LE_FASTCALL_ABI create( char const * pathToFile, unsigned int numberOfChannels, unsigned int sampleRate );

    /// <B>Effect:</B> Writes out the WAVE header and closes the file.<BR>
    LE_NOTHROW void LE_FASTCALL_ABI close();

    /// <B>Effect:</B> Writes interleaved <VAR>numberOfSampleFrames</VAR> * <VAR>numberOfChannels</VAR> samples from <VAR>pInput</VAR> into the underlying file.<BR>
    /// <B>Preconditions:</B>
    ///     - a successful create() call
    ///     - the buffer pointed to by pInput must hold at least <VAR>numberOfSampleFrames</VAR> * <VAR>numberOfChannels</VAR> samples.
    LE_NOTHROW error_msg_t LE_FASTCALL_ABI write( float const * pInput, unsigned int numberOfSampleFrames );
}; // class OutputWaveFile

/// @} // group AudioIO

//------------------------------------------------------------------------------
} // namespace AudioIO
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // outputWaveFile_hpp
