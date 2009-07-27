// -*- C++ -*-
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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
