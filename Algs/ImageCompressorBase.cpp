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
