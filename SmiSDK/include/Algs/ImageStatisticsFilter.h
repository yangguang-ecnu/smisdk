// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
// Filename:	ImageStatisticsFilter.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageStatisticsFilter_H_
#define _PGImageStatisticsFilter_H_
//////////////////////////////////////////////////////////////////////////

#define kPgMaxSNR 1000.0f

#include "ProcessBase.h"
#include "Core/BitImage.h"

namespace PGAlgs
{

template <class T, class U>
class ImageStatisticsFilter : public ProcessBase
{
public:
	ImageStatisticsFilter(void);

	bool Execute();

	void SetSeedPoint(const PGMath::Point3D<int>& iPoint);

	void SetWindowSize(const float& iWindowSize);

	bool GetRange(T& oLowValue, T& oHighValue) const;

	bool GetMoments(T& oMeanValue, float& oStdDev, float &oSNR) const;

	const float& GetSNR() const;

	bool GetOutput(PGCore::BaseDataObject *oDataObject) ;



public:	
	virtual ~ImageStatisticsFilter(void);

protected:

	

private:
	// shd be Point2D here actually
	// for now, the 3rd dimension is ignored
	PGMath::Point3D<int>	m_seedPoint;

	int						m_windowSize;
	float					m_stdDev, m_SNR;
	T						m_minValue, m_maxValue, m_avgValue;
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageStatisticsFilter.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageStatisticsFilter_H_
