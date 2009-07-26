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
#ifndef __gdcmDumper_h
#define __gdcmDumper_h

#include "gdcmPrinter.h"

// Use it to simply dump value read from the file. No interpretation is done.
// But it is real fast ! Almost no overhead
namespace gdcm
{

// It's a sink there is no output
class GDCM_EXPORT Dumper : public Printer
{
public:
  Dumper() { PrintStyle = CONDENSED_STYLE; }
  ~Dumper() {};
};

} // end namespace gdcm

#endif //__gdcmDumper_h
