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
// Filename:	IOPpm.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOPpm.h: interface for the IOPpm class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#include "IO/IOPpm.h"
#include "Core/PixelBase.h"
#include "Core/Image.h"
#include "Core/GaussianKernel.h"

using namespace PGCore;

namespace PGIO
{

	
template <class T>
IOPpm<T>::IOPpm(void) 
{
};	
		
template <class T>
IOPpm<T>::~IOPpm(void) 
{
};	

template <class T>
void IOPpm<T>::StartExecution(void *iParams) 
{
	assert(0 && "Not implemented on purpose");
};

template <class T>
int IOPpm<T>::NotifyFinished() 
{ 
	assert(0 && "Not implemented on purpose");
	return false; 
};

template <class T>
int IOPpm<T>::NotifyError(const char *iMsg=0) 
{ 
	assert(0 && "Not implemented on purpose");
	return false; 
};


template <>
inline bool IOPpm<unsigned char>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	return false;
}


template <>
inline bool IOPpm<short>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	return false;
}

template <>
inline bool IOPpm<unsigned short>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	return false;
}

template <>
inline bool IOPpm<PgUChar_RGBAPixel>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// write meta data first
//	if (!iMetaDataObject) {
//		LOG0("IO/IOPpm::Write: metadata object null");			
//		return false;
//	}
//
//	if (!WriteMetaData(iMetaDataObject, iParams)) {
//		LOG0("IO/IOPpm::Write: failed to read metadata.");			
//		return false;
//	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IO/IOPpm::Write: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IODicom::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &oFileName = iPaths[0];	
	//const std::string &oFileName = iParams.Source();
	if (oFileName.empty())
	{
		LOG0("IO/IOPpm::Write: Invalid input filename.");			
		return false;
	}

	if (!iDataObject) 
	{
		LOG0("IO/IOPpm::Write: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<PgUChar_RGBAPixel> *iImage = (static_cast<PGCore::Image <PgUChar_RGBAPixel > *>(iDataObject));
	if (!iImage) 
	{
		LOG0("IO/IOPpm::Write: Invalid input image.");			
		return false;
	}

	/*
		PGCore::GaussianKernel<T, T> gausskernel(1.0f);

	if (GetSmoothFlag())
	{		
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*iImage, sImage);	
		*iImage = sImage;
		LOG1("IO/IOPpm::Smooth Image [%s] DONE.", oFileName.c_str());			
	}*/

	long ioRows=0, ioCols=0;
	iImage->GetDimensions((long &)ioRows, (long &)ioCols);
	if (ioRows*ioCols <=0) 
	{	
		LOG0("IO/IOPpm::Write: Invalid image dimensions.");			
		return false;
	}

	PGCore::PixelBase<PgUChar_RGBAPixel> tPixel;
	PgUChar_RGBAPixel rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
	iImage->GetDataRange(&rangeMin, &rangeMax);	
	double dataRange[3] = 
	{
		(double)rangeMax.X() - (double)rangeMin.X(),
		(double)rangeMax.Y() - (double)rangeMin.Y(),
		(double)rangeMax.Z() - (double)rangeMin.Z()
	};
	double rangeMind[3] = { (double)rangeMin.X(), (double)rangeMin.Y(), (double)rangeMin.Z()};
	double rangeMaxd[3] = { (double)rangeMax.X(), (double)rangeMax.Y(), (double)rangeMax.Z()};


	if (dataRange[0]<=0.0 || dataRange[1]<=0.0 || dataRange[2]<=0.0) {
		LOG0("IO/IOPpm::Write: Invalid image data range. Using whole pixel range..");			
		//return false;
		//dataRange = (float) (tPixel.GetMaxValue() - tPixel.GetMinValue());
		//dataRange = 1.0f;
		/*if (dataRange<=0)
		{
			LOG0("IO/IOPpm::Write: Invalid image data range and Invalid whole pixel range. Using 1.0...");			
			dataRange = 1.0f;
		}*/
	}

	PgUChar_RGBAPixel* iBuf = iImage->GetBuffer();
	if (!iBuf)
	{
		LOG0("IO/IOPpm::Write: Invalid image buffer.");			
		return false;
	}


	FILE *fptr = fopen (oFileName.c_str(), "wb");
	if (!fptr) {			
		LOG1("IO/IOPpm::Write: failed to open file %s to write.", oFileName.c_str());			
		return false;
	}

	try 
	{		
		const unsigned char kPgMaxNormalizedPixelValue = 255;
		fprintf(fptr, "P6 %ld %ld %d\n", ioCols, ioRows, (int)kPgMaxNormalizedPixelValue);

		long iRowIndex = 0, iColIndex = 0;
		double newVal[3]= {0.0f, 0.0f, 0.0f};
		for (iRowIndex=0; iRowIndex < ioRows; iRowIndex++) 
		{
			for (iColIndex=0; iColIndex < ioCols; iColIndex++) 
			{
				PgUChar_RGBAPixel iVal = iImage->GetValue(iRowIndex, iColIndex);
				newVal[0] = (double)(iVal.X());											
				newVal[1] = (double)(iVal.Y());											
				newVal[2] = (double)(iVal.Z());											

				for (int k=0; k<3; k++)
				{
					if (dataRange[k]>0.0f)
					{
						newVal[k] = floor(255.0f*((newVal[k] - (double)rangeMind[k])/(double) (dataRange[k])));	
					} else 
					{
						//newVal = 0;
						newVal[k] = (newVal[k]<0) ? 0 : newVal[k];
						newVal[k] = (newVal[k]>255) ? 255 : newVal[k];
					}
					
					//fprintf( fout, "%d ", ROUND (newVal ) );			
					unsigned char nValC = (unsigned char)((unsigned int)newVal[k]);
					fwrite((void *)(&nValC), sizeof(unsigned char), 1, fptr);
				}

				
			}
		}

	} 
	catch (...)
	{	
		fclose(fptr);
		LOG1("IO/IOPpm::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
		return false;
	}
	
	LOG1("IO/IOPpm::Write: Image [%s] DONE.", oFileName.c_str());			
	fclose(fptr);
	return true;
}


template <>
inline bool IOPpm<PgUShort_RGBAPixel>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// write meta data first
//	if (!iMetaDataObject) {
//		LOG0("IO/IOPpm::Write: metadata object null");			
//		return false;
//	}
//
//	if (!WriteMetaData(iMetaDataObject, iParams)) {
//		LOG0("IO/IOPpm::Write: failed to read metadata.");			
//		return false;
//	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IO/IOPpm::Write: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IODicom::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &oFileName = iPaths[0];	
	//const std::string &oFileName = iParams.Source();
	if (oFileName.empty())
	{
		LOG0("IO/IOPpm::Write: Invalid input filename.");			
		return false;
	}

	if (!iDataObject) 
	{
		LOG0("IO/IOPpm::Write: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<PgUShort_RGBAPixel> *iImage = (static_cast<PGCore::Image <PgUShort_RGBAPixel > *>(iDataObject));
	if (!iImage) 
	{
		LOG0("IO/IOPpm::Write: Invalid input image.");			
		return false;
	}

	/*
		PGCore::GaussianKernel<T, T> gausskernel(1.0f);

	if (GetSmoothFlag())
	{		
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*iImage, sImage);	
		*iImage = sImage;
		LOG1("IO/IOPpm::Smooth Image [%s] DONE.", oFileName.c_str());			
	}*/

	long ioRows=0, ioCols=0;
	iImage->GetDimensions((long &)ioRows, (long &)ioCols);
	if (ioRows*ioCols <=0) 
	{	
		LOG0("IO/IOPpm::Write: Invalid image dimensions.");			
		return false;
	}

	PGCore::PixelBase<PgUShort_RGBAPixel> tPixel;
	PgUShort_RGBAPixel rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
	iImage->GetDataRange(&rangeMin, &rangeMax);	
	double dataRange[3] = 
	{
		(double)rangeMax.X() - (double)rangeMin.X(),
		(double)rangeMax.Y() - (double)rangeMin.Y(),
		(double)rangeMax.Z() - (double)rangeMin.Z()
	};
	double rangeMind[3] = { (double)rangeMin.X(), (double)rangeMin.Y(), (double)rangeMin.Z()};
	double rangeMaxd[3] = { (double)rangeMax.X(), (double)rangeMax.Y(), (double)rangeMax.Z()};


	if (dataRange[0]<=0.0 || dataRange[1]<=0.0 || dataRange[2]<=0.0) {
		LOG0("IO/IOPpm::Write: Invalid image data range. Using whole pixel range..");			
		//return false;
		//dataRange = (float) (tPixel.GetMaxValue() - tPixel.GetMinValue());
		//dataRange = 1.0f;
		/*if (dataRange<=0)
		{
			LOG0("IO/IOPpm::Write: Invalid image data range and Invalid whole pixel range. Using 1.0...");			
			dataRange = 1.0f;
		}*/
	}

	PgUShort_RGBAPixel* iBuf = iImage->GetBuffer();
	if (!iBuf)
	{
		LOG0("IO/IOPpm::Write: Invalid image buffer.");			
		return false;
	}


	FILE *fptr = fopen (oFileName.c_str(), "wb");
	if (!fptr) {			
		LOG1("IO/IOPpm::Write: failed to open file %s to write.", oFileName.c_str());			
		return false;
	}

	try 
	{		
		const unsigned char kPgMaxNormalizedPixelValue = 255;
		fprintf(fptr, "P6 %ld %ld %d\n", ioCols, ioRows, (int)kPgMaxNormalizedPixelValue);

		long iRowIndex = 0, iColIndex = 0;
		double newVal[3]= {0.0f, 0.0f, 0.0f};
		for (iRowIndex=0; iRowIndex < ioRows; iRowIndex++) 
		{
			for (iColIndex=0; iColIndex < ioCols; iColIndex++) 
			{
				PgUShort_RGBAPixel iVal = iImage->GetValue(iRowIndex, iColIndex);
				newVal[0] = (double)(iVal.X());											
				newVal[1] = (double)(iVal.Y());											
				newVal[2] = (double)(iVal.Z());											

				for (int k=0; k<3; k++)
				{
					if (dataRange[k]>0.0f)
					{
						newVal[k] = floor(255.0f*((newVal[k] - (double)rangeMind[k])/(double) (dataRange[k])));	
					} else 
					{
						//newVal = 0;
						newVal[k] = (newVal[k]<0) ? 0 : newVal[k];
						newVal[k] = (newVal[k]>255) ? 255 : newVal[k];
					}
					
					//fprintf( fout, "%d ", ROUND (newVal ) );			
					unsigned char nValC = (unsigned char)((unsigned int)newVal[k]);
					fwrite((void *)(&nValC), sizeof(unsigned char), 1, fptr);
				}

				
			}
		}

	} 
	catch (...)
	{	
		fclose(fptr);
		LOG1("IO/IOPpm::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
		return false;
	}
	
	LOG1("IO/IOPpm::Write: Image [%s] DONE.", oFileName.c_str());			
	fclose(fptr);
	return true;
}

template <>
inline bool IOPpm<unsigned char>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	return false;
}

template <>
inline bool IOPpm<short>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject) {
		LOG0("IO/IOPpm::Read: metadata object null");			
		return false;
	}

	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IO/IOPpm::Read: failed to read metadata.");			
		return false;
	}

	// now read actual data
	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IO/IOPpm::Read: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IODicom::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &iFileName = iPaths[0];	
	//const std::string &iFileName = iParams.Source();
	if (iFileName.empty())
	{
		LOG0("IO/IOPpm::Read: Invalid input filename.");			
		return false;
	}


	if (!oDataObject) 
	{
		LOG0("IO/IORaw::Read: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<short> *oImage = (static_cast<PGCore::Image < short > *>(oDataObject));
	if (!oImage) 
	{
		LOG0("IO/IORaw::Read: Invalid input image.");			
		return false;
	}
	

	FILE *fptr = fopen (iFileName.c_str(), "rb");
	if (!fptr) {
		oImage->Clear();		
		LOG1("IO/IOPpm::Read: failed to open file %s to read.", iFileName.c_str());			
		return false;
	}

	int maxPixelValue = 255;
	long ioRows = 0, ioCols = 0;		
	char imgTag[5] = {0};
	fscanf(fptr, "%s", imgTag);
	if (abs(strncmp(imgTag, "P6", 2))) 
	{
		LOG1("IO/IOPgm::Read: not a raw PPM file: %s.", imgTag);			
		fclose(fptr);
		return false;
	}
	
	// ignore comments
	char lineStr[256] = {'#'};
	fscanf (fptr, "%s", lineStr);
	while(lineStr[0]=='#')
	{
		fgets (lineStr, 255, fptr);
        fscanf (fptr, "%s", lineStr);		
	}
	
	// read header
	sscanf (lineStr, "%ld", &ioCols);
	fscanf (fptr, "%ld", &ioRows);
	fscanf (fptr, "%d", &maxPixelValue);
	
	if (ioRows*ioCols <=0) 
	{	
		LOG2("IO/IOPpm::Read: Invalid image dimensions: [%d] [%d].", ioRows, ioCols);			
		return false;
	}

	oImage->SetDimensions(ioRows, ioCols);	
	short* oBuf = oImage->GetBuffer();
	if (!oBuf)
	{
		LOG0("IO/IOPpm::Read: Invalid image buffer.");			
		return false;
	}

	unsigned char *imgBuf = new unsigned char[3*ioRows*ioCols];
	if (!imgBuf) 
	{
		LOG0("IO/IOPpm::Read: Invalid temporary image buffer.");	
		return false;
	}

	try 
	{
		
		fread((void *)imgBuf, sizeof(unsigned char), 3*ioRows*ioCols, fptr);

	} 
	catch (...)
	{
		oImage->Clear();		
		fclose(fptr);
		LOG1("IO/IOPpm::Read: failed to read file %s. Possible overrun.", iFileName.c_str());			
		return false;
	}
	
	
/*	if (GetSmoothFlag())
	{		
		PGCore::GaussianKernel<double, short> gausskernel(1.0f);
		PGCore::Image<short> sImage;
		gausskernel.Convolve(*oImage, sImage);	
		*oImage = sImage;
		LOG1("IO/IOPpm::Read: Smooth image [%s] DONE.", iFileName.c_str());			
	}
*/
	//LOG1("IO/IOPpm::Read: Image [%s] DONE.", iFileName.c_str());			

	fclose(fptr);


	

	// copy from temp
	long imgIter=0;
	while(imgIter<(ioRows*ioCols))
	{
		unsigned char iVal = 
			unsigned char(0.33f*float(*(imgBuf+3*imgIter) + *(imgBuf+3*imgIter+1) + *(imgBuf+3*imgIter+2)));	
		*(oBuf+imgIter) = (short)(iVal);
		imgIter++;
	}

	if (imgBuf) 
	{
		delete [] imgBuf;
		imgBuf = 0;
	}

	return true;
}

template <>
inline bool IOPpm<PgUChar_RGBAPixel>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject) {
		LOG0("IO/IOPpm::Read: metadata object null");			
		return false;
	}

	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IO/IOPpm::Read: failed to read metadata.");			
		return false;
	}

	// now read actual data
	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IO/IOPpm::Read: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IODicom::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &iFileName = iPaths[0];	
	//const std::string &iFileName = iParams.Source();
	if (iFileName.empty())
	{
		LOG0("IO/IOPpm::Read: Invalid input filename.");			
		return false;
	}


	if (!oDataObject) 
	{
		LOG0("IO/IORaw::Read: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<PgUChar_RGBAPixel> *oImage = (static_cast<PGCore::Image < PgUChar_RGBAPixel > *>(oDataObject));
	if (!oImage) 
	{
		LOG0("IO/IORaw::Read: Invalid input image.");			
		return false;
	}
	

	FILE *fptr = fopen (iFileName.c_str(), "rb");
	if (!fptr) {
		oImage->Clear();		
		LOG1("IO/IOPpm::Read: failed to open file %s to read.", iFileName.c_str());			
		return false;
	}

	int maxPixelValue = 255;
	long ioRows = 0, ioCols = 0;		
	char imgTag[5] = {0};
	fscanf(fptr, "%s", imgTag);
	if (abs(strncmp(imgTag, "P6", 2))) 
	{
		LOG1("IO/IOPpm::Read: not a raw PPM file: %s.", imgTag);			
		fclose(fptr);
		return false;
	}
	
	// ignore comments
	char lineStr[256] = {'#'};
	fscanf (fptr, "%s", lineStr);
	while(lineStr[0]=='#')
	{
		fgets (lineStr, 255, fptr);
        fscanf (fptr, "%s", lineStr);		
	}
	
	// read header
	sscanf (lineStr, "%ld", &ioCols);
	fscanf (fptr, "%ld", &ioRows);
	fscanf (fptr, "%d", &maxPixelValue);
	
	if (ioRows*ioCols <=0) 
	{	
		LOG2("IO/IOPpm::Read: Invalid image dimensions: [%d] [%d].", ioRows, ioCols);			
		return false;
	}

	oImage->SetDimensions(ioRows, ioCols);	
	PgUChar_RGBAPixel* oBuf = oImage->GetBuffer();
	if (!oBuf)
	{
		LOG0("IO/IOPpm::Read: Invalid image buffer.");			
		return false;
	}

	unsigned char *imgBuf = new unsigned char[3*ioRows*ioCols];
	if (!imgBuf) 
	{
		LOG0("IO/IOPpm::Read: Invalid temporary image buffer.");	
		return false;
	}

	try 
	{
		
		fread((void *)imgBuf, sizeof(unsigned char), 3*ioRows*ioCols, fptr);

	} 
	catch (...)
	{
		oImage->Clear();		
		fclose(fptr);
		LOG1("IO/IOPpm::Read: failed to read file %s. Possible overrun.", iFileName.c_str());			
		return false;
	}
	
	
/*	if (GetSmoothFlag())
	{		
		PGCore::GaussianKernel<double, short> gausskernel(1.0f);
		PGCore::Image<short> sImage;
		gausskernel.Convolve(*oImage, sImage);	
		*oImage = sImage;
		LOG1("IO/IOPpm::Read: Smooth image [%s] DONE.", iFileName.c_str());			
	}
*/
	//LOG1("IO/IOPpm::Read: Image [%s] DONE.", iFileName.c_str());			

	fclose(fptr);


	
	// BGR ??
	// copy from temp
	long imgIter=0;
	while(imgIter<(ioRows*ioCols))
	{
		PgUChar_RGBAPixel iVal(*(imgBuf+3*imgIter+1), *(imgBuf+3*imgIter+2), *(imgBuf+3*imgIter));
		//unsigned char iVal = 
		//	unsigned char(0.33f*float(*(imgBuf+3*imgIter) + *(imgBuf+3*imgIter+1) + *(imgBuf+3*imgIter+2)));	
		*(oBuf+imgIter) = iVal;
		imgIter++;
	}

	if (imgBuf) 
	{
		delete [] imgBuf;
		imgBuf = 0;
	}

	return true;
}


template <>
inline bool IOPpm<PgUShort_RGBAPixel>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	return false;
}

template <class T>
inline bool IOPpm<T>::WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams)
{
	return false;
}

template <class T>
inline bool IOPpm<T>::ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams)
{
	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IO/IOPpm::ReadMetaData: Warning: Invalid input filename vector.");			
		//return false;
	}
	const std::string &iFileName = iPaths[0];
	if (iFileName.empty())
	{
		LOG0("IO/IOPpm::ReadMetaData: Warning: Invalid input filename.");			
		//return false;
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



template <class T>
inline bool IOPpm<T>::GetAttributes(PGCore::MetaData<T>& oMetaDataObject, const std::string &iFilePath)
{
	// read the 

#ifdef _DEBUG
	LOG0("{ IOPpm::GetAttributes");			
#endif

	if (iFilePath.empty())
	{
		LOG0("IO/IOPpm::GetAttributes Warning: Empty filename");			
		//return false;
	}

	// fill in dummy values
	// patient name
	oMetaDataObject.SetPatientName("Anonymous");
	PGCore::ePGModality modality = PGCore::MetaData<T>::StrToModality("Unknown");
	oMetaDataObject.SetModality(modality);	
	
	// size		
	oMetaDataObject.SetSize(PGMath::Vector3D<int>(0, 0, 1));

	// number of bits	
	oMetaDataObject.SetNumberOfBits(8);	

	// spacing
	oMetaDataObject.SetSpacing(PGMath::Vector3D<float>(1, 1, 1));		

	// orientation	
	float rowOr[3] = { 1.0f, 0.0f, 0.0f }, colOr[3] = { 0.0f, 1.0f, 0.0f };	
	std::vector<PGMath::Vector3D<float> > orXs;
	orXs.push_back(PGMath::Vector3D<float>(rowOr[0], rowOr[1], rowOr[2]));
	oMetaDataObject.SetImageOrientationsPatientX(orXs);

	std::vector<PGMath::Vector3D<float> > orYs;
	orYs.push_back(PGMath::Vector3D<float>(colOr[0], colOr[1], colOr[2]));	
	oMetaDataObject.SetImageOrientationsPatientY(orYs);

	// position
	float imgPos[3] = { 0.0f, 0.0f, 0.0f };
	std::vector<PGMath::Vector3D<float> > positions;
	positions.push_back(PGMath::Vector3D<float>(imgPos[0], imgPos[1], imgPos[2]));
	oMetaDataObject.SetImagePositionsPatient(positions);

	// slope / intercept	
	{
		float slope = -1.0f;		
		{ 
			oMetaDataObject.SetSlope(slope);
			//LOG1("IO/IODicom::Read: Cannot fetch pixel Y spacing (%s)", cond.text());			
		}

		float intercept = 0.0f;
		{ 
			oMetaDataObject.SetIntercept(intercept);
		}
	}

#ifdef _DEBUG
	LOG0("} IOPpm::GetAttributes");			
#endif

	return true;

}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class IOPpm<short>; 
	template class IOPpm<unsigned char>; 
	template class IOPpm<PGMath::Point3D<unsigned short>>; 
	template class IOPpm<PGMath::Point3D<unsigned char>>; 
#endif


};



