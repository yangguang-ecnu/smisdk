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
// Filename:	Image.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_IMAGE_H_
#define _PG_IMAGE_H_
//////////////////////////////////////////////////////////////////////////

#include "ObjectBase.h"
#include "BaseDataObject.h"
#include "Functional.h"
#include "PixelBase.h"

namespace PGCore
{

template <class T>
class Image : public ObjectBase, public BaseDataObject
{
public:

	Image(long iRows=0, long iColumns=0);	
	
	Image(const Image<T>& iImageObject);
	
	Image<T>& operator=(const Image<T>& iImageObject);

	Image<T>& operator+=(const Image<T>& iImageObject);

	Image<T>& operator-=(const Image<T>& iImageObject);

	Image<T>& operator*=(const Image<T>& iImageObject);	

	Image<T>& operator*(const float& iScale);
	
	bool GetHistogram(Image<unsigned char>& oImageObject, const T& iMinValue, const T& iMaxValue);
	
	bool Normalize(const T &iMean, const T &iVariance, Image<T> &oImage) const;

	bool Blend(const Image<T>& iImageObject, const double &iWeight, Image<T> &oImage);

	bool FlipVertical();

	virtual ~Image(void);
	
	bool Clear();

	bool Reset(const T& iValue);
	
	bool SetValue(long iRow, long iColumn, const T iValue);
	
	bool Plot(Image<T>& oImage, unsigned int iScale/*=1*/) const;

	const T GetValue(long iRow, long iColumn) const;

	const T GetValueNoBoundCheck(long iRow, long iColumn) const;
	
	bool    Get4NeighborsNoBoundCheck(long iRow, long iColumn, T ioNeighbors[4]) const;
	
	bool SetDimensions(long &iRows, long &iColumns);

	bool GetDimensions(long &iRows, long &iColumns) const;

	long GetDataSize() const { return m_rows*m_columns*sizeof(T); }

	T* GetBuffer() const;
	
	bool GetDataRange(T *oMin, T* oMax) const; // will return false if range is not uptodate

	bool GetDataRange(T *oMin, T* oMax); 

	bool GetMoments(T* oMean, T* oVariance=0) const;

	bool Update();	

	bool Invert(); 

	bool Dump() const; 

protected: 
	long m_rows;
	long m_columns;		
	T*	m_buffer;
	int m_numOfBits;
	T	m_minValue, m_maxValue;	
	bool	m_rangeUpToDate;	

	bool computeDataRange();
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Image.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_IMAGE_H_