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
// Filename:	RegionGrowSegmentation.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGRegionGrowSegmentation_HPP_
#define _PGRegionGrowSegmentation_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/RegionGrowSegmentation.h"
#include "Core/DilationKernel.h"
#include "Core/ErosionKernel.h"
#include "Core/MultiDataBase.h"

#define kPgMinComponentSz 8
#define kPgMaxComponentSz 2048
#define kPgMaxMaskVxCountPerIter 32*32*64 
#define kPgMaxMaskVxCount kPgMaxMaskVxCountPerIter*8
#define kPgMaxCentroidsSz 32767

#define kPgStDevSpreadFac 2.5
#define kPgStDevGradientFac 0.01

namespace PGAlgs
{
	
	template <class T, class U>
	RegionGrowSegmentation<T, U>::RegionGrowSegmentation(void) 
	{
		m_thinMask = true;
	};

	template <class T, class U>
	RegionGrowSegmentation<T, U>::~RegionGrowSegmentation(void) 
	{

	};

	template <class T, class U>
	bool RegionGrowSegmentation<T, U>::Execute() 
	{
#ifdef _DEBUG
	//	::DebugBreak();
#endif

		GetLogger()->Log("Begin RegionGrowSegmentation:Execute");

		if (m_pSeeds.empty())
		{
			LOG0("RegionGrowSegmentation:initVolumes Error: null input");
			return false;
		}

		PGCore::MultiDataBase<T> *inMultiVolume = (dynamic_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("RegionGrowSegmentation:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1 || iMultiVolumeCount > 2)
		{
			LOG0("RegionGrowSegmentation:initVolumes Error: Needs at least 1/most 2 volume(s)");
			return false;
		}

		m_pIVolume = (dynamic_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(0)));
		if (!m_pIVolume) {
			LOG1("RegionGrowSegmentation: Error: failure to fetch input volume %d", 0);
			return false;
		}

		if (GetAsyncEnabled())
		{
			return executeAsync();
		}

		return executeSync();		
	};


	template <class T, class U>
	bool RegionGrowSegmentation<T, U>::executeSync()
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

		// m_modeAddSeedOnly = true;
		if (m_modeAddSeedOnly && !m_pSeeds.empty())
		{
			std::vector<PGMath::Point3D<float> >& ptCloud = m_pIVolume->GetVolumeAccessor()->GetPointCloud();			
			ptCloud.push_back(m_pSeeds[0]); // img space
			return true;
		}

		PGCore::PixelBase<T> tPixel;
		T minVal = tPixel.GetMaxValue(), maxVal = tPixel.GetMinValue();
		T seedValue, avgValue = 0, offset = 0;
		float stdDev=0, snr=0, spread=kPgStDevSpreadFac, valFac=1.0f, gradFac = kPgStDevGradientFac;
		
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
		} 

		m_lowValue	=	seedValue	-	offset;
		m_highValue	=	avgValue	+	offset;
		
		GetLogger()->Log("Range: %d -> %d, GTh: %03d (Std: %3.2f, Avg: %05d, SNR: %3.2f)", m_lowValue, m_highValue, m_gradientHigh, stdDev, int(avgValue), snr);
		
#ifdef _DEBUG
		//DumpSeedsOnImages(m_pSeeds);
#endif
		
		for (int i=0; i<1/*m_pSeeds.size()*/; i++)
		{
			bool rv = m_stack.Push(PGMath::Point3D<int>((int)m_pSeeds[i].X(), (int)m_pSeeds[i].Y(), (int)m_pSeeds[i].Z()));
			if (!rv) return false;
		}
    
		long totalVoxels = min(m_totalCount/2, kPgMaxMaskVxCount);
		long voxelsPerIteration = min(totalVoxels, kPgMaxMaskVxCountPerIter);
		
		
		//bool flag=false;

		/*
		if (m_autoAdjustGradient)
		{
			m_gradientHigh = autoAdjustGradient(m_pSeeds[0]);
		}*/

		int nLoops = 0, breakReason=-1; // 0: stack empty, 1: visit failure
		PGMath::Point3D<int> lastPoint;
		eSegRetCode rvc = SegRetCodeOutOfRange;
		while (nLoops < m_maxLoopCount && m_count<totalVoxels)
		{
			int mCountPerIter=0;
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

				if (m_count%50==0)	
				{
					int progressValue=(int)(50.0f*((float)(m_count)/(float)voxelsPerIteration) + 0.5f);				
					UpdateProgress(progressValue%98);
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

		//anything below the selected Z is not grown into
		UpdateProgress(50);

		// auto-dilate
		m_autoDilationCount=0;//+=2;
		if (m_autoDilationCount && m_count)		
		{
			GetLogger()->Log("Start Morphological Closing...%d voxels", m_autoDilationCount);
			PGCore::DilationKernel<T, T> dilationKernel(m_autoDilationCount);
			PGCore::ErosionKernel<T, T> erosionKernel(m_autoDilationCount-1);
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
				if (m_autoDilationCount>1) 
				{
					erosionKernel.Convolve(sbImage, bImage);	
					// set it back
					maskVol.SetImage(i, bImage);
				} else
				{
					maskVol.SetImage(i, sbImage);
				}
				UpdateProgress(50+25.0f*(float)i/float(mDims.Z()));
			}
			GetLogger()->Log("End Morphological Closing.");
		}	

		// make use of this mask now!!!!!!!!
		/*
#define _DEBUG_DUMP 1
#if (_DEBUG_DUMP)
		{
			PGCore::BitVolume& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1);
			PGMath::Point3D<long> mDims;
			maskVol.GetDimensions(mDims);
			const std::vector<BitImage > & maskImages = maskVol.GetImages();
			int skip = maskImages.size()/128;
			for (int i=0; i<maskImages.size(); i+=skip)
			{
				PGCore::BitImage bImage = maskImages[i];
				PGCore::Image<T> img;

				bImage.UnPack(img);

				char fileName[256] = {0};
				_snprintf(fileName, 255, "C:\\Temp\\Data\\MaskImage_%04d.pgm", i); fileName[255] = '\0';
				PGAlgs::DumpImageAsPGM(img, std::string(fileName));
			}
		}
#endif*/

		if (m_count) m_pIVolume->GetVolumeAccessor()->FinalizeMask();

		if (m_thinMask)
		{
			thinMask();					
		}	
		
		/*
#if (_DEBUG_DUMP)
		{
			PGCore::BitVolume& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1);
			PGMath::Point3D<long> mDims;
			maskVol.GetDimensions(mDims);
			const std::vector<BitImage > & maskImages = maskVol.GetImages();
			int skip = maskImages.size()/128;
			for (int i=0; i<maskImages.size(); i+=skip)
			{
				PGCore::BitImage bImage = maskImages[i];
				PGCore::Image<T> img;

				bImage.UnPack(img);

				char fileName[256] = {0};
				_snprintf(fileName, 255, "C:\\Temp\\Data\\CloudImage_%04d.pgm", i); fileName[255] = '\0';
				PGAlgs::DumpImageAsPGM(img, std::string(fileName));
			}
		}
#endif
		*/


		//anything below the selected Z is not grown into
		UpdateProgress(100);

		GetLogger()->Log("Marked %d voxels.", m_count);
		
		return true;
	}


	template <class T, class U>
	bool RegionGrowSegmentation<T, U>::thinMask()
	{
		GetLogger()->Log("Start thinMask...%d voxels");		
		PGCore::BitVolume& maskVol = m_pIVolume->GetVolumeAccessor()->GetBitVolume(0);
		PGMath::Point3D<long> mDims;
		maskVol.GetDimensions(mDims);
		
		const PGCore::MetaData< T > & metaData = m_pIVolume->GetMetaData();
		PGMath::Vector3D<float> spacings = metaData.GetSpacing();

		//std::vector<std::vector<PGMath::Point3D<int> > > centroidList; // centroid of each component

		std::vector<PGMath::Point3D<float> >& ptCloud = m_pIVolume->GetVolumeAccessor()->GetPointCloud();
		ptCloud.clear();
		ptCloud.reserve(kPgMaxCentroidsSz);

		//PGCore::BitVolume& maskVol1 = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1);
		//maskVol1.Reset(1);
		
		const std::vector<BitImage > & maskImages = maskVol.GetImages();

		const std::vector< PGMath::Vector3D<float> >& imgPosPatient = metaData.GetImagePositionsPatient();

		const PGMath::Vector3D<float>& imgPosPatientOrg = imgPosPatient[0];

		PGCore::BitImage visitedImage(mDims.Y(), mDims.X());

		const int skipLow=8, skipHigh=mDims.Z()-8; 
		for (int i=skipLow; i<skipHigh; i++)
		{			
			visitedImage.Reset(0);

 			//std::vector<std::vector<PGMath::Point3D<int> > > componentList;
			//std::vector<PGMath::Point3D<int> > centroids;

			const PGCore::BitImage& bImage = maskImages[i];

						
			for (int j=0; j<mDims.Y(); j++)
			{
				for (int k=0; k<mDims.X(); k++)
				{
					if (bImage.GetValue(k, j) && !visitedImage.GetValue(k, j))
					{
						PGMath::Point3D<int> nextSeed(k, j, i);
						PGMath::Point3D<int> nextCentroid;
						std::vector<PGMath::Point3D<int> > nextComponent;						

						//nextComponent.push_back(nextSeed);
						//visitedImage.SetValue(k, j, 1);

						// masked voxel. find neighbors
						visitBitPixel(nextSeed, bImage, visitedImage, nextComponent);

						if (nextComponent.size()>kPgMinComponentSz)
						{
							//componentList.push_back(nextComponent);
							findCentroid(nextComponent, nextCentroid);
							//centroids.push_back(nextCentroid);							

							//m_pIVolume->GetVolumeAccessor()->

							//PGMath::Point3D<float> nextPt = PGMath::Point3D<float>(
									//imgPosPatientOrg.X()+ nextCentroid.Y()*spacings.X(), // x and y are swapped
									//imgPosPatientOrg.Y()+ nextCentroid.X()*spacings.Y(),
									//imgPosPatientOrg.Z()+ nextCentroid.Z()*spacings.Z());

							PGMath::Point3D<float> nextPt = PGMath::Point3D<float>(
									nextCentroid.Y(),
									nextCentroid.X(),
									nextCentroid.Z());

							ptCloud.push_back(nextPt); // img space

							//m_pIVolume->GetVolumeAccessor()->SetValue(nextCentroid.X(), nextCentroid.Y(), nextCentroid.Z(), 4095);
							//maskVol1.SetValue(nextCentroid.X(), nextCentroid.Y(), nextCentroid.Z(), 0);
						}
					}
				}
			}	

			UpdateProgress(75+24.0f*(float)i/float(mDims.Z()));

			//if (centroids.size()) centroidList.push_back(centroids);
		}

		//maskVol.Reset(0);

		//maskVol &= maskVol1;

#define _DEBUG_DUMP_PTCLOUD_ 0
#if (_DEBUG_DUMP_PTCLOUD_)
		char fileName[256] = {0};
		_snprintf(fileName, 255, "C:\\Temp\\Data\\PointCloud.txt"); 
		fileName[255] = '\0';
		
		FILE* fp = fopen(fileName, "w");
		if (fp)
		{
			
			

			fprintf(fp, "%d, 0, 0\n", ptCloud.size());
			fprintf(fp, "%3.2f, %3.2f, %3.2f\n", 1.0f, 1.0f, 1.0f);
			
			for (int i=0; i<ptCloud.size(); i++)
			{								
				const PGMath::Point3D<float>& nextMarkedVx = ptCloud[i]; 
				fprintf( fp, "%3.2f, %3.2f, %3.2f\n", nextMarkedVx.X(), nextMarkedVx.Y(), nextMarkedVx.Z());		
			}
			fclose(fp);
		}
#endif

		

		GetLogger()->Log("End thinMask.");

		return true;
	}	

	template <class T, class U>
	bool RegionGrowSegmentation<T, U>::findCentroid(
			const std::vector<PGMath::Point3D<int>>& iComponent,
			PGMath::Point3D<int>& ioCentroid)
	{
		PGMath::Point3D<int> tempPt;
		for (int i=0; i<iComponent.size(); i++)
		{
			tempPt+=iComponent[i];
		}

		if (!iComponent.empty()) tempPt/=iComponent.size();

		ioCentroid = tempPt;

		return true;
	}


	template <class T, class U>
	bool RegionGrowSegmentation<T, U>::visitBitPixel(
			const PGMath::Point3D<int>& iSeed,
			const PGCore::BitImage& bImage,
			PGCore::BitImage& visitedImage,
			std::vector<PGMath::Point3D<int>>& ioComponent
		)
	{	
		if (!bImage.GetValue(iSeed.X(), iSeed.Y()) || visitedImage.GetValue(iSeed.X(), iSeed.Y()) || ioComponent.size()>=kPgMaxComponentSz)
			return false;

		visitedImage.SetValue(iSeed.X(), iSeed.Y(), 1);			
		ioComponent.push_back(iSeed);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X()+1, iSeed.Y(), iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X()+1, iSeed.Y()+1, iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);
		
		visitBitPixel(PGMath::Point3D<int>(iSeed.X(), iSeed.Y()+1, iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X()-1, iSeed.Y()+1, iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X()-1, iSeed.Y(), iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X()-1, iSeed.Y()-1, iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X(), iSeed.Y()-1, iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		visitBitPixel(PGMath::Point3D<int>(iSeed.X()+1, iSeed.Y()-1, iSeed.Z()),
			bImage,
			visitedImage,
			ioComponent);

		return true;
	};


	template <class T, class U>
	bool RegionGrowSegmentation<T, U>::executeAsync()
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
		m_SegParams.m_modeAddSeedOnly = m_modeAddSeedOnly;

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
	eSegRetCode RegionGrowSegmentation<T, U>::conditionCheck(const PGMath::Point3D<int> &iVoxel)
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
	bool RegionGrowSegmentation<T, U>::autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason)
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
 
		GetLogger()->Log("{ RegionGrowSegmentation::New conditions (Reason: %d):", iReason);
			GetLogger()->Log("\tHighGradient: %d", m_gradientHigh);
			GetLogger()->Log("\tLowValue: %d", m_lowValue);
			GetLogger()->Log("\tHighValue: %d", m_highValue);
			GetLogger()->Log("\tNeighborCountTh: %d", m_neighborTh);
		GetLogger()->Log("} RegionGrowSegmentation::New conditions:");

		return true;
	}


	template <class T, class U>
	eSegRetCode RegionGrowSegmentation<T, U>::visitVoxel(const PGMath::Point3D<int> &iVoxel)
	{
		return SegmentationBase<T, U>::visitVoxel(iVoxel);
	}

	template <class T, class U>
	void RegionGrowSegmentation<T, U>::StartExecution(void *iParam)
	{
		if (!iParam) return;

		//using namespace PGIO;
		PGAlgs::ThreadedSegmentationParams<T> * iParams = static_cast<PGAlgs::ThreadedSegmentationParams<T> *>(iParam);

		iParams->ioLogger->Log("{ RegionGrowSegmentation begin");

		if (iParams->m_asyncEnabled)
		{
			iParams->ioLogger->Log("} StartExecution: Error: async flag enabled");
			if (iParams->iParentThreadPtr)
			{
				RegionGrowSegmentation<T, U>* iParentThreadPtr = (RegionGrowSegmentation<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: async flag enabled");
			}
			return;
		}

		RegionGrowSegmentation<T, U> segmentationEngine;
		segmentationEngine.SetProgressCB(iParams->statusCB);
		
		// set data
		PGCore::MultiDataBase<T>* multiVolume = new PGCore::MultiDataBase<T>();
		if (!multiVolume)
		{
			iParams->ioLogger->Log("} StartExecution: Error: no data");
			if (iParams->iParentThreadPtr)
			{
				RegionGrowSegmentation<T, U>* iParentThreadPtr = (RegionGrowSegmentation<T, U> *)(iParams->iParentThreadPtr);
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
				RegionGrowSegmentation<T, U>* iParentThreadPtr = (RegionGrowSegmentation<T, U> *)(iParams->iParentThreadPtr);
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
		segmentationEngine.SetModeAddSeedOnly(iParams->m_modeAddSeedOnly);

		rv = segmentationEngine.Execute();
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to execute");
			if (iParams->iParentThreadPtr)
			{
				RegionGrowSegmentation<T, U>* iParentThreadPtr = (RegionGrowSegmentation<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to execute");
			}
			delete multiVolume;
			return;
		}		
		
		if (iParams->iParentThreadPtr)
		{
			RegionGrowSegmentation<T, U>* iParentThreadPtr = (RegionGrowSegmentation<T, U> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyFinished();
		}
	
		iParams->ioLogger->Log("} RegionGrowSegmentation end");

		delete multiVolume;
		return;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class RegionGrowSegmentation<short, short>; 
	template class RegionGrowSegmentation<unsigned char, unsigned char>;
#endif


};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGRegionGrowSegmentation_HPP_
