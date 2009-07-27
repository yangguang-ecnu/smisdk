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
// Filename:	IODef3D.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IODef3D.h: interface for the IODef3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#include "IO/IODef3D.h"

#ifdef _DRAW_SYNTH_SHAPES_
	#include "Core/DrawHelper.h"
#endif

namespace PGIO
{

template <class T>
inline IODef3D<T>::IODef3D(void) 
{ 
	SetAsyncEnabled(false); m_fileFormat = kPgIOSourceFormatUnknown; 
};

template <class T>
inline IODef3D<T>::~IODef3D(void) 
{ 	
};

template <class T>
inline bool IODef3D<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	return false;
}
 

template <class T>
inline bool IODef3D<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject)
	{
		LOG0("IO/IODef3D::Read: metadata object null");			
		return false;
	}

	/*
	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IO/IODef3D::Read: failed to read metadata.");			
		return false;
	}*/

	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// read one file / multiple based on the type set here
	const std::vector<std::string> &iSourceNamesDef = iParams.Source();
	if (iSourceNamesDef.empty())
	{
		LOG0("IO/IODef3D::Read: Invalid input filename.");			
		return false;
	}
	
	if (!oDataObject) 
	{
		LOG0("IO/IODef3D::Read: Invalid input image.");			
		return false;
	}

	// parse the def file
	std::vector<std::string> iSourceNames;
	std::string formatStr;
	int r=0, c=0, bppCode=1;
	float zSpacing=1.0f;
	bool rv = ParseDefinitionFile(iSourceNamesDef[0], iSourceNames, 
		r, c, bppCode, zSpacing, formatStr);
	if (!rv)
	{
		return false; 
	}

	//PGIO::ePgIOSourceFormat fFormat = kPgIOSourceFormatUnknown;
	if (strncmp(formatStr.c_str(), "PGM", 3)==0)
	{
		m_fileFormat = kPgIOSourceFormatPGM;
	} else if (strncmp(formatStr.c_str(), "PPM", 3)==0)
	{
		m_fileFormat = kPgIOSourceFormatPPM;
	}

	// cast to volume type here
	PGCore::Volume<T> *oVolume = (static_cast<PGCore::Volume < T > *>(oDataObject));
	if (!oVolume) 
	{
		LOG0("IO/IODef3D::Read: Invalid input volume.");			
		return false;
	}

	m_MultiThreadingEnabled = iParams.IsEnabledMultiThreading();

	bool res = false;
	switch(iParams.SourceType())
	{
		case kPgIOSourceTypeFile: 			

		case kPgIOSourceTypeFolder: 
			if (m_MultiThreadingEnabled)
			{
				ioMetaDataObjectMT = oMetaData;
				ioDataObjectMT = oVolume;
				SetAsyncEnabled(iParams.IsEnabledAsynchronousLoad());
				m_autoSkip = iParams.IsEnabledAutoSkip();
				oMetaData->SetSpacing(PGMath::Vector3D<float>(1, 1, zSpacing));

				switch (m_fileFormat)
				{
					case kPgIOSourceFormatPGM:	
						//if (strcmp(formatStr.c_str(), "PGM")) break;
						res = ReadFromFolderMT_PGM(iSourceNames);	
						break;
												
					case kPgIOSourceFormatPPM:	
						//if (strcmp(formatStr.c_str(), "PPM")) break;
						res = ReadFromFolderMT_PPM(iSourceNames);	
						break;

					default:	break;
				}

				

			} else
			{
				switch (m_fileFormat)
				{
					case kPgIOSourceFormatPGM:	
						res = ReadFromFolderPGM(iSourceNames, oMetaData, oVolume);	
						break;
												
					case kPgIOSourceFormatPPM:							
						res = ReadFromFolderPPM(iSourceNames, oMetaData, oVolume);	
						break;

					default:	break;
				}				
			}
			break;
		
		default: break;
	}
	
	return res;
}


template <class T>
inline bool IODef3D<T>::ParseDefinitionFile(const std::string &iDefSourceName, std::vector<std::string>& ioFileNames, 
									 int& ioRows, int& ioCols, int& ioBppCode,
									 float& ioZSpacing, std::string& ioFormatString)
{
	LOG1("IO/IODef3D::ParseDefinitionFile: File: %s", iDefSourceName.c_str());	
	ifstream ifs(iDefSourceName.c_str());
	if (!ifs.good()) return false;

	std::string line, inStr;

	/*-------------------------------------------------------------------------
	.def metadata file format:
	==========================
	directory path, or "." for current working directory
	base filename or "NONE"
	file extension or "NONE"
	number of digits in filename
	"first_number last_number" or "*" for all files with basename and
	extension in directory
	resolution of each slice: "w h" (should be identical to PPM/PGM header)
	bytes per pixel (1: 8 bits,  2: 12 bits,  3: RGB)
	header length or "SKIP" (total bytes - resolution * bytes_per_pixel)
	depth scale (scaling factor in axial direction)
	---------------------------------------------------------------------------*/
	
	// get directory path
	std::string folderPath;
	getline(ifs, line);
	if (strcmp(line.c_str(), ".")==0)
	{
		// get directory path from metadata filepath
		char dNameBuf[512] = {0};
		::GetCurrentDirectory(511, (LPSTR)dNameBuf);
		dNameBuf[511] = '\0';
		folderPath = std::string(dNameBuf) + "\\";
	} else
	{
		folderPath = line + "\\";
	}

	// get basefilename
	std::string baseFileName = "";
	getline(ifs, line);
	if (strcmp(line.c_str(), "NONE")!=0)
	{
		baseFileName = line;
	}

	// get file extension
	bool validExtn=false;
	std::string fileExtn = "";
	getline(ifs, line);
	if (strcmp(line.c_str(), "NONE")!=0)
	{
		fileExtn = line;
		validExtn = true;
	}

	//number of digits in filename
	int numDigitsInFileName=0;
	getline(ifs, line);
	sscanf(line.c_str(), "%d", &numDigitsInFileName);
	if (numDigitsInFileName<0 || numDigitsInFileName>5)
	{
		ifs.close();
		return false;
	}

	// "first_number last_number" or "*" for all files with basename and
	// extension in directory
	int firstNumber=0, lastNumber=0;
	getline(ifs, line);
	if (strcmp(line.c_str(), "*")!=0)
	{
		sscanf(line.c_str(), "%d %d", &firstNumber, &lastNumber);
		if (firstNumber<0 || lastNumber<0)
		{
			ifs.close();
			return false;
		}
	} else
	{
		firstNumber=-1;
		lastNumber=-1;
	}

	
	// resolution of each slice: "w h" (should be identical to PPM/PGM header)
	getline(ifs, line);
	{
		sscanf(line.c_str(), "%d %d", &ioCols, &ioRows);
		if (ioCols<0 || ioRows<0)
		{
			ifs.close();
			return false;
		}
	} 

	
	// bytes per pixel (1: 8 bits,  2: 12 bits,  3: RGB)
	getline(ifs, line);
	{
		sscanf(line.c_str(), "%d", &ioBppCode);
		if (ioBppCode!=1 && ioBppCode!=3) // support only pgm and ppm for now
		{
			ifs.close();
			return false;
		}

		if (ioBppCode==1)
		{
			ioFormatString = "PGM";
		} else if (ioBppCode==3)
		{
			ioFormatString = "PPM";
		}
	} 

	// header length or "SKIP" (total bytes - resolution * bytes_per_pixel)
	int headerLen = 0;
	getline(ifs, line);
	{
		sscanf(line.c_str(), "%d", &headerLen);
		if (headerLen<0)
		{
			ifs.close();
			return false;
		}
	} 

	// depth scale (scaling factor in axial direction)
	getline(ifs, line);
	{
		sscanf(line.c_str(), "%f", &ioZSpacing);
		if (ioZSpacing<1)
		{
			ifs.close();
			return false;
		}
	} 

	// generate filename strings
	if (firstNumber==-1 && lastNumber==-1)
	{
		// get filelist from the directory 

	} else
	{
		for (int i = firstNumber; i<=lastNumber; i++)
		{
			char fIndex[255] = {0};
			sprintf(fIndex, "%d", i);
			std::string indx(fIndex);

			int sz = indx.size();

			for (int i=0; i<(numDigitsInFileName-sz); i++)
			{
				indx = "0" + indx;
			}

			std::string filename = folderPath + "\\" + baseFileName + indx;

			if (validExtn)
			{
				filename = filename + "." + fileExtn;
			}

			ioFileNames.push_back(filename.c_str());
		}
	}

	ifs.close();

	return true;	
}


// protected
template <class T>
inline bool IODef3D<T>::WriteToFolder(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	return false;
}


template <class T>
inline bool IODef3D<T>::ReadFromFolderMT_PPM(const std::vector<std::string>& iFileList)//,	PGCore::BaseDataObject *iMetaDataObject,
		//PGCore::Volume<T> *oDataObject)
{
	LOG0("{ ReadFromFolderMT");

	//m_numThreadsMT = iFileList.size()<_PG_MAX_THREADS_ ? iFileList.size() : _PG_MAX_THREADS_;
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
	PGIO::IOPpm<T> ppmImgIO;
	PGCore::MetaData<T> firstMetaData;
	if (!ppmImgIO.GetAttributes(firstMetaData, iFileList[0]))
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

		//LOG1("Creating thread %d...", i);

		// Create the second thread.
		//hThreadMT[i] = (HANDLE)_beginthreadex( (void *)NULL, 0, &PGIO::LoadFilesPPMThread, (void *) (&(ioParamsMT[i])), 0, &threadIDMT[i] );
		//hThreadMT[i] = CreateThread((void *) (&(ioParamsMT[i])));

		//if (ioAsyncLoadMT) m_threadsRemaining++;

		//LOG3("Created thread %d with threadId [%d], handle [%d]", i, threadIDMT[i], hThreadMT[i]);
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
inline bool IODef3D<T>::ReadFromFolderMT_PGM(const std::vector<std::string>& iFileList)//,	PGCore::BaseDataObject *iMetaDataObject,
		//PGCore::Volume<T> *oDataObject)
{
	LOG0("{ ReadFromFolderMT");

	//m_numThreadsMT = iFileList.size()<_PG_MAX_THREADS_ ? iFileList.size() : _PG_MAX_THREADS_;
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
	PGIO::IOPgm<T> pgmImgIO;
	PGCore::MetaData<T> firstMetaData;
	if (!pgmImgIO.GetAttributes(firstMetaData, iFileList[0]))
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

		if (!CreateNewThread((ThreadedProcess *)this, (void *) (&(ioParamsMT[i]))))
		{
			LOG1("ERROR: Failed to spawn new thread %d.", GetAliveThreadCount()); 
			return false;
		}		


		//LOG1("Creating thread %d...", i);

		// Create the second thread.
		//hThreadMT[i] = (HANDLE)_beginthreadex( (void *)NULL, 0, &PGIO::LoadFilesPGMThread, (void *) (&(ioParamsMT[i])), 0, &threadIDMT[i] );
		//hThreadMT[i] = CreateThread((void *) (&(ioParamsMT[i])));

		//if (ioAsyncLoadMT) m_threadsRemaining++;

		//LOG3("Created thread %d with threadId [%d], handle [%d]", i, threadIDMT[i], hThreadMT[i]);
	}

	//_PG_BREAK_NATIVE

	rv = WaitToFinishIfNeeded();	
	LOG1("} ReadFromFolderMT : %d threads spawned", GetAliveThreadCount());

	return rv;
}


template <class T>
inline bool IODef3D<T>::ReadFromFolderPGM(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	if (!iMetaDataObject || !oDataObject || iFileList.size()<1)
	{
		LOG0("IO/IODef3D::Read: Invalid input data/metadata.");			
		return false;
	}
		
	// get number of slices
	// filter the folder with Pgm file names
	// use folder list
	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IODef3D::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// read one slice first to find out 
	// ReadMetaData
	PGIO::IOPgm<T> PgmImgIO;
	PGIO::IOParams params(iFileList, std::string(""), PGIO::kPgIOSourceTypeFile);
	PGCore::Image<T> firstImage;
	PGCore::MetaData<T> firstMetaData;	
	if (!PgmImgIO.Read(&firstImage, params, &firstMetaData))	
	{
		LOG0("IO/IODef3D::ReadMetaData: Failed to GetAttributes.");			
		return false;
	}
	
	long iRows = 0, iColumns = 0, iSlices = iFileList.size();
	firstImage.GetDimensions(iRows, iColumns);	

	int msbFirst = false;
	int numBits = 8, numBitsInData=8*sizeof(T);	
	bool ignoreBits = false;
	
	PGMath::Vector3D<float> oSpacing(m_skipFactorXY, m_skipFactorXY, m_skipFactorZ);
	PGMath::Vector3D<int> oSize(iColumns/m_skipFactorXY, iRows/m_skipFactorXY, iSlices/m_skipFactorZ);

	if (oMetaData->GetSpacing().Z()>0.0f)
	{
		oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), oMetaData->GetSpacing().Z()));
	} else
	{
		oMetaData->SetSpacing(oSpacing);
	}
	oMetaData->SetSize(oSize);
	oMetaData->SetMSBFirst(msbFirst);
	oMetaData->SetNumberOfBits(numBits);

#ifdef _DEBUG
	LOG0("\tIODef3D::ReadMetaData: Attributes");			
	{
		LOG3("\t\tSize		: %d %d %d", oSize.X(), oSize.Y(), oSize.Z());			
		LOG1("\t\tMSBFirst	: %d", msbFirst);			
		LOG1("\t\tBits		: %d", numBits);			
	}
	LOG0("\tIODef3D::ReadMetaData: Attributes");			
#endif

	// create vectors for position/orientation information
	std::vector<PGMath::Vector3D<float> > pPositions, pOrientationsX, pOrientationsY;

	const std::vector<PGMath::Vector3D<float> >& nextPosVec = firstMetaData.GetImagePositionsPatient();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecX = firstMetaData.GetImageOrientationsPatientX();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecY = firstMetaData.GetImageOrientationsPatientY();

	if (nextPosVec.empty() || nextOrVecX.empty() || nextOrVecY.empty())
	{
		LOG0("IO/IODef3D::ReadMetaData: Empty pos/orientation vectors.");			
		return false;
	}

	std::vector<SlicePosition> slicePositions;
	std::vector<PGCore::Image<T> > tempVolume;

	long fOffset=0, imgSize = iRows*iColumns*sizeof(T);;
	
	PGAlgs::ImageTypeConverter<T, float> forward;
	PGAlgs::ImageTypeConverter<float, T> backward;
	


#if (_SMOOTH_MT_==1)
	PGCore::GaussianKernel<T, T> gausskernel(1.0f), gausskernelZ(1.0f, m_skipFactorZ);
	unsigned int zKernelSize = gausskernelZ.GetDimension();
	int zKMid = zKernelSize/2;
#endif

	PGAlgs::ImageResampler<T, T> resampler;
	resampler.SetScaleFactor(1.0f/(float)m_skipFactorXY);
	
	//SetSmoothFlag(m_skipFactorXY>1);

	double zWt = 1.0f/(double)m_skipFactorZ;
	
	

	for (int i=0; i<iSlices; i+=m_skipFactorZ)
	{
		//LOG2("{ IODef3D<%d>::Read [%d]", this, i);			

		if (pPositions.size()>= oSize.Z()) break;		

		PGCore::Image<T> nextImage(iRows, iColumns);
		PGCore::Image<T> nImage(iRows, iColumns);	
		if (!nextImage.GetBuffer() || !nImage.GetBuffer())
		{
			LOG0("IO/IODef3D::Read: Invalid image buffer.");			
			return false;
		}


		for (int j=0; j<1; j++)
		{
			std::vector<std::string> nextFileName;
			nextFileName.push_back(iFileList[i+j]);
			PGIO::IOParams ioParamsPgm(
				nextFileName,
				std::string(""),
				PGIO::kPgIOSourceTypeFile);	
			PGCore::MetaData<T> nextMetaDataObject;
			if (!PgmImgIO.Read(&nImage, ioParamsPgm, &nextMetaDataObject))
			{			
				LOG1("IO/IODef3D::Read: failed to open file %s to read.", iFileList[i+j].c_str());			
				return false;
				//continue;
			}

			long nRows=0, nCols=0;
			nImage.GetDimensions(nRows, nCols);
			if (nRows!=iRows || nCols!=iColumns)
			{
				LOG4("IO/IODef3D::Read: WARNING: Image size mismatch %d %d v/s %d %d.", nRows, nCols, iRows, iColumns);			
				//return false;
				continue;
			}

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
				double zwt = gausskernelZ.GetValue(zKMid, zKMid-m_skipFactorZ/2+j);
			#endif
			if (j==0) 
			{
				#if (_SMOOTH_MT_==1)
					nextImage = nImage*zwt;
				#else
					nextImage = nImage;
					//memcpy(nextImage.GetBuffer(), nImage.GetBuffer(), nImage.GetDataSize());
				#endif				

				//push new slice attributes into teh meta data structurs
				std::vector<PGMath::Vector3D<float> > nextPosVec; nextPosVec.push_back(PGMath::Vector3D<float>(0, 0, i));
				const std::vector<PGMath::Vector3D<float> >& nextOrVecX = nextMetaDataObject.GetImageOrientationsPatientX();
				const std::vector<PGMath::Vector3D<float> >& nextOrVecY = nextMetaDataObject.GetImageOrientationsPatientY();

				slicePositions.push_back(SlicePosition(i/m_skipFactorZ, nextPosVec[0]));
				pPositions.push_back(nextPosVec[0]);
				pOrientationsX.push_back(nextOrVecX[0]);
				pOrientationsY.push_back(nextOrVecY[0]);

			} else
			{

				#if (_SMOOTH_MT_==1)
					nextImage = nImage*zwt;
				#endif
				//LOG1("\tBlending [%d]..", j);
			}
			
		}	
		
		

		
#if (_SMOOTH_MT_==1)
		if (GetSmoothFlag())
		{
			//LOG1("\t{IODef3D<%d>::Read: smooth.", this);			
			PGCore::Image<T> sImage;
			gausskernel.Convolve(nextImage, sImage);	
			nextImage = sImage;					
			//LOG1("\t}IODef3D<%d>::Read: smooth.", this);			
		}
#endif

		//resample images if needed
		if (m_skipFactorXY!=1)
		{
			//LOG1("\t{IODef3D<%d>::Read: subsample.", this);			
			resampler.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			resampler.Execute();
			resampler.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));				
			//LOG1("\t}IODef3D<%d>::Read: subsample.", this);			
		}			

		//LOG1("IO/IODef3D::Read: Image [%d] DONE.", i);			
		tempVolume.push_back(nextImage);
		//oDataObject->AddImage(nextImage);

		//LOG2("} IODef3D<%d>::Read [%d]", this, i);			

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
		LOG0("IO/IODef3D::Read: Error: Cannot rearrange slices on position");			
		return false;
	}

	//oDataObject->AddImage(nextImage);

	if (pPositions.size()!=oSize.Z() || 
		pOrientationsX.size()!=oSize.Z() || 
		pOrientationsY.size()!=oSize.Z())
	{
		LOG1("IO/IODef3D::Read: Error: Only %d position/orientation vectors read in", pPositions.size());			
		return false;
	}

	// set size
	oMetaData->SetSize(PGMath::Vector3D<int>(oSize.X(), oSize.Y(), oSize.Z()));
	
	// compute spacing
	//std::vector<PGMath::Vector3D<float> > unsortedPositions = pPositions;	
	//std::sort(unsortedPositions.begin(), unsortedPositions.end(), PGMath::Vector3D<float>::BinPred());
	PGMath::Vector3D<float> diffVector = pPositions[oSize.Z()-1] - pPositions[0];
	float spacing = 1.0f;
	if (iSlices>1)
	{
		spacing = diffVector.Length()/(oSize.Z()-1);
	}
	//LOG1("IO/IODef3D::Read: Spacing: %3.5f", spacing);			

	if (oMetaData->GetSpacing().Z()>0.0f)
	{
		oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), oMetaData->GetSpacing().Z()));
	} else
	{
		oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), spacing));
	}

	// set vectors
	oMetaData->SetImagePositionsPatient(pPositions);
	oMetaData->SetImageOrientationsPatientX(pOrientationsX);
	oMetaData->SetImageOrientationsPatientY(pOrientationsY);
	
	//LOG3("IO/IODef3D::Read: Pos[0]: %3.4f %3.4f %3.4f", pPositions[0].X(), pPositions[0].Y(), pPositions[0].Z());
	//LOG3("IO/IODef3D::Read: OrX[0]: %3.4f %3.4f %3.4f", pOrientationsX[0].X(), pOrientationsX[0].Y(), pOrientationsX[0].Z());
	//LOG3("IO/IODef3D::Read: OrY[0]: %3.4f %3.4f %3.4f", pOrientationsY[0].X(), pOrientationsY[0].Y(), pOrientationsY[0].Z());
	//LOG1("IO/IODef3D::Read: AxialScan: %d", oMetaData->GetAxialScanFlag()==true ? 1 : 0);
	//LOG1("IO/IODef3D::Read: AxialScanFeetFirst: %d", oMetaData->GetAxialScanFeetFirstFlag()==true ? 1 : 0);
	
	return (oDataObject->GetSize() == oSize.Z());		
}


template <class T>
inline bool IODef3D<T>::ReadFromFolderPPM(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	if (!iMetaDataObject || !oDataObject || iFileList.size()<1)
	{
		LOG0("IO/IODef3D::Read: Invalid input data/metadata.");			
		return false;
	}
		
	// get number of slices
	// filter the folder with Pgm file names
	// use folder list
	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(iMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IODef3D::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// read one slice first to find out 
	// ReadMetaData
	PGIO::IOPpm<T> PpmImgIO;
	PGIO::IOParams params(iFileList, std::string(""), PGIO::kPgIOSourceTypeFile);
	PGCore::Image<T> firstImage;
	PGCore::MetaData<T> firstMetaData;	
	if (!PpmImgIO.Read(&firstImage, params, &firstMetaData))	
	{
		LOG0("IO/IODef3D::ReadMetaData: Failed to GetAttributes.");			
		return false;
	}
	
	long iRows = 0, iColumns = 0, iSlices = iFileList.size();
	firstImage.GetDimensions(iRows, iColumns);	

	int msbFirst = false;
	int numBits = 8, numBitsInData=8*sizeof(T);	
	bool ignoreBits = false;
	
	PGMath::Vector3D<float> oSpacing(m_skipFactorXY, m_skipFactorXY, m_skipFactorZ);
	PGMath::Vector3D<int> oSize(iColumns/m_skipFactorXY, iRows/m_skipFactorXY, iSlices/m_skipFactorZ);

	if (oMetaData->GetSpacing().Z()>0.0f)
	{
		oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), oMetaData->GetSpacing().Z()));
	} else
	{
		oMetaData->SetSpacing(oSpacing);
	}
	oMetaData->SetSize(oSize);
	oMetaData->SetMSBFirst(msbFirst);
	oMetaData->SetNumberOfBits(numBits);

#ifdef _DEBUG
	LOG0("\tIODef3D::ReadMetaData: Attributes");			
	{
		LOG3("\t\tSize		: %d %d %d", oSize.X(), oSize.Y(), oSize.Z());			
		LOG1("\t\tMSBFirst	: %d", msbFirst);			
		LOG1("\t\tBits		: %d", numBits);			
	}
	LOG0("\tIODef3D::ReadMetaData: Attributes");			
#endif

	// create vectors for position/orientation information
	std::vector<PGMath::Vector3D<float> > pPositions, pOrientationsX, pOrientationsY;

	const std::vector<PGMath::Vector3D<float> >& nextPosVec = firstMetaData.GetImagePositionsPatient();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecX = firstMetaData.GetImageOrientationsPatientX();
	const std::vector<PGMath::Vector3D<float> >& nextOrVecY = firstMetaData.GetImageOrientationsPatientY();

	if (nextPosVec.empty() || nextOrVecX.empty() || nextOrVecY.empty())
	{
		LOG0("IO/IODef3D::ReadMetaData: Empty pos/orientation vectors.");			
		return false;
	}

	std::vector<SlicePosition> slicePositions;
	std::vector<PGCore::Image<T> > tempVolume;

	long fOffset=0, imgSize = iRows*iColumns*sizeof(T);;
	
	PGAlgs::ImageTypeConverter<T, float> forward;
	PGAlgs::ImageTypeConverter<float, T> backward;
	


#if (_SMOOTH_MT_==1)
	PGCore::GaussianKernel<T, T> gausskernel(1.0f), gausskernelZ(1.0f, m_skipFactorZ);
	unsigned int zKernelSize = gausskernelZ.GetDimension();
	int zKMid = zKernelSize/2;
#endif

	PGAlgs::ImageResampler<T, T> resampler;
	resampler.SetScaleFactor(1.0f/(float)m_skipFactorXY);
	
	//SetSmoothFlag(m_skipFactorXY>1);

	double zWt = 1.0f/(double)m_skipFactorZ;
	
	

	for (int i=0; i<iSlices; i+=m_skipFactorZ)
	{
		//LOG2("{ IODef3D<%d>::Read [%d]", this, i);			

		if (pPositions.size()>= oSize.Z()) break;		

		PGCore::Image<T> nextImage(iRows, iColumns);
		PGCore::Image<T> nImage(iRows, iColumns);	
		if (!nextImage.GetBuffer() || !nImage.GetBuffer())
		{
			LOG0("IO/IODef3D::Read: Invalid image buffer.");			
			return false;
		}


		for (int j=0; j<1; j++)
		{
			std::vector<std::string> nextFileName;
			nextFileName.push_back(iFileList[i+j]);
			PGIO::IOParams ioParamsPpm(
				nextFileName,
				std::string(""),
				PGIO::kPgIOSourceTypeFile);	
			PGCore::MetaData<T> nextMetaDataObject;
			if (!PpmImgIO.Read(&nImage, ioParamsPpm, &nextMetaDataObject))
			{			
				LOG1("IO/IODef3D::Read: failed to open file %s to read.", iFileList[i+j].c_str());			
				return false;
				//continue;
			}

			long nRows=0, nCols=0;
			nImage.GetDimensions(nRows, nCols);
			if (nRows!=iRows || nCols!=iColumns)
			{
				LOG4("IO/IODef3D::Read: WARNING: Image size mismatch %d %d v/s %d %d.", nRows, nCols, iRows, iColumns);			
				//return false;
				continue;
			}

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
				double zwt = gausskernelZ.GetValue(zKMid, zKMid-m_skipFactorZ/2+j);
			#endif
			if (j==0) 
			{
				#if (_SMOOTH_MT_==1)
					nextImage = nImage*zwt;
				#else
					nextImage = nImage;
					//memcpy(nextImage.GetBuffer(), nImage.GetBuffer(), nImage.GetDataSize());
				#endif				

				//push new slice attributes into teh meta data structurs
				std::vector<PGMath::Vector3D<float> > nextPosVec; nextPosVec.push_back(PGMath::Vector3D<float>(0, 0, i));
				const std::vector<PGMath::Vector3D<float> >& nextOrVecX = nextMetaDataObject.GetImageOrientationsPatientX();
				const std::vector<PGMath::Vector3D<float> >& nextOrVecY = nextMetaDataObject.GetImageOrientationsPatientY();

				slicePositions.push_back(SlicePosition(i/m_skipFactorZ, nextPosVec[0]));
				pPositions.push_back(nextPosVec[0]);
				pOrientationsX.push_back(nextOrVecX[0]);
				pOrientationsY.push_back(nextOrVecY[0]);

			} else
			{

				#if (_SMOOTH_MT_==1)
					nextImage = nImage*zwt;
				#endif
				//LOG1("\tBlending [%d]..", j);
			}
			
		}	
		
		

		
#if (_SMOOTH_MT_==1)
		if (GetSmoothFlag())
		{
			//LOG1("\t{IODef3D<%d>::Read: smooth.", this);			
			PGCore::Image<T> sImage;
			gausskernel.Convolve(nextImage, sImage);	
			nextImage = sImage;					
			//LOG1("\t}IODef3D<%d>::Read: smooth.", this);			
		}
#endif

		//resample images if needed
		if (m_skipFactorXY!=1)
		{
			//LOG1("\t{IODef3D<%d>::Read: subsample.", this);			
			resampler.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			resampler.Execute();
			resampler.GetOutput(static_cast<PGCore::Image < T > *>(&nextImage));				
			//LOG1("\t}IODef3D<%d>::Read: subsample.", this);			
		}			

		//LOG1("IO/IODef3D::Read: Image [%d] DONE.", i);			
		tempVolume.push_back(nextImage);
		//oDataObject->AddImage(nextImage);

		//LOG2("} IODef3D<%d>::Read [%d]", this, i);			

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
		LOG0("IO/IODef3D::Read: Error: Cannot rearrange slices on position");			
		return false;
	}

	//oDataObject->AddImage(nextImage);

	if (pPositions.size()!=oSize.Z() || 
		pOrientationsX.size()!=oSize.Z() || 
		pOrientationsY.size()!=oSize.Z())
	{
		LOG1("IO/IODef3D::Read: Error: Only %d position/orientation vectors read in", pPositions.size());			
		return false;
	}

	// set size
	oMetaData->SetSize(PGMath::Vector3D<int>(oSize.X(), oSize.Y(), oSize.Z()));
	
	// compute spacing
	//std::vector<PGMath::Vector3D<float> > unsortedPositions = pPositions;	
	//std::sort(unsortedPositions.begin(), unsortedPositions.end(), PGMath::Vector3D<float>::BinPred());
	PGMath::Vector3D<float> diffVector = pPositions[oSize.Z()-1] - pPositions[0];
	float spacing = 1.0f;
	if (iSlices>1)
	{
		spacing = diffVector.Length()/(oSize.Z()-1);
	}
	//LOG1("IO/IODef3D::Read: Spacing: %3.5f", spacing);			

	if (oMetaData->GetSpacing().Z()>0.0f)
	{
		oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), oMetaData->GetSpacing().Z()));
	} else
	{
		oMetaData->SetSpacing(PGMath::Vector3D<float>(oSpacing.X(), oSpacing.Y(), spacing));
	}

	// set vectors
	oMetaData->SetImagePositionsPatient(pPositions);
	oMetaData->SetImageOrientationsPatientX(pOrientationsX);
	oMetaData->SetImageOrientationsPatientY(pOrientationsY);
	
	//LOG3("IO/IODef3D::Read: Pos[0]: %3.4f %3.4f %3.4f", pPositions[0].X(), pPositions[0].Y(), pPositions[0].Z());
	//LOG3("IO/IODef3D::Read: OrX[0]: %3.4f %3.4f %3.4f", pOrientationsX[0].X(), pOrientationsX[0].Y(), pOrientationsX[0].Z());
	//LOG3("IO/IODef3D::Read: OrY[0]: %3.4f %3.4f %3.4f", pOrientationsY[0].X(), pOrientationsY[0].Y(), pOrientationsY[0].Z());
	//LOG1("IO/IODef3D::Read: AxialScan: %d", oMetaData->GetAxialScanFlag()==true ? 1 : 0);
	//LOG1("IO/IODef3D::Read: AxialScanFeetFirst: %d", oMetaData->GetAxialScanFeetFirstFlag()==true ? 1 : 0);
	
	return (oDataObject->GetSize() == oSize.Z());		
}


template <class T>
inline bool IODef3D<T>::RearrangeSlices(const std::vector<SlicePosition>& iSortedPositions, std::vector<PGCore::Image<T> >& iVolume,
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


template <class T>
inline void IODef3D<T>::StartExecution_PGM(void *iParam)
{
	PGIO::IOPgm3D<T> ioPGM3D;	
	return ioPGM3D.StartExecution(iParam);
}

template <class T>
inline void IODef3D<T>::StartExecution_PPM(void *iParam)
{
	PGIO::IOPpm3D<T> ioPPM3D;	
	return ioPPM3D.StartExecution(iParam);
}

template <class T>
inline void IODef3D<T>::StartExecution(void *iParam)
{
	switch (m_fileFormat)
	{
	case kPgIOSourceFormatPGM:	
		StartExecution_PGM(iParam);	
		break;

	case kPgIOSourceFormatPPM:	
		StartExecution_PPM(iParam);	
		break;

	default:	
		PGIO::ThreadedIOParams<T> * iParams = static_cast<PGIO::ThreadedIOParams<T> *>(iParam);
		if (iParams->iParentThreadPtr)
		{
			IOPgm3D<T>* iParentThreadPtr = (IOPgm3D<T> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyError("Invalid file type.");
		}
		break;
	}

	return;
}

};

#ifdef _PG_GENERATE_SDK_LIBS_
	template class PGIO::IODef3D<short>; 
	template class PGIO::IODef3D<unsigned char>; 	
#endif


