////////////////////////////////////////////////////////////////////////////////
///
/// \file abi.hpp
/// -------------
///
///   A collection of complier/platform specific ABI defining macros.
///
/// Copyright (c) 2010 - 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef abi_hpp__10DEE3C0_20E4_4B36_B0A8_B02ADEFD7F38
#define abi_hpp__10DEE3C0_20E4_4B36_B0A8_B02ADEFD7F38
#pragma once
//------------------------------------------------------------------------------
#ifdef __ANDROID__
#include "sys/cdefs.h"
#endif // __ANDROID__
//------------------------------------------------------------------------------
namespace LE
{
//------------------------------------------------------------------------------
namespace Utility
{
//------------------------------------------------------------------------------

#if defined( _MSC_VER ) && !defined( __clang__ )

    #define LE_NOTHROW                __declspec( nothrow                  )
    #define LE_NOEXCEPT
    #define LE_NOALIAS                __declspec( noalias                  )
    #define LE_NOTHROWNOALIAS         __declspec( nothrow noalias          )
    #define LE_RESTRICTNOALIAS        __declspec( restrict noalias         )
    #define LE_NOTHROWRESTRICTNOALIAS __declspec( nothrow restrict noalias )
    #define LE_CONST_FUNCTION         LE_NOALIAS
    #define LE_PURE_FUNCTION          LE_NOALIAS

    #define LE_CDECL        __cdecl
    #define LE_FASTCALL_ABI __fastcall

    #define LE_RESTRICT __restrict

    #define LE_OVERRIDE override
    #define LE_SEALED   sealed

    #define LE_DLL_EXPORT __declspec( dllexport )
    #define LE_DLL_IMPORT __declspec( dllimport )

    #define LE_ASSUME( condition ) __assume( condition )

#elif defined( __GNUC__ )

    #define LE_NOTHROW                __attribute__(( nothrow ))
#ifdef __clang__
    #define LE_NOEXCEPT               LE_NOTHROW
#else
    #define LE_NOEXCEPT               noexcept
#endif // __clang__
    #define LE_NOALIAS
    #define LE_NOTHROWNOALIAS         LE_NOTHROW
    #define LE_RESTRICTNOALIAS        LE_NOALIAS
    #define LE_NOTHROWRESTRICTNOALIAS LE_NOTHROW
    #define LE_CONST_FUNCTION         __attribute__(( const ))
    #define LE_PURE_FUNCTION          __attribute__(( pure  ))

    #define LE_CDECL __attribute__(( cdecl ))
    #ifdef __i386__
        #define LE_FASTCALL_ABI __attribute__(( regparm( 3 ), stdcall ))
    #else
        #define LE_FASTCALL_ABI
    #endif // __i386__

    #define LE_RESTRICT __restrict__

    #if defined( __clang__ ) || ( ( ( __GNUC__ * 10 ) + __GNUC_MINOR__ ) >= 47 )
        #define LE_OVERRIDE override
        #define LE_SEALED   final
    #else
        #define LE_OVERRIDE
        #define LE_SEALED
    #endif

    #define LE_DLL_EXPORT __attribute__(( visibility( "default" ) ))
    #define LE_DLL_IMPORT __attribute__(( visibility( "default" ) ))

    #define LE_ASSUME( condition ) do { if ( !( condition ) ) __builtin_unreachable(); } while ( 0 )

#else

    #error LE unsupported compiler

#endif

//------------------------------------------------------------------------------
} // namespace Utility
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // abi_hpp
