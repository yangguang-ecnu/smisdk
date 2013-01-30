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

#include "Core/BitImage.h"

namespace PGCore
{


BitImage::BitImage(long iRows/*=0 in*/, 
							long iColumns/*=0 in*/)
{
	SetDimensions(iRows, iColumns);
	Reset(false);
	m_type = kPgDataObjectTypeBitImage;	
}



BitImage::BitImage(const BitImage& iBitImageObject)
{
	*this = iBitImageObject;
}


BitImage &BitImage::operator=(const BitImage& iBitImageObject) 
{	
	// asserts here
	if (iBitImageObject.GetBuffer()==0)
	{
		assert(0 && "Invalid input instance");
		LOG0("BitImage::operator= : Invalid input instance.");
		return BitImage();
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



BitImage& BitImage::operator|=(const BitImage& iBitImageObject) // OR
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


BitImage& BitImage::operator&=(const BitImage& iBitImageObject) // AND
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
		*mBuf = (*iBuf) & (*mBuf);
		iBuf++; mBuf++;
	}

	return *this;
}


bool BitImage::Invert() 
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


bool BitImage::Reset(bool iValue)
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


bool BitImage::Clear() 
{		
	return m_image.Clear();
}


bool BitImage::SetValue(long iRow, long iColumn, const bool iValue) 
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



const bool BitImage::GetValue(long iRow, long iColumn) const 
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


bool BitImage::GetDimensions(long &iRows, long &iColumns) const 
{	
	iRows = m_rows;
	iColumns = m_columns;
	return true; 
}



bool BitImage::SetDimensions(long &iRows, long &iColumns) 
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




BitImage::~BitImage(void)
{	
	Clear();
}


#ifdef _PG_GENERATE_SDK_LIBS_
/*	template class BitImage<PGMath::Point3D<unsigned char>>; 
	template class BitImage<PGMath::Point3D<unsigned short>>; 
	template class BitImage<PG_RENDERING_IN_TYPE>; 
	template class BitImage<char>; 
	template class BitImage<unsigned char>; 
	template class BitImage<unsigned short>; 
	*/
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BitImage_HPP_