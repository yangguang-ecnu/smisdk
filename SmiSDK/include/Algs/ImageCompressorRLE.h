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
// Filename:	ImageCompressorRLE.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorRLE_H_
#define _PGImageCompressorRLE_H_
//////////////////////////////////////////////////////////////////////////

#include "ImageCompressorBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageCompressorRLE : public ImageCompressorBase<T, U>
{
public:
	ImageCompressorRLE(void);

	virtual bool Execute();
	
	virtual ~ImageCompressorRLE(void);

	void SetQuantizationRange(const float& iRange); // [0...1]

	const float& GetQuantizationRange() const;

protected:
	
private:
	float				m_range;	

	bool	compress(PGCore::Image<T> *inImage);
	bool	decompress(PGCore::Image<T> *inImage);
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageCompressorRLE.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorRLE_H_
