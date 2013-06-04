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
	template class ImageCasterBase<unsigned char, short>; 	
	template class ImageCasterBase<float, short>; 
	template class ImageCasterBase<short, float>; 
	template class ImageCasterBase<float, unsigned char>; 
	template class ImageCasterBase<unsigned char, float>; 
	template class ImageCasterBase<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>; 	
#endif
};




//////////////////////////////////////////////////////////////////////////
// EOF

