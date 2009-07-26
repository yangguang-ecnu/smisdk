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
// Filename:	ImageBitClipper.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGIMAGEBITCLIPPER_H_
#define _PGIMAGEBITCLIPPER_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageCasterBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageBitClipper : public ImageCasterBase<T, U>
{
public:
	ImageBitClipper(const int iHighBitsToClip, bool iLeaveSignBit=1);

	bool Execute();

public:	
	virtual ~ImageBitClipper(void);

protected:

	const U clipBits(const T &iValue);

private:
	PGCore::Image<U>	*m_oImageObject;

	unsigned short		m_bitClipMask;
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageBitClipper.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGIMAGEBITCLIPPER_H_
