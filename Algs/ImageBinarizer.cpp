// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
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
// Filename:	ImageBinarizer.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageBinarizer_HPP_
#define _PGImageBinarizer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageBinarizer.h"

namespace PGAlgs
{
template <class T, class U>
ImageBinarizer<T, U>::ImageBinarizer(void) 
{ 
	m_oImageObject = 0; m_lowTh = 0; m_highTh = 255; 
};

template <class T, class U>
void ImageBinarizer<T, U>::SetThresholdRange(const T& iLowTh, const T& iHighTh) 
{ 
	m_lowTh = iLowTh; m_highTh = iHighTh; 
};

template <class T, class U>
bool ImageBinarizer<T, U>::GetThresholdRange(T& oLowTh, T& oHighTh) const 
{ 
	oLowTh = m_lowTh; oHighTh = m_highTh; 
	return true; 
};

template <class T, class U>
bool ImageBinarizer<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  
{
	PGCore::BitImage *tempImgPtr	= static_cast<PGCore::BitImage  *>(m_oDataObject);
	PGCore::BitImage *tempOutImgPtr = static_cast<PGCore::BitImage  *>(oDataObject);
	*tempOutImgPtr = *tempImgPtr;
	return true;
}

template <class T, class U>
ImageBinarizer<T, U>::~ImageBinarizer(void) 
{ 
	if (m_oImageObject) 
	{
		delete m_oImageObject;
		m_oImageObject = 0;
	}
};

template <class T, class U>
bool ImageBinarizer<T, U>::Execute() 
{	
	if (!m_iDataObject) 
	{
		LOG0("ImageBinarizer: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) 
	{
		LOG0("ImageBinarizer: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageBinarizer: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageBinarizer: Error: invalid image");
		return false;
	}

	if (m_oImageObject == NULL)	m_oImageObject = new PGCore::BitImage(iRows, iColumns);
	char* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageBinarizer: Error: invalid out image");
		return false;
	}

	{				
		long iR = 0, iC = 0;
		for (int r=0; r<iRows; r++)
		{
			iR = r;
			for (int c=0; c<iColumns; c++)
			{
				// nearest neighbor
				iC = c;
				T inVal = inBuf[iR*iColumns + iC];
				if (inVal >= m_lowTh && inVal <= m_highTh)
				{
					m_oImageObject->SetValue(iR, iC, true);
				}
			}
		}		
	} 	

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}



#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageBinarizer<short, short>; 
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageBinarizer_HPP_
