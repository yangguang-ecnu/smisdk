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
// Filename:	ImageBinarizer.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageBinarizer_HPP_
#define _PGImageBinarizer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "ImageBinarizer.h"

namespace PGAlgs
{

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

	if (m_oImageObject == NULL)	m_oImageObject = new PGCore::BitImage<T>(iRows, iColumns);
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

/*
template <class T, class U>
const U ImageBinarizer<T, U>::swapEndians(const T &iValue) {
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
#endif	// _PGImageBinarizer_HPP_
