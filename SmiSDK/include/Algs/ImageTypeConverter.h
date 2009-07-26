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
// Filename:	ImageTypeConverter.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageTypeConverter_H_
#define _PGImageTypeConverter_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageCasterBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageTypeConverter : public ImageCasterBase<T, U>
{
public:
	ImageTypeConverter(void);

	bool Execute();

public:	
	virtual ~ImageTypeConverter(void);

protected:

	

private:
	PGCore::Image<U>	*m_oImageObject;
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageTypeConverter.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageTypeConverter_H_
