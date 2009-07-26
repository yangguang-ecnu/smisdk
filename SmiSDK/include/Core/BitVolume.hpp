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
// Filename:	BitVolume.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:38:35 AM
//

#ifndef _PG_BitVolume_HPP_
#define _PG_BitVolume_HPP_
//////////////////////////////////////////////////////////////////////////

#include "BitVolume.h"

namespace PGCore 
{

template <class T>
BitVolume<T>::BitVolume()
{
	Clear();			
	m_type = kPgDataObjectTypeBitVolume;	
	m_initialized = false;
}

template <class T>
bool BitVolume<T>::AddImage(const BitImage<T>& iBitImageObject)
{
	BitImage<T> newBitImage(iBitImageObject);
	
	m_BitImages.push_back(newBitImage);	
	int zSize = m_BitImages.size();
	if (zSize==1)
	{
		long iR=0, iC=0;
		newBitImage.GetDimensions(iR, iC);
		m_rows = iR; m_columns = iC;	
	}
	return (zSize>0);
}


template <class T>
bool BitVolume<T>::SetImage(int iImageIndex, const BitImage<T>& iImageObject) 
{
	if (iImageIndex<m_BitImages.size()) 
	{
		m_BitImages[iImageIndex] = iImageObject;
		return true;
	}

	return false;
}

template <class T>
const BitImage<T>& BitVolume<T>::GetImage(int iImageIndex) const 
{
	if (iImageIndex<m_BitImages.size()) 
	{
		return m_BitImages[iImageIndex];	
	}
	return BitImage<T>();
}

template <class T>
inline bool BitVolume<T>::GetValue(const int iRow, const int iColumn, const int iImageIndex) const
{
	if (iImageIndex>=m_BitImages.size()) 
	{
		return false;
	}

	return m_BitImages[iImageIndex].GetValue((long)iRow, (long)iColumn);
}

template <class T>
inline bool BitVolume<T>::SetValue(const int iRow, const int iColumn, const int iImageIndex, const bool iValue)
{
	if (iImageIndex>=m_BitImages.size()) 
	{
		return 0;
	}
	m_initialized = true;
	return m_BitImages[iImageIndex].SetValue((long)iRow, (long)iColumn, iValue);
}


template <class T>
bool BitVolume<T>::Reset(const bool iValue)
{
	for (int i=0; i< m_BitImages.size(); i++)
	{
		if (!m_BitImages[i].Reset(iValue)) 
		{
			return false;
		}
	}
	m_initialized = false;
	return true;
}


template <class T>
bool BitVolume<T>::Invert()
{
	for (int i=0; i< m_BitImages.size(); i++)
	{
		if (!m_BitImages[i].Invert()) 
		{
			return false;
		}
	}
	return true;
}


template <class T>
bool BitVolume<T>::Clear() 
{
	m_BitImages.clear();				
	m_rows = 0; m_columns = 0;
	m_initialized = false;
	return (m_BitImages.size()==0);
}



template <class T>
BitVolume<T>::~BitVolume(void)
{	
	Clear();		
}


template <class T>
BitVolume<T> &BitVolume<T>::operator=(const BitVolume<T> &iBitVolumeObject) 
{	
	Clear();
	// copy all BitImages
	for (int i=0; i<iBitVolumeObject.GetSize(); i++) 
	{
		AddImage(((BitVolume<T>&)iBitVolumeObject).GetImage(i));
	}
	m_initialized = iBitVolumeObject.IsInitialized();
	return *this;
}


template <class T>
BitVolume<T> &BitVolume<T>::operator|=(const BitVolume<T> &iBitVolumeObject) 
{	
	// confirm the dimensions match
	PGMath::Point3D<long> iDims, mDims, diffDims;
	iBitVolumeObject.GetDimensions(iDims);
	GetDimensions(mDims);

	if (iDims.X()!=mDims.X() || iDims.Y()!=mDims.Y() || iDims.Z()!=mDims.Z())
	{
		assert(0 && "Dimensions dont match");
		return *this;
	}

	// OR all BitImages
	for (int i=0; i<iBitVolumeObject.GetSize(); i++) 
	{
		BitImage<T>& mImage = m_BitImages[i];
		const BitImage<T>& iImage = ((BitVolume<T>&)iBitVolumeObject).GetImage(i);
		mImage |= iImage;		
	}
	m_initialized = iBitVolumeObject.IsInitialized();
	return *this;
}



	
};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BitVolume_HPP_
