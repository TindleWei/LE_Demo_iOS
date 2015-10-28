////////////////////////////////////////////////////////////////////////////////
///
/// \file device.hpp
/// ----------------
///
/// Hardware IO.
///
/// Copyright (c) 2013 - 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef device_hpp__1B86D709_0F0F_4D5C_A587_5ABFC3EAE42D
#define device_hpp__1B86D709_0F0F_4D5C_A587_5ABFC3EAE42D
#pragma once
//------------------------------------------------------------------------------
#include "le/utility/abi.hpp"
#include "le/utility/pimpl.hpp"

#include <utility>
//------------------------------------------------------------------------------
#if defined( _MSC_VER ) && !defined( LE_SDK_NO_AUTO_LINK )
    #ifdef _WIN64
        #pragma comment( lib, "AudioIO_Win64_x86-64_SSE3.lib" )
    #else // _WIN32
        #pragma comment( lib, "AudioIO_Win32_x86-32_SSE2.lib" )
    #endif // _WIN32/64
#endif // _MSC_VER && !LE_SDK_NO_AUTO_LINK
//------------------------------------------------------------------------------
namespace LE
{
//------------------------------------------------------------------------------
namespace AudioIO /// \brief Root namespace for all things AudioIO
{
//------------------------------------------------------------------------------

/// \addtogroup AudioIO
/// @{

typedef char const * error_msg_t;

////////////////////////////////////////////////////////////////////////////////
///
/// \class Device
///
/// \brief Models a hardware audio IO device (e.g. a soundcard) capable of
/// capturing audio from an input source (e.g. a microphone) and/or playing
/// audio data through an output device (e.g. headphones).
///
/// \note Currently there is no way to choose the actual underlying device or
/// the low-level API used to access it. The default built-in device is
/// automatically used which should cover most mobile platform use cases.
///
/// \nosubgrouping
///
////////////////////////////////////////////////////////////////////////////////

class Device
#ifdef DOXYGEN_ONLY
#elif defined( __ANDROID__ )
    : public Utility::StackPImpl<Device, 15 * sizeof( void * ) + 7 * sizeof( int )>
#elif defined( __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ )
    : public Utility::StackPImpl<Device,  3 * sizeof( void * )>
#else
    : public Utility::StackPImpl<Device,  3 * sizeof( void * ) + 2 * sizeof( unsigned int )>
#endif // DOXYGEN_ONLY
{
public:
    LE_NOTHROW  Device();
    LE_NOTHROW ~Device();

    /// \name Streaming control
    /// @{

    /// <B>Effect:</B> Starts the streaming process on the device. The function
    /// is non-blocking: data notifications are performed asynchronously using
    /// the callback given in the last call to setCallback().<BR>
    /// <B>Preconditions:</B>
    ///     - a successful call to the setCallback() member function
    ///     - the device must be in the stopped state.
    LE_NOTHROW void LE_FASTCALL_ABI start();
    /// \details
    /// <B>Effect:</B> Stops the streaming on the device.<BR>
    LE_NOTHROW void LE_FASTCALL_ABI stop ();

    /// @}

    /// \name Configuration
    /// @{

    /// <B>Effect:</B> Configures the device instance to consume and/or produce
    /// audio data in the given format.<BR>
    /// A Device instance may be (re)configured any number of times.<BR>
    /// <B>Preconditions:</B> The Device instance must be in the stopped state.
    /// \note
    /// - the actual data type used to transfer samples across the API is always
    ///   float
    /// - the <VAR>latencyInSamples</VAR> parameter is treated merely as a hint
    /// - zero can be passed for any of the three parameters to let the library
    ///   heuristically choose the optimum value for the current device and OS
    /// - Android specific: latency is still very much a moving target and the
    ///   platform offers acceptable latencies only for playback and only on
    ///   latest OS versions (4.1+) and selected devices. Improved recording
    ///   latency is only available with Android L and selected devices.
    ///   Disabling sensors and related background services during sensitive low
    ///   latency playback is known to help with 'crackling' (buffer underruns
    ///   due to unfavourable scheduling of the audio processing thread). For
    ///   use cases that do not require the minimum latency possible, using a
    ///   higher value may help with buffer underrun problems and even with
    ///   battery draining to some degree.
    LE_NOTHROW error_msg_t LE_FASTCALL_ABI setup( unsigned int numberOfChannels, unsigned int sampleRate, unsigned int latencyInSamples );

    /// <B>Effect:</B> Registers <VAR>callback</VAR> to be called periodically
    /// when the device is started in order to produce and/or consume data.<BR>
    /// The type (signature) of the passed callback function determines both the
    /// desired layout of audio data (interleaved or non-interleaved/separated
    /// channels) as well as the streaming direction (input, output or input and
    /// output aka full duplex).<BR>
    /// A different callback can be set any number of times.<BR>
    /// \param callback          A pointer to a function with a signature that
    ///                          matches one of the six documented callback types
    ///                          (InputCallback, OutputCallback,
    ///                          InputOutputCallback, InterleavedInputCallback,
    ///                          InterleavedOutputCallback, or
    ///                          InterleavedInputOutputCallback).
    /// \param pCallbackContext  A pointer to a user data structure that holds
    ///                          the callback state.
    ///
    /// <B>Preconditions:</B>
    /// - a successful setup() call
    /// - the Device instance must be in the stopped state.<BR>
    ///
    /// \note Check the SpectrumWorx SDK demo app for usage examples.
    template <typename Callback>
    LE_NOTHROW error_msg_t LE_FASTCALL_ABI setCallback( Callback callback, void * pCallbackContext );

    /// \return A pair of latency related values:
    ///         - first = approximate total latency in samples
    ///         - second = processing buffer size in samples (always less than
    ///           or equal to the first latency value + the value of
    ///           <VAR>numberOfSamples</VAR> in the callback call will never be
    ///           more than this value).
    /// <B>Preconditions:</B> a successful setCallback() call.
    typedef std::pair<unsigned int, unsigned int> LatencyAndBufferSize;
    LE_NOTHROW LatencyAndBufferSize LE_FASTCALL_ABI latency() const;

    /// @}

public:
    /// \name Callbacks
    /// @{

    typedef float const * LE_RESTRICT const * LE_RESTRICT InputData            ; ///< A pointer to an array of pointers to read-only input channel data
    typedef float       * LE_RESTRICT const * LE_RESTRICT OutputData           ; ///< A pointer to an array of pointers to output channel data
    typedef float const * LE_RESTRICT                     InterleavedInputData ; ///< A pointer to an array of interleaved read-only input channel data
    typedef float       * LE_RESTRICT                     InterleavedOutputData; ///< A pointer to an array of interleaved output channel data

    /// The callback for input/recording only operation (with non-interleaved/separated channel data).
    typedef void (*InputCallback)
    (
        void         * pContext       ,
        InputData      pInputBuffers  ,
        unsigned int   numberOfSamples
    );
    /// The callback for output/playback only operation (with non-interleaved/separated channel data).
    typedef void (*OutputCallback)
    (
        void         * pContext,
        OutputData     pOutputBuffers,
        unsigned int   numberOfSamples
    );
    /// The callback for full-duplex (input/recording + output/playback) operation (with non-interleaved/separated channel data).
    typedef void (*InputOutputCallback)
    (
        void         * pContext       ,
        InputData      pInputBuffers  ,
        OutputData     pOutputBuffers ,
        unsigned int   numberOfSamples
    );
    /// The callback for input/recording only operation (with interleaved channel data).
    typedef void (*InterleavedInputCallback)
    (
        void                 * pContext       ,
        InterleavedInputData   pInputBuffers  ,
        unsigned int           numberOfSamples
    );
    /// The callback for output/playback only operation (with interleaved channel data).
    typedef void (*InterleavedOutputCallback)
    (
        void                  * pContext,
        InterleavedOutputData   pOutputBuffers,
        unsigned int            numberOfSamples
    );
    /// The callback for full-duplex (input/recording + output/playback) operation (with interleaved channel data).
    typedef void (*InterleavedInputOutputCallback)
    (
        void                  * pContext       ,
        InterleavedInputData    pInputBuffers  ,
        InterleavedOutputData   pOutputBuffers ,
        unsigned int            numberOfSamples
    );

    /// @}

}; // class Device


////////////////////////////////////////////////////////////////////////////////
///
/// \class BlockingDevice
///
/// \brief Provides synchronized/blocking access to a Device instance. Useful
///        mostly for single-threaded command line applications.
///
////////////////////////////////////////////////////////////////////////////////

class BlockingDevice
#ifdef DOXYGEN_ONLY
#elif defined( __ANDROID__ ) && __LP64__
    : public Utility::StackPImpl<BlockingDevice, 13 * sizeof( void * )>
#elif defined( __ANDROID__ ) || defined( _WIN32 )
    : public Utility::StackPImpl<BlockingDevice, 4 * sizeof( void * )>
#else
    : public Utility::StackPImpl<BlockingDevice, 14 * sizeof( void * ) + 10 * sizeof( int )>
#endif // DOXYGEN_ONLY
{
public:
    /// \details
    /// <B>Effect:</B> Creates the blocking wrapper for the given Device object which must outlive the created BlockingDevice object.
    LE_NOTHROW  BlockingDevice( Device & );
    LE_NOTHROW ~BlockingDevice();

    /// <B>Effect:</B> Starts the associated Device instance and blocks until the stop() member function is called (from another thread).<BR>
    /// <B>Preconditions:</B> The same as for Device::start().<BR>
    LE_NOTHROW void LE_FASTCALL_ABI startAndWait();
    /// <B>Effect:</B> Stops the associated Device and unblocks the thread blocked by the call to startAndWait().<BR>
    /// <B>Preconditions:</B> A prior call to startAndWait().<BR>
    LE_NOTHROW void LE_FASTCALL_ABI stop        ();

    LE_NOTHROW Device & LE_FASTCALL_ABI device();
}; // class BlockingDevice

/// @} // group AudioIO

//------------------------------------------------------------------------------
} // namespace AudioIO
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // device_hpp
