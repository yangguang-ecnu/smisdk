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
#ifndef __gdcmByteBuffer_h
#define __gdcmByteBuffer_h

#include "gdcmTypes.h"
#include <vector>
#include <assert.h>

#error should not be used 

namespace gdcm
{
/**
 * \brief ByteBuffer
 *
 * Detailled description here
 * \note 
 * looks like a std::streambuf or std::filebuf class with the get and 
 * peek pointer
 */
class ByteBuffer
{
  static const int InitBufferSize = 1024;
public:
  ByteBuffer() : Start(0), End(0),Limit(0) {}
  char *Get(int len)
    {
    char *buffer = &Internal[0];
    if (len > Limit - End)
      {
      // FIXME avoid integer overflow
      int neededSize = len + (End - Start);
      if (neededSize  <= Limit - buffer)
        {
        memmove(buffer, Start, End - Start);
        End = buffer + (End - Start);
        Start = buffer;
        }
      else
        {
        char *newBuf;
        int bufferSize = Limit - Start;
        if ( bufferSize == 0 )
          {
          bufferSize = InitBufferSize;
          }
        do
          {
          bufferSize *= 2;
          } while (bufferSize < neededSize);
        //newBuf = malloc(bufferSize);
        try
          {
          Internal.reserve(bufferSize);
          newBuf = &Internal[0];
          }
        catch(...)
          {
          //errorCode = NoMemoryError;
          return 0;
          }
        Limit = newBuf + bufferSize;

        if (Start) 
          {
          memcpy(newBuf, Start, End - Start);
          }
        End = newBuf + (End - Start);
        Start = /*buffer =*/ newBuf;
        }
      }
    assert( (int)Internal.capacity() >= len );
    return End;
    }

  void UpdatePosition() {}
  void ShitEnd(int len) {
    End += len;
  }
  const char *GetStart() const {
    return Start;
  }

private:
  typedef std::vector<char> CharVector;
  const char *Start;
        char *End;
  const char *Limit;
  CharVector Internal;
};

} // end namespace gdcm

#endif //__gdcmByteBuffer_h
