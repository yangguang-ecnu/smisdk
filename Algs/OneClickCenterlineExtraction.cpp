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
// Filename:	OneClickCenterlineExtraction.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGOneClickCenterlineExtraction_HPP_
#define _PGOneClickCenterlineExtraction_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/OneClickCenterlineExtraction.h"
#include "Core/DilationKernel.h"
#include "Core/MultiDataBase.h"

using namespace PGCore;

namespace PGAlgs
{
	
	template <class T, class U>
	OneClickCenterlineExtraction<T, U>::OneClickCenterlineExtraction(void) 
	{

	};

	template <class T, class U>
	OneClickCenterlineExtraction<T, U>::~OneClickCenterlineExtraction(void) 
	{

	};

	template <class T, class U>
	bool OneClickCenterlineExtraction<T, U>::Execute() 
	{
		GetLogger()->Log("Begin OneClickCenterlineExtraction:Execute");

		if (m_pSeeds.empty())
		{
			LOG0("OneClickCenterlineExtraction:initVolumes Error: null input");
			return false;
		}

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("OneClickCenterlineExtraction:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1 || iMultiVolumeCount > 2)
		{
			LOG0("OneClickCenterlineExtraction:initVolumes Error: Needs at least 1/most 2 volume(s)");
			return false;
		}

		m_pIVolume = (static_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(0)));
		if (!m_pIVolume) {
			LOG1("OneClickCenterlineExtraction: Error: failure to fetch input volume %d", 0);
			return false;
		}

		m_pIVolume->GetVolumeAccessor()->InitializeBitVolume(0);
		m_pIVolume->GetVolumeAccessor()->InitializeBitVolume(1);
		m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).Reset(0);

		m_pIVolume->GetVolumeAccessor()->GetDimensions(m_volumeDimensions);
		m_totalCount = 1 + m_volumeDimensions.Z() * m_volumeDimensions.Y() * m_volumeDimensions.X();
		m_stack.SetZSize(m_volumeDimensions.Z());
	
		if (!transformSeedsVoxelToImg())
		{
			return false;
		}

		PGCore::PixelBase<T> tPixel;
		T minVal = tPixel.GetMaxValue(), maxVal = tPixel.GetMinValue();
		m_pIVolume->GetVolumeAccessor()->GetDataRange(&minVal, &maxVal);
		T seedValue = m_pIVolume->GetVolumeAccessor()->GetValue(m_pSeeds[0].Y(), m_pSeeds[0].X(), m_pSeeds[0].Z());
		T offset = T_ROUND((m_window)*((float)(maxVal-minVal)));
		m_lowValue = seedValue - offset;
		m_highValue= seedValue + offset;

		GetLogger()->Log("Range: %d -> %d", m_lowValue, m_highValue);
		
		
		for (int i=0; i<1/*m_pSeeds.size()*/; i++)
		{
			bool rv = m_stack.Push(PGMath::Point3D<int>((int)m_pSeeds[i].X(), (int)m_pSeeds[i].Y(), (int)m_pSeeds[i].Z()));
			if (!rv) return false;
		}
    
		long totalVoxels = m_totalCount/100;//50;
		totalVoxels = totalVoxels < 500 ? 500 : totalVoxels;
		
		//bool flag=false;

		/*
		if (m_autoAdjustGradient)
		{
			m_gradientHigh = autoAdjustGradient(m_pSeeds[0]);
		}*/

		int nLoops = 0, breakReason=-1; // 0: stack empty, 1: visit failure
		PGMath::Point3D<int> lastPoint;
		eCLineRetCode rvc = CLineRetCodeOutOfRange;
		while (nLoops < m_maxLoopCount)
		{
			int stSize = m_stack.Size();
			if (stSize==0) break;			
			rvc = CLineRetCodeOk;

			//while (1)
			{
				//values garbage when popping
				PGMath::Point3D<int> oPoint;

				if (!m_stack.Pop(oPoint))
				{
					breakReason = 0;
					rvc = CLineRetCodeNeighbor;
					GetLogger()->Log("Encountered first stack pop error. Exitting...");
					break;
				}

				lastPoint = oPoint;

				rvc = marchVoxel(oPoint);
				if (rvc<CLineRetCodeOutOfRange) 
				{
					breakReason = 1;
					GetLogger()->Log("Encountered first stack error (%d). Exitting...", (int)rvc);
					break;
				}

				//if (m_count%16==0)	
				{
					int progressValue=(int)(100.0f*((float)(m_count)/(float)totalVoxels) + 0.5f);				
					UpdateProgress(progressValue%100);
				}
			} 
			
			//bool rv = m_autoAdjustConditions ? autoAdjustConditions(lastPoint, rvc) : false;
			//if (!rv) break;

			

			/*if (breakReason==0)
			{
				// push the last popped seed
				bool rv = m_stack.Push(lastPoint);
				if (!rv) break;					
			}*/

			breakReason = -1;
			nLoops++;		
		}

		// auto-dilate		
		if (m_autoDilationCount && m_count)
		{
			GetLogger()->Log("Start AutoDilation...%d voxels", m_autoDilationCount);
			PGCore::DilationKernel<T, T> dilationKernel(m_autoDilationCount);
			PGCore::BitVolume& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1);
			PGMath::Point3D<long> mDims;
			maskVol.GetDimensions(mDims);

			PGCore::BitImage sbImage(mDims.Y(), mDims.X());
			const std::vector<BitImage > & maskImages = maskVol.GetImages();
			for (int i=0; i<mDims.Z(); i++)
			{
				sbImage.Clear();

				PGCore::BitImage bImage = maskImages[i];
				dilationKernel.Convolve(bImage, sbImage);	

				// set it back
				maskVol.SetImage(i, sbImage);
			}
			GetLogger()->Log("End AutoDilation.");
		}

		if (m_count) m_pIVolume->GetVolumeAccessor()->FinalizeMask();

		//anything below the selected Z is not grown into
		UpdateProgress(100);

		GetLogger()->Log("Marked %d voxels.", m_count);

		// make use of this mask now!!!!!!!!
#ifdef _DEBUG_DUMP
		PGCore::BitVolume& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume();
		PGMath::Point3D<long> mDims;
		maskVol.GetDimensions(mDims);
		const std::vector<BitImage > & maskImages = maskVol.GetImages();
		for (int i=0; i<mDims.Z(); i++)
		{
			PGCore::BitImage bImage = maskImages[i];
			PGCore::Image<T> img;

			bImage.UnPack(img);

			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\MaskImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(img, std::string(fileName));
		}
	
#endif

		return true;//m_pIOVolume->Update();
	};

	 
	template <class T, class U>
	eCLineRetCode OneClickCenterlineExtraction<T, U>::conditionCheck(const PGMath::Point3D<int> &iVoxel)
	{
		eCLineRetCode checkCode = CenterlineExtractionBase<T, U>::conditionCheck(iVoxel);
		if (checkCode!=CLineRetCodeOk)
		{
			return checkCode;
		}
/*
		// add additional check here
		// check that all neighbors satisfy the condition
		int neighCount=0;
		checkCode = CenterlineExtractionBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X()+1, iVoxel.Y(), iVoxel.Z()));
		neighCount+= (checkCode==CLineRetCodeOk ? 1 : 0);
		
		checkCode = CenterlineExtractionBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X()-1, iVoxel.Y(), iVoxel.Z()));
		neighCount+= (checkCode==CLineRetCodeOk ? 1 : 0);

		checkCode = CenterlineExtractionBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()+1, iVoxel.Z()));
		neighCount+= (checkCode==CLineRetCodeOk ? 1 : 0);

		checkCode = CenterlineExtractionBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()-1, iVoxel.Z()));
		neighCount+= (checkCode==CLineRetCodeOk ? 1 : 0);

		checkCode = CenterlineExtractionBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()-1));
		neighCount+= (checkCode==CLineRetCodeOk ? 1 : 0);

		checkCode = CenterlineExtractionBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()+1));
		neighCount+= (checkCode==CLineRetCodeOk ? 1 : 0);

		return (neighCount>m_neighborTh) ? CLineRetCodeOk : CLineRetCodeNeighbor;
		*/
		return CLineRetCodeOk;
	}


	template <class T, class U>
	bool OneClickCenterlineExtraction<T, U>::autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eCLineRetCode iReason)
	{
		bool rv = CenterlineExtractionBase<T, U>::autoAdjustConditions(iSeed, iReason);
		if (!rv) return false;

		// use current seed to readjust the conditions. maybe use local distribution
		if (iReason==CLineRetCodeGradient || iReason==CLineRetCodeNeighbor)
		{
			m_gradientHigh+=10;
		}

		if (iReason==CLineRetCodeValue || iReason==CLineRetCodeNeighbor)
		{
			m_lowValue-=10;
			m_highValue+=10;
		}

		if (iReason==CLineRetCodeNeighbor)
		{
			m_neighborTh = m_neighborTh>2 ? m_neighborTh-1 : m_neighborTh;			
		}
 
		GetLogger()->Log("{ OneClickCenterlineExtraction::New conditions (Reason: %d):", iReason);
			GetLogger()->Log("\tHighGradient: %d", m_gradientHigh);
			GetLogger()->Log("\tLowValue: %d", m_lowValue);
			GetLogger()->Log("\tHighValue: %d", m_highValue);
			GetLogger()->Log("\tNeighborCountTh: %d", m_neighborTh);
		GetLogger()->Log("} OneClickCenterlineExtraction::New conditions:");

		return true;
	}

#ifdef _PG_GENERATE_SDK_LIBS_	
	template class OneClickCenterlineExtraction<short, short>; 
	template class OneClickCenterlineExtraction<unsigned char, unsigned char>;
#endif

};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGOneClickCenterlineExtraction_HPP_
