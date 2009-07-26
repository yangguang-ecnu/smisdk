// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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