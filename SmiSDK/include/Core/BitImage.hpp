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
// Filename:	BitImage.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_BitImage_HPP_
#define _PG_BitImage_HPP_
//////////////////////////////////////////////////////////////////////////

#include "BitImage.h"

namespace PGCore
{

template <class T>
BitImage<T>::BitImage(long iRows/*=0 in*/, 
							long iColumns/*=0 in*/)
{
	SetDimensions(iRows, iColumns);
	Reset(false);
	m_type = kPgDataObjectTypeBitImage;	
}


template <class T>
BitImage<T>::BitImage(const BitImage<T>& iBitImageObject)
{
	*this = iBitImageObject;
}

template <class T>
BitImage<T> &BitImage<T>::operator=(const BitImage<T>& iBitImageObject) 
{	
	// asserts here
	if (iBitImageObject.GetBuffer()==0)
	{
		assert(0 && "Invalid input instance");
		LOG0("BitImage<T>::operator= : Invalid input instance.");
		return BitImage<T>();
	}

	long iRows, iColumns;	
	iBitImageObject.GetDimensions(iRows, iColumns);		
	if(SetDimensions(iRows, iColumns)) 
	{
		memcpy(m_image.GetBuffer(), iBitImageObject.GetBuffer(), GetDataSize());		
		/*
		for (int iR=0; iR<m_rows; iR++)
		{
			for (int iC=0; iC<m_columns; iC++)
			{
				SetValue(iR, iC, iBitImageObject.GetValue(iR, iC));
			}
		}*/
	}		
	return *this;
}


template <class T>
BitImage<T>& BitImage<T>::operator|=(const BitImage<T>& iBitImageObject) // OR
{
	if (GetDataSize() != iBitImageObject.GetDataSize())
	{
		return *this;
	}
	
	unsigned char* mBuf = (unsigned char*)GetBuffer(), *iBuf = (unsigned char*)iBitImageObject.GetBuffer();
	if (!iBuf || !mBuf)
	{
		return *this;
	}

	int imgSize = GetDataSize();
	for (int i=0; i<imgSize; i++)
	{
		*mBuf = (*iBuf) | (*mBuf);
		iBuf++; mBuf++;
	}

	return *this;
}


template <class T>
bool BitImage<T>::Invert() 
{			
	long iR = 0, iC = 0;

	for (int iR=0; iR<m_rows; iR++)
	{
		for (int iC=0; iC<m_columns; iC++)
		{
			SetValue(iR, iC, !GetValue(iR, iC));
		}
	}

	return true;
}

template <class T>
bool BitImage<T>::Reset(bool iValue)
{
	int iVal = (iValue==true) ? 255 : 0;
	memset(m_image.GetBuffer(), iVal, GetDataSize());
	return true;

	
	long iR = 0, iC = 0;

	for (int iR=0; iR<m_rows; iR++)
	{
		for (int iC=0; iC<m_columns; iC++)
		{
			//bool iVal = (iR+iC)%2;
			SetValue(iR, iC, iValue);

			/*bool oVal = GetValue(iR, iC);

			if (iVal != oVal)
			{
				iVal = false;
			}*/
		}
	}
	

	return true;
}

template <class T>
bool BitImage<T>::Clear() 
{		
	return m_image.Clear();
}

template <class T>
bool BitImage<T>::SetValue(long iRow, long iColumn, const bool iValue) 
{
	if (iRow<m_rows && iColumn<m_columns) 
	{
		long bRows=0, bCols=0;
		m_image.GetDimensions(bRows, bCols);

		unsigned char *byteBuf = (unsigned char *)m_image.GetBuffer();
		long targetByteOffset = bCols*iRow + iColumn/8;

		if (targetByteOffset> (bRows*bCols -1))
		{
			return false;
		}

		unsigned char* targetByte = byteBuf + targetByteOffset;

		// find out the bit
		int targetBit = iColumn%8;

		// create target byte
		unsigned char sourceByte = 1;
		sourceByte = sourceByte<<(7-targetBit);

		if (!iValue) 
		{
			sourceByte = ~sourceByte;
			*targetByte = (*targetByte) & sourceByte;
		} else
		{
			*targetByte = (*targetByte) | sourceByte;
		}

		return true;
	}		

	return false;
}


template <class T>
const bool BitImage<T>::GetValue(long iRow, long iColumn) const 
{
	bool oVal=false;

	if (iRow<m_rows && iColumn<m_columns && iRow>-1 && iColumn>-1) 
	{		
		long bRows=0, bCols=0;
		m_image.GetDimensions(bRows, bCols);

		unsigned char *byteBuf = (unsigned char *)m_image.GetBuffer();
		long targetByteOffset = bCols*iRow + iColumn/8;

		unsigned char* targetByte = byteBuf + targetByteOffset;

		// find out the bit
		int targetBit = iColumn%8;

		// create target byte
		unsigned char sourceByte = 1;
		sourceByte = sourceByte<<(7-targetBit);

		if (((*targetByte) & sourceByte) > 0)
		{
			oVal = true;
		}
	}	

	return oVal;
}

template <class T>
bool BitImage<T>::GetDimensions(long &iRows, long &iColumns) const 
{	
	iRows = m_rows;
	iColumns = m_columns;
	return true; 
}


template <class T>
bool BitImage<T>::SetDimensions(long &iRows, long &iColumns) 
{
	/*if (m_rows==iRows && m_columns==iColumns)
	{
		Reset(false);
		return true;
	}*/

	// clear previous memory
	m_image.Clear();
		
	m_rows = iRows;
	m_columns = iColumns;		

	long pRowSize = m_columns/8 + 1; 
	while (pRowSize%8) pRowSize++;

	return m_image.SetDimensions(m_rows, pRowSize);
}



template <class T>
BitImage<T>::~BitImage(void)
{	
	Clear();
}


template <class T>
bool BitImage<T>::UnPack(PGCore::Image<T>& oImage) 
{
	oImage.SetDimensions(m_rows, m_columns);

	oImage.Reset(0);
	T maxVal = 255;

	for (int iR=0; iR<m_rows; iR++)
	{
		for (int iC=0; iC<m_columns; iC++)
		{
			if (GetValue(iR, iC))
			{
				oImage.SetValue(iR, iC, maxVal);
			}
		}
	}

	return true;
}


};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BitImage_HPP_