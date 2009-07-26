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
// Filename:	IODicom3D.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IODicom3D.h: interface for the IODicom3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#include "IO/IODicom3D.h"
#include "Algs/ImageTypeConverter.h"

#if (_DRAW_SYNTH_SHAPES_==1)
	#include "Core/DrawHelper.h"
#endif

namespace PGIO
{

template <class T>
IODicom3D<T>::IODicom3D(void) 
{ 
	SetAsyncEnabled(false); 
};

template <class T>
IODicom3D<T>::~IODicom3D(void) 
{ 
	
};

template <class T>
bool IODicom3D<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// write meta data first
	if (!iMetaDataObject) {
		LOG0("IODicom3D::Write: metadata object null");			
		return false;
	}

	if (!WriteMetaData(iMetaDataObject, iParams)) {
		LOG0("IODicom3D::Write: failed to read metadata.");			
		return false;
	}

	PGCore::MetaData<T> *iMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!iMetaData) 
	{
		LOG0("IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	if (!iDataObject) 
	{
		LOG0("IODicom3D::Write: Invalid input image.");			
		return false;
	}

	const std::vector<std::string> &iSourceNames = iParams.Source();
	if (iSourceNames.empty())
	{
		LOG0("IODicom3D::Read: Invalid input filename.");			
		return false;
	}

	// cast to volume type here
	PGCore::Volume<T> *oVolume = (static_cast<PGCore::Volume < T > *>(iDataObject));
	if (!oVolume) 
	{
		LOG0("IODicom3D::Read: Invalid input volume.");			
		return false;
	}

	bool res = false;
	switch(iParams.SourceType())
	{
		case kPgIOSourceTypeFile: 
			res = WriteToFile(iSourceNames[0],	iMetaData, oVolume);	
			break;

		case kPgIOSourceTypeFolder: 
			res = WriteToFolder(iSourceNames, iMetaData, oVolume);	
			break;
		default: break;

	}

	return res;
}


template <class T>
bool IODicom3D<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject)
	{
		LOG0("IODicom3D::Read: metadata object null");			
		return false;
	}

	/*
	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IODicom3D::Read: failed to read metadata.");			
		return false;
	}*/

	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IOBase::Read: Invalid output container for metadata.");			
		return false;
	}

	// read one file / multiple based on the type set here
	const std::vector<std::string> &iSourceNames = iParams.Source();
	if (iSourceNames.empty())
	{
		LOG0("IODicom3D::Read: Invalid input filename.");			
		return false;
	}
	
	if (!oDataObject) 
	{
		LOG0("IODicom3D::Read: Invalid input image.");			
		return false;
	}

	// cast to volume type here
	PGCore::Volume<T> *oVolume = (static_cast<PGCore::Volume < T > *>(oDataObject));
	if (!oVolume) 
	{
		LOG0("IODicom3D::Read: Invalid input volume.");			
		return false;
	}

	m_MultiThreadingEnabled = iParams.IsEnabledMultiThreading();

	std::vector<std::string> iFileNames;

	
	bool res = false;
	switch(iParams.SourceType())
	{
		case kPgIOSourceTypeFolder: 
			// extract filenames from folder
			res = ExtractFileNamesFromFolder(iSourceNames[0], iFileNames, "dcm");
			if (!res) return false;

			break;

		case kPgIOSourceTypeFile: 	
			iFileNames = iSourceNames;
			break;
		
		default: break;
	}

	
	if (m_MultiThreadingEnabled)
	{
		ioMetaDataObjectMT = oMetaData;
		ioDataObjectMT = oVolume;
		SetAsyncEnabled(iParams.IsEnabledAsynchronousLoad());
		m_autoSkip = iParams.IsEnabledAutoSkip();
		res = ReadFromFolderMT(iFileNames);
	} else
	{
		res = ReadFromFolder(iFileNames, oMetaData, oVolume);	
	}

	return res;
}


// protected
template <class T>
bool IODicom3D<T>::WriteToFolder(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	return false;
}

template <class T>
bool IODicom3D<T>::WriteToFile(const std::string& iFile,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	return false;
}



template <class T>
bool IODicom3D<T>::ReadFromFolderMT(const std::vector<std::string>& iFileList)//,	PGCore::BaseDataObject *iMetaDataObject,
		//PGCore::Volume<T> *oDataObject)
{
	LOG0("{ ReadFromFolderMT");

	int threadsMT = iFileList.size()<_PG_MAX_THREADS_ ? iFileList.size() : _PG_MAX_THREADS_;
	SetMaxThreadCount(threadsMT);

	/*
	HANDLE hThreadMT[_PG_MAX_THREADS_];
    unsigned threadID[_PG_MAX_THREADS_];
	PGCore::Volume<T> oVolumesMT[_PG_MAX_THREADS_];
	PGCore::MetaData<T> oMetaDataMT[_PG_MAX_THREADS_];
	std::vector<std::string> iListMT[_PG_MAX_THREADS_];
	ThreadedIOParams<T> ioParamsMT[_PG_MAX_THREADS_];
	PGCore::Logger nextLoggerMT[_PG_MAX_THREADS_];
	*/

	int i=0, iFileCount = iFileList.size(), itemsPerList = iFileCount/GetMaxThreadCount();

	for (i=0; i<GetMaxThreadCount()-1; i++)
	{
		iListMT[i].resize(itemsPerList);
		std::vector<std::string>::const_iterator sBegin=iFileList.begin() + i*itemsPerList, 
			sEnd = sBegin + itemsPerList;
		std::copy(sBegin, sEnd, iListMT[i].begin());
	}

	// copy the rest now
	int remainingItemCount = iFileCount - (GetMaxThreadCount()-1)*itemsPerList;
	iListMT[GetMaxThreadCount()-1].resize(remainingItemCount);
	std::vector<std::string>::const_iterator sBegin=iFileList.begin() + (GetMaxThreadCount()-1)*itemsPerList, 
			sEnd = sBegin+remainingItemCount;
	std::copy(sBegin, sEnd, iListMT[GetMaxThreadCount()-1].begin());

	// compute skip factors
	PGIO::IODicom<T> dicomImgIO;
	PGCore::MetaData<T> firstMetaData;
	if (!dicomImgIO.GetAttributes(firstMetaData, iFileList[0]))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to GetAttributes.");			
		return false;
	}

	PGMath::Vector3D<int> iSize = firstMetaData.GetSize();
	long iRows = iSize.Y(), iColumns = iSize.X(), iSlices = iFileList.size();
	int msbFirst = firstMetaData.GetMSBFirst();
	int numBits = firstMetaData.GetNumberOfBits(), numBitsInData=8*sizeof(T);	

	if (!ComputeSkipFactors(PGMath::Vector3D<int>(iSize.X(), iSize.Y(), iSlices), (numBits/8),
		m_skipFactorXY, m_skipFactorZ))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to ComputeSkipFactors.");			
		return false;
	}
	
	bool rv = false;
	char szPath[MAX_PATH]; 
	DWORD dwBufferSize = MAX_PATH;
	GetTempPath( dwBufferSize, szPath );	
	std::string pStr = std::string(szPath) + kPgApplicationName + "\\";

	//_PG_BREAK_NATIVE
	// logfilenames
	for (i=0; i<GetMaxThreadCount(); i++)
	{		
		char idxC[5] = {0};
		itoa(i, idxC, 10);
		std::string fName = pStr + std::string("DataLoadThread") + std::string(idxC) + std::string(".txt");

		nextLoggerMT[i].SetLogFile(fName.c_str());
		ioParamsMT[i].iFileNames = (GetMaxThreadCount()==1) ? &iFileList : &(iListMT[i]); //split it here
		ioParamsMT[i].ioAttributes =  (GetMaxThreadCount()==1) ? ioMetaDataObjectMT : 
				(static_cast<PGCore::BaseDataObject *>(&(oMetaDataMT[i])));//iMetaDataObject;
		ioParamsMT[i].ioVolume = (GetMaxThreadCount()==1) ? ioDataObjectMT :
				&(oVolumesMT[i]);
		ioParamsMT[i].statusCB = GetProgressCB();//0;
		ioParamsMT[i].ioLogger = &(nextLoggerMT[i]);//GetLogger();
		ioParamsMT[i].skipXY = m_skipFactorXY;
		ioParamsMT[i].skipZ  = m_skipFactorZ;		
		ioParamsMT[i].iParentThreadPtr = GetAsyncEnabled() ? (void *)this : 0;

		

		// Create the second thread.
		//hThreadMT[i] = (HANDLE)_beginthreadex( (void *)NULL, 0, &PGIO::LoadFilesDICOMThread, (void *) (&(ioParamsMT[i])), 0, &threadIDMT[i] );		
		
		//hThreadMT[i] = CreateThread((void *) (&(ioParamsMT[i])));

		if (!CreateNewThread((ThreadedProcess *)this, (void *) (&(ioParamsMT[i]))))
		{
			LOG1("ERROR: Failed to spawn new thread %d.", GetAliveThreadCount()); 
			return false;
		}		
	}

	ioMetaDataObjectMT->SetSkipFactors(m_skipFactorXY, m_skipFactorZ);

	//_PG_BREAK_NATIVE

	rv = WaitToFinishIfNeeded();	

	LOG1("} ReadFromFolderMT : %d threads spawned", GetAliveThreadCount());

	return rv;
}


template <class T>
bool IODicom3D<T>::ReadFromFolder(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	if (!iMetaDataObject || !oDataObject || iFileList.size()<1)
	{
		LOG0("IODicom3D::Read: Invalid input data/metadata.");			
		return false;
	}
		
	// get number of slices
	// filter the folder with dicom file names
	// use folder list
	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IODicom3D::ReadFromFolder: Invalid output container for metadata.");			
		return false;
	}

	// compute skip factors
	PGIO::IODicom<T> dicomImgIO;
	PGCore::MetaData<T> firstMetaData;
	if (!dicomImgIO.GetAttributes(firstMetaData, iFileList[0]))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to GetAttributes.");			
		return false;
	}

	PGMath::Vector3D<int> iSize = firstMetaData.GetSize();
	long iRows = iSize.Y(), iColumns = iSize.X(), iSlices = iFileList.size();
	int msbFirst = firstMetaData.GetMSBFirst();
	int numBits = firstMetaData.GetNumberOfBits(), numBitsInData=8*sizeof(T);	
	bool ignoreBits = (numBits<numBitsInData);

	int numFrames = firstMetaData.GetFrameCount();
	if (numFrames>1) iSlices = numFrames;

	if (!ComputeSkipFactors(PGMath::Vector3D<int>(iSize.X(), iSize.Y(), iSlices), (numBits/8),
		m_skipFactorXY, m_skipFactorZ))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to ComputeSkipFactors.");			
		return false;
	}

	PGMath::Vector3D<float> oSpacing(firstMetaData.GetSpacing().X()*m_skipFactorXY, 
		firstMetaData.GetSpacing().Y()*m_skipFactorXY, firstMetaData.GetSpacing().Z()*m_skipFactorZ);
	PGMath::Vector3D<int> oSize(iSize.X()/m_skipFactorXY, iSize.Y()/m_skipFactorXY, iSlices/m_skipFactorZ);

	oMetaData->SetSpacing(oSpacing);
	oMetaData->SetSize(oSize);
	oMetaData->SetMSBFirst(msbFirst);
	oMetaData->SetNumberOfBits(numBits);
	oMetaData->SetModality(firstMetaData.GetModality());
	oMetaData->SetSOPClass(firstMetaData.GetSOPClass());
	oMetaData->SetStudyUID(firstMetaData.GetStudyUID());
	oMetaData->SetSeriesUID(firstMetaData.GetSeriesUID());


	/*LOG0("\tIODicom3D::ReadMetaData: Attributes");			
	{
		LOG3("\t\tSize		: %d %d %d", oSize.X(), oSize.Y(), oSize.Z());			
		LOG1("\t\tMSBFirst	: %d", msbFirst);			
		LOG1("\t\tBits		: %d", numBits);			
	}
	LOG0("\tIODicom3D::ReadMetaData: Attributes");			*/

	PGAlgs::ImageEndianSwapper<T, T> caster;
	PGAlgs::ImageBitClipper<T, T> bitclipper(numBitsInData-numBits);

	// create vectors for position/orientation information
	std::vector<PGMath::Vector3D<float> > pPositions, pOrientationsX, pOrientationsY;

	const std::vector<PGMath::Vector3D<float> >& nextPosVec = firstMetaData.GetImagePositionsPatient();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecX = firstMetaData.GetImageOrientationsPatientX();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecY = firstMetaData.GetImageOrientationsPatientY();

	if (nextPosVec.empty() || nextOrVecX.empty() || nextOrVecY.empty())
	{
		LOG0("IODicom3D::ReadMetaData:Warning: Empty pos/orientation vectors.");			
		if (oSize.Z()>1)
		{
			return false;
		}
	}

	std::vector<SlicePosition> slicePositions;
	std::vector<PGCore::Image<T> > tempVolume;

	long fOffset=0, imgSize = iRows*iColumns*sizeof(T);;
	
	PGAlgs::ImageTypeConverter<T, float> forward;
	PGAlgs::ImageTypeConverter<float, T> backward;
	//PGCore::DilationKernel<T, T> dilationKernel(1); bool dilatedOK=false;
	//PGCore::ErosionKernel<char, T> erosionKernel(1);


#if (_SMOOTH_MT_==1)
	int zSkip=1, zSkipFac = min(max(m_skipFactorZ/4, 1), 8);
	PGCore::GaussianKernel<T, T> gausskernel(1.0f), gausskernelZ(1.0f, max(1, m_skipFactorZ/zSkipFac));
	unsigned int zKernelSize = gausskernelZ.GetDimension();
	int zKMid = zKernelSize/2;
	PGAlgs::ImageTypeConverter<T, float> imgConvTtoFloat;
	PGAlgs::ImageTypeConverter<float, T> imgConvFloatToT;
#endif

	PGAlgs::ImageResampler<T, T> resampler;
	resampler.SetScaleFactor(1.0f/(float)m_skipFactorXY);
	
	SetSmoothFlag(m_skipFactorZ>1);

	LOG1("Smoothing status: %d", m_skipFactorZ>1 ? 1 : 0);

	double zWt = 1.0f/(double)m_skipFactorZ;
	
	PGCore::BitImage bImage(iRows, iColumns);
	PGCore::BitImage sbImage(iRows, iColumns);
	
	int		actualImagesRead = 0;
	bool	ignoreImage=false;

	for (int i=0; i<iSlices; i+=m_skipFactorZ)
	{
		//LOG2("{ IODicom3D<%d>::Read [%d]", this, i);			

		ignoreImage = false;

		if (pPositions.size()>= oSize.Z()) break;		

		PGCore::Image<float> tempImage(iRows, iColumns), mixedImage(iRows, iColumns);
		PGCore::Image<T> nextImage(iRows, iColumns);
		PGCore::Image<T> nImage(iRows, iColumns);	
		if (!nextImage.GetBuffer() || !nImage.GetBuffer() || !mixedImage.GetBuffer())
		{
			LOG0("IODicom3D::Read: Invalid image buffer.");			
			return false;
		}

		int zRead = 0;
		int zLim = 1;
#if (_SMOOTH_MT_==1)
		if (GetSmoothFlag()) 
		{
			zLim = m_skipFactorZ;
			zSkip = zSkipFac;
		}
#endif
		for (int j=0; j<zLim; j+=zSkip)
		{
			std::vector<std::string> nextFileName;
			int frameIndex = 0;
			if (numFrames>1)
			{
				nextFileName.push_back(iFileList[0]);
				frameIndex = (i+j);
			} else
			{
				nextFileName.push_back(iFileList[i+j]);				
			}
			PGIO::IOParams ioParamsDicom(
				nextFileName,
				std::string(""),
				PGIO::kPgIOSourceTypeFile);	
			ioParamsDicom.SetActiveFrameIndex(frameIndex);
			PGCore::MetaData<T> nextMetaDataObject;
			if (!dicomImgIO.Read(&nImage, ioParamsDicom, &nextMetaDataObject))
			{			
				LOG1("IODicom3D::Read: failed to open file %s to read.", iFileList[i+j].c_str());							
				continue;
			}

			long nRows=0, nCols=0;
			nImage.GetDimensions(nRows, nCols);
			if (nRows!=iRows || nCols!=iColumns)
			{
				LOG4("IODicom3D::Read: WARNING: Image size mismatch %d %d v/s %d %d.", nRows, nCols, iRows, iColumns);						
				continue;
			}

			// match study and series UIDs
			if (strcmp(firstMetaData.GetStudyUID().c_str(), nextMetaDataObject.GetStudyUID().c_str())
					|| strcmp(firstMetaData.GetSeriesUID().c_str(), nextMetaDataObject.GetSeriesUID().c_str()) )
			{
				LOG0("IODicom3D::Read: WARNING: Image study/series IDs mismatch!");						
				continue;
			}

			zRead++;
#if (_DRAW_SYNTH_SHAPES_)
			// draw a circle
			int zFactor = i/m_skipFactorXY, rad = iRows/8-zFactor;
			memset(nImage.GetBuffer(), 0, nImage.GetDataSize());
			nImage.SetValue(iRows/2, iColumns/2, T(255));
			PGCore::DrawHelper::DrawCircle(iRows/2, iColumns/2, rad, nImage, T(255));
			PGCore::DrawHelper::DrawBox(0, 0, iColumns-1, iRows-1, nImage, T(255));			
			PGCore::DrawHelper::DrawBox(iColumns/2-rad, iRows/2-rad, iColumns/2+rad, iRows/2+rad, nImage, T(255));			
			//PGAlgs::DumpImageAsPGM(nImage, std::string("C:\\Tmp\\Circle.pgm"));		
#endif


			#if (_SMOOTH_MT_==1)			
				double zwt = gausskernelZ.GetValue(zKMid, zKMid-m_skipFactorZ/(2*zSkip) + j);
			#endif
			{		
				imgConvTtoFloat.SetInput(static_cast<PGCore::Image < T > *>(&nImage));
				imgConvTtoFloat.Execute();
				imgConvTtoFloat.GetOutput(static_cast<PGCore::Image < float > *>(&tempImage));	
			}

			if (j==0) 
			{
				#if (_SMOOTH_MT_==1)						
					mixedImage = tempImage*zwt;
				#else
					mixedImage = tempImage;
					//memcpy(nextImage.GetBuffer(), nImage.GetBuffer(), nImage.GetDataSize());
				#endif				

				//push new slice attributes into teh meta data structurs
				const std::vector<PGMath::Vector3D<float> >& nextPosVec = nextMetaDataObject.GetImagePositionsPatient();
				const std::vector<PGMath::Vector3D<float> >& nextOrVecX = nextMetaDataObject.GetImageOrientationsPatientX();
				const std::vector<PGMath::Vector3D<float> >& nextOrVecY = nextMetaDataObject.GetImageOrientationsPatientY();

				slicePositions.push_back(SlicePosition(actualImagesRead, nextPosVec[0]));
				pPositions.push_back(nextPosVec[0]);
				pOrientationsX.push_back(nextOrVecX[0]);
				pOrientationsY.push_back(nextOrVecY[0]);

			} else
			{

				#if (_SMOOTH_MT_==1)
					mixedImage += tempImage*zwt;
				#endif
				//LOG1("\tBlending [%d]..", j);
			}
			
		}	
		
		ignoreImage = (zRead!=(zLim/zSkip));
		if (ignoreImage) continue;

		{		
			imgConvFloatToT.SetInput(static_cast<PGCore::Image < float > *>(&mixedImage));
			imgConvFloatToT.Execute();
			imgConvFloatToT.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
		}

		// if MSB first, swap the endian	
		if (msbFirst)
		{		
			//LOG1("\t{IODicom3D<%d>::Read: Endian Swap.", this);			
			caster.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			caster.Execute();
			caster.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
			//LOG1("\t}IODicom3D<%d>::Read: Endian Swap.", this);			
		}
		// ignore higher bits??
		if (ignoreBits)
		{
			//LOG1("\t{IODicom3D<%d>::Read: ignoreBits.", this);			
			bitclipper.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			bitclipper.Execute();
			bitclipper.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
			//LOG1("\t{IODicom3D<%d>::Read: ignoreBits.", this);			
		}

#if (_SMOOTH_MT_==1)
		if (GetSmoothFlag())
		{
			//LOG1("\t{IODicom3D<%d>::Read: smooth.", this);			
			PGCore::Image<T> sImage;
			gausskernel.Convolve(nextImage, sImage);	
			nextImage = sImage;					
			//LOG1("\t}IODicom3D<%d>::Read: smooth.", this);			
		}
#endif

		//resample images if needed
		if (m_skipFactorXY!=1)
		{
			//LOG1("\t{IODicom3D<%d>::Read: subsample.", this);			
			resampler.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			resampler.Execute();
			resampler.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));				
			//LOG1("\t}IODicom3D<%d>::Read: subsample.", this);			
		}		

		// test frequency domain filtering
		/*
		if (0)
		{
			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\OriginalImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));

			

			PGCore::Image<float> fImage, rImage, iImage;

			//
			forward.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			forward.Execute(); 
			forward.GetOutput(static_cast<PGCore::Image < float > *>(&fImage));	

			PGAlgs::ImageFourierTransform<float, float> fft, ifft;
			// FT image
			fft.SetInput(static_cast<PGCore::Image < float > *>(&fImage));

			fft.Execute();
			fft.GetFourierImages(rImage, iImage);


			//check here
			
			//remove specific frequencies from it
			//PGCore::Image<T> testImage(iRows, iColumns);
			nextImage.Reset(0);
			for (int r=0; r<32; r++)	
			{
				float fVal = ((float)r/32.0f);
				PGCore::DrawHelper::DrawCircleOverlaid(iColumns/2, iRows/2, r, rImage, fVal);
			}
			backward.SetInput(static_cast<PGCore::Image < float > *>(&rImage));
			backward.Execute(); 
			backward.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
			_snprintf(fileName, 255, "C:\\Data\\Dump\\TestImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));
			///PGAlgs::GaussianKernel<float, float> fgk;
			//PGCore::Image<float> ofImage;
			//fgk.Convolve(rImage, ofImage);	
			//rImage = ofImage;
			

			// 
			ifft.SetInversionFlag(true);
			ifft.SetInput(static_cast<PGCore::Image < float > *>(&fImage));
			ifft.SetFourierImages(rImage, iImage);
			ifft.Execute();
			ifft.GetOutput(static_cast<PGCore::Image < float > *>(&fImage));	

			backward.SetInput(static_cast<PGCore::Image < float > *>(&fImage));
			backward.Execute(); 
			backward.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	

			_snprintf(fileName, 255, "C:\\Data\\Dump\\FFTImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));
		}

		// test dilation
		
		if (0)//!dilatedOK)
		{			
			

			
			PGAlgs::ImageBinarizer<T, char> binarizer;
			binarizer.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			binarizer.SetThresholdRange(900, 1024);
			binarizer.Execute();
	
			binarizer.GetOutput(static_cast<PGCore::BitImage *>(&bImage));				

			


			//memset(nextImage.GetBuffer(), 0, nextImage.GetDataSize());
			//nextImage.SetValue(iRows/2, iColumns/2, 255);
			
			//sbImage = bImage;

			bImage.UnPack(nextImage);
			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\OriginalImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));
			//PGCore::Image<T> sImage;
			//dilationKernel.Convolve(nextImage, sImage);	

			//PGCore::BitImage sbImage(iRows, iColumns);
			dilationKernel.Convolve(bImage, sbImage);	
			sbImage.UnPack(nextImage);
			_snprintf(fileName, 255, "C:\\Data\\Dump\\MorphedImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));

			//bImage.UnPack(nextImage);

			// subtract org from this
			//sImage -= nextImage;
			bImage.Reset(false);
			erosionKernel.Convolve(sbImage, bImage);	//nextImage = sImage;
			bImage.UnPack(nextImage);
			_snprintf(fileName, 255, "C:\\Data\\Dump\\ClosedImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));

			//dilatedOK = true;
			//nextImage = sImage;
		}
		

		if (1)
		{
			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\OriginalImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));
			
			nextImage.Reset(0);
			
			// draw a box insted
			
			//for (int r=31; r<32; r++)	PGCore::DrawHelper::DrawCircleFilled(iColumns/2, iRows/2, r, nextImage, T(255));
			

			
			//nextImage.SetValue(63, 63, 255);
			//nextImage.SetValue(63, 64, 255);
			//nextImage.SetValue(64, 64, 255);
			//nextImage.SetValue(64, 63, 255);

			nextImage.SetValue(63, 63, 255);
			nextImage.SetValue(63, 32, 255); nextImage.SetValue(63, 95, 255); // vertical dots
			nextImage.SetValue(32, 63, 255); nextImage.SetValue(95, 63, 255); // vertical dots


			_snprintf(fileName, 255, "C:\\Data\\Dump\\CircleImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));	
		}*/

		//LOG1("IODicom3D::Read: Image [%d] DONE.", i);			
		tempVolume.push_back(nextImage);

		actualImagesRead++;
		//oDataObject->AddImage(nextImage);

		//LOG2("} IODicom3D<%d>::Read [%d]", this, i);			

		////_PG_BREAK_NATIVE
		if (i%4==0)	
		{
			int progressValue = (int)(95.0f*((float)(i)/(float)iSlices) + 0.5f);
			UpdateProgress(progressValue);
			//LOG1("+++ Loading progress %d / 100", progressValue);
			//::Sleep(100);
		}
	}

	//sort slices on position, perform sanitary checks
	std::sort(slicePositions.begin(), slicePositions.end(), SlicePositionSorter());

	//then add image
	bool res = RearrangeSlices(slicePositions, tempVolume, *oDataObject, pPositions, pOrientationsX, pOrientationsY);
	if (!res)
	{
		LOG0("IODicom3D::Read: Error: Cannot rearrange slices on position");			
		return false;
	}

	//oDataObject->AddImage(nextImage);

	// correct the imagecount read
	oSize = PGMath::Vector3D<int>(oSize.X(), oSize.Y(), actualImagesRead);

	if (pPositions.size()!=oSize.Z() || 
		pOrientationsX.size()!=oSize.Z() || 
		pOrientationsY.size()!=oSize.Z())
	{
		LOG1("IODicom3D::Read: Error: Only %d position/orientation vectors read in", pPositions.size());			
		return false;
	}

	// set size
	oMetaData->SetSize(PGMath::Vector3D<int>(oSize.X(), oSize.Y(), oSize.Z()));
	
	// compute spacing
	//std::vector<PGMath::Vector3D<float> > unsortedPositions = pPositions;	
	//std::sort(unsortedPositions.begin(), unsortedPositions.end(), PGMath::Vector3D<float>::BinPred());
	
	float spacing = 1.0f;
	if (actualImagesRead>1)
	{
		PGMath::Vector3D<float> diffVector = pPositions[oSize.Z()-1] - pPositions[0];
		spacing = diffVector.Length()/(oSize.Z()-1);
	}
	//LOG1("IODicom3D::Read: Spacing: %3.5f", spacing);			

	oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), spacing));
	

	// set vectors
	oMetaData->SetImagePositionsPatient(pPositions);
	oMetaData->SetImageOrientationsPatientX(pOrientationsX);
	oMetaData->SetImageOrientationsPatientY(pOrientationsY);
	
	//LOG3("IODicom3D::Read: Pos[0]: %3.4f %3.4f %3.4f", pPositions[0].X(), pPositions[0].Y(), pPositions[0].Z());
	//LOG3("IODicom3D::Read: OrX[0]: %3.4f %3.4f %3.4f", pOrientationsX[0].X(), pOrientationsX[0].Y(), pOrientationsX[0].Z());
	//LOG3("IODicom3D::Read: OrY[0]: %3.4f %3.4f %3.4f", pOrientationsY[0].X(), pOrientationsY[0].Y(), pOrientationsY[0].Z());
	//LOG1("IODicom3D::Read: AxialScan: %d", oMetaData->GetAxialScanFlag()==true ? 1 : 0);
	//LOG1("IODicom3D::Read: AxialScanFeetFirst: %d", oMetaData->GetAxialScanFeetFirstFlag()==true ? 1 : 0);
	
	return (oDataObject->GetSize() == oSize.Z());		
}


template <class T>
bool IODicom3D<T>::ReadFromFile(const std::string& iFile,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	if (!iMetaDataObject || !oDataObject || iFile.empty())
	{
		LOG0("IODicom3D::Read: Invalid input data/metadata.");			
		return false;
	}
		
	// get number of slices
	// filter the folder with dicom file names
	// use folder list
	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IODicom3D::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// compute skip factors
	PGIO::IODicom<T> dicomImgIO;
	PGCore::MetaData<T> firstMetaData;
	if (!dicomImgIO.GetAttributes(firstMetaData, iFile))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to GetAttributes.");			
		return false;
	}

	PGMath::Vector3D<int> iSize = firstMetaData.GetSize();
	long iRows = iSize.Y(), iColumns = iSize.X(), iSlices = 1;//iFileList.size();
	int msbFirst = firstMetaData.GetMSBFirst();
	int numBits = firstMetaData.GetNumberOfBits(), numBitsInData=8*sizeof(T);	

	if (!ComputeSkipFactors(PGMath::Vector3D<int>(iSize.X(), iSize.Y(), iSlices), (numBits/8),
		m_skipFactorXY, m_skipFactorZ))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to ComputeSkipFactors.");			
		return false;
	}

	PGMath::Vector3D<float> oSpacing(firstMetaData.GetSpacing().X()*m_skipFactorXY, 
		firstMetaData.GetSpacing().Y()*m_skipFactorXY, firstMetaData.GetSpacing().Z()*m_skipFactorZ);
	PGMath::Vector3D<int> oSize(iSize.X()/m_skipFactorXY, iSize.Y()/m_skipFactorXY, iSlices/m_skipFactorZ);

	oMetaData->SetSpacing(oSpacing);
	oMetaData->SetSize(oSize);
	oMetaData->SetMSBFirst(msbFirst);
	oMetaData->SetNumberOfBits(numBits);

	/*LOG0("\tIODicom3D::ReadMetaData: Attributes");			
	{
		LOG3("\t\tSize		: %d %d %d", oSize.X(), oSize.Y(), oSize.Z());			
		LOG1("\t\tMSBFirst	: %d", msbFirst);			
		LOG1("\t\tBits		: %d", numBits);			
	}
	LOG0("\tIODicom3D::ReadMetaData: Attributes");	*/		

	PGAlgs::ImageEndianSwapper<T, T> caster;
	PGAlgs::ImageBitClipper<T, T> bitclipper(numBitsInData-numBits);

	// create vectors for position/orientation information
	std::vector<PGMath::Vector3D<float> > pPositions, pOrientationsX, pOrientationsY;

	const std::vector<PGMath::Vector3D<float> >& nextPosVec = firstMetaData.GetImagePositionsPatient();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecX = firstMetaData.GetImageOrientationsPatientX();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecY = firstMetaData.GetImageOrientationsPatientY();

	if (nextPosVec.empty() || nextOrVecX.empty() || nextOrVecY.empty())
	{
		LOG0("IODicom3D::ReadMetaData: Empty pos/orientation vectors.");			
		return false;
	}

	std::vector<SlicePosition> slicePositions;
	std::vector<PGCore::Image<T> > tempVolume;

	long fOffset=0, imgSize = iRows*iColumns*sizeof(T);;
	
	PGCore::GaussianKernel<T, T> gausskernel(1.0f);
	PGAlgs::ImageResampler<T, T> resampler;
	resampler.SetScaleFactor(1.0f/(float)m_skipFactorXY);
	
	SetSmoothFlag(m_skipFactorZ>1);

	
	
	//for (int i=0; i<iSlices; i+=m_skipFactorZ)
	{
		//if (pPositions.size()>= oSize.Z()) break;

		PGCore::Image<T> nextImage(iRows, iColumns);	
		T* oBuf = nextImage.GetBuffer();
		if (!oBuf)
		{
			LOG0("IODicom3D::Read: Invalid image buffer.");			
			return false;
		}
		
		
		{
			PGCore::Image<T> nImage(iRows, iColumns);	

			std::vector<std::string> nextFileName;
			nextFileName.push_back(iFile);
			PGIO::IOParams ioParamsDicom(
				nextFileName,
				std::string(""),
				PGIO::kPgIOSourceTypeFile);	
			PGCore::MetaData<T> nextMetaDataObject;
			if (!dicomImgIO.Read(&nImage, ioParamsDicom, &nextMetaDataObject))
			{			
				LOG1("IODicom3D::Read: failed to open file %s to read.", iFile.c_str());			
				return false;
				//continue;
			}


			nextImage = nImage;


			//push new slice attributes into teh meta data structurs
			const std::vector<PGMath::Vector3D<float> >& nextPosVec = nextMetaDataObject.GetImagePositionsPatient();
			const std::vector<PGMath::Vector3D<float> >& nextOrVecX = nextMetaDataObject.GetImageOrientationsPatientX();
			const std::vector<PGMath::Vector3D<float> >& nextOrVecY = nextMetaDataObject.GetImageOrientationsPatientY();

			slicePositions.push_back(SlicePosition(0/m_skipFactorZ, nextPosVec[0]));
			pPositions.push_back(nextPosVec[0]);
			pOrientationsX.push_back(nextOrVecX[0]);
			pOrientationsY.push_back(nextOrVecY[0]);
		}	

		
		// if MSB first, swap the endian	
		if (msbFirst)
		{			
			caster.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			caster.Execute();
			caster.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
		}
		// ignore higher bits??
		/*if (ignoreBits)
		{
			bitclipper.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			bitclipper.Execute();
			bitclipper.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
		}*/

		
		if (GetSmoothFlag())
		{
			PGCore::Image<T> sImage;
			gausskernel.Convolve(nextImage, sImage);	
			nextImage = sImage;
			//LOG1("\t Smooth Image [%d] DONE.", i);			
		}

		//resample images if needed
		if (m_skipFactorXY!=1)
		{
			resampler.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			resampler.Execute();
			resampler.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));				
		}		

		//LOG1("IODicom3D::Read: Image [%d] DONE.", i);			
		tempVolume.push_back(nextImage);
		//oDataObject->AddImage(nextImage);

		//if (i%4==0)	
		UpdateProgress(100.0f*((float)m_skipFactorZ/(float)iSlices));
	}

	//sort slices on position
    //perform sanitary checks
	std::sort(slicePositions.begin(), slicePositions.end(), SlicePositionSorter());

	//then add image
	bool res = RearrangeSlices(slicePositions, tempVolume, *oDataObject, pPositions, pOrientationsX, pOrientationsY);
	if (!res)
	{
		LOG0("IODicom3D::Read: Error: Cannot rearrange slices on position");			
		return false;
	}

	//oDataObject->AddImage(nextImage);

	if (pPositions.size()!=oSize.Z() || 
		pOrientationsX.size()!=oSize.Z() || 
		pOrientationsY.size()!=oSize.Z())
	{
		LOG1("IODicom3D::Read: Error: Only %d position/orientation vectors read in", pPositions.size());			
		return false;
	}

	// set size
	oMetaData->SetSize(PGMath::Vector3D<int>(oSize.X(), oSize.Y(), oSize.Z()));
	
	// compute spacing
	//std::vector<PGMath::Vector3D<float> > unsortedPositions = pPositions;	
	//std::sort(unsortedPositions.begin(), unsortedPositions.end(), PGMath::Vector3D<float>::BinPred());
	
	float spacing = 1.0f;
	if (iSlices>1)
	{
		PGMath::Vector3D<float> diffVector = pPositions[oSize.Z()-1] - pPositions[0];
		spacing = diffVector.Length()/(oSize.Z()-1);
	}
	//LOG1("IODicom3D::Read: Spacing: %3.5f", spacing);			

	oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), spacing));
	

	// set vectors
	oMetaData->SetImagePositionsPatient(pPositions);
	oMetaData->SetImageOrientationsPatientX(pOrientationsX);
	oMetaData->SetImageOrientationsPatientY(pOrientationsY);
	
	//LOG3("IODicom3D::Read: Pos[0]: %3.4f %3.4f %3.4f", pPositions[0].X(), pPositions[0].Y(), pPositions[0].Z());
	//LOG3("IODicom3D::Read: OrX[0]: %3.4f %3.4f %3.4f", pOrientationsX[0].X(), pOrientationsX[0].Y(), pOrientationsX[0].Z());
	//LOG3("IODicom3D::Read: OrY[0]: %3.4f %3.4f %3.4f", pOrientationsY[0].X(), pOrientationsY[0].Y(), pOrientationsY[0].Z());
	//LOG1("IODicom3D::Read: AxialScan: %d", oMetaData->GetAxialScanFlag()==true ? 1 : 0);
	//LOG1("IODicom3D::Read: AxialScanFeetFirst: %d", oMetaData->GetAxialScanFeetFirstFlag()==true ? 1 : 0);
	
	return (oDataObject->GetSize() == oSize.Z());		
	return false;	
}


template <class T>
bool IODicom3D<T>::RearrangeSlices(const std::vector<SlicePosition>& iSortedPositions, std::vector<PGCore::Image<T> >& iVolume,
		PGCore::Volume<T>& oVolume,
		std::vector<PGMath::Vector3D<float> >& ioSortedPositions, std::vector<PGMath::Vector3D<float> >& ioSortedOrX, 
		std::vector<PGMath::Vector3D<float> >& ioSortedOrY)
{

	LOG1("\t\t{ Begin RearrangeSlices[%d]...", this);

	if (iSortedPositions.empty())
	{
		return false;
	}

	std::vector<PGMath::Vector3D<float> > iSrcPos = ioSortedPositions; ioSortedPositions.clear();
	std::vector<PGMath::Vector3D<float> > iSrcOrx = ioSortedOrX; ioSortedOrX.clear();
	std::vector<PGMath::Vector3D<float> > iSrcOry = ioSortedOrY; ioSortedOrY.clear();
	oVolume.Clear();

	for (int i=iSortedPositions.size()-1; i>=0; i--)
	{
		int srcIndex = iSortedPositions[i].first;

		// copy sorted info
		ioSortedPositions.push_back(iSrcPos[srcIndex]);
		ioSortedOrX.push_back(iSrcOrx[srcIndex]);
		ioSortedOrY.push_back(iSrcOry[srcIndex]);
		
		oVolume.AddImage(iVolume[srcIndex]); 
		iVolume[srcIndex].Clear();
	}

	LOG1("\t\t} End RearrangeSlices[%d]...", this);

	return (oVolume.GetSize() == ioSortedPositions.size());
}

template<class T>
void IODicom3D<T>::StartExecution(void *iParam)
{ 
	if (!iParam) return;

	//using namespace PGIO;
	PGIO::ThreadedIOParams<short> * iParams = static_cast<PGIO::ThreadedIOParams<short> *>(iParam);

	std::vector<std::string> iFileNames = *(iParams->iFileNames);

	//_PG_BREAK_NATIVE
	iParams->ioLogger->Rotate();

	//LOG0("{ LoadFilesDICOMThread");
	iParams->ioLogger->Log("{ LoadFilesDICOMThread");
	if (iParams->iFileNames==NULL || iParams->ioVolume==NULL || iParams->ioAttributes==NULL)
	{
		iParams->ioLogger->Log("} LoadFilesDICOMThread: Error: empty filelist");
		if (iParams->iParentThreadPtr)
		{
			IODicom3D<short>* iParentThreadPtr = (IODicom3D<short> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyError("Empty file list.");
		}
		//_endthreadex( 0 );
		return;
	}			

	if(iParams->iFileNames->empty())
	{
		iParams->ioLogger->Log("} LoadFilesDICOMThread: Error: empty filelist");
		if (iParams->iParentThreadPtr)
		{
			IODicom3D<short>* iParentThreadPtr = (IODicom3D<short> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyError("Empty file list.");
		}
		Sleep(100);
		//_endthreadex( 0 );
		return;
	}

	//const std::vector<std::string> &iFileNames = *(iParams->iFileNames);
	iParams->ioLogger->Log("\t{FileList:");
	for (int i=0; i<iFileNames.size(); i++)
	{
		iParams->ioLogger->Log("\t\t[%d]: [%s]", i, iFileNames[i].c_str());
	}
	iParams->ioLogger->Log("\t}FileList.");

	iParams->ioLogger->Log("%d Files will be read.", iParams->iFileNames->size());		


	iParams->ioLogger->Log("{ 3D IO Read...");
	PGIO::ePgIOSourceType srcType = PGIO::kPgIOSourceTypeFile;//iFileNames.size()==1 ? PGIO::kPgIOSourceTypeFile
	//: PGIO::kPgIOSourceTypeFolder;		
	{
		//read each slice one by one so progress gets updated
		PGIO::IODicom3D<short> pDICOMImgIO3D;
		PGIO::IOParams ioParamsDicom(
			iFileNames,
			std::string(""),
			srcType);
		//pDICOMImgIO3D.SetLogger(iParams->ioLogger);			
		pDICOMImgIO3D.SetProgressCB(iParams->statusCB);	

		pDICOMImgIO3D.SetSkipFactors(iParams->skipXY, iParams->skipZ);
		if (!pDICOMImgIO3D.Read(iParams->ioVolume, ioParamsDicom, iParams->ioAttributes))
		{
			iParams->ioLogger->Log("} LoadFilesDICOMThread: Error: DICOM read failed");
			if (iParams->iParentThreadPtr)
			{
				IODicom3D<short>* iParentThreadPtr = (IODicom3D<short> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Failed to read images.");
			}
			Sleep(100);
			//_endthreadex( 0 );
			return;
		}
	} 
	iParams->ioLogger->Log("} 3D IO Read Done!");

	iParams->ioLogger->Log("} LoadFilesDICOMThread");
	//_endthreadex( 0 );

	//PostMessage(iParams->iParentWindowHandle, 0, 0, 0);

	//_PG_BREAK_NATIVE
	if (iParams->iParentThreadPtr)
	{
		IODicom3D<short>* iParentThreadPtr = (IODicom3D<short> *)(iParams->iParentThreadPtr);
		iParentThreadPtr->NotifyFinished();
	}

	return;
}

#undef _DRAW_SYNTH_SHAPES_

#ifdef _PG_GENERATE_SDK_LIBS_
	template class IODicom3D<short>; 
	template class IODicom3D<unsigned char>; 
#endif

};



