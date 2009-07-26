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
#ifndef __gdcmCP246ExplicitDataElement_h
#define __gdcmCP246ExplicitDataElement_h

#include "gdcmDataElement.h"

namespace gdcm
{
// Data Element (CP246Explicit)
/**
 * \brief Class to read/write a DataElement as CP246Explicit Data Element
 * \note Some system are producing SQ, declare them as UN, but encode the SQ as 'Explicit'
 * instead of Implicit
 */
class GDCM_EXPORT CP246ExplicitDataElement : public DataElement
{
public:
  VL GetLength() const;

  template <typename TSwap>
  std::istream &Read(std::istream &is);

  template <typename TSwap>
  std::istream &ReadWithLength(std::istream &is, VL & length);

  // PURPOSELY do not provide an implementation for writing !
  //template <typename TSwap>
  //const std::ostream &Write(std::ostream &os) const;
};

} // end namespace gdcm

#include "gdcmCP246ExplicitDataElement.txx"

#endif //__gdcmCP246ExplicitDataElement_h

