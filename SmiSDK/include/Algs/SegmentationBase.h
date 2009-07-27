// -*- C++ -*-
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
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
// Filename:	SegmentationBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGSegmentationBASE_H_
#define _PGSegmentationBASE_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"
#include "ImageStatisticsFilter.h"
#include "Core/Volume.h"
#include "Core/Voxel3D.h"
#include "Core/Stack.h"
#include "Core/Stack3D.h"


namespace PGAlgs
{
typedef enum eSegRetCode
{
	SegRetCodeUnknown = -2,
	SegRetCodeStackError = -1,
	SegRetCodeOutOfRange = 0,
	SegRetCodeOk = 1,
	SegRetCodeGradient = 2,
	SegRetCodeValue = 3,
	SegRetCodeNeighbor = 4,
	SegRetCodeAlreadyMarked = 5
} _eSegRetCode;


template <class T>
class ThreadedSegmentationParams : public AsyncProcessParams
{
public: 
	ThreadedSegmentationParams<T>();

	std::vector< PGMath::Point3D< float > >		m_pSeeds;	
	int											m_autoDilationCount; // in voxels	
	bool										m_autoAdjustConditions;

	PGCore::Voxel3D<T>							*m_pIVolume;
	float										m_window; // 0.0f...1.0f. around the seedvalue
	
	int											m_gradientHigh;
	int											m_maxLoopCount;
	int											m_neighborTh;
};


template <class T, class U>
class SegmentationBase : public ImageFilterBase<T, U>
{
public:
	SegmentationBase(void);

	virtual bool Execute() = 0;
	
	// -0.5 to 0.5
	bool SetSeeds(const std::vector< PGMath::Point3D< float > > &iSeeds);

	// 0.0 to 1.0
	bool SetWindow(const float& iWindow);

	bool SetAutoDilationCount(const int& iCount);

	bool SetAutoAdjustConditions(const bool iFlag);

	bool SetGradientHigh(const int& iGradient);
	
	
public:	
	virtual ~SegmentationBase(void);

	virtual eSegRetCode conditionCheck(const PGMath::Point3D<int> &iVoxel);

protected:
	PGCore::Logger					 m_threadLogger;
	ThreadedSegmentationParams<T>	 m_SegParams;
	int								 m_autoDilationCount; // in voxels	
	bool							 m_autoAdjustConditions;

	PGCore::Stack3D<int>			 m_stack;
	
	PGCore::Voxel3D<T>				*m_pIVolume;
	std::vector<PGMath::Point3D< float > >			m_pSeeds;
	T								 m_lowValue;
	T								 m_highValue;
	float							 m_window; // 0.0f...1.0f. around the seedvalue
	
	long							 m_count, m_totalCount;	
	PGMath::Point3D<long>			 m_volumeDimensions;

	int								 m_gradientHigh;
	int								 m_maxLoopCount;
	int								 m_neighborTh;

	
	virtual bool analyzeSeedPoint(const PGMath::Point3D<float>& iSeed, T& oMinValue, T& oMaxValue, 
		T& oMeanValue, float& oStdDev, float& oSNR);

	virtual bool autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason);

	virtual eSegRetCode	visitVoxel(const PGMath::Point3D<int> &iVoxel) = 0;

	bool    transformSeedsVoxelToImg();
	
	// in image co-ordinates
	bool	setVoxel(const PGMath::Point3D<int> &iVoxel, const bool iValue);	

	void DumpSeedsOnImages(const std::vector<PGMath::Point3D< float > >& iSeeds);
	
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "SegmentationBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGSegmentationBASE_H_
