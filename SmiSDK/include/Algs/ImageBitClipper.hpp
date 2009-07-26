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
// Filename:	ImageBitClipper.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGIMAGEBITCLIPPER_HPP_
#define _PGIMAGEBITCLIPPER_HPP_
//////////////////////////////////////////////////////////////////////////


#include "ImageBitClipper.h"

namespace PGAlgs
{

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



};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGIMAGEBITCLIPPER_HPP_
