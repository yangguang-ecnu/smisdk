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
#ifndef __gdcmException_h
#define __gdcmException_h

#include <string>
#include <stdexcept>
#include <sstream> // ostringstream

#include "gdcmTypes.h"

#include <assert.h> // FIXME


namespace gdcm
{

/** \class Exception
 * \brief Standard exception handling object.
 *
 */
class Exception : public std::exception
{
public:
  Exception(const char *desc = "None", 
            const char *file = __FILE__,
            unsigned int lineNumber = __LINE__,
            const char *loc = ""/*__FUNCTION__*/ )
  {
    Description = desc;
    File = file;
    Line = lineNumber;
    Location = loc;
  }
  
  virtual ~Exception() throw() {}

  /** Assignment operator. */
  Exception &operator= ( const Exception &orig )
  {
//TODO
(void)orig;
    return *this;
  }
  
  /** Equivalence operator. */
  virtual bool operator==( const Exception &orig )
  {
(void)orig;
    return true;
  }
          
  // Multiple calls to what ??
  const char* what() const throw()
    { 
    static std::string strwhat;
    std::ostringstream oswhat;
    oswhat << File << ":" << Line << ":\n";
    oswhat << Description;
    strwhat = oswhat.str();
    return strwhat.c_str();
    }

  const char * GetDescription() const { return Description.c_str(); }

private:
  std::string  Description;
  std::string  File;
  unsigned int Line;
  std::string  Location;
};

} // end namespace gdcm

#endif

