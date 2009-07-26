/*=========================================================================

  Program: GDCM (Grass Root DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2008 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __gdcmTypes_h
#define __gdcmTypes_h

//#include "gdcmConfigure.h"

#include "gdcmWin32.h"

#include "gdcmException.h"

//-----------------------------------------------------------------------------
#ifdef CMAKE_HAVE_STDINT_H
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif // __STDC_LIMIT_MACROS
#include <stdint.h>
//#undef __STDC_LIMIT_MACROS
#else
#ifdef CMAKE_HAVE_INTTYPES_H
// Old system only have this
#include <inttypes.h>   // For uint8_t uint16_t and uint32_t
#else
// Broken plateforms do not respect C99 and do not provide those typedef
// Special case for recent Borland compiler, comes with stdint.h
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x0560) || defined(__MINGW32__)
typedef  signed char         int8_t;
typedef  signed short        int16_t;
typedef  signed int          int32_t;
typedef  unsigned char       uint8_t;
typedef  unsigned short      uint16_t;
typedef  unsigned int        uint32_t;
typedef  unsigned __int64    uint64_t;
#elif defined(_MSC_VER)
#include "stdint.h"
#else
#error "Sorry your plateform is not supported"
#endif // defined(_MSC_VER) || defined(__BORLANDC__) && (__BORLANDC__ < 0x0560)  || defined(__MINGW32__)
#endif // CMAKE_HAVE_INTTYPES_H
#endif // CMAKE_HAVE_STDINT_H

// Basically for VS6 and bcc 5.5.1:
#ifndef UINT32_MAX
#define UINT32_MAX    (4294967295U)
#endif

//-----------------------------------------------------------------------------
#endif //__gdcmTypes_h
