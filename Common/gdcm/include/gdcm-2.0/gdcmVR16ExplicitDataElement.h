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
#ifndef __gdcmVR16ExplicitDataElement_h
#define __gdcmVR16ExplicitDataElement_h

#include "gdcmDataElement.h"

namespace gdcm
{
// Data Element (Explicit)
/**
 * \brief Class to read/write a DataElement as Explicit Data Element
 * \note This class support 16 bits when finding an unkown VR:
 * For instance:
 * Siemens_CT_Sensation64_has_VR_RT.dcm
 */
class GDCM_EXPORT VR16ExplicitDataElement : public DataElement
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

#include "gdcmVR16ExplicitDataElement.txx"

#endif //__gdcmVR16ExplicitDataElement_h

