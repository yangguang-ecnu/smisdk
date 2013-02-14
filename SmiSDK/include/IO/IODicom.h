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
// Filename:	IODicom.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IODicom.h: interface for the IODicom class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IODicom_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IODicom_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _PG_GDDCM_ "_PG_GDDCM_"

#ifdef _PG_GDDCM_
	#include "../Common/gdcm/include/gdcm-2.0/gdcmFile.h"	
#else
	#include "dcmtk/dcmdata/dcfilefo.h"
	#include "dcmtk/dcmdata/dcdeftag.h"
	#include "dcmtk/dcmdata/dcmetinf.h"
	#include "dcmtk/dcmjpeg/djdecode.h"
#endif


#include "IO/IOBase.h"
#include "Algs/ImageEndianSwapper.h"
#include "Algs/ImageBitClipper.h"

// http://www.sph.sc.edu/comd/rorden/dicom.html
#define kPgTransferSyntaxUIDRawImplicitVRLittleEndian "1.2.840.10008.1.2"
#define kPgTransferSyntaxUIDRawExplicitVR "1.2.840.10008.1.2." // 1.2.840.10008.1.2.x, x:1 (lil) or 2 (big)
#define kPgTransferSyntaxUIDRawExplicitVRLittle "1.2.840.10008.1.2.1" // 1.2.840.10008.1.2.x, x:1 (lil) or 2 (big)
#define kPgTransferSyntaxUIDRawExplicitVRBig "1.2.840.10008.1.2.2" // 1.2.840.10008.1.2.x, x:1 (lil) or 2 (big)
#define kPgTransferSyntaxUIDJPEG "1.2.840.10008.1.2.4." //1.2.840.10008.1.2.4.xx, 50-64: lossy, 65-70: lossless JPEG
#define kPgTransferSyntaxUIDLossLessRLE "1.2.840.10008.1.2.5" // lossless RLE

// Photo interpretation
#define kPgPhotometricRepresentationMChrome1 "MONOCHROME1" 
#define kPgPhotometricRepresentationMChrome2 "MONOCHROME2" 
#define kPgPhotometricRepresentationRGB "RGB" 
#define kPgPhotometricRepresentationYBR "YBR" 
#define kPgPhotometricRepresentationCMYK "CMYK" 
#define kPgPhotometricRepresentationPalette "Palette" 


namespace PGIO
{

template <class T>
class IODicom : public IOBase<T> 
{
public:
	IODicom(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	virtual bool GetAttributes(PGCore::MetaData<T>& oMetaDataObject, const std::string &iFilePath);		

	~IODicom(void);

	virtual void StartExecution(void *iParams);
	virtual int NotifyFinished();
	virtual int NotifyError(const char *iMsg=0);
	
protected:

	virtual bool WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams);

	virtual bool ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams);	


private:

#ifdef _PG_GDDCM_

#else
	DcmFileFormat	m_fileformat;
#endif

	bool decodeJpeg(char* oBuf, char* iBuf, const int iMaxBytes);
};


};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IO/IODicom.hpp"
#endif


#endif // !defined(AFX_IODicom_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
