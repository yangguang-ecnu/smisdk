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
// Filename:	Image.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_IMAGE_HPP_
#define _PG_IMAGE_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Image.h"

namespace PGCore
{

template <class T>
Image<T>::Image(long iRows/*=0 in*/, 
							long iColumns/*=0 in*/)
{
	m_numOfBits = 8*sizeof(T);
	SetDimensions(iRows, iColumns);
	m_type = kPgDataObjectTypeImage;

	PGCore::PixelBase<T> tPixel;
	
	m_minValue = tPixel.GetMinValue();// 0 
	m_maxValue = tPixel.GetMinValue();// 0
}


template <class T>
Image<T>::Image(const Image<T>& iImageObject) {
	*this = iImageObject;
}

//template <class T>
//Image<T>::Image(const T* iImageBuffer, long iRows, long iColumns,
//							bool iSwapEndians/*=0*/,
//							int iIgnoreHigherBits/*=0*/) {
//
//	if(SetDimensions(iRows, iColumns)) {
//		if (iSwapEndians && sizeof(T)==sizeof(short)) {				
//			long imgIter = 0;				
//			while (imgIter< m_rows*m_columns) {
//				T nextVal = *(iImageBuffer+imgIter);
//				nextVal = swapEndians(nextVal);				
//				*(m_buffer+imgIter) = nextVal;
//
//				imgIter++;
//			}
//
//		} else {
//			memcpy(m_buffer, iImageBuffer, iRows*iColumns*sizeof(T));
//		}
//	}		
//
//}

template <class T>
T* Image<T>::GetBuffer() const {
	return m_buffer;
}

template <class T>
bool Image<T>::Update() 
{
	return computeDataRange();
}

template <class T>
Image<T> &Image<T>::operator=(const Image<T> &iImageObject) {	
	long iRows, iColumns;	
	iImageObject.GetDimensions(iRows, iColumns);		
	if(SetDimensions(iRows, iColumns)) 
	{
		memcpy(m_buffer, iImageObject.GetBuffer(), iRows*iColumns*sizeof(T));		
	}		
	Update();
	return *this;
}



template <class T>
Image<T> &Image<T>::operator-=(const Image<T> &iImageObject) {	
	long iRows, iColumns;	
	iImageObject.GetDimensions(iRows, iColumns);		

	if (iRows!=m_rows || iColumns!=m_columns) {
		return *this;
	}
	
	PGCore::PixelBase<T> tPixel;
	T rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
	T irangeMin = tPixel.GetMaxValue(), irangeMax = tPixel.GetMinValue();

	GetDataRange(&rangeMin, &rangeMax);	
	iImageObject.GetDataRange(&irangeMin, &irangeMax);	
	
	rangeMin = rangeMin<irangeMin ? rangeMin : irangeMin;
	rangeMax = rangeMax>irangeMax ? rangeMax : irangeMax;
	long double rangeTotal = (double)(rangeMax - rangeMin);
	rangeTotal = (rangeTotal<=0.0f) ? 1.0f : rangeTotal;

	long double rangeOrg = (double)(255.0f);//tPixel.GetMaxValue() - tPixel.GetMinValue());

	T* iBuffer = (T*)iImageObject.GetBuffer();
	if (!iBuffer) return *this;

	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{		
		long double signedDiffValue = *(m_buffer+iSize) - *(iBuffer+iSize);
		//signedDiffValue += (double)rangeTotal;
		//signedDiffValue = signedDiffValue/(2.0f*rangeTotal);
		//signedDiffValue *= rangeOrg;	
		T diffValue = (T)abs(signedDiffValue);
		*(m_buffer+iSize) = diffValue;
		iSize++;
	}

	Update();

	return *this;
}


template <class T>
Image<T>& Image<T>::operator*(const float& iScale)
{
	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{		
		float prodValue = (float)*(m_buffer+iSize) * iScale;
		*(m_buffer+iSize) = (T)( prodValue );
		iSize++;
	}

	Update();

	return *this;

}


template <class T>
Image<T> &Image<T>::operator*=(const Image<T> &iImageObject) {	
	long iRows, iColumns;	
	iImageObject.GetDimensions(iRows, iColumns);		

	if (iRows!=m_rows || iColumns!=m_columns) {
		return *this;
	}
	
	/*
	PGCore::PixelBase<T> tPixel;
	T rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
	T irangeMin = tPixel.GetMaxValue(), irangeMax = tPixel.GetMinValue();

	GetDataRange(&rangeMin, &rangeMax);	
	iImageObject.GetDataRange(&irangeMin, &irangeMax);	
	
	rangeMin = rangeMin<irangeMin ? rangeMin : irangeMin;
	rangeMax = rangeMax>irangeMax ? rangeMax : irangeMax;
	long double rangeTotal = (double)(rangeMax - rangeMin);
	rangeTotal = (rangeTotal<=0.0f) ? 1.0f : rangeTotal;

	long double rangeOrg = (double)(255.0f);//tPixel.GetMaxValue() - tPixel.GetMinValue());
	*/

	T* iBuffer = (T*)iImageObject.GetBuffer();
	if (!iBuffer) return *this;

	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{		
		double prodValue = *(m_buffer+iSize) * (( *(iBuffer+iSize)));
		*(m_buffer+iSize) = (T)( prodValue );
		iSize++;
	}

	Update();

	return *this;
}



template <class T>
bool Image<T>::Blend(const Image<T>& iImageObject, const double &iWeight, Image<T> &oImage) 
{
	long iRows, iColumns;	
	iImageObject.GetDimensions(iRows, iColumns);		

	if (iRows!=m_rows || iColumns!=m_columns || iWeight<0 || iWeight>1) {
		return false;
	}
	
	T* iBuffer = (T*)iImageObject.GetBuffer();
	T* oBuffer = (T*)oImage.GetBuffer();

	if (!iBuffer || !oBuffer) return false;

	double iwt0 = 1.0f-iWeight, iwt1 = iWeight;
	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{		
		T prodValue = 	(*(m_buffer+iSize))*iwt0 + (*(iBuffer+iSize))*iwt1;

		//T prodValue = 	*(iBuffer+iSize) ? *(m_buffer+iSize) : 0;// + iwt1 * (double));

		*(oBuffer+iSize) = (T)( prodValue );
		iSize++;
	}

	Update();

	return false;
}


template <class T>
bool Image<T>::FlipVertical() 
{
	T* iBuffer = (T*)GetBuffer();
	if (!iBuffer) return false;

	// swap rows
	int halfHeight = (m_rows%2) ? m_rows/2 : m_rows/2-1;
	int rowLength = m_columns*sizeof(T);
	std::vector<T> tempRow(m_columns);
	for (int r=0; r<=halfHeight; r++)
	{
		T* srcRow = iBuffer + (m_rows-1-r)*m_columns, *dstRow = iBuffer + r*m_columns;

		// get top half row
		memcpy((void *)&(tempRow[0]), (const void *)dstRow, rowLength);

		// copy bottom one here
		memcpy((void *)dstRow, (const void *)srcRow, rowLength);

		// copy top half to the bottom one
		memcpy((void *)srcRow, (const void *)&(tempRow[0]), rowLength);
	}

	return true;
}


template <class T>
bool Image<T>::Normalize(const double &iMean, const double &iVariance, Image<T> &oImage) const
{
	if (iVariance<=0.0f)
	{
		return false;
	}

	// init dims
	oImage = *this;

	T *oBuffer = oImage.GetBuffer();
	if (!oBuffer)
	{
		return false;
	}

	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{
		double diffValueF = (double) ( ( *(m_buffer+iSize) - iMean ) / (sqrt(iVariance)) ) ;
		
		diffValueF = diffValueF*127.0f;// + 127.0f;		

		//T diffValue = (T)(diffValueF+0.5f) > (T)(diffValueF) ? (T)(diffValueF+0.5f) : (T)(diffValueF);
		T diffValue = (T)(diffValueF+0.5f) > (T)(diffValueF) ? (T)(diffValueF+0.5f) : (T)(diffValueF);

		*(oBuffer+iSize) = diffValue;
		iSize++;
	}

	oImage.Update();

	return true;
}


template <class T>
bool Image<T>::GetHistogram(Image<unsigned char>& oImageObject, const T& iMinValue, const T& iMaxValue)
{
	assert(0 && "Not implemented yet");

	return false;

	/*
	if (iMaxValue==iMinValue) return false;

	oImageObject = Image<unsigned char>(m_rows, m_columns);

	histogram;
	histogram.setBinCount(64);
	double oneByRange = 255.0f/((double)(iMaxValue-iMinValue));
	histogram.setActiveRangeRef(0, 255); 
	histogram.setActiveRangeSec(0, 255); 

	// add values to hist
	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{
		double iValue = ((double)(*(m_buffer+iSize)-iMinValue))*oneByRange;
		histogram.addValueStd(iValue, 0);
		iSize++;
	}
	
	return histogram.fillImage1D(0, (unsigned char *)(oImageObject.GetBuffer()), m_rows, m_columns);
	*/
}


template <class T>
Image<T> &Image<T>::SqDifference(const Image<T> &iImageObject) {	
	long iRows, iColumns;	
	iImageObject.GetDimensions(iRows, iColumns);		

	if (iRows!=m_rows || iColumns!=m_columns) 
	{
		return *this;
	}
	
	T* iBuffer = (T*)iImageObject.GetBuffer();
	if (!iBuffer) return *this;

	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{
		long double signedDiffValue = *(m_buffer+iSize) - *(iBuffer+iSize);
		double diffValue = (signedDiffValue*signedDiffValue);
		*(m_buffer+iSize) = T(sqrt(diffValue));
		iSize++;
	}

	Update();
	return *this;
}


template <class T>
bool Image<T>::Invert() {			

	T rMin=0, rMax=0, rRange=0;
	if (!GetDataRange(&rMin, &rMax)) {
		return false;
	}
	rRange = rMax-rMin;

	if (rRange<=0) return false;

	long iSize = 0;
	while (iSize< m_rows*m_columns) {
		T inValue = *(m_buffer+iSize);		
		*(m_buffer+iSize) = rMin + (rRange - inValue);		
		iSize++;
	}

	Update();
	return true;
}

template <class T>
bool Image<T>::Dump() const
{
		LOG0("The Image:");		
		{
			for(int i=0;i<(m_rows);i++)
			{
				for(int j=0;j<(m_columns);j++)
				{
					LOG3("Image[%d][%d] = %3.4f", i, j, (double)(m_buffer[i*m_columns + j]));
				}
			}
		}

		return true;
}


template <class T>
bool Image<T>::Plot(Image<T>& oImage, unsigned int iScale/*=1*/) const
{
		if (iScale<1 || m_rows>1)
		{
			return false;
		}

		long rows=m_columns*iScale, cols=m_columns*iScale;
		if (!oImage.SetDimensions(rows, cols))
		{
			return false;
		}

		T* oBuf			= (T*)oImage.GetBuffer();
		if (!oBuf)
		{
			return false;
		}		
		
		bool res = false;
		if (1)
		{
			int i=0;
			T minValue = 9999, maxValue = -9999;
			GetDataRange(&minValue, &maxValue);
			
			float iRange= (float)(maxValue-minValue), invRange=1.0f;
			if (iRange>0.0f)
				invRange = 1.0f/iRange;
			
			int oHeight = 0;
			for (i=0; i<cols; i++)
			{
				if (iRange>0.0f)
					oHeight = (int)((float)(rows-1)*(float)(1.0f - invRange*(m_buffer[i/iScale] - minValue)));
				else 
					oHeight = rows/2;
				oBuf[oHeight*cols + i] =  (T)255;	
			}
			res = true;			
			
		} 		

#ifdef _DEBUG
		//PGAlgs::DumpImageAsPGM(oImage, std::string("C:\\Data\\ImagePlot.pgm"));
#endif

		return res;
}



template <class T>
bool Image<T>::Reset(const T& iValue)
{
	long iSize = 0;
	while (iSize< m_rows*m_columns) 
	{
		*(m_buffer+iSize) = iValue;
		iSize++;
	}

	Update();
	return true;
}

template <class T>
bool Image<T>::Clear() {		
	bool cleared = ObjectBase::Clear();
	m_buffer = 0;	
	m_rows = 0;
	m_columns = 0;	
	//Update();
	return cleared;
}

template <class T>
bool Image<T>::GetMoments(double* oMean, double* oVariance/*=0*/) const
{
	if (!oMean)
	{
		return false;
	}

	long imgIter = 0;
	double pixelDenom=1.0f/(double)(m_rows*m_columns), mean=-0.0f;
	T nextVal = 0;
	while (imgIter< m_rows*m_columns) {
		nextVal = *(m_buffer+imgIter);		
		mean += (double)((double)nextVal * pixelDenom);
		imgIter++;
	}		

	*oMean = mean;
	if (oVariance)
	{		
		nextVal = 0;	
		imgIter=0;
		while (imgIter< m_rows*m_columns) {
			nextVal = *(m_buffer+imgIter);			
			*oVariance += ((double)((nextVal - mean)*(nextVal - mean))
										* pixelDenom);
			imgIter++;
		}		

		
	}

	return true;
}

template <class T>
bool Image<T>::computeDataRange()
{	
	// go over the data and get min/max
	long imgIter = 0;	
	

	PixelBase<T> tPixel;
	m_minValue = (T)tPixel.GetMaxValue();
	m_maxValue = (T)tPixel.GetMinValue();
	T nextVal = (T)tPixel.GetMinValue();//0;

	//PGCore::Functional<T> functional;
	while (imgIter< m_rows*m_columns) 
	{
		nextVal = *(m_buffer+imgIter);
		m_minValue = PGCore::Functional<T>::Min(m_minValue, nextVal);//m_minValue > nextVal ? nextVal : m_minValue;
		m_maxValue = PGCore::Functional<T>::Max(m_maxValue, nextVal);//m_maxValue < nextVal ? nextVal : m_maxValue;
		//m_minValue = m_minValue > nextVal ? nextVal : m_minValue;
		//m_maxValue = m_maxValue < nextVal ? nextVal : m_maxValue;
		imgIter++;
	}		
	return true;
}


// doesnt recompute
template <class T>
bool Image<T>::GetDataRange(T *oMin, T* oMax)  const 
{
	if (!oMin || !oMax) {
		return false;
	}
	
	// go over the data and get min/max
	*oMin = m_minValue;
	*oMax = m_maxValue;
	return true;
}


template <class T>
bool Image<T>::GetDataRange(T *oMin, T* oMax) 
{
	if (!oMin || !oMax) {
		return false;
	}
	
	Update();

	// go over the data and get min/max
	*oMin = m_minValue;
	*oMax = m_maxValue;
	return true;
}


/*

template <>
bool Image<char>::GetDataRange(char *oMin, char* oMax) const {
	if (!oMin || !oMax) {
		return false;
	}
	// go over the data and get min/max
	long imgIter = 0;
	char nextVal;
	while (imgIter< m_rows*m_columns) {
		nextVal = *(m_buffer+imgIter);
		*oMin = *oMin > nextVal ? nextVal : *oMin;
		*oMax = *oMax < nextVal ? nextVal : *oMax;
		imgIter++;
	}		
	return true;
}


template <>
bool Image<short>::GetDataRange(short *oMin, short* oMax) const {
	if (!oMin || !oMax) {
		return false;
	}
	// go over the data and get min/max
	long imgIter = 0;
	short nextVal;
	while (imgIter< m_rows*m_columns) {
		nextVal = *(m_buffer+imgIter);	

		*oMin = *oMin > nextVal ? nextVal : *oMin;
		*oMax = *oMax < nextVal ? nextVal : *oMax;
		imgIter++;
	}		
	return true;
}

template <>
bool Image<unsigned char>::GetDataRange(unsigned char *oMin, unsigned char* oMax) const {
	if (!oMin || !oMax) {
		return false;
	}
	// go over the data and get min/max
	long imgIter = 0;
	unsigned char nextVal;
	while (imgIter< m_rows*m_columns) {
		nextVal = *(m_buffer+imgIter);
		*oMin = *oMin > nextVal ? nextVal : *oMin;
		*oMax = *oMax < nextVal ? nextVal : *oMax;
		imgIter++;
	}		
	return true;
}
*/
//
//template <class T>
//bool Image<T>::GetNormalizedDataRange(T *oMin, T* oMax, int iIgnoreBits) {
//	if (!oMin || !oMax || iIgnoreBits > m_numOfBits) {
//		return false;
//	}
//	// go over the data and get min/max
//	long imgIter = 0;
//	T nextVal;
//	while (imgIter< m_rows*m_columns) {
//		nextVal = *(m_buffer+imgIter);
//		//nextVal = nextVal<<iIgnoreBits;
//		//nextVal = nextVal>>iIgnoreBits;
//		//nextVal = maskBits(nextVal, iIgnoreBits);
//		
//		nextVal = nextVal>>iIgnoreBits;
//
//		*oMin = *oMin > nextVal ? nextVal : *oMin;
//		*oMax = *oMax < nextVal ? nextVal : *oMax;
//		imgIter++;
//	}		
//	return true;
//}
//
//
//template <>
//bool Image<short>::GetNormalizedDataRange(short *oMin, short* oMax, int iIgnoreBits) {
//	if (!oMin || !oMax || iIgnoreBits > m_numOfBits) {
//		return false;
//	}
//	// go over the data and get min/max
//	// make sure the sign doesnt get affected
//	long imgIter = 0;
//	short nextVal;
//	while (imgIter< m_rows*m_columns) {
//		nextVal = *(m_buffer+imgIter);			
//		
//		*oMin = *oMin > nextVal ? nextVal : *oMin;
//		*oMax = *oMax < nextVal ? nextVal : *oMax;
//		imgIter++;
//	}		
//	return true;
//}
//
//
//template <>
//bool Image<unsigned short>::GetNormalizedDataRange(unsigned short *oMin, unsigned short* oMax, int iIgnoreBits) {
//	if (!oMin || !oMax || iIgnoreBits > m_numOfBits) {
//		return false;
//	}
//	// go over the data and get min/max
//	long imgIter = 0;
//	unsigned short nextVal;
//	while (imgIter< m_rows*m_columns) {
//		nextVal = *(m_buffer+imgIter);
//		nextVal = nextVal<<iIgnoreBits;
//		nextVal = nextVal>>iIgnoreBits;
//		*oMin = *oMin > nextVal ? nextVal : *oMin;
//		*oMax = *oMax < nextVal ? nextVal : *oMax;
//		imgIter++;
//	}		
//	return true;
//}



template <class T>
bool Image<T>::SetValue(long iRow, long iColumn, const T iValue) 
{
	if (iRow<m_rows && iColumn<m_columns) 
	{
		*(m_buffer + iRow*m_columns + iColumn) = iValue;
		m_minValue = m_minValue > iValue ? iValue : m_minValue;
		m_maxValue = m_maxValue < iValue ? iValue : m_maxValue;
		return true;
	}	
	
	return false;
}


template <class T>
bool Image<T>:: Get4NeighborsNoBoundCheck(long iRow, long iColumn, T ioNeighbors[4]) const
{
	long offset = iRow*m_columns + iColumn;
	ioNeighbors[0] = *(m_buffer + offset); // rc
	ioNeighbors[1] = *(m_buffer + offset+1); // r, c+1
	ioNeighbors[2] = *(m_buffer + offset + m_columns); // r+1, c
	ioNeighbors[3] = *(m_buffer + offset + m_columns+1); // r+1, c
	
	return true;
}


template <class T>
const T Image<T>::GetValueNoBoundCheck(long iRow, long iColumn) const 
{			
	return *(m_buffer + iRow*m_columns + iColumn);
}


template <class T>
const T Image<T>::GetValue(long iRow, long iColumn) const {
	if (iRow<m_rows && iColumn<m_columns && iRow>-1 && iColumn>-1) {		
		return *(m_buffer + iRow*m_columns + iColumn);
	}	
	//PixelBase<T> tPixel;
	//return (T)tPixel.GetMinValue();	
	return m_minValue;
}

//
//template <class T>
//const T Image<T>::GetNormalizedValue(long iRow, long iColumn, int iIgnoreBits) {	
//	if (iRow<m_rows && iColumn<m_columns && iIgnoreBits < m_numOfBits) {		
//		T outValue = *(m_buffer + iRow*m_columns + iColumn);
//		outValue = outValue<<iIgnoreBits;
//		outValue = outValue>>iIgnoreBits;
//		return outValue;
//	}	
//	return 0;
//}
//
//
//template <>
//const short Image<short>::GetNormalizedValue(long iRow, long iColumn, int iIgnoreBits) {	
//	if (iRow<m_rows && iColumn<m_columns && iIgnoreBits < m_numOfBits) {		
//		// make sure sign doesnt get affected
//		short outValue = *(m_buffer + iRow*m_columns + iColumn);
//		return outValue;
//	}	
//	return 0;
//}


template <class T>
bool Image<T>::GetDimensions(long &iRows, long &iColumns) const {	
	iRows = m_rows;
	iColumns = m_columns;
	return true; 
}


template <class T>
bool Image<T>::SetDimensions(long &iRows, long &iColumns) 
{
	/*
	if (m_rows==iRows && m_columns==iColumns)
	{
		PixelBase<T> tPixel;
		//return (T)tPixel.GetMinValue();	
		//Reset(0);
		Reset(tPixel.GetMinValue());
		return true;
	}*/

	// clear previous memory
	Clear();
	
	long pSize = iRows*iColumns*sizeof(T);
	m_buffer = (T *)Alloc(pSize);
	if (!m_buffer) 
	{
		//LOG0("Error allocating image.");
		return false;
	}

	m_rows = iRows;
	m_columns = iColumns;		

	return true;
}



template <class T>
Image<T>::~Image(void)
{	
	Clear();
}



//
//template <> 
//short Image<short>::shiftLeft(short iValue, int iBits) {
//	int i=0;
//	short oValue=iValue;
//	while (oValue < SHRT_MAX/2 && i<iBits) {
//		oValue = oValue<<1;
//		i++;
//	}
//	return oValue;
//}
//
//
//template <> 
//short Image<short>::shiftRight(short iValue, int iBits) {
//	int i=0;
//	short oValue=iValue;
//	while (abs(oValue) < SHRT_MAX/2 && i<iBits) {
//		oValue = oValue>>1;
//		i++;
//	}
//	return oValue;
//}
//
//template <> 
//short Image<short>:: maskBits(short iValue, int iHigherBits) {	
//	if (iHigherBits > (8*sizeof(short)-1)) {
//		iHigherBits = sizeof(short)-1;
//	}
//
//	short maskBits = 0;
//	// leave the highest bit alive - sign bit
//	for (int bitIndex=0; bitIndex<(iHigherBits-1); bitIndex++) {
//		maskBits+= (short)pow(2.0, bitIndex);
//	}
//	
//	int toShift = 16-(iHigherBits);
//	maskBits = maskBits<<toShift;
//
//	// shift left 15-(iHigherBits-1) bits
//	// negate it
//	maskBits = ~maskBits;
//
//	// and it with the value
//	short oValue = iValue & maskBits;
//	
//	//if (oValue<0)  
//	{
//		int iChar[16], oChar[16], mChar[16];
//		short tVal = 1;
//		for (int bitIndex=0; bitIndex<16; bitIndex++) { 
//			iChar[15-bitIndex] = iValue & tVal;
//			mChar[15-bitIndex] = maskBits & tVal;
//			oChar[15-bitIndex] = oValue & tVal;
//			tVal*=2;
//		}
//	//
//	}
//		
//	
//	return oValue;
//}
//
//
//template <> 
//bool Image<short>::extractBits(short iValue, int oBits[16]) {
//	if (!oBits) {
//		return false;
//	}
//
//	short tVal = 1;
//	for (int bitIndex=0; bitIndex<16; bitIndex++) { 
//		oBits[15-bitIndex] = iValue & tVal;	
//		tVal*=2;
//	}
//	return true;
//}
//
//
//template <> 
//short Image<short>::swapEndians(short iValue) {
//	short highByte = iValue, lowByte = iValue;
//
//	highByte = iValue; highByte = highByte<<8;	
//	lowByte  = iValue; lowByte = lowByte>>8;
//
//	lowByte |= highByte;
//
//	return (short)lowByte;
//}

//
//template <> 
//unsigned short Image<unsigned short>::swapEndians(unsigned short iValue) {
//	unsigned short highByte = iValue, lowByte = iValue;
//
//	highByte = iValue; highByte = highByte<<8;	
//	lowByte  = iValue; lowByte = lowByte>>8;
//
//	lowByte |= highByte;
//
//	return lowByte;
//}
//
//
//template <> 
//unsigned char Image<unsigned char>::swapEndians(unsigned char iValue) {
//	/*unsigned char highByte = iValue, lowByte = iValue;
//
//	highByte = iValue; highByte = highByte<<8;	
//	lowByte  = iValue; lowByte = lowByte>>8;
//
//	lowByte |= highByte;
//
//	return lowByte;*/
//
//	return iValue;
//}
//
//template <class T>
//bool Image<T>::RescaleToUShort(const unsigned short iMinValue, const unsigned short iMaxValue, 
//									 Image<unsigned short> &oImage) {
//	if (iMaxValue<iMinValue) {
//		return false;
//	}
//	
//	T *imgBuffer = (T *)GetBuffer();
//	if (!imgBuffer) {		
//		return false;
//	}
//
//	T rangeMin=0, rangeMax=0;
//	GetDataRange(&rangeMin, &rangeMax);		
//	float dataRange = (float)rangeMax - (float)rangeMin;
//	//if (dataRange<=0.0) {
//	//	return false;
//	//}	
//	
//	float newVal = 0.0, newValDiff = (float)(iMaxValue - iMinValue);	
//	long imgIter = 0;
//	T inVal=0;
//
//	bool memAllocatedHere=false;	
//	unsigned short *oBuf = (unsigned short *)oImage.GetBuffer();
//	if (!oBuf) {
//		// allocate a temp buffer
//		oBuf = new unsigned short[m_rows*m_columns];
//		if (!oBuf) {
//			return false;
//		}
//		memAllocatedHere = true;
//	}
//	
//	while (imgIter < m_rows*m_columns) {
//		inVal = *imgBuffer++;
//		newVal = iMinValue + newValDiff*((float)(inVal - rangeMin));		
//		if (dataRange>0) {
//			newVal/= (float) (dataRange);
//		}
//		unsigned short eNewVal = (unsigned short)(newVal);
//		*(oBuf+imgIter) = eNewVal;		
//		imgIter++;
//	}
//
//
//	if (memAllocatedHere) {
//		oImage = Image<unsigned short>((const unsigned short*) oBuf, m_rows, m_columns);
//		delete []oBuf;
//	}
//
//	return true;
//}
//
//
//template <class T>
//bool Image<T>::RescaleToUChar(const unsigned char iMinValue, const unsigned char iMaxValue, 
//									Image<unsigned char> &oImage) {
//
//	if (iMaxValue<iMinValue) {
//		return false;
//	}
//
//	
//	T *imgBuffer = (T *)GetBuffer();
//	if (!imgBuffer) {		
//		return false;
//	}
//
//	T rangeMin=0, rangeMax=0;
//	GetDataRange(&rangeMin, &rangeMax);		
//	float dataRange = (float)rangeMax - (float)rangeMin;
//	//if (dataRange<=0.0) {
//	//	return false;
//	//}	
//	
//	float newVal = 0.0, newValDiff = (float)(iMaxValue - iMinValue);	
//	long imgIter = 0;
//	T inVal=0;
//
//	bool memAllocatedHere=false;	
//	unsigned char *oBuf = (unsigned char *)oImage.GetBuffer();
//	if (!oBuf) {
//		// allocate a temp buffer
//		oBuf = new unsigned char[m_rows*m_columns];
//		if (!oBuf) {
//			return false;
//		}
//		memAllocatedHere = true;
//	}
//	
//	while (imgIter < m_rows*m_columns) {
//		inVal = *imgBuffer++;
//		newVal = iMinValue + newValDiff*((float)(inVal - rangeMin));		
//		if (dataRange>0) {
//			newVal/= (float) (dataRange);
//		}
//		unsigned char eNewVal = (unsigned char)(newVal);
//		*(oBuf+imgIter) = eNewVal;		
//		imgIter++;
//	}
//
//
//	if (memAllocatedHere) {
//		oImage = Image<unsigned char>((const unsigned char*) oBuf, m_rows, m_columns);
//		delete []oBuf;
//	}
//
//	return true;
//}

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_IMAGE_HPP_