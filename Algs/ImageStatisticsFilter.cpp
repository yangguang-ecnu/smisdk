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
// Filename:	ImageStatisticsFilter.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageStatisticsFilter_HPP_
#define _PGImageStatisticsFilter_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageStatisticsFilter.h"

namespace PGAlgs
{

template <class T, class U>
ImageStatisticsFilter<T, U>::ImageStatisticsFilter(void)
{ 
	m_windowSize = 1;
	m_minValue=0; 
	m_maxValue=0; 
	m_avgValue=0; 
	m_stdDev=0; 
	m_SNR=0; 
	m_seedPoint = PGMath::Point3D<int>(0, 0, 0); 
};

template <class T, class U>
void ImageStatisticsFilter<T, U>::SetSeedPoint(const PGMath::Point3D<int>& iPoint) 
{ 
	m_seedPoint = iPoint; 
};

template <class T, class U>
void ImageStatisticsFilter<T, U>::SetWindowSize(const float& iWindowSize) 
{ 
	m_windowSize = iWindowSize; 
};

template <class T, class U>
bool ImageStatisticsFilter<T, U>::GetRange(T& oLowValue, T& oHighValue) const 
{ 
	oLowValue = m_minValue; 
	oHighValue = m_maxValue; 
	return true; 
};

template <class T, class U>
bool ImageStatisticsFilter<T, U>::GetMoments(T& oMeanValue, float& oStdDev, float &oSNR) const 
{ 
	oMeanValue = m_avgValue; 
	oStdDev = m_stdDev; 
	oSNR = m_SNR; 
	return true; 
};

template <class T, class U>
const float&  ImageStatisticsFilter<T, U>::GetSNR() const 
{ 
	return m_SNR; 
};

template <class T, class U>
bool ImageStatisticsFilter<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  
{
	return false;
}

template <class T, class U>
ImageStatisticsFilter<T, U>::~ImageStatisticsFilter(void) 
{ 

};

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


#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageStatisticsFilter<short, short>; 
	template class ImageStatisticsFilter<unsigned char, unsigned char>;
#endif



};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageStatisticsFilter_HPP_
