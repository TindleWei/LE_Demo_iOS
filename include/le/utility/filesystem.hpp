////////////////////////////////////////////////////////////////////////////////
///
/// \file filesystem.hpp
/// --------------------
///
/// Copyright (c) 2012 - 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef filesystem_hpp__656E7522_88F0_4EA5_9E7D_3944AC55FE77
#define filesystem_hpp__656E7522_88F0_4EA5_9E7D_3944AC55FE77
#pragma once
//------------------------------------------------------------------------------
#include "abi.hpp"

#include "sys/types.h"

#include "fcntl.h"

#include <cassert>
#include <cstddef>
#include <utility>
//------------------------------------------------------------------------------
#ifdef __ANDROID__
    #include "jni.h"
    struct AAssetManager;
    struct ANativeActivity;
    namespace ndk_helper { class JNIHelper; }
#endif // __ANDROID__
//------------------------------------------------------------------------------
#if defined( _MSC_VER ) && !defined( LE_SDK_NO_AUTO_LINK )
    #ifdef _WIN64
        #pragma comment( lib, "Utility_Win64_x86-64_SSE3.lib" )
    #else // _WIN32
        #pragma comment( lib, "Utility_Win32_x86-32_SSE2.lib" )
    #endif // _WIN32/64
#endif // _MSC_VER && !LE_SDK_NO_AUTO_LINK
//------------------------------------------------------------------------------
namespace LE
{
//------------------------------------------------------------------------------
namespace Utility /// \brief Root namespace for all the shared LE thingies
{
//------------------------------------------------------------------------------

/// \addtogroup Utility
/// \brief Shared, utility code used by LE SDKs and example applications.
/// \details Not to be confused with the documented API of the accompanying LE
/// SDK(s).
/// @{

/// \brief Common file locations used by most OS UIs/shells to group user data.
/// Certain OSs offer different ways/APIs to access some of the predefined 
/// locations (e.g. some locations are not even classic filesystem directories
/// but rather compressed directories within resources files).
/// For this reason, LE SDK classes that require file access provide function
/// templates that can be specialized with the values of this enum to select the
/// desired location thereby offering a portable solution to the underlying
/// problem.

enum SpecialLocations
{
    AbsolutePath   , ///< Interpret a given path as an absolute path (i.e. pass it unchanged to the underlying function)
    AppData        , ///< <B>Android:</B> ANativeActivity::internalDataPath / (Java) Activity::getFilesDir()<BR>
                     ///< <B>iOS:</B> NSHomeDirectory()<BR>
                     ///< <B>OSX:</B> [[NSBundle mainBundle] bundlePath]<BR>
                     ///< <B>Windows:</B> FOLDERID_ProgramData<BR>
    CWD            , ///< Current Working Directory (in effect equivalent to <VAR>AbsolutePath</VAR> on all platforms)
    Documents      , ///< <B>Android:</B> same as AppData<BR>
                     ///< <B>iOS&OSX:</B> NSDocumentDirectory<BR>
                     ///< <B>Windows:</B> FOLDERID_PublicDocuments<BR>
    Library        , ///< <B>Android:</B> same as AppData<BR>
                     ///< <B>iOS&OSX:</B> NSLibraryDirectory<BR>
                     ///< <B>Windows:</B> FOLDERID_Libraries<BR>
    Resources      , ///< <B>Android:</B> APK assets<BR>
                     ///< <B>iOS&OSX:</B> [[NSBundle mainBundle] resourcePath]<BR>
                     ///< <B>Windows:</B> same as AppData<BR>
    ExternalStorage, ///< <B>Android:</B> ANativeActivity::externalDataPath / (Java) Environment.getExternalStorageAppFilesDirectory()<BR>
                     ///< <B>iOS&OSX:</B> N/A<BR>
                     ///< <B>Windows:</B> N/A<BR>
    Temporaries      ///< <B>Android:</B> (Java) Activity::getCacheDir()<BR>
                     ///< <B>iOS:</B> <VAR>AppData</VAR>/tmp<BR>
                     ///< <B>OSX:</B> /var/tmp<BR>
                     ///< <B>Windows:</B> ::GetTempPath()<BR>
}; // enum SpecialLocation


#if defined( __ANDROID__ ) || defined( DOXYGEN_ONLY )
////////////////////////////////////////////////////////////////////////////////
///
/// \class ResourceFile
///
////////////////////////////////////////////////////////////////////////////////

class ResourceFile
{
public:
    class MemoryMapping;
    class Stream;

    static LE_NOTHROWNOALIAS MemoryMapping LE_FASTCALL_ABI map ( char const * relativeFilePath );
    static LE_NOTHROWNOALIAS Stream        LE_FASTCALL_ABI open( char const * relativeFilePath );
}; // class ResourceFile
#endif // __ANDROID__


////////////////////////////////////////////////////////////////////////////////
///
/// \class File
///
////////////////////////////////////////////////////////////////////////////////

class File
{
public:
    template <SpecialLocations> struct Impl { typedef File type; };

public:
    class MemoryMapping;
    class Stream       ;

    template <SpecialLocations location> static LE_NOTHROWNOALIAS typename Impl<location>::type::MemoryMapping LE_FASTCALL_ABI map ( char const * relativeFilePath                     );
    template <SpecialLocations location> static LE_NOTHROWNOALIAS typename Impl<location>::type::Stream        LE_FASTCALL_ABI open( char const * relativeFilePath, unsigned int flags );
}; // class ResourceFile


////////////////////////////////////////////////////////////////////////////////
///
/// \class File::MemoryMapping
///
////////////////////////////////////////////////////////////////////////////////

class File::MemoryMapping
    :
    private std::pair<char const *, char const *>
{
public:
    typedef std::pair<char const *, char const *> Range;

    typedef char const value_type;

    LE_NOTHROWNOALIAS  MemoryMapping();
    LE_NOTHROW         MemoryMapping( MemoryMapping && ) LE_NOEXCEPT;
    LE_NOTHROWNOALIAS ~MemoryMapping();

    LE_NOTHROWNOALIAS value_type * LE_FASTCALL_ABI begin() const { return first ; }
    LE_NOTHROWNOALIAS value_type * LE_FASTCALL_ABI end  () const { return second; }

    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI size() const { return static_cast<unsigned int>( end() - begin() ); }

    LE_NOTHROWNOALIAS char LE_FASTCALL_ABI operator[]( unsigned int const index ) const { assert( index < size() ); return begin()[ index ]; }

    LE_NOTHROWNOALIAS bool LE_FASTCALL_ABI operator! () const;

    LE_NOTHROW MemoryMapping & LE_FASTCALL_ABI operator=( MemoryMapping && ) LE_NOEXCEPT;

private: friend class File;
    LE_NOTHROWNOALIAS explicit MemoryMapping( Range const & );

private:
    explicit MemoryMapping( MemoryMapping const & );
    void     operator=    ( MemoryMapping const & );
}; // class MemoryMapping


////////////////////////////////////////////////////////////////////////////////
///
/// \class File::Stream
///
////////////////////////////////////////////////////////////////////////////////

class File::Stream
{
public:
    LE_NOTHROWNOALIAS  Stream();
    LE_NOTHROW         Stream( Stream && ) LE_NOEXCEPT;
    LE_NOTHROWNOALIAS ~Stream();


    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI read ( void       * pBuffer, unsigned int numberOfBytesToRead  );
    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI write( void const * pBuffer, unsigned int numberOfBytesToWrite );

    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI size    (                                 ) const;
    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI position(                                 ) const;
    LE_NOTHROWNOALIAS bool         LE_FASTCALL_ABI seek    ( int offset, unsigned int whence )      ;

    LE_NOTHROWNOALIAS int LE_FASTCALL_ABI asPOSIXFile( ::off_t & startOffset, std::size_t & size ) const;


    LE_NOTHROW        Stream & LE_FASTCALL_ABI operator=( Stream && ) LE_NOEXCEPT;
    LE_NOTHROWNOALIAS bool     LE_FASTCALL_ABI operator! () const;

private: friend class File;
    LE_NOTHROWNOALIAS Stream( int );

private:
    explicit Stream   ( Stream const & );
    void     operator=( Stream const & );

private:
    int handle_;
}; // class Stream


#if defined( __ANDROID__ ) || defined( DOXYGEN_ONLY )

////////////////////////////////////////////////////////////////////////////////
///
/// \class ResourceFile::MemoryMapping
///
////////////////////////////////////////////////////////////////////////////////

class ResourceFile::MemoryMapping
{
public:
    LE_NOTHROWNOALIAS  MemoryMapping();
    LE_NOTHROW         MemoryMapping( MemoryMapping && ) LE_NOEXCEPT;
    LE_NOTHROWNOALIAS ~MemoryMapping();


    typedef char const value_type;

    LE_NOTHROWNOALIAS value_type * LE_FASTCALL_ABI begin() const;
    LE_NOTHROWNOALIAS value_type * LE_FASTCALL_ABI end  () const;

    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI size() const;

    LE_NOTHROWNOALIAS char LE_FASTCALL_ABI operator[]( unsigned int index ) const;


    LE_NOTHROW        MemoryMapping & LE_FASTCALL_ABI operator=( MemoryMapping && );
    LE_NOTHROWNOALIAS bool            LE_FASTCALL_ABI operator! () const;
    LE_NOTHROWNOALIAS LE_FASTCALL_ABI explicit operator bool () const;

private: friend class ResourceFile;
    explicit MemoryMapping( void * );
    explicit MemoryMapping( MemoryMapping const & );

private:
    void * handle_;
}; // class MemoryMapping


////////////////////////////////////////////////////////////////////////////////
///
/// \class ResourceFile::Stream
///
////////////////////////////////////////////////////////////////////////////////

class ResourceFile::Stream
{
public:
    LE_NOTHROWNOALIAS  Stream();
    LE_NOTHROW         Stream( Stream && );
    LE_NOTHROWNOALIAS ~Stream();


    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI read( void * pBuffer, unsigned int numberOfBytesToRead );

    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI size    (                                 ) const;
    LE_NOTHROWNOALIAS unsigned int LE_FASTCALL_ABI position(                                 ) const;
    LE_NOTHROWNOALIAS bool         LE_FASTCALL_ABI seek    ( int offset, unsigned int whence )      ;

    LE_NOTHROWNOALIAS int LE_FASTCALL_ABI asPOSIXFile( ::off_t & startOffset, std::size_t & size ) const;


    LE_NOTHROW        Stream & LE_FASTCALL_ABI operator=( Stream && );
    LE_NOTHROWNOALIAS bool     LE_FASTCALL_ABI operator! () const;
    LE_NOTHROWNOALIAS LE_FASTCALL_ABI explicit operator bool () const;

private: friend class ResourceFile;
    explicit Stream( void * );
    Stream( Stream const & );

private:
    void * handle_;
}; // class Stream

template <> struct File::Impl<Resources> { typedef ResourceFile type; };
template <> LE_NOTHROWNOALIAS inline typename File::Impl<Resources>::type::MemoryMapping LE_FASTCALL_ABI File::map <Resources>( char const * const relativeFilePath                     ) { return ResourceFile::map ( relativeFilePath ); }
template <> LE_NOTHROWNOALIAS inline typename File::Impl<Resources>::type::Stream        LE_FASTCALL_ABI File::open<Resources>( char const * const relativeFilePath, unsigned int flags ) { return ResourceFile::open( relativeFilePath ); }

#else // other than Android

class ResourceFile : public File
{
public:
    static LE_NOTHROWNOALIAS MemoryMapping LE_FASTCALL_ABI map ( char const * const relativeFilePath ) { return File::map <Resources>( relativeFilePath           ); }
    static LE_NOTHROWNOALIAS Stream        LE_FASTCALL_ABI open( char const * const relativeFilePath ) { return File::open<Resources>( relativeFilePath, O_RDONLY ); }
}; // class ResourceFile

#endif // __ANDROID__ || DOXYGEN_ONLY


template <SpecialLocations location>
LE_NOTHROWNOALIAS char const * LE_FASTCALL_ABI fullPath( char const * relativeFilePath );


template <SpecialLocations location, bool writeAccess>
LE_NOTHROWNOALIAS bool LE_FASTCALL_ABI accessible();


#if defined( __ANDROID__ ) || defined( DOXYGEN_ONLY )
    /// <B>Android specific:</B> in order to access the app's assets and
    /// app-specific directories, access to the app's AAssetManager and activity
    /// objects is required which should therefore be set/provided once on app
    /// startup using one of the below functions.
    ///
    /// The Android environment allows for more than one way to access the
    /// mentioned objects and, as a courtesy, the LE.Utility library tries to
    /// cover all use cases. The choice of which setAppContext() overload to use
    /// is solely one of user preference.
    ///
    /// This support requires linking with the "android" system-provided
    /// library.

    LE_NOTHROW void LE_FASTCALL_ABI setAppContext( ::ANativeActivity const &, ::ndk_helper::JNIHelper const & );
    LE_NOTHROW void LE_FASTCALL_ABI setAppContext( ::ANativeActivity const &                                  ); ///< \overload
    LE_NOTHROW void LE_FASTCALL_ABI setAppContext( ::JavaVM &, ::jobject activity, ::jobject assetManager     ); ///< \overload
    LE_NOTHROW void LE_FASTCALL_ABI setAppContext( ::JavaVM &, ::jobject activity                             ); ///< \overload

    LE_NOTHROW LE_CONST_FUNCTION ::AAssetManager & LE_FASTCALL_ABI resourceManager();
#endif // __ANDROID__

/// @} // group Utility

//------------------------------------------------------------------------------
} // namespace Utility
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // filsystem_hpp
