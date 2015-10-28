////////////////////////////////////////////////////////////////////////////////
///
/// \file file.hpp
/// --------------
///
/// Copyright (c) 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef file_hpp__6A9131FE_0E31_467F_9A30_C207F86E591E
#define file_hpp__6A9131FE_0E31_467F_9A30_C207F86E591E
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

typedef char const * error_msg_t; ///< A static error message string.

////////////////////////////////////////////////////////////////////////////////
///
/// \class File
///
/// \brief Reads audio files in different formats from the filesystem. The list
/// of supported audio formats depends on the underlying OS.
///
/// \note <B>Android specific:</B> Use of this class requires Android 4.0 (API
/// level 14) or later.
///
////////////////////////////////////////////////////////////////////////////////

class File
#ifndef DOXYGEN_ONLY
    :
    public Utility::StackPImpl
    <
        File,
    #if defined( __ANDROID__ ) && __LP64__
        19 * sizeof( void * )
    #elif defined( __ANDROID__ )
        9 * sizeof( void * )
    #elif defined( _WIN32 )
        25 * sizeof( void * ) + 20 * sizeof( int )
    #else
        5 * sizeof( void * )
    #endif // __ANDROID__
        , 16
    >
#endif // DOXYGEN_ONLY
{
public:
    LE_NOTHROWNOALIAS  File();
    LE_NOTHROW        ~File(); ///< \details Implicitly calls close().

    /// <B>Effect:</B> Opens the file pointed to by <VAR>relativePathToFile</VAR> within/relative to <VAR>rootLocation</VAR>.<BR>
    /// The function can be called many times, i.e. one File instance can be used to read as many audio files as desired. Note that the function offers only the basic (error safety) guarantee.<BR>
    /// <B>Postconditions:</B> Unless an error is reported, the file was successfully opened and its header parsed making it ready for audio and metadata extraction.<BR>
    /// \tparam rootLocation The filesystem location at which to look for
    ///         <VAR>relativePathToFile</VAR>. @see Utility::SpecialLocations
    template <Utility::SpecialLocations rootLocation>
    LE_NOTHROW error_msg_t LE_FASTCALL_ABI open( char    const * relativePathToFile );
#ifdef _WIN32
    template <Utility::SpecialLocations rootLocation>
    LE_NOTHROW error_msg_t LE_FASTCALL_ABI open( wchar_t const * relativePathToFile );
#endif // _WIN32

    /// <B>Effect:</B> Closes the open file (if any).<BR>
    LE_NOTHROW void LE_FASTCALL_ABI close();

    /// <B>Effect:</B> Reads up to <VAR>numberOfSampleFrames</VAR> * numberOfChannels() interleaved samples into <VAR>pOutput</VAR> and adjusts the current file position (as reported by remainingSamples()).<BR>
    /// <B>Preconditions:</B>
    ///     - a successful open() call
    ///     - the buffer pointed to by pOutput must be large enough to hold <VAR>numberOfSampleFrames</VAR> * numberOfChannels() samples.
    /// \return Number of sample frames actually read (0 on failure, always <= <VAR>numberOfSampleFrames</VAR>).
    /// 
    /// .
    /// 
    /// \note
    /// It is important to check the return value of this function because
    /// it may be less than the requested number of sample frames. Beyond the
    /// usual EOF condition this may also happen because the values reported by
    /// the lengthInSamples() and remainingSamples() member functions are only
    /// approximate. This slight uncertainty in the audio file length
    /// calculation is due to two causes:
    /// - the File class supports reading from a multitude of file
    ///   formats some of which do not support efficient yet exact seeking or
    ///   duration extraction (e.g. VBR MP3s)
    /// - rounding errors in conversions between durations/time stamps expressed
    ///   in samples and milliseconds.
    /// This discrepancy between the requested and returned/read
    /// <VAR>numberOfSampleFrames</VAR> should only occur at the very end of a
    /// file (or when reading the entire file at once, of course).
    LE_NOTHROW        unsigned int   LE_FASTCALL_ABI read( float * pOutput, unsigned int numberOfSampleFrames ) const;

    LE_NOTHROWNOALIAS unsigned short LE_FASTCALL_ABI numberOfChannels() const; ///< Number of channels reported by the OS decoder for the audio file. <BR> \details <B>Preconditions:</B> A successful open() call.
    LE_NOTHROWNOALIAS unsigned int   LE_FASTCALL_ABI sampleRate      () const; ///< Sample rate reported by the OS decoder for the audio file. <BR> \details <B>Preconditions:</B> A successful open() call.
    LE_NOTHROWNOALIAS unsigned int   LE_FASTCALL_ABI remainingSamples() const; ///< Approximate number of sample frames remaining/not yet read from the audio file as reported by the OS decoder. <BR> \details <B>Preconditions:</B> A successful open() call.
    LE_NOTHROWNOALIAS unsigned int   LE_FASTCALL_ABI lengthInSamples () const; ///< Approximate total number of sample frames in the currently opened file. <BR> \details <B>Preconditions:</B> A successful open() call.

    LE_NOTHROW void LE_FASTCALL_ABI setTimePosition  ( unsigned int positionInMilliseconds ); ///< Seek to a specific position measured in milliseconds. <BR> \details <B>Preconditions:</B> <BR> - a successful open() call <BR> - <VAR>positionInMilliseconds</VAR> must be valid.
    LE_NOTHROW void LE_FASTCALL_ABI setSamplePosition( unsigned int positionInSampleFrames ); ///< Seek to a specific position measured in sample frames. <BR> \details <B>Preconditions:</B> <BR> - a successful open() call <BR> - <VAR>positionInSampleFrames</VAR> must be valid.

    LE_NOTHROW void LE_FASTCALL_ABI restart(); ///< (Re)Start reading from the beginning (equivalent to set*Position( 0 )).

    LE_NOTHROWNOALIAS bool LE_FASTCALL_ABI operator!() const;

    /// \brief A semicolon separated list of supported audio file formats/containers.
    /// \details
    ///     - Android (4.0+): *.3gp;*.mp4;*.m4a;*.aac;*.ts;*.flac;*.mp3;*.mid;*.xmf;*.mxmf;*.rtttl;*.rtx;*.ota;*.imy;*.ogg;*.mkv;*.wav
    ///     - iOS & OSX     : *.aac;*.ac3;*.adts;*.aif;*.aiff;*.au;*.caf;*.mpa;*.mp3;*.mp4;*.m4a;*.sd2;*.snd;*.wav
    ///     - Windows       : *.aac;*.aif;*.aiff;*.au;*.mpa;*.mp3;*.snd;*.wav;*.wma
    static char const supportedFormats[];
}; // class File

//------------------------------------------------------------------------------
} // namespace AudioIO
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // file_hpp
