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
// Filename:	RegistrationBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGRegistrationBASE_H_
#define _PGRegistrationBASE_H_
//////////////////////////////////////////////////////////////////////////


#include "ProcessBase.h"
#include "../Core/Volume.h"
#include "../Math/AffineTransform.h"

namespace PGAlgs
{

template <class T, class U>
class RegistrationBase : public ProcessBase
{
public:
	RegistrationBase(void) 
	{
	
	};

	virtual bool Execute() = 0;
	

	bool GetOutput(PGCore::BaseDataObject *oDataObject)  
	{
		PGMath::AffineTransform< U > *tempTransformPtr		= static_cast<PGMath::AffineTransform< U > *>(m_oDataObject);
		PGMath::AffineTransform< U > *tempOutTransformPtr	= static_cast<PGMath::AffineTransform< U > *>(oDataObject);
		*tempOutTransformPtr = *tempTransformPtr;
		return true;
	}	

public:	
	virtual ~RegistrationBase(void) {};

protected:
	PGMath::AffineTransform< U >	m_pTransform;
	
};


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGRegistrationBASE_H_
