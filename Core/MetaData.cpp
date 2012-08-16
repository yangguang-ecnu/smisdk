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
// Filename:	MetaData.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGMetaData_HPP_
#define _PGMetaData_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/MetaData.h"
#include "Math/Point3D.h"

namespace PGCore
{
	template <class T>
	MetaData<T>::MetaData()
	{
		m_sourceTypeSingleFile = true;
		m_modality		= kPGModalityUnknown;
		m_SOPClass		= kPGSOPClassUnknown;
		m_patientName	= std::string("AnonymousPatient");
		m_studyUID		= std::string("AnonymousStudy");
		m_seriesUID		= std::string("AnonymousSeries");

		m_fileNamePrefix= std::string("slice"); 

		m_size			= PGMath::Vector3D<int>(0, 0, 0);
		m_spacing		= PGMath::Vector3D<float>(0.0f, 0.0f, 0.0f);;	

		m_samplesPerPixel = 1;
		m_numberOfBits	= 12;
		m_msbFirst		= 0;
		m_fileNameNumberDigits = 3;

		m_scanDirection = PGCore::kPgScanDirectionUnknown;

		m_imagePosAlongDirectionAxis = false;

		m_imagePositionsPatient.clear();		
		m_imageOrientationsPatientX.clear();
		m_imageOrientationsPatientY.clear();		
		m_imageSOPInstanceUIDs.clear();

		m_slope = 1;
		m_intercept = 0;
		m_numFrames = 0;
		m_compressedDataFlag = 0;
		m_dimensionality = 3;
	}

	template <class T>	
	const char* MetaData<T>::ModalityToStr(const ePGModality iModality)
	{
		switch(iModality)
		{
		case	kPGModalityCT	:	return "CT";
		case	kPGModalityMR	:	return "MR";
		case	kPGModalityPT	:	return "PET";
		default					:	break;
		}

		return "Unknown";
	}

	template <class T>	
	const char* MetaData<T>::SOPClassToStr(const ePGSOPClass iSOPClass)
	{
		switch(iSOPClass)
		{
			case	kPGSOPClassCR	:	return "CR";
			case	kPGSOPClassCT	:	return "CT";
			case	kPGSOPClassMR	:	return "MR";
			case	kPGSOPClassUS	:	return "US";
			case	kPGSOPClassSC	:	return "SC";

			default					:	break;
		}

		return "Unknown";
	}

	template <class T>	
	const ePGSOPClass MetaData<T>::StrToSOPClass(const char* iStr)
	{
		if (!iStr) 
		{
			return kPGSOPClassUnknown;
		}

		if (!strcmp(iStr, kPGSOPClassCR_STR)) 
		{	
			return kPGSOPClassCR;

		} else	if (!strcmp(iStr, kPGSOPClassMR_STR)) 
		{	
			return kPGSOPClassMR;

		} else if (!strcmp(iStr, kPGSOPClassCT_STR)) 
		{	
			return kPGSOPClassCT;

		} else if (!strcmp(iStr, kPGSOPClassUS_STR)) 
		{	
			return kPGSOPClassUS;

		} else if (!strcmp(iStr, kPGSOPClassSC_STR)) 
		{	
			return kPGSOPClassSC;
		} 

		return kPGSOPClassUnknown;
	}

	template <class T>	
	const ePGModality MetaData<T>::StrToModality(const char* iStr)
	{
		if (!iStr) 
		{
			return kPGModalityUnknown;
		}

		if (!strcmp(iStr, "CT")) 
		{	
			return kPGModalityCT;
		} else
			if (!strcmp(iStr, "MR")) 
			{	
				return kPGModalityMR;
			} else
				if (!strcmp(iStr, "PET")) 
				{	
					return kPGModalityPT;
				} 

				return kPGModalityUnknown;
	}

	template <class T>	
	void MetaData<T>::SetSourceTypeSingleFile(const bool iSourceTypeSingleFile)
	{
		m_sourceTypeSingleFile = iSourceTypeSingleFile;
	}

	template <class T>	
	void MetaData<T>::SetModality(const ePGModality iModality)
	{
		m_modality = iModality;
	}

	template <class T>
	void MetaData<T>::SetSOPClass(const ePGSOPClass iSOPClass)
	{
		m_SOPClass = iSOPClass;
	}

	template <class T>	
	void MetaData<T>::SetSize(const PGMath::Vector3D<int>& iSize)
	{
		m_size = iSize;
	}

	template <class T>	
	void MetaData<T>::SetNumberOfBits(const int& iBits)
	{
		m_numberOfBits = iBits;
	}
	
	template <class T>
	void MetaData<T>::SetSamplesPerPixel(const int& iSamplesPPixel)
	{
		m_samplesPerPixel = iSamplesPPixel;
	}

	template <class T>
	void MetaData<T>::SetFrameCount(const int& iCount)
	{
		m_numFrames = iCount;
	}

	template <class T>	
	void MetaData<T>::SetMSBFirst(const int& iFlag)
	{
		m_msbFirst = iFlag;
	}

	template <class T>	
	void MetaData<T>::SetDimensionality(const int& iDim)
	{
		m_dimensionality = iDim;
	}
	
	template <class T>	
	void MetaData<T>::SetCompressedDataFlag(const int& iFlag)
	{
		m_compressedDataFlag = iFlag;
	}	

	template <class T>	
	void MetaData<T>::SetSpacing(const PGMath::Vector3D<float>& iSpacing)
	{
		m_spacing = iSpacing;
	}	

	template <class T>	
	void MetaData<T>::SetImagePositionsPatient(const std::vector< PGMath::Vector3D<float> >& iPositions)
	{
		m_imagePositionsPatient = iPositions;			
	}	

	template <class T>	
	void MetaData<T>::SetImageOrientationsPatientX(const std::vector< PGMath::Vector3D<float> >& iOrientationsX)
	{
		m_imageOrientationsPatientX = iOrientationsX;		
	}	

	template <class T>	
	void MetaData<T>::SetImageOrientationsPatientY(const std::vector< PGMath::Vector3D<float> >& iOrientationsY)
	{
		m_imageOrientationsPatientY = iOrientationsY;
	}	

	template <class T>	
	void MetaData<T>::SetPatientName(const std::string & iName) 
	{
		m_patientName = iName;
	}

	template <class T>	
	void MetaData<T>::SetFileNamePrefix(const std::string & iPrefix) 
	{
		m_fileNamePrefix = iPrefix;
	}

	template <class T>	
	void MetaData<T>::SetFileNameNumberDigits(const int& iFileNameNumberDigits)
	{
		m_fileNameNumberDigits = iFileNameNumberDigits;
	}

	template <class T>	
	void MetaData<T>::SetStudyUID(const std::string & iStudyUID) 
	{
		m_studyUID = iStudyUID;
	}

	template <class T>	
	void MetaData<T>::SetSeriesUID(const std::string & iSeriesUID) 
	{
		m_seriesUID = iSeriesUID;
	}

	template <class T>	
	void MetaData<T>::SetImageSOPInstanceUIDs(const std::vector<std::string> & iSOPInstanceUIDs)
	{
		m_imageSOPInstanceUIDs = iSOPInstanceUIDs;
	}

	template <class T>	
	void MetaData<T>::SetSlope(const float& iSlope)
	{
		m_slope = iSlope;
	}

	template <class T>	
	void MetaData<T>::SetIntercept(const float& iIntercept)
	{
		m_intercept = iIntercept;
	}

	template <class T>	
	const bool MetaData<T>::GetSourceTypeSingleFile() const
	{
		return m_sourceTypeSingleFile;
	}

	template <class T>	
	const bool MetaData<T>::GetImagesAlongScanDirection() 
	{
		// first update what kind of scan it is
		updateScanDirectionFlag();

		// then update the direction flag
		updateImagePosAlongScanDirectionFlag();

		return m_imagePosAlongDirectionAxis;		
	}

	/*template <class T>	
	const bool MetaData<T>::GetAxialScanFlag() const
	{
		return m_isAxialScan;
	}*/

	template <class T>	
	const int& MetaData<T>::GetSamplesPerPixel() const
	{
		return m_samplesPerPixel;
	}

	template <class T>	
	const PGMath::Vector3D<int>& MetaData<T>::GetSize() const 
	{
		return m_size;
	}

	template <class T>	
	const PGMath::Vector3D<float>& MetaData<T>::GetSpacing() const 
	{
		return m_spacing;
	}

	template <class T>
	const std::vector< PGMath::Vector3D<float> >& MetaData<T>::GetImagePositionsPatient() const 
	{
		return m_imagePositionsPatient;
	}

	template <class T>
	const std::vector< PGMath::Vector3D<float> >& MetaData<T>::GetImageOrientationsPatientX() const 
	{
		return m_imageOrientationsPatientX;
	}

	template <class T>
	const std::vector< PGMath::Vector3D<float> >& MetaData<T>::GetImageOrientationsPatientY() const 
	{
		return m_imageOrientationsPatientY;
	}	

	// get
	template <class T>
	const ePGModality MetaData<T>::GetModality() const 
	{
		return m_modality;
	}

	template <class T>
	const ePGSOPClass MetaData<T>::GetSOPClass() const
	{
		return m_SOPClass;
	}

	template <class T>
	const std::string& MetaData<T>::GetPatientName() const 
	{
		return m_patientName;
	}

	template <class T>
	const std::string& MetaData<T>::GetFileNamePrefix() const 
	{
		return m_fileNamePrefix;
	}

	template <class T>
	const int & MetaData<T>::GetFileNameNumberDigits()
	{
		return m_fileNameNumberDigits;
	}

	template <class T>
	const int & MetaData<T>::GetFrameCount() const
	{
		return m_numFrames;
	}

	template <class T>
	const std::string & MetaData<T>::GetStudyUID() const
	{
		return m_studyUID;
	}

	template <class T>
	const std::string & MetaData<T>::GetSeriesUID() const
	{
		return m_seriesUID;
	}

	template <class T>
	const std::vector<std::string> & MetaData<T>::GetImageSOPInstanceUIDs() const
	{
		return m_imageSOPInstanceUIDs;
	}

	template <class T>
	const int & MetaData<T>::GetNumberOfBits() const
	{
		return m_numberOfBits;
	}

	template <class T>
	const int & MetaData<T>::GetMSBFirst() const
	{
		return m_msbFirst;
	}

	template <class T>
	const int & MetaData<T>::GetDimensionality() const
	{
		return m_dimensionality;
	}	

	template <class T>
	const int & MetaData<T>::GetCompressedDataFlag() const
	{
		return m_compressedDataFlag;
	}

	template <class T>
	const float & MetaData<T>::GetSlope() const
	{
		return m_slope;
	}

	template <class T>
	const float & MetaData<T>::GetIntercept() const
	{
		return m_intercept;
	}

	template <class T>
	MetaData<T>::~MetaData(void) 
	{
	
	};

	template <class T>
	void MetaData<T>::updateImagePosAlongScanDirectionFlag()
	{
		if (m_imagePositionsPatient.empty() || m_scanDirection==PGCore::kPgScanDirectionUnknown) 
			return;

		int posCount = m_imagePositionsPatient.size();
		float imgPosZFirst = 0, imgPosZLast=0;

		switch (m_scanDirection)
		{
			case PGCore::kPgScanDirectionAxial:
				imgPosZFirst = m_imagePositionsPatient[0].Z(), 
				imgPosZLast  = m_imagePositionsPatient[posCount-1].Z();
				break;

			case PGCore::kPgScanDirectionCoronal:
				imgPosZFirst = m_imagePositionsPatient[0].Y(), 
				imgPosZLast  = m_imagePositionsPatient[posCount-1].Y();
				break;

			case PGCore::kPgScanDirectionSagittal:
				imgPosZFirst = m_imagePositionsPatient[0].X(), 
				imgPosZLast  = m_imagePositionsPatient[posCount-1].X();
				break;

			default: break;
		}
			
		// if first slice Z < last slice Z, it is following scan direction
		m_imagePosAlongDirectionAxis = (imgPosZFirst<imgPosZLast);		
	};


	template <class T>
	void MetaData<T>::updateScanDirectionFlag()
	{
		m_scanDirection = PGCore::kPgScanDirectionUnknown;

		if (m_imageOrientationsPatientX.empty() || m_imageOrientationsPatientY.empty())
		{
			return;
		}

		std::vector<PGMath::Vector3D<float>> scanDirs;
		scanDirs.push_back(PGMath::Vector3D<float>(0, 0, 1)); // Transverse/axial
		scanDirs.push_back(PGMath::Vector3D<float>(0, 1, 0)); // Coronal
		scanDirs.push_back(PGMath::Vector3D<float>(1, 0, 0)); // Sagittal		


		PGMath::Vector3D<float> firstOrX = m_imageOrientationsPatientX[0];
		PGMath::Vector3D<float> firstOrY = m_imageOrientationsPatientY[0];

		PGMath::Vector3D<float> firstOrZ = firstOrX^firstOrY;
		firstOrZ.Normalize();

		for (int d=0; d<=2; d++)
		{
			// check for each direction
			PGMath::Vector3D<float> zVec = scanDirs[d];//(0, 0, 1);

			float dotProd = firstOrZ*zVec;
			float angle = acos(dotProd);

			// return as soon as a first match is found
			if (fabs(angle*PG_RADIANS_TO_DEGREES) < 10.0f)
			{
				m_scanDirection = PGCore::ePGScanDirection(d);
				return;
			}			
		}		
	}



	template <class T>
	MetaData<T>::MetaData(const MetaData<T>& iObject)
	{
		*this = iObject;	
	}

#ifdef _PG_GENERATE_SDK_LIBS_	
	template class MetaData<char>; 
	template class MetaData<short>; 
	template class MetaData<float>; 
	template class MetaData<unsigned short>; 
	template class MetaData<unsigned char>; 
	template class MetaData<PGMath::Point3D<unsigned short> >;
	template class MetaData<PGMath::Point3D<unsigned char> >;
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGMetaData_HPP_
