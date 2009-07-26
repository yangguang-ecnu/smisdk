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
#ifndef __gdcmCodec_h
#define __gdcmCodec_h

#include "gdcmCoder.h"
#include "gdcmDecoder.h"

namespace gdcm
{

/** \class Codec
 */
class GDCM_EXPORT Codec : public Coder, public Decoder
{
};

} // end namespace gdcm

#endif //__gdcmCodec_h
