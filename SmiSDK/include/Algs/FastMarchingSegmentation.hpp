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
// Filename:	FastMarchingSegmentation.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGFastMarchingSegmentation_HPP_
#define _PGFastMarchingSegmentation_HPP_
//////////////////////////////////////////////////////////////////////////


#include "FastMarchingSegmentation.h"
#include "Core/DilationKernel.h"


namespace PGAlgs
{
	
	template <class T, class U>
	bool FastMarchingSegmentation<T, U>::Execute() 
	{
		GetLogger()->Log("Begin FastMarchingSegmentation:Execute");

		if (m_pSeeds.empty())
		{
			LOG0("FastMarchingSegmentation:initVolumes Error: null input");
			return false;
		}

		PGCore::MultiDataBase<T> *inMultiVolume = (dynamic_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("FastMarchingSegmentation:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1 || iMultiVolumeCount > 2)
		{
			LOG0("FastMarchingSegmentation:initVolumes Error: Needs at least 1/most 2 volume(s)");
			return false;
		}

		m_pIVolume = (dynamic_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(0)));
		if (!m_pIVolume) {
			LOG1("FastMarchingSegmentation: Error: failure to fetch input volume %d", 0);
			return false;
		}

		if (GetAsyncEnabled())
		{
			return executeAsync();
		}

		return executeSync();		
	};


	template <class T, class U>
	bool FastMarchingSegmentation<T, U>::executeSync()
	{
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

		T minVal=9999, maxVal=-9999;
		T seedValue, avgValue = 0, offset = 0;
		float stdDev=0, snr=0, spread=2.5f, valFac=1.0f, gradFac = 0.1f;
		
		seedValue = m_pIVolume->GetVolumeAccessor()->GetValue(m_pSeeds[0].Y(), m_pSeeds[0].X(), m_pSeeds[0].Z());
		avgValue = seedValue;
		m_pIVolume->GetVolumeAccessor()->GetDataRange(&minVal, &maxVal);
		offset = T_ROUND((m_window)*((float)(maxVal-minVal)));

		if (analyzeSeedPoint(m_pSeeds[0], minVal, maxVal, avgValue, stdDev, snr))
		{
			if (stdDev)	
			{
				offset = valFac*spread*stdDev;
				m_gradientHigh *= (gradFac*stdDev);
			}
			//else offset = spread;
		} 

		m_lowValue = seedValue - offset;
		m_highValue= avgValue + offset;
		
		GetLogger()->Log("Range: %d -> %d, GTh: %03d (Std: %3.2f, Avg: %05d, SNR: %3.2f)", m_lowValue, m_highValue, m_gradientHigh, stdDev, int(avgValue), snr);
		
#ifdef _DEBUG
		//DumpSeedsOnImages(m_pSeeds);
#endif
		
		for (int i=0; i<1/*m_pSeeds.size()*/; i++)
		{
			bool rv = m_stack.Push(PGMath::Point3D<int>((int)m_pSeeds[i].X(), (int)m_pSeeds[i].Y(), (int)m_pSeeds[i].Z()));
			if (!rv) return false;
		}
    
		long totalVoxels = max(m_totalCount/2, 500);
		long voxelsPerIteration = totalVoxels/5;//min(totalVoxels, 10000);
		int voxelsIncrPerIteration = 1000;//max(voxelsPerIteration/100, 1);
		//voxelsIncrPerIteration = //min(m_totalCount/100, voxelsIncrPerIteration);
		
		
		//bool flag=false;

		/*
		if (m_autoAdjustGradient)
		{
			m_gradientHigh = autoAdjustGradient(m_pSeeds[0]);
		}*/

		
		int segProgressShare = 70;
		int nLoops = 0, breakReason=-1; // 0: stack empty, 1: visit failure
		PGMath::Point3D<int> lastPoint;
		eSegRetCode rvc = SegRetCodeOutOfRange;
		while (nLoops < m_maxLoopCount)
		{
			int stSize = m_stack.Size();
			if (stSize==0) break;			
			rvc = SegRetCodeOk;

			while (m_count<totalVoxels)
			{
				PGMath::Point3D<int> oPoint;

				if (!m_stack.Pop(oPoint))
				{
					breakReason = 0;
					rvc = SegRetCodeNeighbor;
					GetLogger()->Log("Encountered first stack pop error. Exitting...");
					break;
				}

				lastPoint = oPoint;

				rvc = visitVoxel(oPoint);
				if (rvc<SegRetCodeOutOfRange) 
				{
					//breakReason = 1;
					//GetLogger()->Log("Encountered first stack error (%d). Exitting...", (int)rvc);
					//break;
					continue;
				}

				if (m_count%voxelsIncrPerIteration==0)	
				{
					int progressValue=(int)(100.0f*((float)(m_count)/(float)voxelsPerIteration) + 0.5f);				
					UpdateProgress(progressValue%segProgressShare);
				}
			} 
			
			bool rv = m_autoAdjustConditions ? autoAdjustConditions(lastPoint, rvc) : false;
			if (!rv) break;

			

			if (breakReason==0)
			{
				// push the last popped seed
				bool rv = m_stack.Push(lastPoint);
				if (!rv) break;					
			}

			breakReason = -1;
			nLoops++;		
		}

		// auto-dilate
		if (m_autoDilationCount && m_count)
		{
			GetLogger()->Log("Start AutoDilation...%d voxels", m_autoDilationCount);
			PGCore::DilationKernel<char, T> dilationKernel(m_autoDilationCount);
			PGCore::BitVolume<T>& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1);
			PGMath::Point3D<long> mDims;
			maskVol.GetDimensions(mDims);

			PGCore::BitImage<T> sbImage(mDims.Y(), mDims.X());
			const std::vector<BitImage <T > > & maskImages = maskVol.GetImages();
			for (int i=0; i<mDims.Z(); i++)
			{
				sbImage.Clear();

				PGCore::BitImage<T> bImage = maskImages[i];
				dilationKernel.Convolve(bImage, sbImage);	

				// set it back
				maskVol.SetImage(i, sbImage);

				float prog = segProgressShare + (100.0f-segProgressShare)*(float(i)/float(mDims.Z()));
				UpdateProgress(int(prog));
			}
			

			GetLogger()->Log("End AutoDilation.");
		}

		if (m_count) m_pIVolume->GetVolumeAccessor()->FinalizeMask();

		//anything below the selected Z is not grown into
		UpdateProgress(100);

		GetLogger()->Log("Marked %d voxels.", m_count);

		// make use of this mask now!!!!!!!!
#define _DEBUG_DUMP 0
#if (_DEBUG_DUMP)
		PGCore::BitVolume<T>& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume();
		PGMath::Point3D<long> mDims;
		maskVol.GetDimensions(mDims);
		const std::vector<BitImage <T > > & maskImages = maskVol.GetImages();
		for (int i=0; i<mDims.Z(); i++)
		{
			PGCore::BitImage<T> bImage = maskImages[i];
			PGCore::Image<T> img;

			bImage.UnPack(img);

			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\MaskImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(img, std::string(fileName));
		}
	
#endif
		return true;
	}


	template <class T, class U>
	bool FastMarchingSegmentation<T, U>::executeAsync()
	{
		SetMaxThreadCount(1);

		bool rv = false;
		char szPath[MAX_PATH]; 
		DWORD dwBufferSize = MAX_PATH;
		GetTempPath( dwBufferSize, szPath );	
		std::string pStr = std::string(szPath) + kPgApplicationName + "\\";

		// fill params and send to thread
		std::string fName = pStr + std::string("Segmentation") + std::string(".txt");

		m_threadLogger.SetLogFile(fName.c_str());
		
		m_SegParams.m_pSeeds = m_pSeeds;
		m_SegParams.m_autoDilationCount = m_autoDilationCount;
		m_SegParams.m_autoAdjustConditions = m_autoAdjustConditions;
		m_SegParams.m_pIVolume = m_pIVolume;
		m_SegParams.m_window = m_window;
		m_SegParams.m_gradientHigh = m_gradientHigh;
		m_SegParams.m_maxLoopCount = m_maxLoopCount;
		m_SegParams.m_neighborTh = m_neighborTh;

		// base params settings
		m_SegParams.statusCB = GetProgressCB();
		m_SegParams.ioLogger = &(m_threadLogger);			
		m_SegParams.iParentThreadPtr = GetAsyncEnabled() ? (void *)this : 0;

		if (!CreateNewThread((ThreadedProcess *)this, (void *) (&(m_SegParams))))
		{
			LOG1("ERROR: Failed to spawn new thread %d.", GetAliveThreadCount()); 
			return false;
		}	

		rv = WaitToFinishIfNeeded();	

		LOG1("} ReadFromFolderMT : %d threads spawned", GetAliveThreadCount());

		return rv;		
	}
	 
	template <class T, class U>
	eSegRetCode FastMarchingSegmentation<T, U>::conditionCheck(const PGMath::Point3D<int> &iVoxel)
	{
		eSegRetCode checkCode = SegmentationBase<T, U>::conditionCheck(iVoxel);
		if (checkCode!=SegRetCodeOk)
		{
			return checkCode;
		}

		// add additional check here
		// check that all neighbors satisfy the condition
		int neighCount=0;
		checkCode = SegmentationBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X()+1, iVoxel.Y(), iVoxel.Z()));
		neighCount+= (checkCode==SegRetCodeOk ? 1 : 0);
		
		checkCode = SegmentationBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X()-1, iVoxel.Y(), iVoxel.Z()));
		neighCount+= (checkCode==SegRetCodeOk ? 1 : 0);

		checkCode = SegmentationBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()+1, iVoxel.Z()));
		neighCount+= (checkCode==SegRetCodeOk ? 1 : 0);

		checkCode = SegmentationBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()-1, iVoxel.Z()));
		neighCount+= (checkCode==SegRetCodeOk ? 1 : 0);

		checkCode = SegmentationBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()-1));
		neighCount+= (checkCode==SegRetCodeOk ? 1 : 0);

		checkCode = SegmentationBase<T, U>::conditionCheck(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()+1));
		neighCount+= (checkCode==SegRetCodeOk ? 1 : 0);

		return (neighCount>m_neighborTh) ? SegRetCodeOk : SegRetCodeNeighbor;
	}


	template <class T, class U>
	bool FastMarchingSegmentation<T, U>::autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason)
	{
		bool rv = SegmentationBase<T, U>::autoAdjustConditions(iSeed, iReason);
		if (!rv) return false;

		// use current seed to readjust the conditions. maybe use local distribution
		if (iReason==SegRetCodeGradient || iReason==SegRetCodeNeighbor)
		{
			m_gradientHigh+=10;
		}

		if (iReason==SegRetCodeValue || iReason==SegRetCodeNeighbor)
		{
			m_lowValue-=10;
			m_highValue+=10;
		}

		if (iReason==SegRetCodeNeighbor)
		{
			m_neighborTh = m_neighborTh>2 ? m_neighborTh-1 : m_neighborTh;			
		}
 
		GetLogger()->Log("{ FastMarchingSegmentation::New conditions (Reason: %d):", iReason);
			GetLogger()->Log("\tHighGradient: %d", m_gradientHigh);
			GetLogger()->Log("\tLowValue: %d", m_lowValue);
			GetLogger()->Log("\tHighValue: %d", m_highValue);
			GetLogger()->Log("\tNeighborCountTh: %d", m_neighborTh);
		GetLogger()->Log("} FastMarchingSegmentation::New conditions:");

		return true;
	}


	template <class T, class U>
	void FastMarchingSegmentation<T, U>::StartExecution(void *iParam)
	{
		if (!iParam) return;

		//using namespace PGIO;
		PGAlgs::ThreadedSegmentationParams<T> * iParams = static_cast<PGAlgs::ThreadedSegmentationParams<T> *>(iParam);

		iParams->ioLogger->Log("{ FastMarchingSegmentation begin");

		if (iParams->m_asyncEnabled)
		{
			iParams->ioLogger->Log("} StartExecution: Error: async flag enabled");
			if (iParams->iParentThreadPtr)
			{
				FastMarchingSegmentation<T, U>* iParentThreadPtr = (FastMarchingSegmentation<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: async flag enabled");
			}
			return;
		}

		FastMarchingSegmentation<T, U> segmentationEngine;
		segmentationEngine.SetProgressCB(iParams->statusCB);
		
		// set data
		PGCore::MultiDataBase<T>* multiVolume = new PGCore::MultiDataBase<T>();
		if (!multiVolume)
		{
			iParams->ioLogger->Log("} StartExecution: Error: no data");
			if (iParams->iParentThreadPtr)
			{
				FastMarchingSegmentation<T, U>* iParentThreadPtr = (FastMarchingSegmentation<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: no data");
			}
			return;
		}

		multiVolume->AddDataObject(static_cast<PGCore::BaseDataObject *> (iParams->m_pIVolume));
		bool rv = segmentationEngine.SetInput(static_cast<PGCore::BaseDataObject *> (multiVolume));
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to set input");
			if (iParams->iParentThreadPtr)
			{
				FastMarchingSegmentation<T, U>* iParentThreadPtr = (FastMarchingSegmentation<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to set input");
			}
			delete multiVolume;
			return;
		}


		//  mask will be implicit to the volume
		segmentationEngine.SetAutoDilationCount(iParams->m_autoDilationCount);
		segmentationEngine.SetSeeds(iParams->m_pSeeds);
		segmentationEngine.SetWindow(iParams->m_window);
		segmentationEngine.SetGradientHigh(iParams->m_gradientHigh);
		segmentationEngine.SetAutoAdjustConditions(iParams->m_autoAdjustConditions);

		rv = segmentationEngine.Execute();
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to execute");
			if (iParams->iParentThreadPtr)
			{
				FastMarchingSegmentation<T, U>* iParentThreadPtr = (FastMarchingSegmentation<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to execute");
			}
			delete multiVolume;
			return;
		}		
		
		if (iParams->iParentThreadPtr)
		{
			FastMarchingSegmentation<T, U>* iParentThreadPtr = (FastMarchingSegmentation<T, U> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyFinished();
		}
	
		iParams->ioLogger->Log("} FastMarchingSegmentation end");

		delete multiVolume;
		return;
	}

	// 0: out of range, 1: ok, -1: stack error
	template <class T, class U>
	eSegRetCode FastMarchingSegmentation<T, U>::visitVoxel(const PGMath::Point3D<int> &iVoxel)
	{			
		eSegRetCode	checkCode = conditionCheck(iVoxel);

		if (checkCode!=SegRetCodeOk)
		{
			return  checkCode;
		}
		
		setVoxel(iVoxel, true);
		m_count++;
		//ProcessBase::StatusCallBackFn(100.0f*( (double)m_count/(double)m_totalCount ), std::string("Segmenting..."), 0, 0);
		
		bool rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()+1));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()-1));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()+1, iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()-1, iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X()+1, iVoxel.Y(), iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X()-1, iVoxel.Y(), iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;
		

		return SegRetCodeOk;
	};


};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGFastMarchingSegmentation_HPP_
