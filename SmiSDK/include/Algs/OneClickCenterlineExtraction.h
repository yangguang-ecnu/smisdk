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
// Filename:	OneClickCenterlineExtraction.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGOneClickCenterlineExtraction_H_
#define _PGOneClickCenterlineExtraction_H_
//////////////////////////////////////////////////////////////////////////


#include "CenterlineExtractionBase.h"

// implement hessian analysis based centerline extraction

namespace PGAlgs
{

	template <class T, class U>
	class OneClickCenterlineExtraction : public CenterlineExtractionBase<T, U>
	{
	public:

		OneClickCenterlineExtraction(void);		

		virtual bool Execute();

		virtual eCLineRetCode conditionCheck(const PGMath::Point3D<int> &iVoxel);
	

	public:	
		virtual ~OneClickCenterlineExtraction(void);

	protected:
		virtual bool autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eCLineRetCode iReason);

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "OneClickCenterlineExtraction.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGOneClickCenterlineExtraction_H_
