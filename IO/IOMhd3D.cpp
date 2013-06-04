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
// Filename:	IOMhd3D.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOMhd3D.h: interface for the IOMhd3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#include "IO/IOMhd3D.h"

#ifdef _DRAW_SYNTH_SHAPES_
	#include "Core/DrawHelper.h"
#endif

namespace PGIO
{

template <class T>
bool IOMhd3D<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!iMetaDataObject)
	{
		LOG0("IO/IOMhd3D::Write: metadata object null");			
		return false;
	}	

	/*
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IOMhd3D::Write: Invalid output container for metadata.");			
		return false;
	}

	// read one file / multiple based on the type set here
	const std::vector<std::string> &iSourceNamesMhd = iParams.Source();
	if (iSourceNamesMhd.empty())
	{
		LOG0("IO/IOMhd3D::Write: Invalid input filename.");			
		return false;
	}
	
	if (!oDataObject) 
	{
		LOG0("IO/IOMhd3D::Write: Invalid input image.");			
		return false;
	}

	// parse the def file
	std::vector<std::string> iSourceNames;
	std::string formatStr;
	int msbFirst=0;
	PGMath::Vector3D<int> dims;
	ePgIOSourceType ioSrcType = kPgIOSourceTypeUnknown;
	PGMath::Vector3D<float> spacingVec(1.0f, 1.0f, 1.0f);
	bool rv = ParseDefinitionFile(iSourceNamesMhd[0], ioSrcType, iSourceNames, 
		formatStr, *oMetaData);
	if (!rv)
	{
		return false; 
	}

	m_fileFormat = kPgIOSourceFormatUnknown;
	if (strncmp(formatStr.c_str(), "RAW", 3)==0)
	{
		m_fileFormat = kPgIOSourceFormatRAW;
	} else
	{
		LOG0("IO/IOMhd3D::Write: Invalid input file format. Should be RAW.");			
		return false;
	}
	
	// set all metadata here
	// then call Raw3D IO, and in there decide whether to thread or not

	IOParams rawParams(iSourceNames, iParams.MetaSource(), iParams.SourceType(),
		iParams.IsEnabledMultiThreading(), iParams.IsEnabledAsynchronousLoad(),
		iParams.IsEnabledAutoSkip(), iParams.SourceFormat());
	

	m_rawImgIO.SetProgressCB(GetProgressCB());
	m_rawImgIO.SetSkipFactors(m_skipFactorXY, m_skipFactorZ);
	m_rawImgIO.SetByteLimitInKB(GetByteLimitInKB());

	bool res = m_rawImgIO.Read(oDataObject, rawParams, oMetaDataObject);

	return res;
	*/
}


template <class T>
bool IOMhd3D<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject)
	{
		LOG0("IO/IOMhd3D::Read: metadata object null");			
		return false;
	}

	/*
	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IO/IOMhd3D::Read: failed to read metadata.");			
		return false;
	}*/

	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	// read one file / multiple based on the type set here
	const std::vector<std::string> &iSourceNamesMhd = iParams.Source();
	if (iSourceNamesMhd.empty())
	{
		LOG0("IO/IOMhd3D::Read: Invalid input filename.");			
		return false;
	}
	
	if (!oDataObject) 
	{
		LOG0("IO/IOMhd3D::Read: Invalid input image.");			
		return false;
	}

	// parse the def file
	std::vector<std::string> iSourceNames;
	std::string formatStr;
	int msbFirst=0;
	PGMath::Vector3D<int> dims;
	ePgIOSourceType ioSrcType = kPgIOSourceTypeUnknown;
	PGMath::Vector3D<float> spacingVec(1.0f, 1.0f, 1.0f);
	bool rv = ParseDefinitionFile(iSourceNamesMhd[0], ioSrcType, iSourceNames, 
		formatStr, *oMetaData);
	if (!rv)
	{
		return false; 
	}

	m_fileFormat = kPgIOSourceFormatUnknown;
	if (strncmp(formatStr.c_str(), "RAW", 3)==0)
	{
		m_fileFormat = kPgIOSourceFormatRAW;
	} else
	{
		LOG0("IO/IOMhd3D::Read: Invalid input file format. Should be RAW.");			
		return false;
	}


	

	// set all metadata here
	// then call Raw3D IO, and in there decide whether to thread or not

	

	IOParams rawParams(iSourceNames, iParams.MetaSource(), iParams.SourceType(),
		iParams.IsEnabledMultiThreading(), iParams.IsEnabledAsynchronousLoad(),
		iParams.IsEnabledAutoSkip(), iParams.SourceFormat());
	

	m_rawImgIO.SetProgressCB(GetProgressCB());
	m_rawImgIO.SetSkipFactors(m_skipFactorXY, m_skipFactorZ);
	m_rawImgIO.SetByteLimitInKB(GetByteLimitInKB());

	bool res = m_rawImgIO.Read(oDataObject, rawParams, oMetaDataObject);

	//UpdateProgress(100);

	return res;
}


template <class T>
bool IOMhd3D<T>::ParseDefinitionFile(const std::string &iDefSourceName, 
	    ePgIOSourceType& ioSrcType, std::vector<std::string>& ioFileNames, 
		std::string& ioFormatString,
		PGCore::MetaData<T> &ioMetaData)
{
	LOG1("IO/IOMhd3D::ParseDefinitionFile: File: %s", iDefSourceName.c_str());	
	ifstream ifs(iDefSourceName.c_str());
	if (!ifs.good()) return false;

	
	/*-------------------------------------------------------------------------
	.def metadata file format:
	==========================
	ObjectType = Image
	NDims = 3
	BinaryData = True
	BinaryDataByteOrderMSB = False
	TransformMatrix = 1 0 0 0 1 0 0 0 1
	Offset = 0 0 0
	CenterOfRotation = 0 0 0
	ElementSpacing = 0.5 0.5 0.5
	DimSize = 256 256 256
	AnatomicalOrientation = ???
	ElementType = MET_UCHAR
	ElementDataFile = test.raw

	ObjectType = Image
	NDims = 4
	BinaryData = True
	BinaryDataByteOrderMSB = False
	CompressedData = True
	CompressedDataSize = 10767902
	TransformMatrix = 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
	Offset = 0 0 0 0
	CenterOfRotation = 0 0 0 0
	ElementSpacing = 0.740234 0.740234 2 1
	DimSize = 512 512 69 1
	AnatomicalOrientation = ????
	ElementType = MET_SHORT
	ElementDataFile = NYU-20081103-1133.zraw


	---------------------------------------------------------------------------*/
	
	std::string line, inStr;	
	char cBuffer[256] = {0};

	while(getline(ifs, line)) 
	{
			cout << "[ " << line << " ]" << endl;
			LOG1("IOMhd3D<T>::ParseDefinitionFile: Read line %s", line.c_str());	

			if (strstr ( line.c_str(), kPGMHD3DTagElementDataFile))
			{
				std::string FileStr = getValueS(line, std::string(kPGMHD3DTagElementDataFile));
				ioMetaData.SetSourceTypeSingleFile(true);			
				ioSrcType = kPgIOSourceTypeFile;
				ioFileNames.push_back(FileStr);
				ioFormatString = "RAW";
			}

			if(!fillMetaValue(line, &ioMetaData))
			{
				LOG1("IOMhd3D<T>::ParseDefinitionFile: line %s is a comment.", line.c_str());	
				continue;
			}		
		}

	if (ioMetaData.GetCompressedDataFlag())
	{
		LOG0("IOMhd3D<T>::ParseDefinitionFile: ERROR: This dataset is compressed. Please unzip the raw data file!");	
		return false;
	}
	
	return true;		
}


// protected
template <class T>
bool IOMhd3D<T>::WriteToFolder(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject)
{
	return false;
}

template <class T>
void IOMhd3D<T>::StartExecution(void *iParam)
{
	// not implemented here
	
	return;
}

template <class T>
	bool IOMhd3D<T>::fillMetaValue(const std::string &iStr, PGCore::MetaData<T> *oMetaData) const
	{
		/*-------------------------------------------------------------------------
	.def metadata file format:
	==========================
	ObjectType = Image
	NDims = 3
	BinaryData = True
	BinaryDataByteOrderMSB = False
	TransformMatrix = 1 0 0 0 1 0 0 0 1
	Offset = 0 0 0
	CenterOfRotation = 0 0 0
	ElementSpacing = 0.5 0.5 0.5
	DimSize = 256 256 256
	AnatomicalOrientation = ???
	ElementType = MET_UCHAR
	ElementDataFile = test.raw

	ObjectType = Image
	NDims = 4
	BinaryData = True
	BinaryDataByteOrderMSB = False
	CompressedData = True
	CompressedDataSize = 10767902
	TransformMatrix = 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
	Offset = 0 0 0 0
	CenterOfRotation = 0 0 0 0
	ElementSpacing = 0.740234 0.740234 2 1
	DimSize = 512 512 69 1
	AnatomicalOrientation = ????
	ElementType = MET_SHORT
	ElementDataFile = NYU-20081103-1133.zraw

	static const char* kPGMHD3DTagObjectType			=	"ObjectType";
static const char* kPGMHD3DTagNDims					=	"NDims";
static const char* kPGMHD3DTagBinaryData			=	"BinaryData";
static const char* kPGMHD3DTagBinaryDataByteOrderMSB=	"BinaryDataByteOrderMSB";
static const char* kPGMHD3DTagCompressedData		=	"CompressedData";
static const char* kPGMHD3DTagCompressedDataSize	=	"CompressedDataSize";
static const char* kPGMHD3DTagTransformMatrix		=	"TransformMatrix";
static const char* kPGMHD3DTagOffset				=	"Offset";
static const char* kPGMHD3DTagCenterOfRotation		=	"CenterOfRotation";
static const char* kPGMHD3DTagElementSpacing		=	"ElementSpacing";
static const char* kPGMHD3DTagDimSize				=	"DimSize";
static const char* kPGMHD3DTagAnatomicalOrientation	=	"AnatomicalOrientation";
static const char* kPGMHD3DTagElementType			=	"ElementType";
static const char* kPGMHD3DTagElementDataFile		=	"ElementDataFile";

	---------------------------------------------------------------------------*/
		if (!oMetaData) 
		{
			return false;
		}
		bool res = true;		
		bool binaryData = false;		
		int binaryDataCompressedSize = 0;
		if (strstr ( iStr.c_str(), kPGMHD3DTagObjectType))
		{
			std::string ObjectTypeStr = getValueS(iStr, std::string(kPGMHD3DTagObjectType));							
		} 
		else if (strstr ( iStr.c_str(), kPGMHD3DTagNDims))
		{
			int nDims = getValueI(iStr, std::string(kPGMHD3DTagNDims));
			oMetaData->SetDimensionality(nDims);
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagDimSize))
		{
			int nDims = oMetaData->GetDimensionality();
			if (nDims<=0)
			{
				LOG1("IOMhd3D<T>::fillMetaValue: Invalid dimensions : %d!", nDims);		
				return false;
			}

			std::vector<int> dimsVec;
			getVectorI(iStr, kPGMHD3DTagDimSize, nDims, dimsVec);
			oMetaData->SetSize(PGMath::Vector3D<int>(dimsVec[0], dimsVec[1], dimsVec[2]));		

			// create dummy orientation vectors and set 
			std::vector< PGMath::Vector3D<float> > orXs, orYs;
			PGMath::Vector3D<float>	orX(1.0f, 0.0f, 0.0f), orY(0.0f, 1.0f, 0.0f);
			int i=0;
			for (i=0; i<dimsVec[2]; i++)
			{
				orXs.push_back(orX);
				orYs.push_back(orY);
			}
			oMetaData->SetImageOrientationsPatientX(orXs);
			oMetaData->SetImageOrientationsPatientY(orYs);				

			// create dummy position vectors and set 				
			std::vector< PGMath::Vector3D<float> > imagePositions;
			PGMath::Vector3D<float>	firstImagePos(0.0f, 0.0f, 0.0f), lastImagePos(0.0f, 0.0f, (float)(dimsVec[2]-1));
			for (i=0; i<dimsVec[2]; i++)
			{
				imagePositions.push_back(firstImagePos + PGMath::Vector3D<float>(0.0f, 0.0f, 1.0f));			
			}
			oMetaData->SetImagePositionsPatient(imagePositions);	
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagBinaryData))
		{
			std::string BinaryDataStr = getValueS(iStr, std::string(kPGMHD3DTagBinaryData));
			if (strstr ( BinaryDataStr.c_str(), kPGMHD3DValueBoolTrue)) binaryData = true;
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagBinaryDataByteOrderMSB))
		{
			bool binaryDataMSB = false;
			std::string BinaryDataMSBStr = getValueS(iStr, std::string(kPGMHD3DTagBinaryDataByteOrderMSB));
			if (strstr ( BinaryDataMSBStr.c_str(), kPGMHD3DValueBoolTrue)) binaryDataMSB = true;
			oMetaData->SetMSBFirst(binaryDataMSB);
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagCompressedData))
		{
			int binaryDataCompressed=0;
			std::string BinaryDataCompressedStr = getValueS(iStr, std::string(kPGMHD3DTagCompressedData));
			if (strstr ( BinaryDataCompressedStr.c_str(), kPGMHD3DValueBoolTrue)) 
			{
				binaryDataCompressed = 1;
			    oMetaData->SetCompressedDataFlag(binaryDataCompressed);
			} else if (strstr ( BinaryDataCompressedStr.c_str(), kPGMHD3DValueBoolFalse)) 
			{
				binaryDataCompressed = 0;
				oMetaData->SetCompressedDataFlag(binaryDataCompressed);
			} else 
			{
				LOG1("IOMhd3D<T>::fillMetaValue: Shadowing tag ignored: %s!", BinaryDataCompressedStr.c_str());
			}			
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagCompressedDataSize))
		{
			binaryDataCompressedSize = getValueI(iStr,std::string(kPGMHD3DTagCompressedDataSize));
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagTransformMatrix))
		{
			PGMath::Vector3D<int>	dims = oMetaData->GetSize();
			int nDims = oMetaData->GetDimensionality();

			if (nDims<=0 || nDims >4 || dims.X()<=0 || dims.Y()<=0 || dims.Z()<=0)
			{
				LOG4("IOMhd3D<T>::fillMetaValue: Invalid dimensions : %d, %d %d %d!", nDims, dims.X(), dims.Y(), dims.Z());		
				return false;
			}
			
			std::vector<float> transformMatrix;
			int matrixOffset=0;
			if (nDims<=3)
			{
				matrixOffset=3;			
			} else if (nDims==4)
			{
				matrixOffset=4;								
			} 

			getMatrixF(iStr, kPGMHD3DTagTransformMatrix, nDims, transformMatrix);

			// X component
			std::vector< PGMath::Vector3D<float> > orXs = oMetaData->GetImageOrientationsPatientX();
			if (orXs.size() < dims.Z())
			{
				LOG2("IOMhd3D<T>::fillMetaValue: OrX vecs have not been initialized! %d vs %d!", orXs.size(), dims.Z());		
				return false;
			}
			if (!orXs.empty())
			{
				PGMath::Vector3D<float>	orX = orXs[0];
				orX = PGMath::Vector3D<float>(transformMatrix[0], transformMatrix[1], transformMatrix[2]);							
				orXs.clear();
				for (int i=0; i<dims.Z(); i++)
				{
					orXs.push_back(orX);				
				}
				oMetaData->SetImageOrientationsPatientX(orXs);			
			}

			// Y component
			std::vector< PGMath::Vector3D<float> > orYs = oMetaData->GetImageOrientationsPatientY();
			if (orYs.size() < dims.Z())
			{
				LOG2("IOMhd3D<T>::fillMetaValue: OrY vecs have not been initialized! %d vs %d!", orYs.size(), dims.Z());		
				return false;
			}
			if (!orYs.empty())
			{
				PGMath::Vector3D<float>	orY = orYs[0];
				orY = PGMath::Vector3D<float>(transformMatrix[0+matrixOffset], transformMatrix[1+matrixOffset], transformMatrix[2+matrixOffset]);							
				orYs.clear();
				for (int i=0; i<dims.Z(); i++)
				{
					orYs.push_back(orY);				
				}
				oMetaData->SetImageOrientationsPatientY(orYs);			
			}		
		}
		else if (strstr ( iStr.c_str(), kPGMHD3DTagOffset))
		{
			int nDims = oMetaData->GetDimensionality();
			if (nDims<=0)
			{
				LOG1("IOMhd3D<T>::fillMetaValue: Invalid dimensions : %d!", nDims);		
				return false;
			}

			std::vector<float> offsetVec;
			getVectorF(iStr, kPGMHD3DTagOffset, nDims, offsetVec);
			// no one uses it yet	
		}
		/*		
static const char* kPGMHD3DTagAnatomicalOrientation	=	"AnatomicalOrientation";
static const char* kPGMHD3DTagElementType			=	"ElementType"; 
static const char* kPGMHD3DTagElementDataFile		=	"ElementDataFile";*/

		else if (strstr ( iStr.c_str(), kPGMHD3DTagCenterOfRotation))
		{
			int nDims = oMetaData->GetDimensionality();
			if (nDims<=0)
			{
				LOG1("IOMhd3D<T>::fillMetaValue: Invalid dimensions : %d!", nDims);		
				return false;
			}

			std::vector<float> cRotVec;
			getVectorF(iStr, kPGMHD3DTagCenterOfRotation, nDims, cRotVec);
			// no one uses it yet	
		}				
		else if (strstr ( iStr.c_str(), kPGMHD3DTagElementSpacing))
		{
			int nDims = oMetaData->GetDimensionality();
			if (nDims<=0)
			{
				LOG1("IOMhd3D<T>::fillMetaValue: Invalid dimensions : %d!", nDims);		
				return false;
			}

			std::vector<float> spcVec;
			getVectorF(iStr, kPGMHD3DTagElementSpacing, nDims, spcVec);						
			oMetaData->SetSpacing(PGMath::Vector3D<float>(spcVec[0], spcVec[1], spcVec[2]));		
		}				
		else if (strstr ( iStr.c_str(), kPGMHD3DTagElementType))
		{
			std::string ElemTypeStr = getValueS(iStr, std::string(kPGMHD3DTagElementType));
			if (strstr ( ElemTypeStr.c_str(), kPGMHD3DValueElementTypeShort)) 
			{
				oMetaData->SetNumberOfBits(8*sizeof (short));
				oMetaData->SetSamplesPerPixel(sizeof (short));
			} else
			{
				oMetaData->SetNumberOfBits(8*sizeof (unsigned char));
				oMetaData->SetSamplesPerPixel(sizeof (unsigned char));
			}
		} 		
		else if (strstr ( iStr.c_str(), kPGMHD3DTagElementDataFile))
		{
			std::string FileStr = getValueS(iStr, std::string(kPGMHD3DTagElementDataFile));
			oMetaData->SetSourceTypeSingleFile(true);			
		}		
		else
		{
			LOG1("IOMhd3D<T>::fillMetaValue: WARNING: Unrecognized tag: %s!!!", iStr.c_str());									
			res = false;
		};

		return res;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class IOMhd3D<short>; 
	template class IOMhd3D<unsigned char>; 
#endif

};

