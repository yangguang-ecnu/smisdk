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

#ifndef __gdcmWriter_h
#define __gdcmWriter_h

#include "gdcmFile.h"

namespace gdcm
{
/**
 * \brief Writer ala DOM (Document Object Model)
 *
 * Detailled description here
 * To avoid GDCM being yet another broken DICOM lib we try to 
 * be user level and avoid writing illegal stuff (odd length,
 * non-zero value for Item start/end length ...)
 * Therefore you cannot (well unless you are really smart) write
 * DICOM with even length tag.
 * All the checks are consider basics:
 * - Correct Meta Information Header (see gdcm::FileMetaInformation)
 * - Zero value for Item Length (0xfffe, 0xe00d/0xe0dd)
 * - Even length for any elements
 * - Alphabetical order for elements (garanteed by design of internals)
 * - 32bits VR will be rewritten with 00
 */
class FileMetaInformation;
class GDCM_EXPORT Writer
{
public:
  Writer():Stream(),F(new File),CheckFileMetaInformation(true) {}
  virtual ~Writer();

  virtual bool Write(); // Execute()
  void SetFileName(const char *filename) {
    //std::cerr << "Stream: " << filename << std::endl;
    Stream.open(filename, std::ios::out | std::ios::binary );
    assert( Stream.is_open() );
    assert( !Stream.fail() );
    //std::cerr << Stream.is_open() << std::endl;
#ifndef NDEBUG
    DebugFileName = filename;
#endif
   }

  void SetFile(const File& f) { F = &f; }
  File &GetFile() { return *F; }

  void SetCheckFileMetaInformation(bool b) { CheckFileMetaInformation = b; }

protected:
  std::ofstream Stream;

private:
  SmartPointer<File> F;
  bool CheckFileMetaInformation;
#ifndef NDEBUG
  std::string DebugFileName;
#endif
};

} // end namespace gdcm

#endif //__gdcmWriter_h
