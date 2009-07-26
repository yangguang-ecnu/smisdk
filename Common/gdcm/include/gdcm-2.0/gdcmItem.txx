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
#ifndef __gdcmItem_txx
#define __gdcmItem_txx

#include "gdcmItem.h"

namespace gdcm
{

template <typename TDE>
VL Item::GetLength() const
{
  if( ValueLengthField.IsUndefined() )
    {
    assert( !NestedDataSet.GetLength<TDE>().IsUndefined() );
    // Item Start             4
    // Item Length            4
    // DataSet                ?
    // Item End Delimitation  4
    // Item End Length        4
    return TagField.GetLength() /* 4 */ + ValueLengthField.GetLength() /* 4 */
      + NestedDataSet.GetLength<TDE>() + 4 + 4;
    }
  else
    {
    // Item Start             4
    // Item Length            4
    // DataSet                ?
    return TagField.GetLength() /* 4 */ + ValueLengthField.GetLength() /* 4 */
      + ValueLengthField;
    }
}

} // end namespace gdcm

#endif // __gdcmItem_txx
