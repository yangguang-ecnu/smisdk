// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	ImageEndianSwapper.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGIMAGEENDIANSWAPPER_HPP_
#define _PGIMAGEENDIANSWAPPER_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageEndianSwapper.h"

namespace PGAlgs
{

template <class T, class U>
ImageEndianSwapper<T, U>::ImageEndianSwapper(void) 
{ 
	m_oImageObject = 0;
};

template <class T, class U>
ImageEndianSwapper<T, U>::~ImageEndianSwapper(void)
{ 
	if (m_oImageObject) delete m_oImageObject;
};

template <class T, class U>
bool ImageEndianSwapper<T, U>::Execute() {
	// swap endians here
	if (sizeof(T) != sizeof(U) || sizeof(T)!=sizeof(short)) {
		LOG2("ImageEndianSwapper: Error swapping endians. Types different sizes: %d - %d", sizeof(T), sizeof(U));
		return false;
	}

	if (!m_iDataObject) {
		LOG0("ImageEndianSwapper: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) {
		LOG0("ImageEndianSwapper: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageEndianSwapper: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageEndianSwapper: Error: invalid image");
		return false;
	}

	if (m_oImageObject==NULL)
	{
		m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
	}
	U* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageEndianSwapper: Error: invalid out image");
		return false;
	}

	{				
		long imgIter = 0;				
		while (imgIter< iRows*iColumns) {

			T inVal = *(inBuf+imgIter);
			U outVal = swapEndians(inVal);				
			*(outBuf+imgIter) = outVal;			
			imgIter++;

		}		
	} 	
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}

/*
template <class T, class U>
const U ImageEndianSwapper<T, U>::swapEndians(const T &iValue) {
	T highByte = iValue;
	U lowByte = iValue;

	highByte = iValue; highByte = highByte<<8;	
	lowByte  = iValue; lowByte = lowByte> >8;

	lowByte |= highByte;

	return (U)lowByte;
}
*/

template <class T, class U>
const U ImageEndianSwapper<T, U>::swapEndians(const T &iValue) {

	unsigned short  highByte = iValue;
	unsigned short  lowByte = iValue;

	lowByte		= ((unsigned short)iValue);
	lowByte		= lowByte%256;//; highByte = highByte<<8;	;

	highByte	= ((unsigned short)iValue);
	highByte	= (highByte/256)%256;// lowByte = lowByte> >8;

	//lowByte |= highByte;
	lowByte = (256*lowByte) + highByte;
	return (U)lowByte;
}

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


#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageEndianSwapper<short, short>; 
	template class ImageEndianSwapper<unsigned char, unsigned char>; 
#endif




};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGIMAGEENDIANSWAPPER_HPP_
