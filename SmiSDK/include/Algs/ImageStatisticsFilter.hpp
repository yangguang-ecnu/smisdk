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
// Filename:	ImageStatisticsFilter.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageStatisticsFilter_HPP_
#define _PGImageStatisticsFilter_HPP_
//////////////////////////////////////////////////////////////////////////


#include "ImageStatisticsFilter.h"

namespace PGAlgs
{

template <class T, class U>
bool ImageStatisticsFilter<T, U>::Execute() 
{
	
	if (!m_iDataObject) 
	{
		LOG0("ImageStatisticsFilter: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) 
	{
		LOG0("ImageStatisticsFilter: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageStatisticsFilter: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageStatisticsFilter: Error: invalid image");
		return false;
	}

	/*
	int					m_windowSize;
	float				m_stdDev, m_SNR;
	T					m_minValue, m_maxValue, m_avgValue;
	*/

	int minRow = max(m_seedPoint.Y()-m_windowSize, 0);
	int minCol = max(m_seedPoint.X()-m_windowSize, 0);

	int maxRow = min(m_seedPoint.Y()+m_windowSize, iRows-1);
	int maxCol = min(m_seedPoint.X()+m_windowSize, iColumns-1);

	long double mean=0; 
	PGCore::PixelBase<T> tPixel;
	m_minValue = tPixel.GetMaxValue();
	m_maxValue = tPixel.GetMinValue();

	{				
		long iR = 0, iC = 0;
		for (int r=minRow; r<=maxRow; r++)
		{
			iR = r;
			for (int c=minCol; c<=maxCol; c++)
			{
				iC = c;
				T inVal = inBuf[iR*iColumns + iC];/*inImage->GetValue(iR, iC);*/
				mean+= inVal;	
				m_minValue = min(m_minValue, inVal);
				m_maxValue = max(m_maxValue, inVal);
			}
		}		
	} 	

	int winDim = 2*m_windowSize+1;
	if (winDim)
	{
		mean/=(winDim*winDim);
	}
	m_avgValue = T(mean);

	long double stdDev = 0;
	{				
		long iR = 0, iC = 0;
		for (int r=minRow; r<=maxRow; r++)
		{
			iR = r;
			for (int c=minCol; c<=maxCol; c++)
			{
				iC = c;
				T inVal = inBuf[iR*iColumns + iC];
				stdDev+= (inVal-m_avgValue) * (inVal-m_avgValue);				
			}
		}		
	} 

	stdDev = sqrt(stdDev);
	if (winDim)
	{
		stdDev/= winDim;
	}

	m_stdDev = stdDev;

	if (m_stdDev>0.0f)
	{
		m_SNR = m_avgValue/m_stdDev;
	} else
	{
		m_SNR = kPgMaxSNR;
	}

	return true;
}

};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageStatisticsFilter_HPP_
