// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	ImageBitClipper.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGIMAGEBITCLIPPER_HPP_
#define _PGIMAGEBITCLIPPER_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageBitClipper.h"

namespace PGAlgs
{


template <class T, class U>
ImageBitClipper<T, U>::ImageBitClipper(const int iHighBitsToClip, bool iLeaveSignBit=1) 
{ 
	m_oImageObject = 0;
	int pLeaveSignBit = iLeaveSignBit;
	int bitsToMark = iHighBitsToClip-pLeaveSignBit;

	int i=0;
	m_bitClipMask=0;
	while (i<bitsToMark)
	{
		m_bitClipMask += (unsigned short)pow(2.0f, (float)(i));
		i++;
	}

	// now move these bits
	int bitsToMove = 8*sizeof(T)-iHighBitsToClip;

	m_bitClipMask = m_bitClipMask<<bitsToMove;
	m_bitClipMask = ~m_bitClipMask;

};

template <class T, class U>
ImageBitClipper<T, U>::~ImageBitClipper(void)
{ 
	if (m_oImageObject) delete m_oImageObject;
};

template <class T, class U>
bool ImageBitClipper<T, U>::Execute() 
{
	// swap endians here
	if (sizeof(T) != sizeof(U) || sizeof(T)!=sizeof(short)) 
	{
		LOG2("ImageBitClipper: Error clipping bits. Types different sizes: %d - %d", sizeof(T), sizeof(U));
		return false;
	}

	if (!m_iDataObject) {
		LOG0("ImageBitClipper: Error: no input");
		return false;
	}
	
	// also check base image types here
	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) {
		LOG0("ImageBitClipper: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageBitClipper: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageBitClipper: Error: invalid image");
		return false;
	}

	if (m_oImageObject==NULL)
	{
		m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
	}
	U* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageBitClipper: Error: invalid out image");
		return false;
	}

	{				
		long imgIter = 0;				
		while (imgIter< iRows*iColumns) 
		{
			T inVal = *(inBuf+imgIter);
			U outVal = clipBits(inVal);				
			*(outBuf+imgIter) = outVal;			
			imgIter++;
		}		
	} 	
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}

template <class T, class U>
const U ImageBitClipper<T, U>::clipBits(const T &iValue) 
{
	unsigned short usValue = (unsigned short)iValue;
	usValue = usValue & m_bitClipMask;	
	return (U)usValue;
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageBitClipper<short, short>; 
	template class ImageBitClipper<unsigned char, unsigned char>; 
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGIMAGEBITCLIPPER_HPP_
