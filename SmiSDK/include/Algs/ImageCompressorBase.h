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
// Filename:	ImageCompressorBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorBase_H_
#define _PGImageCompressorBase_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"
#include "../Core/Image.h"

namespace PGAlgs
{

template <class T, class U>
class ImageCompressorBase : public ImageFilterBase<T, U>
{
public:
	ImageCompressorBase();

	void SetCompressionFlag(bool iForward);

	virtual bool Execute() = 0;
	
public:	
	virtual ~ImageCompressorBase(void);

protected:
	bool				m_Forward;

	PGCore::Image<U>   *m_oImageObject;		
	
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageCompressorBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorBase_H_
