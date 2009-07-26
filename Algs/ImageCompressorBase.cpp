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
// Filename:	ImageCompressorBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorBase_CPP_
#define _PGImageCompressorBase_CPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageCompressorBase.h"

namespace PGAlgs
{

	
template <class T, class U>
ImageCompressorBase<T, U>::ImageCompressorBase()
{ 
	m_oImageObject = 0; 	
	SetCompressionFlag(true);
};


template <class T, class U>
ImageCompressorBase<T, U>::~ImageCompressorBase(void)
{ 
	if (m_oImageObject) 
	{
		delete m_oImageObject;
		m_oImageObject = 0;
	};
}

template <class T, class U>
void ImageCompressorBase<T, U>::SetCompressionFlag(bool iForward)
{
	m_Forward = iForward;	
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageCompressorBase<short, short>; 
	template class ImageCompressorBase<unsigned char, unsigned char>;
	template class ImageCompressorBase<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>;	
	template class ImageCompressorBase<PGMath::Point3D<unsigned char>, unsigned char>;	
	template class ImageCompressorBase<unsigned char, PGMath::Point3D<unsigned char>>;	
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorBase_CPP_
