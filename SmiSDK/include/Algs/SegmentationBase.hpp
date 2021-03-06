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
// Filename:	SegmentationBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGSegmentationBASE_HPP_
#define _PGSegmentationBASE_HPP_
//////////////////////////////////////////////////////////////////////////


#include "SegmentationBase.h"

namespace PGAlgs
{

	using namespace PGAlgs;

	template <class T, class U>
	SegmentationBase<T, U>::SegmentationBase(void) 
	{
		//m_pIOVolume = 0;
		m_pIVolume = 0;
		m_count = 0;
		m_window = 0.1f;
		m_lowValue = 0;
		m_highValue = 255;
		m_totalCount = 1;
		m_autoDilationCount = 1;
		m_gradientHigh = 60;
		m_maxLoopCount = 5;
		m_neighborTh = 3;
		m_autoAdjustConditions = false;
	};

	// -0.5 to 0.5
	template <class T, class U>
	bool SegmentationBase<T, U>::SetSeeds(const std::vector< PGMath::Point3D< float > > &iSeeds)
	{
		m_pSeeds = iSeeds;
		return m_pSeeds.size()>0;
	};

	template <class T, class U>
	bool SegmentationBase<T, U>::SetGradientHigh(const int& iGradient)
	{
		m_gradientHigh = iGradient;
		return true;
	}

	template <class T, class U>
	bool SegmentationBase<T, U>::SetAutoAdjustConditions(const bool iFlag)
	{
		m_autoAdjustConditions = iFlag;
		return true;
	}

	// 0.0 to 1.0
	template <class T, class U>
	bool SegmentationBase<T, U>::SetWindow(const float& iWindow)
	{	
		if (iWindow<0.0f || iWindow >1.0f)
		{
			return false;
		}

		m_window = iWindow;
		return true;
	};

	template <class T, class U>
	bool SegmentationBase<T, U>::SetAutoDilationCount(const int& iCount)
	{
		m_autoDilationCount = iCount;
		return true;
	};

	template <class T, class U>
	SegmentationBase<T, U>::~SegmentationBase(void) {  };

	template <class T, class U>
	bool SegmentationBase<T, U>::transformSeedsVoxelToImg()
	{
		if (m_pIVolume->GetVolumeAccessor()==NULL)
		{
			return false;
		}

		// transform seeds into volume space
		
		

		//PGMath::AffineTransform<float> voxelToImage = m_pIVolume->GetTransformVoxelToImage();
		
		//const PGCore::MetaData< T >& oMetaData = m_pIVolume->GetMetaData();
		//bool feetFirst = oMetaData.GetAxialScanFeetFirstFlag();
		//bool axialScan = oMetaData.GetAxialScanFlag();

		for (int i=0; i<m_pSeeds.size(); i++)
		{
			PGMath::Point3D< float > seed = m_pSeeds[i];
			float ox=0, oy=0, oz=0;

			bool rv = m_pIVolume->GetStdToImgCoord(seed.X(), seed.Y(), seed.Z(), ox, oy, oz);
			if (!rv) return false;

			//voxelToImage.Apply(seed, m_pSeeds[i]);
			m_pSeeds[i] = PGMath::Point3D<float>(ox, oy, oz);

			GetLogger()->Log("Seed[%d]: [%f %f %f] -> [%f %f %f]", i, seed.X(), seed.Y(), seed.Z(), 
					m_pSeeds[i].X(), m_pSeeds[i].Y(), m_pSeeds[i].Z());

			// shouldnt have to do this
			/*if (feetFirst) 
			{
				m_pSeeds[i] = PGMath::Point3D<float>(m_pSeeds[i].X(), m_pSeeds[i].Y(), m_volumeDimensions.Z()-1-m_pSeeds[i].Z());
				GetLogger()->Log("\tFlippedZSeed[%d]: [%f %f %f]", i, m_pSeeds[i].X(), m_pSeeds[i].Y(), m_pSeeds[i].Z());
			}*/	
			
		}		
		
		return true;
	};

	// in image co-ordinates
	template <class T, class U>
	bool SegmentationBase<T, U>::setVoxel(const PGMath::Point3D<int> &iVoxel, const bool iValue)
	{
		//if (m_pIOVolume->GetVolumeAccessor()==NULL)
		{
		//	return false;
		}

		//GetLogger()->Log("Marking %d %d %d", iVoxel.X(), iVoxel.Y(), iVoxel.Z());		
		return m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).SetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z(), iValue);						
	};


	template <class T, class U>
	bool SegmentationBase<T, U>::autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason)
	{
		return true;

		//int gradHigh = m_gradientHigh;
		//return gradHigh;

		/*
		// sample in a 3x3x3 cube, compute avg gradient
		PGMath::Vector3D<float> gradientValue;
		m_pIVolume->GetVolumeAccessor()->GetGradient(iVoxel.X(), iVoxel.Y(), iVoxel.Z(), gradientValue);
		float gVal = gradientValue.Length();
		*/
	}

	// 0: out of range, 1: ok, -1: stack error
	//template <class T, class U>
	//eSegRetCode SegmentationBase<T, U>::visitVoxel(const PGMath::Point3D<int> &iVoxel)
	
	template <class T, class U>
	eSegRetCode SegmentationBase<T, U>::conditionCheck(const PGMath::Point3D<int> &iVoxel)
	{
		if (//m_count>10000 || 
			iVoxel.X() < 1 || iVoxel.X() > m_volumeDimensions.X()-2 ||
			iVoxel.Y() < 1 || iVoxel.Y() > m_volumeDimensions.Y()-2 ||
			iVoxel.Z() < 1 || iVoxel.Z() > m_volumeDimensions.Z()-2
			)
			return SegRetCodeOutOfRange;

		bool maskValue = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).GetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());

		if (maskValue)
		{
			return SegRetCodeAlreadyMarked;
		}

		//T iValue = m_pIVolume->GetVolumeAccessor()->GetAvgValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());
		T iValue = m_pIVolume->GetVolumeAccessor()->GetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());

		//GetLogger()->Log("visitVoxel[%d]: Considering %d %d %d: M:%d V: %d", m_count, iVoxel.X(), iVoxel.Y(), iVoxel.Z(),
		//	maskValue, iValue);

		//if (fabs((float)(iValue-m_seedValue)) > 10) 
		if (iValue<m_lowValue || iValue>m_highValue)
		{
			//setVoxel(iVoxel, 5);
			return SegRetCodeValue;
		}


		if (1)
		{
			PGMath::Vector3D<T> gradientValue;
			m_pIVolume->GetVolumeAccessor()->GetGradient(iVoxel.Y(), iVoxel.X(), iVoxel.Z(), gradientValue);
			//if (gradientValue.X() > 50 || gradientValue.Y() > 50 || gradientValue.Z() > 50)
			float gVal = gradientValue.Length();
			if (gVal > m_gradientHigh)
			{
				//GetLogger()->Log("visitVoxel[%d]: rejecting %3.4f / 50", m_count, gVal);
				return SegRetCodeGradient;
			}
		}
		
		return SegRetCodeOk;
	}

	template <class T, class U>
	void SegmentationBase<T, U>::DumpSeedsOnImages(const std::vector<PGMath::Point3D< float > >& iSeeds)
	{
		int crossSize = 8;
		for (int i=0; i<iSeeds.size(); i++)
		{
			const PGMath::Point3D< float >& iPoint = iSeeds[i];
			PGCore::Image<T> iImage = m_pIVolume->GetVolumeAccessor()->GetImage(iPoint.Z());
			T minVal=9999, maxVal=-9999;
			iImage.GetDataRange(&minVal, &maxVal);

			for (int j=-crossSize/2; j<=crossSize/2; j++)
			{
				if (j!=0) iImage.SetValue(int(iPoint.Y()+j+0.5f), int(iPoint.X()+0.5f), maxVal+100);
			}
			
			for (int i=-crossSize/2; i<=crossSize/2; i++)
			{
				if (i!=0) iImage.SetValue(int(iPoint.Y()+0.5f), int(iPoint.X()+i+0.5f), maxVal+100);
			}
			

			iImage.Update();
			
			char imgPath[512] = {0};
			_snprintf(imgPath, 511, "C:/Tmp/Dump/SeedImage_%03.2f_%03.2f_%03.2f.pgm", iPoint.X(), iPoint.Y(), iPoint.Z());
			imgPath[511] = '\0';	
	
			DumpImageAsPGM(iImage, imgPath);		

		}

	}

	template <class T, class U>
	bool SegmentationBase<T, U>::analyzeSeedPoint(const PGMath::Point3D<float>& iSeed, T& oMinValue, T& oMaxValue, 
		T& oMeanValue, float& oStdDev, float& oSNR)
	{
		PGCore::Image<T> iImage = m_pIVolume->GetVolumeAccessor()->GetImage(int(iSeed.Z()+0.5f));

		PGAlgs::ImageStatisticsFilter<T, U> statFilter;
		
		statFilter.SetInput(static_cast<PGCore::BaseDataObject *>(&iImage));
		statFilter.SetSeedPoint(PGMath::Point3D<int>(int(iSeed.X()+0.5f), 
									int(iSeed.Y()+0.5f), int(iSeed.Z()+0.5f)));
		statFilter.SetWindowSize(2);

		bool rv = statFilter.Execute();
		if (!rv) return false;

		statFilter.GetRange(oMinValue, oMaxValue);
		statFilter.GetMoments(oMeanValue, oStdDev, oSNR);
		
		return true;
	}

};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGSegmentationBASE_HPP_
