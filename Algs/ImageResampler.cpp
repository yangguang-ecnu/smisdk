// -*- C++ -*-
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
// Filename:	ImageResampler.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageResampler_HPP_
#define _PGImageResampler_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageResampler.h"

namespace PGAlgs
{

	
template <class T, class U>
ImageResampler<T, U>::ImageResampler(void)
{ 
	m_oImageObject = 0; 
	m_sc = 1.0f; 
};

template <class T, class U>
void ImageResampler<T, U>::SetScaleFactor(const float& iScale) 
{ 
	m_sc = iScale; 
};

template <class T, class U>
const float& ImageResampler<T, U>::GetScaleFactor() const 
{ 
	return m_sc; 
};

	
template <class T, class U>
ImageResampler<T, U>::~ImageResampler(void)
{ 
	if (m_oImageObject) 
	{
		delete m_oImageObject;
		m_oImageObject = 0;
	};
}


template <class T, class U>
bool ImageResampler<T, U>::Execute() 
{
	// swap endians here
	//if (sizeof(T) != sizeof(U) || sizeof(T)!=sizeof(short)) {
	//	LOG2("ImageResampler: Error swapping endians. Types different sizes: %d - %d", sizeof(T), sizeof(U));
	//	return false;
	//}

	if (!m_iDataObject) 
	{
		LOG0("ImageResampler: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) 
	{
		LOG0("ImageResampler: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageResampler: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageResampler: Error: invalid image");
		return false;
	}

	long oR = (long)( (float)iRows*m_sc + 0.5f);
	long oC = (long)( (float)iColumns*m_sc + 0.5f);

	if (m_oImageObject == NULL)	m_oImageObject = new PGCore::Image<U>(oR, oC);
	U* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageResampler: Error: invalid out image");
		return false;
	}

	float scInv = 1.0f/m_sc;

	{				
		long iR = 0, iC = 0;
		for (int r=0; r<oR; r++)
		{
			iR = (long)(((float)r*scInv)+0.5f);
			long iR_C = iR*iColumns;
			long ir_c = r*oC;
			for (int c=0; c<oC; c++)
			{
				// nearest neighbor
				iC = (long)(((float)c*scInv) + 0.5f);
				T inVal = inBuf[iR_C + iC];
				outBuf[ir_c + c] = (U)inVal;							
			}
		}		
	} 	

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageResampler<short, short>; 
	template class ImageResampler<unsigned char, unsigned char>;
	template class ImageResampler<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>;	
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageResampler_HPP_
