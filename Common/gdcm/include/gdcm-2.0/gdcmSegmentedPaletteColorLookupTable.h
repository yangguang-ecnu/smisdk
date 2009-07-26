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

#ifndef __gdcmSegmentedPaletteColorLookupTable_h
#define __gdcmSegmentedPaletteColorLookupTable_h

#include "gdcmLookupTable.h"

namespace gdcm
{
  
class GDCM_EXPORT SegmentedPaletteColorLookupTable : public LookupTable
{
public:
  SegmentedPaletteColorLookupTable();
  ~SegmentedPaletteColorLookupTable();
  void Print(std::ostream &) const {}

  void SetLUT(LookupTableType type, const unsigned char *array,
    unsigned int length);

};

} // end namespace gdcm

#endif //__gdcmSegmentedPaletteColorLookupTable_h
