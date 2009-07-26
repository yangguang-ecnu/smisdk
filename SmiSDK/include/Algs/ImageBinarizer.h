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
// Filename:	ImageBinarizer.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageBinarizer_H_
#define _PGImageBinarizer_H_
//////////////////////////////////////////////////////////////////////////


#include "ProcessBase.h"
#include "../Core/BitImage.h"

namespace PGAlgs
{

template <class T, class U>
class ImageBinarizer : public ProcessBase
{
public:
	ImageBinarizer(void);

	bool Execute();

	void SetThresholdRange(const T& iLowTh, const T& iHighTh);
	bool GetThresholdRange(T& oLowTh, T& oHighTh) const;

	bool GetOutput(PGCore::BaseDataObject *oDataObject);

public:	
	virtual ~ImageBinarizer(void);

protected:

	

private:
	PGCore::BitImage	*m_oImageObject;

	T					m_lowTh, m_highTh;
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageBinarizer.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageBinarizer_H_
