////////////////////////////////////////////////////////////////////////////////
///
/// \file melodifyer.hpp
/// --------------------
///
/// Copyright (c) 2013 - 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef melodifyer_hpp__0CA4FB8C_13AD_4343_856E_E0484EE5A393
#define melodifyer_hpp__0CA4FB8C_13AD_4343_856E_E0484EE5A393
#pragma once
//------------------------------------------------------------------------------
#include "le/utility/abi.hpp"
#include "le/utility/filesystem.hpp"
#include "le/utility/pimpl.hpp"
//------------------------------------------------------------------------------
#if defined( _MSC_VER ) && !defined( LE_SDK_NO_AUTO_LINK )
    #ifdef _WIN64
        #pragma comment( lib, "SpectrumWorxMelodifySDK_Win64_x86-64_SSE3.lib" )
    #else // _WIN32
        #pragma comment( lib, "SpectrumWorxMelodifySDK_Win32_x86-32_SSE2.lib" )
    #endif // _WIN32/64
#endif // _MSC_VER && !LE_SDK_NO_AUTO_LINK
//------------------------------------------------------------------------------
namespace LE
{
//------------------------------------------------------------------------------
namespace SW
{
//------------------------------------------------------------------------------

/// \addtogroup Melodify
/// \brief Home of the Melodifyer
/// @{

////////////////////////////////////////////////////////////////////////////////
///
/// \class Melodifyer
///
/// \brief Turns talking into singing.
///
/// Melodifyer accepts three input streams of data:
///     - the input voice signal
///     - the, optional, background signal (e.g. instrumental music)
///     - the melody (in the form of a MIDI file) to which the voice signal
///       should be tuned
/// .
/// to create one output:
///     - tuned and melodified voice mixed with the, optional, background.
///
/// \note Except for the gain and latency related functions, none of the member
/// functions are reentrant or thread safe with respect to each other.
///
/// \nosubgrouping
///
////////////////////////////////////////////////////////////////////////////////

class Melodifyer
#ifndef DOXYGEN_ONLY
    :
    public Utility::StackPImpl<Melodifyer, 476 + 6 * sizeof( float ) + 12 * sizeof( void * ), 16>
#endif // DOXYGEN_ONLY
{
public:
    /// \name Setup:
    /// @{

    LE_NOTHROWNOALIAS  Melodifyer();
    LE_NOTHROWNOALIAS ~Melodifyer();


    ////////////////////////////////////////////////////////////////////////////
    //
    // Melodifyer::setup()
    // -------------------
    //
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Prepares the Melodifyer instance for processing.
    ///
    /// \param sampleRate       All subsequent process() calls (until the next
    ///                         call to setup()) will assume this sampling rate.
    /// \param numberOfChannels All subsequent process() calls (until the next
    ///                         call to setup()) will assume this many channels.
    ///
    /// \return                 True if successful, false if unable to allocate
    ///                         enough memory.
    ///
    ////////////////////////////////////////////////////////////////////////////

    LE_NOTHROWNOALIAS bool LE_FASTCALL_ABI setup( unsigned int sampleRate, unsigned int numberOfChannels );


    ////////////////////////////////////////////////////////////////////////////
    //
    // Melodifyer::setAutomaticVoiceGainCorrection()
    // ---------------------------------------------
    //
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Enables or disables internal automatic voice gain correction
    /// (based on the background sound levels). Defaults to false (disabled).
    ///
    ////////////////////////////////////////////////////////////////////////////

    LE_NOTHROWNOALIAS void LE_FASTCALL_ABI setAutomaticVoiceGainCorrection( bool enabled );


    ////////////////////////////////////////////////////////////////////////////
    //
    // Melodifyer::setExtraVoiceGainCorrection(_dB)()
    // ----------------------------------------------
    //
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Sets an additional gain to be applied to the input/voice signal
    /// before it is sent to the mixer. Defaults to 1 (0dB).
    ///
    ////////////////////////////////////////////////////////////////////////////

    LE_NOTHROWNOALIAS void LE_FASTCALL_ABI setExtraVoiceGainCorrection   ( float linearValue      );
    LE_NOTHROWNOALIAS void LE_FASTCALL_ABI setExtraVoiceGainCorrection_dB( float logarithmicValue ); ///< \overload


    ////////////////////////////////////////////////////////////////////////////
    //
    // Melodifyer::setMelodyMIDIFile()
    // -------------------------------
    //
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Specifies the MIDI file that defines the melody to which the
    /// input voice signal should be tuned.
    ///
    /// The MIDI file must contain only one audible track.
    ///
    /// \param fileName        path to the MIDI file containing the melody.
    /// \return                nullptr if successful, pointer to an error
    ///                        message string otherwise.
    ///
    ////////////////////////////////////////////////////////////////////////////

    template <Utility::SpecialLocations rootLocation>
    LE_NOTHROWNOALIAS char const * LE_FASTCALL_ABI setMelodyMIDIFile( char const * fileName, unsigned int melodyTrack, unsigned int melodyChannel );


    ////////////////////////////////////////////////////////////////////////////
    //
    // Melodifyer::latencyInSamples()
    // ------------------------------
    //
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Virtually any DSP incurs a certain amount of latency into the
    /// audio stream. The is especially true for frequency domain DSP, as used
    /// in the SpectrumWorx engine at the heart of the Melodify SDK. In
    /// non-realtime scenarios this can be offset if the amount of latency is
    /// fixed and known. This member function is provided for exactly that
    /// purpose (the current version of the Melodify SDK has a fixed latency of
    /// 1024 samples in all circumstances).
    ///
    ////////////////////////////////////////////////////////////////////////////

    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI latencyInSamples() const;

    /// @}

public:
    /// \name Processing:
    /// @{

    ////////////////////////////////////////////////////////////////////////////
    //
    // Melodifyer::process()
    // ---------------------
    //
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Main process function.
    ///
    /// \param pVoiceData      Pointer to an array of interleaved input channels
    ///                        (which must provide data for exactly the number
    ///                        of channels as was specified by the
    ///                        numberOfChannels parameter in the last call
    ///                        to setup()).
    /// \param pBackgroundData Pointer to an array of interleaved input channels
    ///                        (which must provide data for exactly the number
    ///                        of channels as was specified by the
    ///                        numberOfChannels parameter in the last call
    ///                        to setup()).
    ///                        This parameter is optional, if nullptr is passed
    ///                        no mixing will be performed.
    /// \param pOutputData     Pointer to an array of interleaved output channels
    ///                        (which must provide space for exactly the number
    ///                        of channels as was specified by the
    ///                        numberOfChannels parameter in the last call
    ///                        to setup()).
    /// \param numberOfSamples Number of samples to process (there is no limit
    ///                        on the number of samples that can be processed in
    ///                        one call).
    ///
    ////////////////////////////////////////////////////////////////////////////

    LE_NOTHROWNOALIAS void LE_FASTCALL_ABI process
    (
        float const * LE_RESTRICT pVoiceData,
        float const * LE_RESTRICT pBackgroundData,
        float       * LE_RESTRICT pOutputData,
        unsigned int numberOfSamples
    ) const;

    LE_NOTHROWNOALIAS void LE_FASTCALL_ABI reset(); ///< Reset the Melodifyer instance (e.g. before processing a new stream of data).

    /// @}
}; // class Melodifyer

/// @} // group Melodify

//------------------------------------------------------------------------------
} // namespace SW
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // melodifyer_hpp
