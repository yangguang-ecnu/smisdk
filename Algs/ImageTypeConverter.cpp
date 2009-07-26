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
// Filename:	ImageTypeConverter.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageTypeConverter_HPP_
#define _PGImageTypeConverter_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageTypeConverter.h"

namespace PGAlgs
{

template <class T, class U>
ImageTypeConverter<T, U>::ImageTypeConverter(void) 
{ 
	m_oImageObject = 0;
};

template <class T, class U>
ImageTypeConverter<T, U>::~ImageTypeConverter(void) 
{ 
	if (m_oImageObject) 
	{
		delete m_oImageObject;
		m_oImageObject = 0;
	}
};

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

	/*
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
	*/

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
			//float tempVal = (float)(inVal - tMinValue)*invTRange*uRange + (float)uMinValue;
			//outBuf[imgIter] = (U)tempVal;			
			outBuf[imgIter] = (U)inVal;			
			imgIter++;

		}		
	} 	

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageTypeConverter<short, float>; 
	template class ImageTypeConverter<float, short>; 
	template class ImageTypeConverter<unsigned char, float>;
	template class ImageTypeConverter<float, unsigned char>;
#endif




};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageTypeConverter_HPP_
