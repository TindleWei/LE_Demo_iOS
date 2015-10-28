////////////////////////////////////////////////////////////////////////////////
///
/// \file trace.hpp
/// ---------------
///
/// Copyright (c) 2013 - 2014. Little Endian Ltd. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
#ifndef trace_hpp__0BA90AC6_C69C_428C_965B_915C2BD7781B
#define trace_hpp__0BA90AC6_C69C_428C_965B_915C2BD7781B
#pragma once
//------------------------------------------------------------------------------
#include "abi.hpp"
//------------------------------------------------------------------------------
namespace LE
{
//------------------------------------------------------------------------------
namespace Utility
{
//------------------------------------------------------------------------------

/// <B>Android specific:</B> requires linking with the "log" system-provided
/// library.

struct Tracer
{
    static LE_NOTHROWNOALIAS void LE_FASTCALL_ABI error           ( char const * pErrorString       );
    static LE_NOTHROWNOALIAS void LE_FASTCALL_ABI message         ( char const * pMessageString     );
    static LE_NOTHROWNOALIAS void                 formattedMessage( char const * pFormatString, ... );
    static LE_NOTHROWNOALIAS void                 formattedError  ( char const * pFormatString, ... );

    static char const * pTagString;
}; // struct Tracer

#ifndef NDEBUG

    #define LE_TRACE(                   formatString, ... ) LE::Utility::Tracer::formattedMessage( formatString, ##__VA_ARGS__ )
    #define LE_TRACE_IF(     condition, formatString, ... ) if ( (condition) ) LE_TRACE( formatString, ##__VA_ARGS__ )
    #define LE_TRACE_RETURN( result   , formatString, ... ) ( LE_TRACE( formatString, ##__VA_ARGS__ ), result )

#else

    #define LE_TRACE(                   formatString, ... )
    #define LE_TRACE_IF(     condition, formatString, ... )
    #define LE_TRACE_RETURN( result   , formatString, ... ) result

#endif // _DEBUG

//------------------------------------------------------------------------------
} // namespace Utility
//------------------------------------------------------------------------------
} // namespace LE
//------------------------------------------------------------------------------
#endif // trace_hpp
