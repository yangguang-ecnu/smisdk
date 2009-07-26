// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
