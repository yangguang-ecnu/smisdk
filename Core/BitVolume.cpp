// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	BitVolume.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:38:35 AM
//
//////////////////////////////////////////////////////////////////////////

#include "Core/BitVolume.h"

namespace PGCore 
{


	BitVolume::BitVolume()
	{
		Clear();			
		m_type = kPgDataObjectTypeBitVolume;	
		m_initialized = false;
	}

	int BitVolume::GetSize() const 
	{
		return m_BitImages.size();			
	};

	bool BitVolume::SetDimensions(const PGMath::Vector3D<long> &iDimensions)
	{
		Clear();

		for (int i=0; i< iDimensions.Z(); i++)
		{
			BitImage nextImage(iDimensions.Y(), iDimensions.X());
			AddImage(nextImage);
		}

		return GetSize()==iDimensions.Z();
	}

	void BitVolume::GetDimensions(PGMath::Point3D<long> &oDimensions) const 
	{
		long xSize = 0, ySize=0, zSize= GetSize();
		if (zSize>0) 
		{			
			m_BitImages[0].GetDimensions(ySize, xSize); 
		}		
		oDimensions = PGMath::Point3D<long>(xSize, ySize, zSize);
		return;
	};

	const std::vector<BitImage> &BitVolume::GetImages() const 
	{
		return m_BitImages;
	}

	bool BitVolume::AddImage(const BitImage& iBitImageObject)
	{
		BitImage newBitImage(iBitImageObject);

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



	bool BitVolume::SetImage(int iImageIndex, const BitImage& iImageObject) 
	{
		if (iImageIndex<m_BitImages.size()) 
		{
			m_BitImages[iImageIndex] = iImageObject;
			return true;
		}

		return false;
	}


	const BitImage& BitVolume::GetImage(int iImageIndex) const 
	{
		if (iImageIndex<m_BitImages.size()) 
		{
			return m_BitImages[iImageIndex];	
		}
		return BitImage();
	}


	bool BitVolume::GetValue(const int iRow, const int iColumn, const int iImageIndex) const
	{
		if (iImageIndex>=m_BitImages.size()) 
		{
			return false;
		}

		return m_BitImages[iImageIndex].GetValue((long)iRow, (long)iColumn);
	}


	bool BitVolume::SetValue(const int iRow, const int iColumn, const int iImageIndex, const bool iValue)
	{
		if (iImageIndex>=m_BitImages.size()) 
		{
			return 0;
		}
		m_initialized = true;
		return m_BitImages[iImageIndex].SetValue((long)iRow, (long)iColumn, iValue);
	}



	bool BitVolume::Reset(const bool iValue)
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



	bool BitVolume::Invert()
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



	bool BitVolume::Clear() 
	{
		m_BitImages.clear();				
		m_rows = 0; m_columns = 0;
		m_initialized = false;
		return (m_BitImages.size()==0);
	}




	BitVolume::~BitVolume(void)
	{	
		Clear();		
	}



	BitVolume &BitVolume::operator=(const BitVolume &iBitVolumeObject) 
	{	
		Clear();
		// copy all BitImages
		for (int i=0; i<iBitVolumeObject.GetSize(); i++) 
		{
			AddImage(((BitVolume&)iBitVolumeObject).GetImage(i));
		}
		m_initialized = iBitVolumeObject.IsInitialized();
		return *this;
	}



	BitVolume &BitVolume::operator|=(const BitVolume &iBitVolumeObject) 
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
			BitImage& mImage = m_BitImages[i];
			const BitImage& iImage = ((BitVolume&)iBitVolumeObject).GetImage(i);
			mImage |= iImage;		
		}
		m_initialized = iBitVolumeObject.IsInitialized();
		return *this;
	}

	bool BitVolume::IsInitialized() const
	{
		return m_initialized;
	}


};

//////////////////////////////////////////////////////////////////////////
// EOF
