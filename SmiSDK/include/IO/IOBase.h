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
// Filename:	IOBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 12, 2006 at 11:56:30 PM
//

// IOBase.h: interface for the IOBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBase_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IOBase_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _PG_Core_H_
	#include "Core/Core.h"
#endif
#include "Core/AsyncProcess.h"
#include "Core/ThreadedProcess.h"
#include "Core/BaseDataObject.h"
#include "Core/MetaData.h"
#include "Core/LoggerBase.h"
#include "Core/Volume.h"
#include "Process.h"

#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <ctype.h>

#define _SMOOTH_MT_ 1


using namespace std;

namespace PGIO
{

//Sample file
//Modality = MR
//PatientName = mrt16_angio2
//Bits = 10
//MSBFirst = 0
//Dimensions = 3 
//DimX = 416
//DimY = 512
//DimZ = 112
//SpacingX = 0.412
//SpacingY = 0.412
//SpacingZ = 0.412
//NumberOfFiles = 1
//OrientationXX = 1.0
//OrientationXY = 0.0
//OrientationXZ = 0.0
//OrientationYX = 0.0
//OrientationYY = 1.0
//OrientationYZ = 0.0
//FirstImagePositionX = 0.0
//FirstImagePositionY = 0.0
//FirstImagePositionZ = 0.0
//LastImagePositionX = 0.0
//LastImagePositionY = 0.0
//LastImagePositionZ = 0.0


static const char* kPGTagModality			=	"Modality";
static const char* kPGTagPatientName		=	"PatientName";
static const char* kPGTagBits				=	"Bits";
static const char* kPGTagMSBFirst			=	"MSBFirst";
static const char* kPGTagDimensions			=	"Dimensions";	
static const char* kPGTagDimX				=	"DimX";
static const char* kPGTagDimY				=	"DimY";
static const char* kPGTagDimZ				=	"DimZ";
static const char* kPGTagSpacingX			=	"SpacingX";
static const char* kPGTagSpacingY			=	"SpacingY";
static const char* kPGTagSpacingZ			=	"SpacingZ";
static const char* kPGTagNumberOfFiles		=	"NumberOfFiles";
static const char* kPGOrientationXX			=	"OrientationXX";
static const char* kPGOrientationXY			=	"OrientationXY";
static const char* kPGOrientationXZ			=	"OrientationXZ";
static const char* kPGOrientationYX			=	"OrientationYX";
static const char* kPGOrientationYY			=	"OrientationYY";
static const char* kPGOrientationYZ			=	"OrientationYZ";
static const char* kPGFirstImagePositionX	=	"FirstImagePositionX";
static const char* kPGFirstImagePositionY	=	"FirstImagePositionY";
static const char* kPGFirstImagePositionZ	=	"FirstImagePositionZ";
static const char* kPGLastImagePositionX	=	"LastImagePositionX";
static const char* kPGLastImagePositionY	=	"LastImagePositionY";
static const char* kPGLastImagePositionZ	=	"LastImagePositionZ";
static const char* kPGFileNamePrefix		=	"FileNamePrefix";
static const char* kPGFileNameNumberDigits	=	"FileNameNumberDigits"; 

typedef enum ePgIOSourceType
{
	kPgIOSourceTypeFile=0,
	kPgIOSourceTypeFolder=1,
	kPgIOSourceTypeUnknown=2
};

typedef enum ePgIOSourceFormat
{
	kPgIOSourceFormatDICOM=0,
	kPgIOSourceFormatPGM=1,
	kPgIOSourceFormatPPM=2,
	kPgIOSourceFormatRAW=3,
	kPgIOSourceFormatDEF=4,
	kPgIOSourceFormatMHD=5,
	kPgIOSourceFormatUnknown=-1
};


class IOParams
{
public:
	IOParams(const std::vector<std::string> &iSource,
		const std::string &iMetaDataSource,
		const ePgIOSourceType iSourceType,
		const bool iEnableMultiThreading=false,
		const bool iEnableAsyncLoading=false,
		const bool iEnableAutoSkip=false,
		const ePgIOSourceFormat iSourceFormat=kPgIOSourceFormatUnknown
		)
	{
		//_PG_BREAK_NATIVE
		m_pDataSource = iSource;
		m_pMetaDataSource = iMetaDataSource; 

		SetSourceType(iSourceType);
		SetMultiThreadingFlag(iEnableMultiThreading);
		SetAsynchronousLoadFlag(iEnableAsyncLoading);		
		SetAutoSkipFlag(iEnableAutoSkip);
		SetSourceFormat(iSourceFormat);
		SetActiveFrameIndex(0);
	}

	void  SetActiveFrameIndex(const int& iFrameIndex) { m_frameIndex=iFrameIndex; }	
	const int& GetActiveFrameIndex()					const { return m_frameIndex; }	

	void  SetAutoSkipFlag(const bool iFlag)				{ m_autoSkip = iFlag; }
	const bool IsEnabledAutoSkip()						const { return m_autoSkip; }
	
	void  SetAsynchronousLoadFlag(const bool iFlag)		{ m_asynchLoad = iFlag; }
	const bool IsEnabledAsynchronousLoad()				const { return m_asynchLoad; }
	
	void  SetMultiThreadingFlag(const bool iFlag)		{ m_enableMultiThreading = iFlag; }
	const bool IsEnabledMultiThreading()				const { return m_enableMultiThreading; }
	
	void  SetSourceFormat(const ePgIOSourceFormat iSourceFormat) { m_sourceFormat = iSourceFormat; }
	const ePgIOSourceFormat SourceFormat()				const { return m_sourceFormat; }

	const std::vector<std::string>& Source()			const { return m_pDataSource; }
	const std::string& MetaSource()						const { return m_pMetaDataSource; }
	
	void  SetSourceType(const ePgIOSourceType iType)    { m_pSourceType = iType; }
	const ePgIOSourceType SourceType()					const { return m_pSourceType; }	
		
protected:

	
private:
	std::vector<std::string>		m_pDataSource;
	std::string						m_pMetaDataSource;	
	ePgIOSourceType					m_pSourceType;
	ePgIOSourceFormat				m_sourceFormat;

	bool							m_enableMultiThreading;
	bool							m_asynchLoad;
	bool							m_autoSkip;

	int								m_frameIndex;

};

using namespace PGCore;

template <class T>
class ThreadedIOParams : public AsyncProcessParams
{
public: 

	ThreadedIOParams<T>() 
	{
		iFileNames			=	0;
		ioVolume			=	0;
		ioAttributes		=	0;		
		skipXY				=	1;
		skipZ				=	1;
		smoothWhenSkipping  =   false;
	};

	const std::vector<std::string>* iFileNames;
	PGCore::Volume<T>*				ioVolume;
	PGCore::BaseDataObject*			ioAttributes;	
	int								skipXY;
	int								skipZ;
	bool							smoothWhenSkipping;
};


template <class T>
class IOBase : public PGCore::AsyncProcess
{
public:

	IOBase(void);

	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0) = 0;

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0) = 0;		

	void	SetSmoothFlag(bool iFlag);
	bool	GetSmoothFlag();

	void	SetByteLimitInKB(const long& iLimit);
	const   long&	GetByteLimitInKB() const;

	StatusCallBackFunctionD GetProgressCB();	
	void SetProgressCB(StatusCallBackFunctionD iCB);

	void  SetSkipFactors(const int& iXYSkip, const int& iZSkip);	
	bool  GetSkipFactors(int& oXYSkip, int& oZSkip);

	virtual int NotifyFinished();
	virtual int NotifyError(const char *iMsg=0);
	virtual bool ConsolidateWhenDone();

protected:
	long	m_byteLimit;
	int		m_skipFactorXY, m_skipFactorZ;
	bool    m_MultiThreadingEnabled;
	bool	m_autoSkip;

	// begin thread specific section
	ThreadedIOParams<T>			ioParamsMT[_PG_MAX_THREADS_];
	PGCore::Volume<T>			oVolumesMT[_PG_MAX_THREADS_];
	PGCore::MetaData<T>			oMetaDataMT[_PG_MAX_THREADS_];
	std::vector<std::string>	iListMT[_PG_MAX_THREADS_];	
	PGCore::Logger				nextLoggerMT[_PG_MAX_THREADS_];
	PGCore::Volume<T>			*ioDataObjectMT;
	PGCore::MetaData<T>			*ioMetaDataObjectMT;
	// end thread specific section

	virtual bool WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams);

	virtual bool ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams);	

	bool ExtractFileNamesFromFolder(const std::string& iFolderPath, std::vector<std::string>& ioFileNames, const char* iFilterStr=0);

	bool ComputeSkipFactors(const PGMath::Vector3D<int>& iVolSize, const int& iBytesPerVoxel,
			int& oSkipXY, int& oSkipZ);

	void UpdateProgress(float iProgress);	

	virtual ~IOBase(void);

	// merge images coming from different threads
	virtual bool ConsolidateImages();

	typedef std::pair<int, Vector3D<float> > SlicePosition;
	class SlicePositionSorter : public std::binary_function< SlicePosition, SlicePosition, bool >
	{
	public:
		bool operator() ( const SlicePosition& iA , const SlicePosition& iB )
		{
			return iA.second.Z() < iB.second.Z();
		}
	};


private:	
	mutable		StatusCallBackFunctionD m_CBFn;

	bool		m_smoothDataWhenLoading; 

	void		clampToNextPowOfTwo(PGMath::Point3D<int>& ioDim);

	bool		fillMetaValue(const std::string &iStr, PGCore::MetaData<T> *oMetaData) const;

	int			getValueI(const std::string &iStr, const std::string &iTag) const;
	float		getValueF(const std::string &iStr, const std::string &iTag) const;
	std::string	getValueS(const std::string &iStr, const std::string &iTag) const;
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IO/IOBase.hpp"
#endif


#endif // !defined(AFX_IOBase_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
