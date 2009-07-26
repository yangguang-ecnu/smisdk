// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	ImageTypeConverter.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageTypeConverter_HPP_
#define _PGImageTypeConverter_HPP_
//////////////////////////////////////////////////////////////////////////


#include "ImageTypeConverter.h"

namespace PGAlgs
{
template <class T, class U>
bool ImageTypeConverter<T, U>::Execute() {
	// swap endians here
	//if (sizeof(T) != sizeof(U) || sizeof(T)!=sizeof(short)) {
	//	LOG2("ImageTypeConverter: Error swapping endians. Types different sizes: %d - %d", sizeof(T), sizeof(U));
	//	return false;
	//}

	if (!m_iDataObject) {
		LOG0("ImageTypeConverter: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) {
		LOG0("ImageTypeConverter: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageTypeConverter: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageTypeConverter: Error: invalid image");
		return false;
	}

	PGCore::PixelBase<U> uPixel;
	U uMinValue = uPixel.GetMinValue(), uMaxValue = uPixel.GetMaxValue();
	float uRange = (uMaxValue - uMinValue), invURange = 1.0f/(uRange);

	PGCore::PixelBase<T> tPixel;
	T tMinValue = tPixel.GetMaxValue(), tMaxValue = tPixel.GetMinValue();
	if (!inImage->GetDataRange(&tMinValue, &tMaxValue))
	{
		LOG0("ImageTypeConverter: Error: failed to get range of input image");
		return false;
	}

	float tRange = (tMaxValue - tMinValue), invTRange = 1;
	if (tRange>=0)
	{
		invTRange = 1.0f/tRange;
	} 

	if (m_oImageObject == NULL)
		m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
	U* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageTypeConverter: Error: invalid out image");
		return false;
	}

	{				
		long imgIter = 0;	

		while (imgIter< iRows*iColumns) 
		{
			T inVal = inBuf[imgIter];
			float tempVal = (float)(inVal - tMinValue)*invTRange*uRange + (float)uMinValue;
			outBuf[imgIter] = (U)tempVal;			
			imgIter++;

		}		
	} 	

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}

/*
template <class T, class U>
const U ImageTypeConverter<T, U>::swapEndians(const T &iValue) {
	T highByte = iValue;
	U lowByte = iValue;

	highByte = iValue; highByte = highByte<<8;	
	lowByte  = iValue; lowByte = lowByte> >8;

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
#endif	// _PGImageTypeConverter_HPP_
