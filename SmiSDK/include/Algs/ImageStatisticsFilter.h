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
