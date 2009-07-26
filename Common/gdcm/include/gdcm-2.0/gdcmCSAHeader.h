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
#ifndef __gdcmCSAHeader_h
#define __gdcmCSAHeader_h

#include "gdcmTypes.h"
#include "gdcmDataSet.h"
#include "gdcmCSAElement.h"

namespace gdcm
{
/*
 * \brief Class for CSAHeader
 *
 * WARNING: the API of this class is subject to change in the near future. DO NOT USE.
 *
 * TODO:
 * MrEvaProtocol in 29,1020 contains ^M that would be nice to get rid of on UNIX system...
 *
 * See also
 * 5.1.3.2.4.1 MEDCOM History Information
 * and 5.1.4.3 CSA Non-Image Module
 * in 
 * http://tamsinfo.toshiba.com/docrequest/pdf/E.Soft_v2.0.pdf
 */
//-----------------------------------------------------------------------------
class DataElement;
class PrivateTag;
class GDCM_EXPORT CSAHeader
{
public :
  CSAHeader():InternalDataSet(),InternalType(UNKNOWN) {};
  ~CSAHeader() {};

  typedef enum {
    UNKNOWN = 0,
    SV10,
    NOMAGIC,
    DATASET_FORMAT,
    ZEROED_OUT
  } CSAHeaderType;

  template <typename TSwap>
  std::istream &Read(std::istream &is);

  template <typename TSwap>
  const std::ostream &Write(std::ostream &os) const;

  void Print(DataElement const &de);

  const DataSet& GetDataSet() const { return InternalDataSet; }

  CSAHeaderType GetFormat() const;

  static const PrivateTag & GetCSAImageHeaderInfoTag();
  static const PrivateTag & GetCSASeriesHeaderInfoTag();

  const CSAElement &GetCSAElementByName(const char *name);

private:
  CSAElement Dummy;
  std::set<CSAElement> InternalCSADataSet;
  DataSet InternalDataSet;
  CSAHeaderType InternalType;
};
} // end namespace gdcm
//-----------------------------------------------------------------------------
#endif //__gdcmCSAHeader_h
