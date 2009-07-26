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
#ifndef __gdcmSwapCode_h
#define __gdcmSwapCode_h

#include "gdcmTypes.h"
#include <iostream>

namespace gdcm 
{

/* \brief SwapCode class
 */
class GDCM_EXPORT SwapCode
{
public:
  typedef enum {
    Unknown         = 0,
    LittleEndian    = 1234,
    BigEndian       = 4321,
    BadLittleEndian = 3412,
    BadBigEndian    = 2143
  } SwapCodeType;

  operator SwapCode::SwapCodeType() const { return SwapCodeValue; }
  SwapCode(SwapCodeType sc = Unknown):SwapCodeValue(sc) { }
  static const char* GetSwapCodeString(SwapCode const & sc);

  friend std::ostream& operator<<(std::ostream& os, const SwapCode& sc);
protected:
  static int GetIndex(SwapCode const & sc);

private:
  SwapCodeType SwapCodeValue;
};
//-----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const SwapCode& sc)
{
  os << SwapCode::GetSwapCodeString(sc);
  return os;
}

} // end namespace gdcm

#endif //__gdcmSwapCode_h
