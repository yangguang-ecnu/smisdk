// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
