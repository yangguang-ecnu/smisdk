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
// Filename:	IOBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 12, 2006 at 11:56:30 PM
//

// IOBase.h: interface for the IOBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBase_H__C4EA1B91_0881_44C7_927E_3CD2769137C9HPP__INCLUDED_)
#define AFX_IOBase_H__C4EA1B91_0881_44C7_927E_3CD2769137C9HPP__INCLUDED_

#include "IOBase.h"

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

template <class T>
bool IOBase<T>::WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams)
{

	return true;
}

template <class T>
bool IOBase<T>::ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams)
{
	const std::string &iMetaFileName = iParams.MetaSource();
	if (iMetaFileName.empty())
	{
		LOG0("IOBase::ReadMetaData: Invalid input filename.");			
		return false;
	}

	if (!oMetaDataObject) 
	{
		LOG0("IOBase::ReadMetaData: Invalid input image.");			
		return false;
	}

	// cast to appropriate type here
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// read the meta file here
	LOG1("IOBase::ReadMetaData: File: %s", iMetaFileName.c_str());	
	ifstream ifs(iMetaFileName.c_str());
	std::string line, inStr;

	while(getline(ifs, line)) {
		cout << "[ " << line << " ]" << endl;
		LOG1("IOBase::ReadMetaData: Read line %s", line.c_str());	
		if(!fillMetaValue(line, oMetaData))
		{
			LOG1("IOBase::ReadMetaData: line %s is a comment.", line.c_str());	
			continue;
		}		
	}


	return true;
}

template <class T>
int IOBase<T>::getValueI(const std::string &iStr, const std::string &iTag) const
{
	char oTag[512] = {0}, oStr[512] = {0};
	sscanf(iStr.c_str(), "%s = %s", oTag, oStr);
	if (strcmp(iTag.c_str(), oTag)) 
	{
		return 0;
	}
	return atoi(oStr);
}


template <class T>
float IOBase<T>::getValueF(const std::string &iStr, const std::string &iTag) const
{
	char oTag[512] = {0}, oStr[512] = {0};
	sscanf(iStr.c_str(), "%s = %s", oTag, oStr);
	if (strcmp(iTag.c_str(), oTag)) 
	{
		return 0.0f;
	}
	return atof(oStr);
}


template <class T>
std::string IOBase<T>::getValueS(const std::string &iStr, const std::string &iTag) const
{
	char oTag[512] = {0}, oStr[512] = {0};
	sscanf(iStr.c_str(), "%s = %s", oTag, oStr);
	if (strcmp(iTag.c_str(), oTag)) 
	{
		return std::string("Unknown");
	}
	return std::string(oStr);
}



template <class T>
bool IOBase<T>::fillMetaValue(const std::string &iStr, PGCore::MetaData<T> *oMetaData) const
{
	if (!oMetaData) 
	{
		return false;
	}
	bool res = true;
	if (strstr ( iStr.c_str(), kPGTagModality))
	{
		std::string modalityStr = getValueS(iStr, std::string(kPGTagModality));
		PGCore::ePGModality modality = 	PGCore::MetaData<T>::StrToModality(modalityStr.c_str());
		oMetaData->SetModality(modality);				
	} 
	else if (strstr ( iStr.c_str(), kPGTagPatientName))
	{
		oMetaData->SetPatientName(getValueS(iStr, std::string(kPGTagPatientName)));
	}
	else if (strstr ( iStr.c_str(), kPGFileNamePrefix))
	{
		oMetaData->SetFileNamePrefix(getValueS(iStr, std::string(kPGFileNamePrefix)));
	}
	else if (strstr ( iStr.c_str(), kPGFileNameNumberDigits))
	{
		oMetaData->SetFileNameNumberDigits(getValueI(iStr, std::string(kPGFileNameNumberDigits)));
	}
	else if (strstr ( iStr.c_str(), kPGTagBits))
	{
		oMetaData->SetNumberOfBits(getValueI(iStr, std::string(kPGTagBits)));
	}
	else if (strstr ( iStr.c_str(), kPGTagMSBFirst))
	{
		oMetaData->SetMSBFirst(getValueI(iStr,std::string(kPGTagMSBFirst)));
	}
	else if (strstr ( iStr.c_str(), kPGTagDimensions))
	{
		//oMetaData->SetMSBFirst(getValueI(std::string(kPGTagDimensions)));
	}
	else if (strstr ( iStr.c_str(), kPGTagDimX))
	{
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		int			dimX  = getValueI(iStr, std::string(kPGTagDimX));  
		oMetaData->SetSize(PGMath::Vector3D<int>(dimX, dims.Y(), dims.Z()));
	}
	else if (strstr ( iStr.c_str(), kPGTagDimY))
	{
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		int			dimY  = getValueI(iStr, std::string(kPGTagDimY));  
		oMetaData->SetSize(PGMath::Vector3D<int>(dims.X(), dimY, dims.Z()));
	}
	else if (strstr ( iStr.c_str(), kPGTagDimZ))
	{
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		int			dimZ  = getValueI(iStr, std::string(kPGTagDimZ));  
		oMetaData->SetSize(PGMath::Vector3D<int>(dims.X(), dims.Y(), dimZ));

		// create dummy orientation vectors and set 
		std::vector< PGMath::Vector3D<float> > orXs, orYs;
		PGMath::Vector3D<float>	orX(1.0f, 0.0f, 0.0f), orY(0.0f, 1.0f, 0.0f);
		int i=0;
		for (i=0; i<dimZ; i++)
		{
			orXs.push_back(orX);
			orYs.push_back(orY);
		}
		oMetaData->SetImageOrientationsPatientX(orXs);
		oMetaData->SetImageOrientationsPatientY(orYs);				

		// create dummy position vectors and set 				
		std::vector< PGMath::Vector3D<float> > imagePositions;
		PGMath::Vector3D<float>	firstImagePos(0.0f, 0.0f, 0.0f), lastImagePos(0.0f, 0.0f, (float)(dimZ-1));
		for (i=0; i<dimZ; i++)
		{
			imagePositions.push_back(firstImagePos + PGMath::Vector3D<float>(0.0f, 0.0f, 1.0f));			
		}
		oMetaData->SetImagePositionsPatient(imagePositions);		
	}	
	else if (strstr ( iStr.c_str(), kPGTagNumberOfFiles))
	{
		int	numberOfFiles  = getValueI(iStr, std::string(kPGTagNumberOfFiles));  
		oMetaData->SetSourceTypeSingleFile((numberOfFiles==1));
	}
	else if (strstr ( iStr.c_str(), kPGTagSpacingX))
	{
		PGMath::Vector3D<float>	dims = oMetaData->GetSpacing();
		float		dimX  = getValueF(iStr, std::string(kPGTagSpacingX));  
		oMetaData->SetSpacing(PGMath::Vector3D<float>(dimX, dims.Y(), dims.Z()));		
	}
	else if (strstr ( iStr.c_str(), kPGTagSpacingY))
	{
		PGMath::Vector3D<float>	dims = oMetaData->GetSpacing();
		float		dimY  = getValueF(iStr, std::string(kPGTagSpacingY));  
		oMetaData->SetSpacing(PGMath::Vector3D<float>(dims.X(), dimY, dims.Z()));		
	}
	else if (strstr ( iStr.c_str(), kPGTagSpacingZ))
	{
		PGMath::Vector3D<float>	dims = oMetaData->GetSpacing();
		float		dimZ  = getValueF(iStr, std::string(kPGTagSpacingZ));  
		oMetaData->SetSpacing(PGMath::Vector3D<float>(dims.X(), dims.Y(), dimZ));		
	}
	else if (strstr ( iStr.c_str(), kPGOrientationXX))
	{
		float		orXX  = getValueF(iStr, std::string(kPGOrientationXX));  
		std::vector< PGMath::Vector3D<float> > orXs = oMetaData->GetImageOrientationsPatientX();
		if (!orXs.empty())
		{
			PGMath::Vector3D<float>	orX = orXs[0];
			orX = PGMath::Vector3D<float>(orXX, orX.Y(), orX.Z());			
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			orXs.clear();
			for (int i=0; i<dims.Z(); i++)
			{
				orXs.push_back(orX);				
			}
			oMetaData->SetImageOrientationsPatientX(orXs);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGOrientationXY))
	{
		float		orXY  = getValueF(iStr, std::string(kPGOrientationXY));  
		std::vector< PGMath::Vector3D<float> > orXs = oMetaData->GetImageOrientationsPatientX();
		if (!orXs.empty())
		{
			PGMath::Vector3D<float>	orX = orXs[0];
			orX = PGMath::Vector3D<float>(orX.X(), orXY, orX.Z());			
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			orXs.clear();
			for (int i=0; i<dims.Z(); i++)
			{
				orXs.push_back(orX);				
			}
			oMetaData->SetImageOrientationsPatientX(orXs);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGOrientationXZ))
	{
		float		orXZ  = getValueF(iStr, std::string(kPGOrientationXZ));  
		std::vector< PGMath::Vector3D<float> > orXs = oMetaData->GetImageOrientationsPatientX();
		if (!orXs.empty())
		{
			PGMath::Vector3D<float>	orX = orXs[0];
			orX = PGMath::Vector3D<float>(orX.X(), orX.Y(), orXZ);			
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			orXs.clear();
			for (int i=0; i<dims.Z(); i++)
			{
				orXs.push_back(orX);				
			}
			oMetaData->SetImageOrientationsPatientX(orXs);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGOrientationYX))
	{
		float		orYX  = getValueF(iStr, std::string(kPGOrientationYX));  
		std::vector< PGMath::Vector3D<float> > orYs = oMetaData->GetImageOrientationsPatientY();
		if (!orYs.empty())
		{
			PGMath::Vector3D<float>	orY = orYs[0];
			orY = PGMath::Vector3D<float>(orYX, orY.Y(), orY.Z());			
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			orYs.clear();
			for (int i=0; i<dims.Z(); i++)
			{
				orYs.push_back(orY);				
			}
			oMetaData->SetImageOrientationsPatientY(orYs);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGOrientationYY))
	{
		float		orYY  = getValueF(iStr, std::string(kPGOrientationYY));  
		std::vector< PGMath::Vector3D<float> > orYs = oMetaData->GetImageOrientationsPatientY();
		if (!orYs.empty())
		{
			PGMath::Vector3D<float>	orY = orYs[0];
			orY = PGMath::Vector3D<float>(orY.X(), orYY, orY.Z());			
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			orYs.clear();
			for (int i=0; i<dims.Z(); i++)
			{
				orYs.push_back(orY);				
			}
			oMetaData->SetImageOrientationsPatientY(orYs);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGOrientationYZ))
	{
		float		orYZ  = getValueF(iStr, std::string(kPGOrientationYZ));  
		std::vector< PGMath::Vector3D<float> > orYs = oMetaData->GetImageOrientationsPatientY();
		if (!orYs.empty())
		{
			PGMath::Vector3D<float>	orY = orYs[0];
			orY = PGMath::Vector3D<float>(orY.X(), orY.Y(), orYZ);			
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			orYs.clear();
			for (int i=0; i<dims.Z(); i++)
			{
				orYs.push_back(orY);				
			}
			oMetaData->SetImageOrientationsPatientY(orYs);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGFirstImagePositionX))
	{
		float		firstPositionX  = getValueF(iStr, std::string(kPGFirstImagePositionX));  
		std::vector< PGMath::Vector3D<float> > imagePositions = oMetaData->GetImagePositionsPatient();
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		if (imagePositions.size()==dims.Z())
		{			
			PGMath::Vector3D<float>	lastImagePosition = imagePositions[dims.Z()-1];
			PGMath::Vector3D<float>	firstImagePosition = imagePositions[0];
			PGMath::Vector3D<float>	newFirstImagePosition = PGMath::Vector3D<float>(firstPositionX, firstImagePosition.Y(), firstImagePosition.Z());			
			//PGMath::Vector3D<float>	deltaVector = (lastImagePosition - newFirstImagePosition)/(float (dims.Z()-1));
						
			imagePositions.clear();
			imagePositions.push_back(newFirstImagePosition);

			for (int i=1; i<dims.Z(); i++)
			{
				imagePositions.push_back(lastImagePosition);				
			}
			oMetaData->SetImagePositionsPatient(imagePositions);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGFirstImagePositionY))
	{
		float		firstPositionY  = getValueF(iStr, std::string(kPGFirstImagePositionY));  
		std::vector< PGMath::Vector3D<float> > imagePositions = oMetaData->GetImagePositionsPatient();
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		if (imagePositions.size()==dims.Z())
		{			
			PGMath::Vector3D<float>	lastImagePosition = imagePositions[dims.Z()-1];
			PGMath::Vector3D<float>	firstImagePosition = imagePositions[0];
			PGMath::Vector3D<float>	newFirstImagePosition = PGMath::Vector3D<float>(firstImagePosition.X(), firstPositionY, firstImagePosition.Z());			
			//PGMath::Vector3D<float>	deltaVector = (lastImagePosition - newFirstImagePosition)/(float (dims.Z()-1));
						
			imagePositions.clear();
			imagePositions.push_back(newFirstImagePosition);

			for (int i=1; i<dims.Z(); i++)
			{
				imagePositions.push_back(lastImagePosition);				
			}
			oMetaData->SetImagePositionsPatient(imagePositions);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGFirstImagePositionZ))
	{
		float		firstPositionZ  = getValueF(iStr, std::string(kPGFirstImagePositionZ));  
		std::vector< PGMath::Vector3D<float> > imagePositions = oMetaData->GetImagePositionsPatient();
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		if (imagePositions.size()==dims.Z())
		{			
			PGMath::Vector3D<float>	lastImagePosition = imagePositions[dims.Z()-1];
			PGMath::Vector3D<float>	firstImagePosition = imagePositions[0];
			PGMath::Vector3D<float>	newFirstImagePosition = PGMath::Vector3D<float>(firstImagePosition.X(), firstImagePosition.Y(), firstPositionZ);			
			//PGMath::Vector3D<float>	deltaVector = (lastImagePosition - newFirstImagePosition)/(float (dims.Z()-1));
						
			imagePositions.clear();
			imagePositions.push_back(newFirstImagePosition);

			for (int i=1; i<dims.Z(); i++)
			{
				imagePositions.push_back(lastImagePosition);				
			}
			oMetaData->SetImagePositionsPatient(imagePositions);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGLastImagePositionX))
	{
		float		lastPositionX  = getValueF(iStr, std::string(kPGLastImagePositionX));  
		std::vector< PGMath::Vector3D<float> > imagePositions = oMetaData->GetImagePositionsPatient();
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		if (imagePositions.size()==dims.Z())
		{			
			PGMath::Vector3D<float>	lastImagePosition = imagePositions[dims.Z()-1];
			PGMath::Vector3D<float>	firstImagePosition = imagePositions[0];
			PGMath::Vector3D<float>	newLastImagePosition = PGMath::Vector3D<float>(lastPositionX, lastImagePosition.Y(), lastImagePosition.Z());			
			//PGMath::Vector3D<float>	deltaVector = (newLastImagePosition - firstImagePosition)/(float (dims.Z()-1));
						
			imagePositions.clear();	

			for (int i=0; i<(dims.Z()-1); i++)
			{
				imagePositions.push_back(firstImagePosition);				
			}
			imagePositions.push_back(newLastImagePosition);				
			oMetaData->SetImagePositionsPatient(imagePositions);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGLastImagePositionY))
	{
		float		lastPositionY  = getValueF(iStr, std::string(kPGLastImagePositionY));  
		std::vector< PGMath::Vector3D<float> > imagePositions = oMetaData->GetImagePositionsPatient();
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		if (imagePositions.size()==dims.Z())
		{			
			PGMath::Vector3D<float>	lastImagePosition = imagePositions[dims.Z()-1];
			PGMath::Vector3D<float>	firstImagePosition = imagePositions[0];
			PGMath::Vector3D<float>	newLastImagePosition = PGMath::Vector3D<float>(lastImagePosition.X(), lastPositionY, lastImagePosition.Z());			
			//PGMath::Vector3D<float>	deltaVector = (newLastImagePosition - firstImagePosition)/(float (dims.Z()-1));
						
			imagePositions.clear();
			

			for (int i=0; i<dims.Z()-1; i++)
			{
				imagePositions.push_back(firstImagePosition);				
			}
			imagePositions.push_back(newLastImagePosition);				
			oMetaData->SetImagePositionsPatient(imagePositions);			
		}
	} 
	else if (strstr ( iStr.c_str(), kPGLastImagePositionZ))
	{
		float		lastPositionZ  = getValueF(iStr, std::string(kPGLastImagePositionZ));  
		std::vector< PGMath::Vector3D<float> > imagePositions = oMetaData->GetImagePositionsPatient();
		PGMath::Vector3D<int>	dims = oMetaData->GetSize();
		if (imagePositions.size()==dims.Z())
		{			
			PGMath::Vector3D<float>	lastImagePosition = imagePositions[dims.Z()-1];
			PGMath::Vector3D<float>	firstImagePosition = imagePositions[0];
			PGMath::Vector3D<float>	newLastImagePosition = PGMath::Vector3D<float>(lastImagePosition.X(), lastImagePosition.Y(), lastPositionZ);
			//PGMath::Vector3D<float>	deltaVector = (newLastImagePosition - firstImagePosition)/(float (dims.Z()-1));
						
			imagePositions.clear();			

			for (int i=0; i<dims.Z()-1; i++)
			{
				imagePositions.push_back(firstImagePosition);				
			}
			imagePositions.push_back(newLastImagePosition);
			oMetaData->SetImagePositionsPatient(imagePositions);			
		}
	} 
	else
	{
		res = false;
	};

	return res;
}

template <class T>
void IOBase<T>::clampToNextPowOfTwo(PGMath::Point3D<int>& ioDim)
{
	for (int i=0; i<=2; i++)
	{
		int j=0;

		while (pow((float)2.0f, (float)j) < ioDim[i])
		{
			j++;
		}

		ioDim[i] = pow(2.0f, j);
	}

	return;
}

template <class T>
bool IOBase<T>::ComputeSkipFactors(const PGMath::Vector3D<int>& iVolSize, const int& iBytesPerVoxel,
								   int& oSkipXY, int& oSkipZ)
{
	if (!m_autoSkip)
	{
		int oSkipZ=m_skipFactorZ;// > (iFileCount/(2*GetMaxThreadCount())) ? (iFileCount/(8*GetMaxThreadCount())) :  m_skipFactorZ;
		oSkipZ = max(oSkipZ, 1);
		int oSkipXY=m_skipFactorXY;
		oSkipXY = max(oSkipXY, 1);	
		return true;
	}

	{
		PGMath::Point3D<int> volSize(iVolSize.X(), iVolSize.Y(), iVolSize.Z());

		clampToNextPowOfTwo(volSize);

		// in KBs
		long iVoxelBytes = ((volSize.X()*volSize.Y()*volSize.Z())/kPgBytesInKB)*iBytesPerVoxel;
		long limitBytes = GetByteLimit();


		oSkipXY = 1;
		oSkipZ = 1;

		if (iVoxelBytes > limitBytes)
		{
			int oFac = iVoxelBytes/limitBytes;	
			oFac = (oFac%2) ? oFac+1 : oFac;

			if (oFac>0 && oFac<=2)
			{

				oSkipZ = oFac;
				oSkipXY = oFac;

			} else
			{
				int perD = int(pow(float(oFac), 0.33f) + 0.5f);
				int perDimXY = int(float(perD/PG_SQRT_TWO)+0.5f);
				int perDimZ = int(float(perD*2.0f)+0.5f);

				PGMath::Point3D<int> skipFacZ(perDimZ, 0, 0);
				clampToNextPowOfTwo(skipFacZ);
				oSkipZ = max(1, skipFacZ.X());


				PGMath::Point3D<int> skipFacXY(perDimXY, 0, 0);
				clampToNextPowOfTwo(skipFacXY);
				oSkipXY = max(1, skipFacXY.X());
			}

			/*

			int perDim = int(pow(float(oFac), 0.33f) + 0.5f);
			perDim = max(1, perDim);

			PGMath::Point3D<int> skipFacs(perDim, 0, 0);
			clampToNextPowOfTwo(skipFacs);

			//perDim = (perDim%2) ? (perDim+1) : perDim;

			oSkipXY = max(1, skipFacs.X());
			oSkipZ = max(1, skipFacs.X());
			*/
			/*
			if (iVolSize.Z()/oSkipZ > 512) 
			{
				oSkipZ = int((iVolSize.Z()/512.0f)+0.5f);
				oSkipZ = (oSkipZ%2) ? (oSkipZ+1) : oSkipZ;
			}

			if (iVolSize.X()/oSkipXY > 512) 
			{
				oSkipXY = int((iVolSize.X()/512.0f)+0.5f);
				oSkipXY = (oSkipXY%2) ? (oSkipXY+1) : oSkipXY;
			}
			*/


			/*
			if (sqrt((double)oFac) <=2 )
			{
				if (iVolSize.X()/2<128)//128
				{
					oSkipZ = 2;
				} else
				{
					oSkipXY = 2;
				}

			} else
			{
				if (sqrt((double)oFac) < 3)
				{		
					oSkipXY = 1;
					oSkipZ = 2;

				} else
				{
					oSkipXY = 2;//4;
					oSkipZ = 4;//4;
				}
			}
			*/

			LOG4("In voxel bytes (%d) greater than limit (%d). Will sub sample with skipXY (%d) skipZ (%d).", iVoxelBytes, limitBytes, oSkipXY, oSkipZ);		
		}
	}

	return true;
}

template <class T>
bool IOBase<T>::ExtractFileNamesFromFolder(const std::string& iFolderPath, std::vector<std::string>& ioFileNames, const char* iFilterStr/*=0*/)
{
	std::string searchPattern = iFolderPath + "\\*.*";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(searchPattern.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return false;

	BOOL nextFound = true;
	while (nextFound) 
	{		
		bool isOk = strcmp(".", FindFileData.cFileName) && strcmp("..", FindFileData.cFileName);
		
		if (isOk && iFilterStr)
		{
			std::string fname(FindFileData.cFileName);
			std::string filterStr(iFilterStr);
			PGCore::ToUpper(fname);
			PGCore::ToUpper(filterStr);

			size_t found=fname.find(filterStr);
			bool isOkFilter = (found!=string::npos);
			isOk = isOk && (isOkFilter);
		}

		if (isOk)				
		{			
			std::string nextFile = iFolderPath + "\\" + FindFileData.cFileName;
#ifdef _DEBUG
			LOG1("ExtractFileNamesFromFolder: FindFirstFile next file: %s\n", nextFile.c_str());
#endif
			ioFileNames.push_back(nextFile);				
		}
		nextFound = FindNextFile(hFind, &FindFileData);
	} 

	FindClose(hFind);

	return ioFileNames.size()>0;
}


template<class T>
bool IOBase<T>::ConsolidateImages()
{
	if (GetMaxThreadCount()<=1) return true;

	//merge volumes here		
	{
		// first find out the order of images
		const std::vector<PGMath::Vector3D<float> >& posVec0 = oMetaDataMT[0].GetImagePositionsPatient();
		const std::vector<PGMath::Vector3D<float> >& posVec1 = oMetaDataMT[1].GetImagePositionsPatient();	

		bool reverseOrder = posVec0[0].Z() < posVec1[0].Z();

		int firstIndex = reverseOrder ? GetMaxThreadCount()-1 : 0;
		int incr = reverseOrder ? -1 : 1;

		*ioDataObjectMT = oVolumesMT[firstIndex];
		oVolumesMT[firstIndex].Clear();

		PGCore::MetaData<T>* firstMetaData = (static_cast<PGCore::MetaData<T> *>((ioMetaDataObjectMT)));
		*firstMetaData = oMetaDataMT[0];

		std::vector<PGMath::Vector3D<float> > orXVec = firstMetaData->GetImageOrientationsPatientX();
		std::vector<PGMath::Vector3D<float> > orYVec = firstMetaData->GetImageOrientationsPatientY();
		std::vector<PGMath::Vector3D<float> > posVec = firstMetaData->GetImagePositionsPatient();

		for (int j=1; j<GetMaxThreadCount(); j++)
		{
			int nextIndex = firstIndex+j*incr;

			PGMath::Point3D<long> oDim;
			oVolumesMT[nextIndex].GetDimensions(oDim);
			int k=0;
			// concatenate rest of the images
			for (k=0; k<oDim.Z(); k++)
			{
				ioDataObjectMT->AddImage(oVolumesMT[nextIndex].GetImage(k));
			}

			oVolumesMT[nextIndex].Clear();

			// concatenate rest of the attributes
			std::vector<PGMath::Vector3D<float> > orXVecOut = oMetaDataMT[nextIndex].GetImageOrientationsPatientX();
			std::vector<PGMath::Vector3D<float> > orYVecOut = oMetaDataMT[nextIndex].GetImageOrientationsPatientY();
			std::vector<PGMath::Vector3D<float> > posVecOut = oMetaDataMT[nextIndex].GetImagePositionsPatient();

			for (k=0; k<oDim.Z(); k++)
			{
				orXVec.push_back(orXVecOut[k]);
				orYVec.push_back(orYVecOut[k]);
				posVec.push_back(posVecOut[k]);
			}
		}

		firstMetaData->SetImageOrientationsPatientX(posVec);
		firstMetaData->SetImageOrientationsPatientX(orXVec);
		firstMetaData->SetImageOrientationsPatientY(orYVec);
	}

	return true;
}



template<class T>
int IOBase<T>::NotifyError(const char *iMsg/*=0*/)
{		
	LOG0("Begin NotifyError");

	if (AsyncProcess::NotifyError() <= 0) return 0;

	int progressValue = -1;

	UpdateProgress(progressValue);

	LOG0("End NotifyError");

	return true;
}


template<class T>
int IOBase<T>::NotifyFinished()
{		
	LOG0("IO/IODicom3D::Begin NotifyFinished");
	//_PG_BREAK_NATIVE

	if (AsyncProcess::NotifyFinished()<=0)
	{
		return 0;
	}

	int progressValue = 100;
	UpdateProgress(progressValue);
	//LOG1("+++ Loading progress %d / 100", progressValue);

	return 1;
}


};



#endif // !defined(AFX_IOBase_H__C4EA1B91_0881_44C7_927E_3CD2769137C9HPP__INCLUDED_)
