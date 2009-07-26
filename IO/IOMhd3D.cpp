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
	return false;
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
	---------------------------------------------------------------------------*/
	
	std::string line, inStr;

	
	
	char cBuffer[256] = {0};

	// get object type 
	getline(ifs, line);
	_snscanf(line.c_str(), 255, "ObjectType = %s", cBuffer); cBuffer[255] = '\0';
	if (strcmp(cBuffer, "Image")!=0)
	{
		LOG0("IOMhd3D::Read failed: Invalid object type");
		ifs.close();
		return false;
	}

	// get ndims
	int nDims=0;
	getline(ifs, line);
	_snscanf(line.c_str(), 255, "NDims = %d", &nDims); 
	if (nDims<2 || nDims>3)
	{
		LOG0("IOMhd3D::Read failed: Invalid dimension count");
		ifs.close();
		return false;
	}

	ioSrcType = (nDims==3) ? kPgIOSourceTypeFile : kPgIOSourceTypeFolder;
	


	// BinaryData = True
	memset(cBuffer, 0, 255); cBuffer[255] = '\0';
	getline(ifs, line); 
	_snscanf(line.c_str(), 255, "BinaryData = %s", cBuffer); cBuffer[255] = '\0';
	if (strcmp(cBuffer, "True")!=0)
	{
		LOG0("IOMhd3D::Read failed: Invalid binary data type");
		ifs.close();
		return false;
	}
	ioFormatString = "RAW";


	// BinaryDataByteOrderMSB = False
	bool ioMsbFirstFlag=false;
	memset(cBuffer, 0, 255); cBuffer[255] = '\0';
	getline(ifs, line); 
	_snscanf(line.c_str(), 255, "BinaryDataByteOrderMSB = %s", cBuffer); cBuffer[255] = '\0';
	if (strcmp(cBuffer, "True")==0)
	{
		ioMsbFirstFlag = true;

	} else if (strcmp(cBuffer, "False")==0)
	{
		ioMsbFirstFlag = false;

	} else
	{
		LOG0("IOMhd3D::Read failed: Invalid MSB flag");
		ifs.close();
		return false;
	}
	ioMetaData.SetMSBFirst(ioMsbFirstFlag);
	

	// TransformMatrix = 1 0 0 0 1 0 0 0 1
	float xOrientation[3] = {1, 0, 0}, yOrientation[3] = {0, 1, 0}, zOrientation[3] = {0, 0, 1};
	getline(ifs, line); 
	_snscanf(line.c_str(), 255, "TransformMatrix = %f %f %f %f %f %f %f %f %f", 
		&(xOrientation[0]), &(xOrientation[1]), &(xOrientation[2]),
		&(yOrientation[0]), &(yOrientation[1]), &(yOrientation[2]),
		&(zOrientation[0]), &(zOrientation[1]), &(zOrientation[2])
		); 
	
	
	

	// Offset = 0 0 0
	int offsets[3]= {0, 0, 0};
	getline(ifs, line);
	_snscanf(line.c_str(), 255, "Offset = %d %d %d", &(offsets[0]), &(offsets[1]), &(offsets[2])); 
	if (offsets[0]+offsets[1]+offsets[2]>0)
	{
		LOG0("IOMhd3D::Read failed: Invalid offsets");
		ifs.close();
		return false;
	}
	

	// CenterOfRotation = 0 0 0
	int centerRtn[3]= {0, 0, 0};
	getline(ifs, line);
	_snscanf(line.c_str(), 255, "CenterOfRotation = %d %d %d", &(centerRtn[0]), &(centerRtn[1]), &(centerRtn[2])); 
	if (centerRtn[0]+centerRtn[1]+centerRtn[2]>0)
	{
		LOG0("IOMhd3D::Read failed: Invalid CenterOfRotation");
		ifs.close();
		return false;
	}
	

	// ElementSpacing = 0.5 0.5 0.5
	float spacings[3]= {0, 0, 0};
	getline(ifs, line);
	_snscanf(line.c_str(), 255, "ElementSpacing = %f %f %f", &(spacings[0]), &(spacings[1]), &(spacings[2])); 
	if (spacings[0]*spacings[1]*spacings[2]<=0)
	{
		LOG0("IOMhd3D::Read failed: Invalid spacing");
		ifs.close();
		return false;
	}
	ioMetaData.SetSpacing(PGMath::Vector3D<float>(spacings[0], spacings[1], spacings[2]));


	// DimSize = 256 256 256
	int dims[3]= {0, 0, 0};
	getline(ifs, line);
	_snscanf(line.c_str(), 255, "DimSize = %d %d %d", &(dims[0]), &(dims[1]), &(dims[2])); 
	if (dims[0]*dims[1]*dims[2]<=0)
	{
		LOG0("IOMhd3D::Read failed: Invalid Dimensions");
		ifs.close();
		return false;
	}
	ioMetaData.SetSize(PGMath::Vector3D<int>(dims[0], dims[1], dims[2]));
	ioMetaData.SetFrameCount(dims[2]);

	// AnatomicalOrientation = ???
	memset(cBuffer, 0, 255); cBuffer[255] = '\0';
	getline(ifs, line); 
	_snscanf(line.c_str(), 255, "AnatomicalOrientation = %s", cBuffer); cBuffer[255] = '\0';
	if (strcmp(cBuffer, "???")==0)
	{
		LOG1("IOMhd3D::Read Warning: Invalid AnatomicalOrientation: %s", cBuffer);		
	}


	// ElementType = MET_UCHAR
	int bytesPerPixel = 0;
	memset(cBuffer, 0, 255); cBuffer[255] = '\0';
	getline(ifs, line); 
	_snscanf(line.c_str(), 255, "ElementType = %s", cBuffer); cBuffer[255] = '\0';
	if (strcmp(cBuffer, "MET_UCHAR")==0 || strcmp(cBuffer, "MET_CHAR")==0)
	{
		bytesPerPixel = 1;

	} else if (strcmp(cBuffer, "MET_USHORT")==0 || strcmp(cBuffer, "MET_SHORT")==0)
	{
		bytesPerPixel = 2;
	} else
	{
		LOG0("IOMhd3D::Read failed: Invalid ElementType");
		ifs.close();
		return false;
	}
	LOG1("IOMhd3D::Read: Bits per pixel: %d", bytesPerPixel);		
	ioMetaData.SetSamplesPerPixel(bytesPerPixel);
	ioMetaData.SetNumberOfBits(8*bytesPerPixel);

	// ElementDataFile = test.raw
	memset(cBuffer, 0, 255); cBuffer[255] = '\0';
	getline(ifs, line); 
	_snscanf(line.c_str(), 255, "ElementDataFile = %s", cBuffer); cBuffer[255] = '\0';
	ioFileNames.push_back(std::string(cBuffer));
	

	// now also set orientation vectors
	// now also set position vectors
	std::vector<PGMath::Vector3D<float>> orXs, orYs, poSs;
	PGMath::Vector3D<float> orX(xOrientation[0], xOrientation[1], xOrientation[2]);
	PGMath::Vector3D<float> orY(yOrientation[0], yOrientation[1], yOrientation[2]);
	PGMath::Vector3D<float> orZ = orX^orY;

	for (int z=0; z<dims[2]; z++)
	{
		orXs.push_back(orX);
		orYs.push_back(orY);

		PGMath::Vector3D<float> posVec = orZ*z;

		poSs.push_back(posVec);
	}

	ioMetaData.SetImagePositionsPatient(poSs);
	ioMetaData.SetImageOrientationsPatientX(orXs);
	ioMetaData.SetImageOrientationsPatientY(orYs);

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


#ifdef _PG_GENERATE_SDK_LIBS_
	template class IOMhd3D<short>; 
	template class IOMhd3D<unsigned char>; 
#endif

};

