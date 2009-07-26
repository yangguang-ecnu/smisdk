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
// Filename:	IORaw3D.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IORaw3D.h: interface for the IORaw3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IO/IORaw3D.h"
#include "Core/GaussianKernel.h"
#include "Algs/ImageResampler.h"

namespace PGIO
{
	
template <class T>
IORaw3D<T>::IORaw3D(void) 
{
	SetAsyncEnabled(false);
};	
		
template <class T>
IORaw3D<T>::~IORaw3D(void) 
{
};	


template <class T>
bool IORaw3D<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	return false;
}


template <class T>
bool IORaw3D<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject) {
		LOG0("IORaw3D::Read: metadata object null");			
		return false;
	}

	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IORaw3D::Read: Expecting single file path.");			
		return false;
	} 

	// read one file / multiple based on the type set here
	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IODicom::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &iSourceName = iPaths[0];
	if (iSourceName.empty())
	{
		LOG0("IORaw3D::Read: Invalid input filename.");			
		return false;
	}
	
	if (!oDataObject) 
	{
		LOG0("IORaw3D::Read: Invalid input image.");			
		return false;
	}

	// cast to volume type here
	PGCore::Volume<T> *oVolume = (static_cast<PGCore::Volume < T > *>(oDataObject));
	if (!oVolume) 
	{
		LOG0("IORaw3D::Read: Invalid input volume.");			
		return false;
	}

	// decide on multithreading here
	m_MultiThreadingEnabled = iParams.IsEnabledMultiThreading();

	bool res = false;
	if (m_MultiThreadingEnabled)
	{
		ioMetaDataObjectMT = oMetaData;
		ioDataObjectMT = oVolume;
		SetAsyncEnabled(iParams.IsEnabledAsynchronousLoad());
		m_autoSkip = iParams.IsEnabledAutoSkip();
		res = ReadFromFileMT(iSourceName);
	} else
	{
		res = ReadFromFile(iSourceName, oMetaData, oVolume);	
	}
	
	return res;
}

// protected
template <class T>
bool IORaw3D<T>::WriteToFile(const std::string& iFile,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	if (!iMetaDataObject || !oDataObject)
	{
		LOG0("IORaw3D::Read: Invalid input data/metadata.");			
		return false;
	}
	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IORaw3D::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// a single file with big volume
	FILE *fptr = fopen (iFile.c_str(), "w+b");
	if (!fptr) {				
		LOG1("IORaw3D::Read: failed to open file %s to write.", iFile.c_str());			
		return false;
	}
	
	// get number of slices
	PGMath::Vector3D<int> iSize = oMetaData->GetSize();
	long iRows = iSize.Y(), iColumns = iSize.X();

	


	int msbFirst = oMetaData->GetMSBFirst();
	

	
	long fOffset=0, imgSize = iRows*iColumns*sizeof(T), totalWritten=0;
	
	PGCore::GaussianKernel<T, T> gausskernel(1.0f);

	

	LOG1("Smoothing status: %d", (GetSmoothFlag()==1) ? 1 : 0);

	
	
	for (int i=0; i<iSize.Z();  i++)
	{
		PGCore::Image<T> nextImage = oDataObject->GetImage(i);	
		
		// reverse z
		//const PGCore::Image<T>& nextImage = oDataObject->GetImage(iSize.Z()-1-i);	

		// if MSB first, swap the endian	
//		if (msbFirst)
//		{
//			PGAlgs::ImageEndianSwapper<T, T> caster;
//			caster.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
//			caster.Execute();
//			caster.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
//		}

		if (GetSmoothFlag())
		{
			PGCore::Image<T> sImage;
			gausskernel.Convolve(nextImage, sImage);	
			nextImage = sImage;
			LOG1("\t Smooth Image [%d] DONE.", i);			
		}

		LOG1("IORaw3D::Read: Image [%d] DONE.", i);			

		T* oBuf = nextImage.GetBuffer();
		if (!oBuf)
		{
			LOG0("IORaw3D::Write: Invalid image buffer.");			
			return false;
		}
		

		fOffset = i*imgSize;
		try
		{			
			totalWritten+= (fwrite((void *)oBuf, 1, imgSize, fptr))/imgSize;
		} catch(...)
		{
			fclose(fptr);
			LOG1("IORaw3D::Write: failed to write file %s. Possible overrun.", iFile.c_str());			
			return false;
		}
	
	}
	
	fclose(fptr);
	return (totalWritten == iSize.Z());
}

template <class T>
void IORaw3D<T>::StartExecution(void *iParam)
{
	if (!iParam) return;

	//using namespace PGIO;
	PGIO::ThreadedIOParams<short> * iParams = static_cast<PGIO::ThreadedIOParams<short> *>(iParam);

	std::vector<std::string> iFileNames = *(iParams->iFileNames);

	//_PG_BREAK_NATIVE
	iParams->ioLogger->Rotate();

	//LOG0("{ LoadFilesDICOMThread");
	iParams->ioLogger->Log("{ StartExecution");
	if (iParams->iFileNames==NULL || iParams->ioVolume==NULL || iParams->ioAttributes==NULL)
	{
		iParams->ioLogger->Log("} StartExecution: Error: empty filelist");
		if (iParams->iParentThreadPtr)
		{
			IORaw3D<short>* iParentThreadPtr = (IORaw3D<short> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyError("Empty file list.");
		}
		//_endthreadex( 0 );
		return;
	}			

	if(iParams->iFileNames->empty())
	{
		iParams->ioLogger->Log("} StartExecution: Error: empty filelist");
		if (iParams->iParentThreadPtr)
		{
			IORaw3D<short>* iParentThreadPtr = (IORaw3D<short> *)(iParams->iParentThreadPtr);
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
		PGIO::IORaw3D<short> pRAWImgIO3D;
		PGIO::IOParams ioParamsRaw(
			iFileNames,
			std::string(""),
			srcType);
		//pDICOMImgIO3D.SetLogger(iParams->ioLogger);			
		pRAWImgIO3D.SetProgressCB(iParams->statusCB);	

		// set smoothing flag
		pRAWImgIO3D.SetSmoothFlag(iParams->smoothWhenSkipping);

		pRAWImgIO3D.SetSkipFactors(iParams->skipXY, iParams->skipZ);
		if (!pRAWImgIO3D.Read(iParams->ioVolume, ioParamsRaw, iParams->ioAttributes))
		{
			iParams->ioLogger->Log("} LoadFilesDICOMThread: Error: DICOM read failed");
			if (iParams->iParentThreadPtr)
			{
				IORaw3D<short>* iParentThreadPtr = (IORaw3D<short> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Failed to read images.");
			}
			Sleep(100);
			//_endthreadex( 0 );
			return;
		}
	} 
	iParams->ioLogger->Log("} 3D IO Read Done!");

	iParams->ioLogger->Log("} StartExecution");
	//_endthreadex( 0 );

	//PostMessage(iParams->iParentWindowHandle, 0, 0, 0);

	//_PG_BREAK_NATIVE
	if (iParams->iParentThreadPtr)
	{
		IORaw3D<short>* iParentThreadPtr = (IORaw3D<short> *)(iParams->iParentThreadPtr);
		iParentThreadPtr->NotifyFinished();
	}

	return;
}

template <class T>
bool IORaw3D<T>::ReadFromFileMT(const std::string& iFile)
{
    LOG0("{ ReadFromFileMT");

	std::vector<std::string> iFileList;
	iFileList.push_back(iFile);

	int threadsMT = iFileList.size()<_PG_MAX_THREADS_ ? iFileList.size() : _PG_MAX_THREADS_;
	SetMaxThreadCount(threadsMT);

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
	PGMath::Vector3D<int> iSize = ioMetaDataObjectMT->GetSize();
	long iRows = iSize.Y(), iColumns = iSize.X(), iSlices = iSize.Z();
	int msbFirst = ioMetaDataObjectMT->GetMSBFirst();
	int numBits = ioMetaDataObjectMT->GetNumberOfBits(), numBitsInData=8*sizeof(T);	

	if (!ComputeSkipFactors(PGMath::Vector3D<int>(iSize.X(), iSize.Y(), iSlices), (numBits/8),
		m_skipFactorXY, m_skipFactorZ))
	{
		LOG0("ReadFromFileMT::ReadMetaData: Failed to ComputeSkipFactors.");			
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
		ioParamsMT[i].smoothWhenSkipping = GetSmoothFlag();
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

	LOG1("} ReadFromFileMT : %d threads spawned", GetAliveThreadCount());

	return rv;
}


template <class T>
bool IORaw3D<T>::ReadFromFile(const std::string& iFile,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	if (!iMetaDataObject || !oDataObject)
	{
		LOG0("IORaw3D::Read: Invalid input data/metadata.");			
		return false;
	}
	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IORaw3D::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// a single file with big volume
	FILE *fptr = fopen (iFile.c_str(), "rb");
	if (!fptr) {				
		LOG1("IORaw3D::Read: failed to open file %s to read.", iFile.c_str());			
		return false;
	}

	int msbFirst = oMetaData->GetMSBFirst();
	int numBits = oMetaData->GetNumberOfBits(), numBitsInData = 8*sizeof(T);	
	bool ignoreBits = (numBits<numBitsInData);

	PGAlgs::ImageEndianSwapper<T, T> caster;
	PGAlgs::ImageBitClipper<T, T> bitclipper(numBitsInData-numBits);

	// get number of slices
	PGMath::Vector3D<int> iSize = oMetaData->GetSize();
	long iRows = iSize.Y(), iColumns = iSize.X(), iSlices = iSize.Z();
	long fOffset=0, imgSize = iRows*iColumns, imgSizeDst = imgSize*sizeof(T), imgSizeSrc = imgSize*oMetaData->GetSamplesPerPixel();;
	
	if (imgSizeDst<imgSizeSrc)
	{
		LOG0("IORaw3D::Read: image size mismatch");
		fclose(fptr);
		return false;
	}

	int numFrames = oMetaData->GetFrameCount();
	if (numFrames>1) iSlices = numFrames;

	if (!ComputeSkipFactors(PGMath::Vector3D<int>(iSize.X(), iSize.Y(), iSlices), (numBits/8),
		m_skipFactorXY, m_skipFactorZ))
	{
		LOG0("ReadFromFolderMT::ReadMetaData: Failed to ComputeSkipFactors.");			
		return false;
	}

	const PGMath::Vector3D<float>& iSpc = oMetaData->GetSpacing();
	PGMath::Vector3D<float> oSpacing(iSpc.X()*m_skipFactorXY, iSpc.Y()*m_skipFactorXY, iSpc.Z()*m_skipFactorZ);
	PGMath::Vector3D<int> oSize(iSize.X()/m_skipFactorXY, iSize.Y()/m_skipFactorXY, max(iSlices/m_skipFactorZ, 1));

	oMetaData->SetSpacing(oSpacing);
	oMetaData->SetSize(oSize);

	PGCore::GaussianKernel<T, T> gausskernel(1.0f);

	PGAlgs::ImageResampler<T, T> resampler;
	resampler.SetScaleFactor(1.0f/(float)m_skipFactorXY);

	PGCore::Image<T> nextImage(iRows, iColumns);	
	T* oBuf = nextImage.GetBuffer();
	if (!oBuf)
	{
		LOG0("IORaw3D::Read: Invalid image buffer.");			
		fclose(fptr);
		return false;
	}

	for (int i=0; i<iSlices; i+=m_skipFactorZ)
	{
		//LOG2("IORaw3D::Read: Image [%d / %d]", i, iSize.Z());	
		fOffset = i*imgSizeSrc;

		try
		{
			fseek(fptr, fOffset, SEEK_SET);
			fread((void *)oBuf, imgSizeSrc, 1, fptr);
		} catch(...)
		{
			fclose(fptr);
			LOG1("IORaw3D::Read: failed to read file %s. Possible overrun.", iFile.c_str());			
			return false;
		}

		// if MSB first, swap the endian. we expect LSB first for intel cnvention
		if (msbFirst)
		{
			LOG0("IORaw3D::Read: msbFirst");						
			caster.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			caster.Execute();
			caster.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));	
			LOG0("End IORaw3D::Read: msbFirst");						
		}
		
		// ignore higher bits??
		if (ignoreBits)
		{
			LOG0("IORaw3D::Read: ignoreBits");						
			bitclipper.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			bitclipper.Execute();
			bitclipper.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));		
			LOG0("End IORaw3D::Read: ignoreBits");						
		}

		//resample images if needed
		if (m_skipFactorXY!=1)
		{
			//LOG1("\t{IORaw3D<%d>::Read: subsample.", this);			
			resampler.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			resampler.Execute();
			resampler.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));				
			//LOG1("\t}IORaw3D<%d>::Read: subsample.", this);			
		}

		if (GetSmoothFlag())
		{
			LOG0("IORaw3D::Read: smooth");						
			PGCore::Image<T> sImage;
			gausskernel.Convolve(nextImage, sImage);	
			nextImage = sImage;
			LOG1("IORaw3D::Read: smooth Image [%d] DONE.", i);			
		}

		if (oDataObject->GetSize() < oSize.Z())	oDataObject->AddImage(nextImage);
		else break;

		if (i%4==0)	
		{
			int progressValue = (int)(95.0f*((float)(i)/(float)iSlices) + 0.5f);
			UpdateProgress(progressValue);
			//LOG1("+++ Loading progress %d / 100", progressValue);
			//::Sleep(100);
		}

		//LOG1("IORaw3D::Read: Image [%d] DONE.", i);			
	}
	
	fclose(fptr);

	int oSizeZ = oDataObject->GetSize(), readImages = oSize.Z();
	bool res = (oSizeZ==readImages);

	return res;	
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class IORaw3D<short>; 
	template class IORaw3D<unsigned char>; 
#endif

};



