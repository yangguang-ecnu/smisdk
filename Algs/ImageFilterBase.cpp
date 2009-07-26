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
// Filename:	ImageFilterBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageFilterBase.h"

namespace PGAlgs
{

template <class T, class U>
ImageFilterBase<T, U>::ImageFilterBase(void) 
{

};

template <class T, class U>
bool ImageFilterBase<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  
	{
		PGCore::Image<U> *tempImgPtr	= static_cast<PGCore::Image < U > *>(m_oDataObject);
		PGCore::Image<U> *tempOutImgPtr = static_cast<PGCore::Image<U> *>(oDataObject);
		*tempOutImgPtr = *tempImgPtr;
		return true;
	}	

template <class T, class U>
ImageFilterBase<T, U>::~ImageFilterBase(void) 
{

};

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageFilterBase<short, short>; 
	template class ImageFilterBase<short, float>; 
	template class ImageFilterBase<float, float>; 
	template class ImageFilterBase<unsigned char, unsigned char>; 	
	template class ImageFilterBase<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>;	
	template class ImageFilterBase<PGMath::Point3D<unsigned char>, unsigned char>;	
	template class ImageFilterBase<unsigned char, PGMath::Point3D<unsigned char>>;	
#endif

};





//////////////////////////////////////////////////////////////////////////
// EOF

