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
// Filename:	RegionGrowSegmentation.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGRegionGrowSegmentation_H_
#define _PGRegionGrowSegmentation_H_
//////////////////////////////////////////////////////////////////////////


#include "SegmentationBase.h"


namespace PGAlgs
{

	template <class T, class U>
	class RegionGrowSegmentation : public SegmentationBase<T, U>, public PGCore::ThreadedProcess
	{
	public:
		RegionGrowSegmentation(void);

		virtual bool Execute();

		virtual eSegRetCode conditionCheck(const PGMath::Point3D<int> &iVoxel);
	
		// overload for threadedprocess inheritance
		virtual void StartExecution(void *iParams);

	public:	
		virtual ~RegionGrowSegmentation(void);

	protected:
		virtual bool autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason);
		virtual eSegRetCode	visitVoxel(const PGMath::Point3D<int> &iVoxel);
		
		virtual bool executeSync();
		virtual bool executeAsync();

	private:
		bool m_thinMask;

		bool thinMask();
		bool findCentroid(
			const std::vector<PGMath::Point3D<int>>& iComponent,
			PGMath::Point3D<int>& ioCentroid);
		bool visitBitPixel(
			const PGMath::Point3D<int>& iSeed,
			const PGCore::BitImage& bImage,
			PGCore::BitImage& visitedImage,
			std::vector<PGMath::Point3D<int>>& ioComponent);
	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "RegionGrowSegmentation.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGRegionGrowSegmentation_H_
