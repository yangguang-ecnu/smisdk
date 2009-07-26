// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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
// Filename:	FastMarchingSegmentation.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGFastMarchingSegmentation_H_
#define _PGFastMarchingSegmentation_H_
//////////////////////////////////////////////////////////////////////////


#include "SegmentationBase.h"


namespace PGAlgs
{

	template <class T, class U>
	class FastMarchingSegmentation : public SegmentationBase<T, U>, public PGCore::ThreadedProcess
	{
	public:
		FastMarchingSegmentation(void) {};

		virtual bool Execute();

		virtual eSegRetCode conditionCheck(const PGMath::Point3D<int> &iVoxel);
	
		// overload for threadedprocess inheritance
		virtual void StartExecution(void *iParams);

	public:	
		virtual ~FastMarchingSegmentation(void) {};

	protected:
		virtual bool autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason);
		virtual eSegRetCode	visitVoxel(const PGMath::Point3D<int> &iVoxel);
		
		virtual bool executeSync();
		virtual bool executeAsync();

		


	};

};

#include "FastMarchingSegmentation.hpp"

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGFastMarchingSegmentation_H_
