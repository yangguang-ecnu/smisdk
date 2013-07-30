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
// Filename:	MetaData.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGMetaData_H_
#define _PGMetaData_H_
//////////////////////////////////////////////////////////////////////////

#include "Math/Vector3D.h"
#include "Core/BaseDataObject.h"

namespace PGCore
{
	
/*
	CR Image Storage  1.2.840.10008.5.1.4.1.1.1 
	CT Image Storage  1.2.840.10008.5.1.4.1.1.2 
	MR Image Storage  1.2.840.10008.5.1.4.1.1.4 
	US Image Storage  1.2.840.10008.5.1.4.1.1.6 
	SC Image Storage  1.2.840.10008.5.1.4.1.1.7 
*/

#define kPGSOPClassCR_STR "1.2.840.10008.5.1.4.1.1.1"
#define kPGSOPClassCT_STR "1.2.840.10008.5.1.4.1.1.2"
#define kPGSOPClassMR_STR "1.2.840.10008.5.1.4.1.1.4"
#define kPGSOPClassUS_STR "1.2.840.10008.5.1.4.1.1.6"
#define kPGSOPClassSC_STR "1.2.840.10008.5.1.4.1.1.7"

typedef enum ePGModality
{
	kPGModalityCT=0,
	kPGModalityMR=1,
	kPGModalityPT=2,
	kPGModalityUnknown=-1
};

typedef enum ePGScanDirection
{
	kPgScanDirectionAxial=0,
	kPgScanDirectionCoronal=1,
	kPgScanDirectionSagittal=2,
	kPgScanDirectionUnknown=-1
};

typedef enum ePGSOPClass
{
	kPGSOPClassCT=0,
	kPGSOPClassCR=1,
	kPGSOPClassMR=2,
	kPGSOPClassUS=3,
	kPGSOPClassSC=4,
	kPGSOPClassUnknown=-1
};


template <class T>
class MetaData: public BaseDataObject
{
public:

	MetaData();
	
	static const char* ModalityToStr(const ePGModality iModality);

	static const ePGModality StrToModality(const char* iStr);

	static const ePGSOPClass MetaData<T>::StrToSOPClass(const char* iStr);

	static const char* SOPClassToStr(const ePGSOPClass iSOPClass);
	
	MetaData(const MetaData<T>& iObject);	
		
	void SetSOPClass(const ePGSOPClass iSOPClass);
	const ePGSOPClass GetSOPClass() const;
	
	void SetSize(const PGMath::Vector3D<int>& iSize);
	const PGMath::Vector3D<int>& GetSize() const;

	void SetSamplesPerPixel(const int& iSamplesPPixel);
	const int& GetSamplesPerPixel() const;

	void SetFrameCount(const int& iCount);
	const int& GetFrameCount() const;

	void SetMSBFirst(const int& iFlag);
	const int &GetMSBFirst() const;

	void SetDimensionality(const int& iDim);
	const int &GetDimensionality() const;

	void SetCompressedDataFlag(const int& iFlag);
	const int &GetCompressedDataFlag() const;
	
	void SetSpacing(const PGMath::Vector3D<float>& iSpacing);
	const PGMath::Vector3D<float>& GetSpacing() const;
	
	void SetImagePositionsPatient(const std::vector< PGMath::Vector3D<float> >& iPositions);
	const std::vector< PGMath::Vector3D<float> >& GetImagePositionsPatient() const;	
	
	void SetImageOrientationsPatientX(const std::vector< PGMath::Vector3D<float> >& iOrientationsX);
	const std::vector< PGMath::Vector3D<float> >& GetImageOrientationsPatientX() const;
	
	void SetImageOrientationsPatientY(const std::vector< PGMath::Vector3D<float> >& iOrientationsY);
	const std::vector< PGMath::Vector3D<float> >& GetImageOrientationsPatientY() const;
	
	void SetStudyUID(const std::string & iStudyUID);	
	const std::string &GetSeriesUID() const;
	
	void SetImageSOPInstanceUIDs(const std::vector<std::string> & iSOPInstanceUIDs);
	const std::vector<std::string> &GetImageSOPInstanceUIDs() const;	

	void SetIntercept(const float& iIntercept);
	const float& GetIntercept() const;

	void SetSourceTypeSingleFile(const bool iSourceTypeSingleFile);
	const bool GetSourceTypeSingleFile() const;
	
	void  SetModality(const ePGModality iModality);
	const ePGModality GetModality() const; 

	void SetPatientName(const std::string & iName);
	const std::string& GetPatientName() const;

	void SetSeriesUID(const std::string & iSeriesUID);
	const std::string &GetStudyUID() const;
	
	void SetNumberOfBits(const int& iBits);	
	const int &GetNumberOfBits() const;
	
	void SetSlope(const float& iSlope);
	const float& GetSlope() const;

	void  SetSkipFactors(const int& iXYSkip, const int& iZSkip) { m_skipFactorXY	= iXYSkip;	m_skipFactorZ	= iZSkip; }
	bool  GetSkipFactors(int& oXYSkip, int& oZSkip) {	oXYSkip = m_skipFactorXY; oZSkip = m_skipFactorZ; return true; }
	
	const bool GetImagesAlongScanDirection(); // not const because it need to call update internally
	const ePGScanDirection GetScanDirection();
	
	void SetFileNameNumberDigits(const int& iFileNameNumberDigits);
	const int &GetFileNameNumberDigits();		

	const std::string& GetFileNamePrefix() const;	
	void SetFileNamePrefix(const std::string & iPrefix);	
	
	void SetPixelDataFormatJpeg(const int iFlag);
	const int& GetPixelDataFormatJpeg() const;

	virtual ~MetaData(void);

private:
	
	ePGModality								m_modality;
	ePGSOPClass								m_SOPClass;
	std::string								m_patientName;
	std::string								m_studyUID;
	std::string								m_seriesUID;
	bool									m_sourceTypeSingleFile;
	
	ePGScanDirection						m_scanDirection;
	bool									m_imagePosAlongDirectionAxis;

	std::string								m_fileNamePrefix;

	PGMath::Vector3D<int>					m_size;
	PGMath::Vector3D<float>					m_spacing;	
	
	int										m_samplesPerPixel;
	int										m_numberOfBits;
	int										m_msbFirst;
	int										m_fileNameNumberDigits;
	int										m_dimensionality;

	int										m_compressedDataFlag;

	int										m_pixelDataFormatJPEG;

	float									m_slope;
	float									m_intercept;

	std::vector< PGMath::Vector3D<float> >	m_imagePositionsPatient;
	std::vector< PGMath::Vector3D<float> >	m_imageOrientationsPatientX;
	std::vector< PGMath::Vector3D<float> >	m_imageOrientationsPatientY;	
	std::vector<std::string>				m_imageSOPInstanceUIDs;
	
	int										m_skipFactorXY;
	int										m_skipFactorZ;
	int										m_numFrames;

	void updateScanDirectionFlag();
	void updateImagePosAlongScanDirectionFlag();
	
};

};

#ifndef _PG_GENERATE_SDK_LIBS_	
	#include "MetaData.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGMetaData_H_
