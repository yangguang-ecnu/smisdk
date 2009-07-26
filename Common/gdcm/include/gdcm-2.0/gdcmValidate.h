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
#ifndef __gdcmValidate_h
#define __gdcmValidate_h

#include "gdcmFile.h"

namespace gdcm
{

class GDCM_EXPORT Validate
{
public:
  Validate();
  ~Validate();

  void SetFile(File const &f) { F = &f; }
  const File& GetValidatedFile() { return V; }

  void Validation();

protected:
  const File *F;
  File V; // Validated file
};

} // end namespace gdcm

#endif //__gdcmValidate_h
