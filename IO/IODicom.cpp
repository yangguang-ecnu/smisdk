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
// Filename:	IODicom.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IODicom.h: interface for the IODicom class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#include "IO/IODicom.h"
#include "Core/GaussianKernel.h"

namespace PGIO
{

template <class T>
IODicom<T>::IODicom(void)
{

}


template <class T>
IODicom<T>::~IODicom(void)
{

}

template <class T>
void IODicom<T>::StartExecution(void *iParams) 
{
	assert(0 && "Not implemented on purpose");
};

template <class T>
int IODicom<T>::NotifyFinished() 
{ 
	assert(0 && "Not implemented on purpose");
	return false; 
};

template <class T>
int IODicom<T>::NotifyError(const char *iMsg=0) 
{ 
	assert(0 && "Not implemented on purpose");
	return false; 
};

template <class T>
bool IODicom<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject) {
		LOG0("IO/IODicom::Read: metadata object null");			
		return false;
	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IO/IODicom::Read: Expecting single file path.");			
		return false;
	}

	if (!ReadMetaData(oMetaDataObject, iParams))
	{
		LOG0("IO/IODicom::Read: ReadMetaData failed.");			
		return false;
	}
	
	if (!oDataObject) 
	{
		LOG0("IO/IODicom::Read: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<T> *oImage = (static_cast<PGCore::Image < T > *>(oDataObject));
	if (!oImage) 
	{
		LOG0("IO/IODicom::Read: Invalid input image.");			
		return false;
	}

	
				
	//return true;


	
	//this is where to read the image in
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	

    //calculate the full size of the scalars
	const PGMath::Vector3D<int>& oSize = oMetaData->GetSize();
	long ix=oSize.Y(), iy=oSize.X();
	oImage->SetDimensions(ix, iy);

	T* oBuf = oImage->GetBuffer();
	if (!oBuf)
	{
		LOG0("IO/IODicom::Read: Invalid image buffer.");			
		return false;
	}

	bool needConversion = false;
	int nBitsPerPixel = oMetaData->GetNumberOfBits();	
	if (nBitsPerPixel!=8*sizeof(T))
	{
		LOG2("IO/IODicom::Read:Warning: Pixel type mismatch. Need %d bits per pixel, but reporting %d.", nBitsPerPixel, 8*sizeof(T));			
		needConversion = true;
		//return false;
	}

	int oByteCount = (nBitsPerPixel/8) * ix * iy;
	unsigned char *iBuf=0;

#ifdef _PG_GDDCM_

#else
	// read pixel data
	//DcmFileFormat fileformat;
	DcmDataset *dataset = m_fileformat.getDataset();
	if (!dataset)
	{
		LOG0("IO/IODicom::Read: Cannot fetch pixel data");			
		return false;	
	}

	switch (nBitsPerPixel)
	{
			case 8: 
				dataset->findAndGetUint8Array(DCM_PixelData, (const Uint8 *&) iBuf);
				break;

			case 16:
				dataset->findAndGetUint16Array(DCM_PixelData, (const Uint16 *&) iBuf);
				break;

			default:
				LOG0("IO/IODicom::Read: failed to fetch image buffer.");			
				return false;
	}
#endif

	if (iBuf==0)
	{
		LOG0("IO/IODicom::Read: Invalid image buffer.");			
		return false;
	}

    // we have to copy the data into the buffer already allocated
	if (!needConversion)
	{
		memcpy(oBuf, iBuf, oByteCount);
	} else
	{
		// first create a local image
		PGCore::Image<unsigned char> ucharImage;
		PGCore::Image<short> shortImage;

		unsigned char* localBuf = 0;
		switch (nBitsPerPixel)
		{
			case 8: ucharImage.SetDimensions(ix, iy);
					localBuf = (unsigned char *)ucharImage.GetBuffer();
					break;

			case 16:shortImage.SetDimensions(ix, iy);
					localBuf = (unsigned char *)shortImage.GetBuffer();
					break;
			
			default: break;
		}

		if (localBuf==0)
		{
			LOG0("IO/IODicom::Read: Invalid local image buffer.");			
			return false;
		}

		// copy buffer locally
		memcpy(localBuf, iBuf, oByteCount);

		// now convert the pixels
		long i=0;
		while (i<ix*iy)
		{
			*(oBuf++) = T(*(localBuf++));
			i++;
		}
	}
	

	if (GetSmoothFlag())
	{		
		PGCore::GaussianKernel<T, T> gausskernel(1.0f);
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*oImage, sImage);	
		*oImage = sImage;
		LOG0("\tIODicom::Smooth: Image DONE.");			
	}

	//LOG0("IO/IODicom::Read: Image DONE.");			
	//fclose(fptr);
	return true;
}



template <class T>
bool IODicom<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// write meta data first
/*	if (!iMetaDataObject) {
		LOG0("IO/IODicom::Write: metadata object null");			
		return false;
	}
*/
	if (!WriteMetaData(iMetaDataObject, iParams)) {
		LOG0("IO/IODicom::Write: failed to read metadata.");			
		return false;
	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IO/IODicom::Write: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IODicom::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &oFileName = iPaths[0];
	if (oFileName.empty())
	{
		LOG0("IO/IODicom::Write: Invalid input filename.");			
		return false;
	}

	if (!iDataObject) 
	{
		LOG0("IO/IODicom::Write: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<T> *iImage = (static_cast<PGCore::Image < T > *>(iDataObject));
	if (!iImage) 
	{
		LOG0("IO/IODicom::Write: Invalid input image.");			
		return false;
	}

	long ioRows=0, ioCols=0;
	iImage->GetDimensions((long &)ioRows, (long &)ioCols);
	if (ioRows*ioCols <=0) 
	{	
		LOG0("IO/IODicom::Write: Invalid image dimensions.");			
		return false;
	}

	PGCore::GaussianKernel<T, T> gausskernel(1.0f);
	if (GetSmoothFlag())
	{
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*iImage, sImage);	
		*iImage = sImage;		
		LOG1("IO/IODicom::Smooth Image [%s] DONE.", oFileName.c_str());			
	}

	LOG1("IO/IODicom::Write: Image [%s] DONE.", oFileName.c_str());			

	T* iBuf = iImage->GetBuffer();
	if (!iBuf)
	{
		LOG0("IO/IODicom::Write: Invalid image buffer.");			
		return false;
	}


	FILE *fptr = fopen (oFileName.c_str(), "wb");
	if (!fptr) {			
		LOG1("IO/IODicom::Write: failed to open file %s to write.", oFileName.c_str());			
		return false;
	}

	try 
	{		
		long rv = fwrite((const void *)iBuf, sizeof(T), ioRows*ioCols, fptr);
		if (rv!= ioRows*ioCols)
		{
			fclose(fptr);
			LOG1("IO/IODicom::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
			return false;
		}

	} 
	catch (...)
	{	
		fclose(fptr);
		LOG1("IO/IODicom::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
		return false;
	}
	
	fclose(fptr);
	return true;
}

// protectetd
template <class T>
bool IODicom<T>::WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams)
{
	return false;
}


template <class T>
bool IODicom<T>::GetAttributes(PGCore::MetaData<T>& oMetaDataObject, const std::string &iFilePath)
{
#ifdef _DEBUG
	LOG0("{ IODicom::GetAttributes");			
#endif

	if (iFilePath.empty())
	{
		LOG0("IO/IODicom::GetAttributes Empty filename");			
		return false;
	}

#ifdef _PG_GDDCM_


#else
	OFCondition status = m_fileformat.loadFile(iFilePath.c_str());
	if (!status.good())
	{
		LOG2("IO/IODicom::GetAttributes: Cannot read dicom file %s: (%s).", iFilePath.c_str(), status.text());			
		return false;
	}
	
#ifdef _DEBUG
	LOG2("\tIODicom::GetAttributes: Successfully read dicom file %s: (%s).", iFilePath.c_str(), status.text());
#endif

	// read the meta file here
	//LOG1("IO/IOBase::ReadMetaData: File: %s", iMetaFileName.c_str());	
	
	DcmDataset *dataset = m_fileformat.getDataset();
	if (dataset==NULL)
	{
		return false;
	}

	// patient name
	OFString patientsName;
	bool res = dataset->findAndGetOFString(DCM_PatientsName, patientsName).good();
	if (!res)
	{
		LOG0("IO/IODicom::Read: Cannot fetch patient name");			
		return false;		
	}
#ifdef _DEBUG
	LOG1("\tIODicom::Read: Successfully read PatientName: (%s).", patientsName.c_str());	
#endif

	// modality
	OFString modalityStr;
	res = dataset->findAndGetOFString(DCM_Modality, modalityStr).good();	
	if (!res)
	{
		LOG0("IO/IODicom::Read: Cannot fetch modality");			
		return false;		
	}
	PGCore::ePGModality modality = 	PGCore::MetaData<T>::StrToModality(modalityStr.c_str());
	oMetaDataObject.SetModality(modality);	
#ifdef _DEBUG
	LOG1("\tIODicom::Read: Successfully read Modality: (%s).", modalityStr.c_str());	
#endif
		
	

	// SOP Class UID	
	OFString sopClassUIDStr;
	DcmMetaInfo *metaInfo = m_fileformat.getMetaInfo(); 
	if (metaInfo)
	{
		metaInfo->findAndGetOFString(DCM_MediaStorageSOPClassUID, sopClassUIDStr);	
		PGCore::ePGSOPClass sopClass = 	PGCore::MetaData<T>::StrToSOPClass(sopClassUIDStr.c_str());
		oMetaDataObject.SetSOPClass(sopClass);		


		// DCM_TransferSyntaxUID
		OFString transferSyntaxStr;
		res = metaInfo->findAndGetOFString(DCM_TransferSyntaxUID, transferSyntaxStr).good();	
		if (!res)
		{
			LOG0("IO/IODicom::Read:Warning: Cannot fetch TransferSyntaxUID");			
		} else
		{

			if (!strcmp(kPgTransferSyntaxUIDRawImplicitVRLittleEndian, transferSyntaxStr.c_str()))
			{

				oMetaDataObject.SetMSBFirst(0);

			} else if (!strcmp(kPgTransferSyntaxUIDRawExplicitVRLittle, transferSyntaxStr.c_str()))
			{

				oMetaDataObject.SetMSBFirst(0);

			} else if (!strcmp(kPgTransferSyntaxUIDRawExplicitVRLittle, transferSyntaxStr.c_str()))
			{

				oMetaDataObject.SetMSBFirst(1);

			} else if (!strcmp(kPgTransferSyntaxUIDLossLessRLE, transferSyntaxStr.c_str()))
			{
				LOG0("IO/IODicom::Read: Cannot read RLE compressed images yet");			
				return false;

			} else
			{
				std::string tstr(transferSyntaxStr.c_str());
				std::string filterStr(kPgTransferSyntaxUIDJPEG);
				PGCore::ToUpper(tstr);
				PGCore::ToUpper(filterStr);

				size_t found=tstr.find(filterStr);
				bool isJPEG = (found!=string::npos);
				if (isJPEG)
				{
					LOG0("IO/IODicom::Read: Cannot read JPEG images yet");			
					return false;
				} 
			} 		
		}
	}

	DcmItem *ditem = OFstatic_cast(DcmItem *, dataset);
	if (!ditem)
	{
		LOG0("IO/IODicom::Read: Cannot fetch ditem");			
		return false;		
	}
	
	// Samples Per Pixel 
	Uint16 samplesPerPixel = 0;
	res = ditem->findAndGetUint16(DCM_SamplesPerPixel, samplesPerPixel).good();	
	if (!res)
	{
		LOG0("IO/IODicom::Read: Warning: Cannot fetch SamplesPerPixel");					
	} else if (samplesPerPixel>1)
	{
		LOG1("IO/IODicom::Read: Cannot read SamplesPerPixel: %d", samplesPerPixel);					
		return false;
	}

	oMetaDataObject.SetSamplesPerPixel(samplesPerPixel);
	

	// DCM_PhotometricInterpretation
	OFString photoInterpretationStr;
	res = metaInfo->findAndGetOFString(DCM_PhotometricInterpretation, photoInterpretationStr).good();	
	if (!res)
	{
		LOG0("IO/IODicom::Read:Warning Cannot fetch PhotometricInterpretation");			

	} else if (strcmp(kPgPhotometricRepresentationMChrome2, photoInterpretationStr.c_str()))
	{
		LOG1("IO/IODicom::Read: Cannot read PhotometricInterpretation %s", photoInterpretationStr.c_str());			
		return false;
	}


	// size	
	Uint16 imageRows = 0;
    Uint16 imageColumns = 0;
    res = ditem->findAndGetUint16(DCM_Rows, imageRows).good();
	if (!res)
	{
		LOG0("IO/IODicom::Read: Cannot fetch rows");			
		return false;		
	}
    res = ditem->findAndGetUint16(DCM_Columns, imageColumns).good();
	if (!res)
	{
		LOG0("IO/IODicom::Read: Cannot fetch columns");			
		return false;		
	}   
	oMetaDataObject.SetSize(PGMath::Vector3D<int>(imageColumns, imageRows, 1));
#ifdef _DEBUG
	LOG2("\tIODicom::Read: Size: Rows: %d, Columns: %d", imageRows, imageColumns);			
#endif

	// number of bits
	Uint16 imageBitsAllocated = 0;
	res = ditem->findAndGetUint16(DCM_BitsAllocated, imageBitsAllocated).good();
	if (!res)
	{
		LOG0("IO/IODicom::Read: Cannot fetch bits allocated");			
		return false;		
	} 
	oMetaDataObject.SetNumberOfBits(imageBitsAllocated);	
#ifdef _DEBUG
	LOG1("\tIODicom::Read: bits allocated: %d", imageBitsAllocated);			
#endif

	// spacing
	Float64 sx=1, sy=1;
	DcmElement *elem = 0;
	OFCondition	cond = dataset->findAndGetElement(DCM_PixelSpacing, elem);
	if(!cond.good()) 
	{
		LOG1("IO/IODicom::Read:Warning Cannot fetch pixel spacing (%s)", cond.text());						
	} else
	{
		cond = elem->getFloat64(sx, 0);
		if(!cond.good())
		{ 
			LOG1("IO/IODicom::Read:Warning Cannot fetch pixel X spacing (%s)", cond.text());					
		}
		cond = elem->getFloat64(sy, 1);
		if(!cond.good())
		{ 
			LOG1("IO/IODicom::Read:Warning Cannot fetch pixel Y spacing (%s)", cond.text());			
		}
	}

	if (sx!=sy)
	{
		LOG2("IO/IODicom::Read:Warning: X/Y spacings (%3.2f, %3.2f) mismatch. Ignoring anisotrpy", sx, sy);			
		sy = sx;
	}

#ifdef _DEBUG
	LOG2("\tIODicom::Read: pixel X and Y spacings (%3.5f %3.5f)",sx, sy);			
#endif

	oMetaDataObject.SetSpacing(PGMath::Vector3D<float>(sx, sy, 1));		

	// STUDY UID	
	OFString studyUIDStr;
	res = dataset->findAndGetOFString(DCM_StudyInstanceUID, studyUIDStr).good();	
	if (!res)
	{
		LOG0("IO/IODicom::Read:Warning Cannot fetch StudyUID");			
	} else
	{
		oMetaDataObject.SetStudyUID(std::string(studyUIDStr.c_str()));
	}

	// series UID
	OFString seriesUIDStr;
	res = dataset->findAndGetOFString(DCM_SeriesInstanceUID, seriesUIDStr).good();	
	if (!res)
	{
		LOG0("IO/IODicom::Read:Warning Cannot fetch seriesUID");			
	} else
	{
		oMetaDataObject.SetSeriesUID(std::string(seriesUIDStr.c_str()));
	}


	// orientation		
	Float64 rowOr[3] = { 1.0f, 0.0f, 0.0f }, colOr[3] = { 0.0f, 1.0f, 0.0f };
	std::vector<PGMath::Vector3D<float> > orXs;
	std::vector<PGMath::Vector3D<float> > orYs;
	cond = dataset->findAndGetElement(DCM_ImageOrientationPatient, elem);
	if(!cond.good()) 
	{
		LOG1("IO/IODicom::Read:Warning Cannot fetch DCM_ImageOrientationPatient (%s)", cond.text());					
	} else
	{		
		elem->getFloat64(rowOr[0], 0);
		elem->getFloat64(rowOr[1], 1);
		elem->getFloat64(rowOr[2], 2);		
		elem->getFloat64(colOr[0], 3);
		elem->getFloat64(colOr[1], 4);
		elem->getFloat64(colOr[2], 5);			
	}

	orXs.push_back(PGMath::Vector3D<float>(rowOr[0], rowOr[1], rowOr[2]));
	oMetaDataObject.SetImageOrientationsPatientX(orXs);
#ifdef _DEBUG
	LOG3("\tIODicom::Read: Image Row vector orientation (%3.5f %3.5f %3.5f)", rowOr[0], rowOr[1], rowOr[2]);			
#endif
	orYs.push_back(PGMath::Vector3D<float>(colOr[0], colOr[1], colOr[2]));	
	oMetaDataObject.SetImageOrientationsPatientY(orYs);
#ifdef _DEBUG
	LOG3("\tIODicom::Read: Image Col vector orientation (%3.5f %3.5f %3.5f)", colOr[0], colOr[1], colOr[2]);			
#endif

	// position	
	Float64 imgPos[3] = { 0.0f, 0.0f, 0.0f };
	std::vector<PGMath::Vector3D<float> > positions;
	cond = dataset->findAndGetElement(DCM_ImagePositionPatient, elem);
	if(!cond.good()) 
	{
		LOG1("IO/IODicom::Read:Warning Cannot fetch DCM_ImagePositionPatient (%s)", cond.text());					
	} else
	{		
		elem->getFloat64(imgPos[0], 0);
		elem->getFloat64(imgPos[1], 1);
		elem->getFloat64(imgPos[2], 2);				
	}
	positions.push_back(PGMath::Vector3D<float>(imgPos[0], imgPos[1], imgPos[2]));
	oMetaDataObject.SetImagePositionsPatient(positions);
#ifdef _DEBUG
	LOG3("\tIODicom::Read: Image Position (%3.5f %3.5f %3.5f)", imgPos[0], imgPos[1], imgPos[2]);			
#endif

	// slope / intercept
	if (modality==PGCore::kPGModalityCT || modality==PGCore::kPGModalityPT)
	{
		Float64 slope = -999.0f;
		cond = dataset->findAndGetElement(DCM_RescaleSlope, elem);
		if(!cond.good())
		{
			LOG1("IO/IODicom::Read:Warning Cannot fetch DCM_RescaleSlope (%s)", cond.text());								
		} else
		{
			cond = elem->getFloat64(slope, 0);
			if(cond.good())
				//if (dataset->findAndGetFloat64(DCM_RescaleIntercept, slope) == EC_Normal)
			{ 
				oMetaDataObject.SetSlope(slope);
				//LOG1("IO/IODicom::Read: Cannot fetch pixel Y spacing (%s)", cond.text());			
			}
		}

		Float64 intercept = -999.0f;
		//dataset->findAndGetFloat64(DCM_RescaleIntercept, intercept);
		cond = dataset->findAndGetElement(DCM_RescaleIntercept, elem);
		if(!cond.good())
		{
			LOG1("IO/IODicom::Read:Warning Cannot fetch DCM_RescaleIntercept (%s)", cond.text());								
		} else
		{
			cond = elem->getFloat64(intercept, 0);
			if(cond.good())
				//if (dataset->findAndGetFloat64(DCM_RescaleIntercept, intercept) == EC_Normal)
			{ 
				oMetaDataObject.SetIntercept(intercept);
			}
		}
	}
	/*
    Uint16 imageBytesAllocated = 0;
    Uint16 imagePlanarConfiguration = 0;
	Uint16 imageSamplesPerPixel = 0;
   
    Sint32 imageFrames = 1;
    Uint16 imageBitsAllocated = 0;

	
	if (result.good()) result = ditem->findAndGetUint16(DCM_BitsAllocated, imageBitsAllocated);
	res = ditem->findAndGetUint16(DCM_SamplesPerPixel, imageSamplesPerPixel);

	// slope / intercept	

	// high bit

	// planar configuration

	// endianness
	oMetaData->SetMSBFirst(getValueI(iStr,std::string(kPGTagMSBFirst)));
	*/
#endif

#ifdef _DEBUG
	LOG0("} IODicom::GetAttributes");			
#endif

	return true;

}


template <class T>
bool IODicom<T>::ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams)
{
	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IODicom::ReadMetaData: Invalid input filename vector.");			
		return false;
	}
	const std::string &iFileName = iPaths[0];
	if (iFileName.empty())
	{
		LOG0("IO/IODicom::ReadMetaData: Invalid input filename.");			
		return false;
	}
			
	//this is where to read the image in
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IO/IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	return GetAttributes(*oMetaData, iFileName);
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class IODicom<short>; 
	template class IODicom<unsigned char>; 
#endif

};


