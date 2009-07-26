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
#ifndef __gdcmPrivateTag_h
#define __gdcmPrivateTag_h

#include "gdcmTag.h"
#include "gdcmSystem.h" // FIXME

#include <iostream>
#include <iomanip>
#include <algorithm>

#include <string.h> // strlen

namespace gdcm
{

/**
 * \brief Class to represent a Private DICOM Data Element (Attribute) Tag (Group, Element, Owner)
 */

class GDCM_EXPORT PrivateTag : public Tag
{
  friend std::ostream& operator<<(std::ostream &_os, const PrivateTag &_val);
public:
  PrivateTag(uint16_t group = 0, uint16_t element = 0, const char *owner = ""):Tag(group,element),Owner(owner) {
    std::transform(Owner.begin(), Owner.end(), Owner.begin(), tolower);
  }

  const char *GetOwner() const { return Owner.c_str(); }
  void SetOwner(const char *owner) { Owner = owner; }

  bool operator<(const PrivateTag &_val) const
    {
    const Tag & t1 = *this;
    const Tag & t2 = _val;
    if( t1 == t2 )
      {
      const char *s1 = Owner.c_str();
      const char *s2 = _val.GetOwner();
      assert( s1[strlen(s1)-1] != ' ' );
      assert( s2[strlen(s2)-1] != ' ' );
      bool res = strcmp(s1, s2) < 0;
#ifndef NDEBUG
      if( *s1 && *s2 && gdcm::System::StrCaseCmp(s1,s2) == 0 && strcmp(s1,s2) != 0 )
        {
        // FIXME:
        // Typically this should only happen with the "Philips MR Imaging DD 001" vs "PHILIPS MR IMAGING DD 001"
        // or "Philips Imaging DD 001" vr "PHILIPS IMAGING DD 001"
        //assert( strcmp(Owner.c_str(), _val.GetOwner()) == 0 );
        //return true;
        res = gdcm::System::StrCaseCmp(s1,s2) < 0;
abort();
        }
#endif
      return res;
      }
    else return t1 < t2;
    }

private:
  // SIEMENS MED, GEMS_PETD_01 ...
  std::string Owner;
};

inline std::ostream& operator<<(std::ostream &os, const PrivateTag &val)
{
  //assert( !val.Owner.empty() );
  os.setf( std::ios::right );
  os << std::hex << '(' << std::setw( 4 ) << std::setfill( '0' )
    << val[0] << ',' << std::setw( 4 ) << std::setfill( '0' )
    << val[1] << ')' << std::setfill( ' ' ) << std::dec;
  os << val.Owner;
  return os;
}

} // end namespace gdcm

#endif //__gdcmPrivateTag_h

