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
#ifndef __gdcmPatient_h
#define __gdcmPatient_h

#include "gdcmTypes.h"
#include "gdcmStudy.h"

namespace gdcm
{
/**
 * \brief
 * See PS 3.3 - 2007
 * DICOM MODEL OF THE REAL-WORLD, p 54
 */
class GDCM_EXPORT Patient
{
public:
  Patient() { 
  }

private:
  std::vector<Study> StudyList;
};

} // end namespace gdcm

#endif //__gdcmPatient_h
