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
#ifndef __gdcmXMLPrivateDictReader_h
#define __gdcmXMLPrivateDictReader_h

#include "gdcmTableReader.h"
#include "gdcmDict.h"
#include "gdcmDictEntry.h"
#include "gdcmTag.h"

namespace gdcm
{
/**
 * \brief Class for representing a XMLPrivateDictReader
 * \note bla
 * Will read the Private.xml file
 */
class GDCM_EXPORT XMLPrivateDictReader : public TableReader
{
public:
  XMLPrivateDictReader();
  ~XMLPrivateDictReader() {}

  void StartElement(const char *name, const char **atts);
  void EndElement(const char *name);
  void CharacterDataHandler(const char *data, int length);

  const PrivateDict & GetPrivateDict() { return PDict; }

protected:
  void HandleEntry(const char **atts);
  void HandleDescription(const char **atts);

private:
  PrivateDict PDict;
  PrivateTag CurrentTag;
  DictEntry CurrentDE;
  bool ParsingDescription;
  std::string Description;
};

} // end namespace gdcm

#endif //__gdcmXMLPrivateDictReader_h
