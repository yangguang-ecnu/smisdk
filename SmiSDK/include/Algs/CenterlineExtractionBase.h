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
// Filename:	CenterlineExtractionBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGCenterlineExtractionBase_H_
#define _PGCenterlineExtractionBase_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"
#include "Core/Volume.h"
#include "Core/Voxel3D.h"
#include "Core/Stack.h"
#include "Core/Stack3D.h"


namespace PGAlgs
{
typedef enum eCLineRetCode
{
	CLineRetCodeUnknown = -2,
	CLineRetCodeStackError = -1,
	CLineRetCodeOutOfRange = 0,
	CLineRetCodeOk = 1,
	CLineRetCodeGradient = 2,
	CLineRetCodeValue = 3,
	CLineRetCodeNeighbor = 4,
	CLineRetCodeAlreadyMarked = 5
} _eCLineRetCode;


template <class T, class U>
class CenterlineExtractionBase : public ImageFilterBase<T, U>
{
public:
	CenterlineExtractionBase(void);

	virtual bool Execute() = 0;
	
	// -0.5 to 0.5
	bool SetSeeds(const std::vector< PGMath::Point3D< float > > &iSeeds);

	// 0.0 to 1.0
	bool SetWindow(const float& iWindow);

	bool SetAutoDilationCount(const int& iCount);

	bool SetAutoAdjustConditions(const bool iFlag);

	bool SetGradientHigh(const int& iGradient);

	
	
public:	
	virtual ~CenterlineExtractionBase(void);

	virtual eCLineRetCode conditionCheck(const PGMath::Point3D<int> &iVoxel);

	virtual eCLineRetCode directionCheck(const PGMath::Point3D<int> &iVoxel, 
		PGMath::Vector3D<float> &oNextDirection, PGMath::Point3D<int> &oAdjustedVoxel);	

protected:
	bool							 m_autoDilationCount; // in voxels	
	bool							 m_autoAdjustConditions;

	PGCore::Stack3D<int>			 m_stack;	
	PGCore::Voxel3D<T>				*m_pIVolume;
	std::vector<PGMath::Point3D< float > >			m_pSeeds;
	T								 m_lowValue;
	T								 m_highValue;
	float							 m_window; // 0.0f...1.0f. around the seedvalue
	
	long							 m_count, m_totalCount;	
	PGMath::Point3D<long>			 m_volumeDimensions;
	PGMath::Vector3D<float>			 m_lastDirection;

	int								 m_gradientHigh;
	int								 m_maxLoopCount;
	int								 m_neighborTh;
	

	eCLineRetCode sampleCrossSection(const PGMath::Point3D<int> &iVoxel, 
		const PGMath::Vector3D<float> &iCurrentDirection, 
		PGMath::Point3D<int> &oAdjustedVoxel);

	float estimateRadiusAlongVector(const PGMath::Point3D<int> &iVoxel,
			const PGMath::Vector3D<float>& iDirection, PGMath::Point3D<int>& oRadialPoint);

	virtual bool autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eCLineRetCode iReason);

	bool    transformSeedsVoxelToImg();
	
	// in image co-ordinates
	bool	setVoxel(const PGMath::Point3D<int> &iVoxel, const bool iValue);
	
	// 0: out of range, 1: ok, -1: stack error
	eCLineRetCode	marchVoxel(const PGMath::Point3D<int> &iVoxel);	
	
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "CenterlineExtractionBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGCenterlineExtractionBase_H_
