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
// Filename:	ImageCasterBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageCasterBase.h"

namespace PGAlgs
{

template <class T, class U>
ImageCasterBase<T, U>::ImageCasterBase(void) 
{

};

template <class T, class U>
bool ImageCasterBase<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  
{
	PGCore::Image<U> *tempImgPtr	= static_cast<PGCore::Image < U > *>(m_oDataObject);
	PGCore::Image<U> *tempOutImgPtr = static_cast<PGCore::Image<U> *>(oDataObject);
	*tempOutImgPtr = *tempImgPtr;
	return true;
}	

template <class T, class U>
ImageCasterBase<T, U>::~ImageCasterBase(void) 
{

};


#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageCasterBase<short, short>; 
	template class ImageCasterBase<unsigned char, unsigned char>; 	
	template class ImageCasterBase<float, short>; 
	template class ImageCasterBase<short, float>; 
	template class ImageCasterBase<float, unsigned char>; 
	template class ImageCasterBase<unsigned char, float>; 
	template class ImageCasterBase<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>; 	
#endif
};




//////////////////////////////////////////////////////////////////////////
// EOF

