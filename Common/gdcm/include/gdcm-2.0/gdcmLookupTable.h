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

#ifndef __gdcmLookupTable_h
#define __gdcmLookupTable_h

#include "gdcmTypes.h"
#include "gdcmObject.h"
#include <stdlib.h>

namespace gdcm
{
  
class LookupTableInternal;
class GDCM_EXPORT LookupTable : public Object
{
public:
  typedef enum {
    RED = 0,  // Keep RED == 0 
    GREEN,
    BLUE,
    GRAY,
    UNKNOWN
  } LookupTableType;

  LookupTable();
  ~LookupTable();
  void Print(std::ostream &) const {}

  void Allocate( unsigned short bitsample = 8 );
  // Generic interface:
  void InitializeLUT(LookupTableType type, unsigned short length,
    unsigned short subscript, unsigned short bitsize);
  unsigned short GetLUTLength(LookupTableType type) const;
  void SetLUT(LookupTableType type, const unsigned char *array,
    unsigned int length);
  void GetLUT(LookupTableType type, unsigned char *array, unsigned int &length) const;
  void GetLUTDescriptor(LookupTableType type, unsigned short &length,
    unsigned short &subscript, unsigned short &bitsize) const;

  // RED / GREEN / BLUE specific:
  void InitializeRedLUT(unsigned short length, unsigned short subscript,
    unsigned short bitsize);
  void SetRedLUT(const unsigned char *red, unsigned int length);
  void InitializeGreenLUT(unsigned short length, unsigned short subscript,
    unsigned short bitsize);
  void SetGreenLUT(const unsigned char *green, unsigned int length);
  void InitializeBlueLUT(unsigned short length, unsigned short subscript,
    unsigned short bitsize);
  void SetBlueLUT(const unsigned char *blue, unsigned int length);

  void Decode(std::istream &is, std::ostream &os);

  LookupTable(LookupTable const &lut):Object(lut)
    {
    abort();
    }

  bool GetBufferAsRGBA(unsigned char *rgba) const;
  const unsigned char *GetPointer() const;
  bool WriteBufferAsRGBA(unsigned char *rgba);

private:
protected:
  LookupTableInternal *Internal;
  unsigned short BitSample; // refer to the pixel type (not the bit size of LUT)
  bool IncompleteLUT:1;
};

} // end namespace gdcm

#endif //__gdcmLookupTable_h
