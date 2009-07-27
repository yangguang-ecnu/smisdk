// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
//
// Filename:	ImageTransferFunctor.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageTransferFunctor_H_
#define _PGImageTransferFunctor_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageTransferFunctor : public public ImageFilterBase<T, U>
{
public:
	ImageTransferFunctor(void) { m_oImageObject = 0;};

	bool Execute();

public:	
	virtual ~ImageTransferFunctor(void) { if (m_oImageObject) delete m_oImageObject;};

protected:

	

private:
	PGCore::Image<U>	*m_oImageObject;
};


template <class T, class U>
bool ImageTransferFunctor<T, U>::Execute() {
	
	if (!m_iDataObject) {
		LOG0("ImageTransferFunctor: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) {
		LOG0("ImageTransferFunctor: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageTransferFunctor: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageTransferFunctor: Error: invalid image");
		return false;
	}

	if (m_oImageObject == NULL)
		m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
	U* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageTransferFunctor: Error: invalid out image");
		return false;
	}

	{				
		long imgIter = 0;	

		while (imgIter< iRows*iColumns) 
		{
			T inVal = inBuf[imgIter];
			outBuf[imgIter] = (U)inVal;			
			imgIter++;

		}		
	} 	

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}

/*
template <class T, class U>
const U ImageTransferFunctor<T, U>::swapEndians(const T &iValue) {
	T highByte = iValue;
	U lowByte = iValue;

	highByte = iValue; highByte = highByte<<8;	
	lowByte  = iValue; lowByte = lowByte>>8;

	lowByte |= highByte;

	return (U)lowByte;
}
*/

/*
Function Reverse (N:LongInt) : LongInt ;
     Var B0, B1, B2, B3 : Byte ;
    Begin
        B0 := N Mod 256 ;
        N  := N Div 256 ;
        B1 := N Mod 256 ;
        N  := N Div 256 ;
        B2 := N Mod 256 ;
        N  := N Div 256 ;
        B3 := N Mod 256 ;
        Reverse := (((B0 * 256 + B1) * 256 + B2) * 256 + B3) ;
    End ;
*/


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageTransferFunctor_H_
