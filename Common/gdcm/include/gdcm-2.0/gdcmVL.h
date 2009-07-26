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
#ifndef __gdcmVL_h
#define __gdcmVL_h

#include "gdcmTypes.h"

#include <iostream>

/* \brief Value Length
 * \warning this is a 4bytes value ! Do not try to use it for 2bytes value
 * length
 */
namespace gdcm
{

class GDCM_EXPORT VL
{
public:
  VL(uint32_t vl = 0) : ValueLength(vl) { }

  bool IsUndefined() const {
    return ValueLength == 0xFFFFFFFF;
  }
  void SetToUndefined() {
    ValueLength = 0xFFFFFFFF;
  }

  bool IsOdd() const {
    return !IsUndefined() && ValueLength % 2;
  }

  VL& operator+=(VL const &vl) {
    ValueLength += vl.ValueLength;
    return *this;
  }
  VL& operator++() {
    ++ValueLength;
    return *this;
  }
  VL operator++(int) {
    uint32_t tmp(ValueLength);
    ++ValueLength;
    return tmp;
  }

  operator uint32_t () const { return ValueLength; }

  VL GetLength() const {
	  // VL cannot know it's length...well in implicit yes...
	  // TODO: need to check we cannot call this function from an Explicit element
    return 4;
  }

  friend std::ostream& operator<<(std::ostream& os, const VL& vl);

  // PURPOSELY not implemented (could not differenciate 16bits vs 32bits VL)
  //friend std::istream& operator>>(std::istream& is, VL& n);

  template <typename TSwap>
  std::istream &Read(std::istream &is)
    {
    is.read((char*)(&ValueLength), sizeof(uint32_t));
    TSwap::SwapArray(&ValueLength,1);
    return is;
    }

  template <typename TSwap>
  std::istream &Read16(std::istream &is)
    {
    uint16_t copy;
    is.read((char*)(&copy), sizeof(uint16_t));
    TSwap::SwapArray(&copy,1);
    ValueLength = copy;
    assert( ValueLength <=  65535 /*UINT16_MAX*/ ); // ?? doh !
    return is;
    }

  template <typename TSwap>
  const std::ostream &Write(std::ostream &os) const
    {
    uint32_t copy = ValueLength;
#ifndef GDCM_WRITE_ODD_LENGTH
    if( IsOdd() )
      {
      ++copy;
      }
#endif
    TSwap::SwapArray(&copy,1);
    return os.write((char*)(&copy), sizeof(uint32_t));
    }

  template <typename TSwap>
  const std::ostream &Write16(std::ostream &os) const
    {
    assert( ValueLength <=   65535 /*UINT16_MAX*/ );
    uint16_t copy = ValueLength;
#ifndef GDCM_WRITE_ODD_LENGTH
    if( IsOdd() )
      {
      ++copy;
      }
#endif
    TSwap::SwapArray(&copy,1);
    return os.write((char*)(&copy), sizeof(uint16_t));
    }



private:
  uint32_t ValueLength;
};
//----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const VL& val)
{
  os << /*std::hex <<*/ val.ValueLength;
  return os;
}

} // end namespace gdcm

#endif //__gdcmVL_h
