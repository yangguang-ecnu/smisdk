// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	IOPgm.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOPgm.h: interface for the IOPgm class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#include "IO/IOPgm.h"
#include "Core/GaussianKernel.h"

namespace PGIO
{

	
template <class T>
IOPgm<T>::IOPgm(void) 
{
};	
		
template <class T>
IOPgm<T>::~IOPgm(void) 
{
};	

template <class T>
void IOPgm<T>::StartExecution(void *iParams) 
{
};

template <class T>
int IOPgm<T>::NotifyFinished() 
{ 
	return false; 
};

template <class T>
int IOPgm<T>::NotifyError(const char *iMsg=0) 
{ 
	return false; 
};


template <class T>
bool IOPgm<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// write meta data first
//	if (!iMetaDataObject) {
//		LOG0("IOPgm::Write: metadata object null");			
//		return false;
//	}
//
//	if (!WriteMetaData(iMetaDataObject, iParams)) {
//		LOG0("IOPgm::Write: failed to read metadata.");			
//		return false;
//	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IOPgm::Write: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IOPgm::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &oFileName = iPaths[0];	
	if (oFileName.empty())
	{
		LOG0("IOPgm::Write: Invalid input filename.");			
		return false;
	}

	if (!iDataObject) 
	{
		LOG0("IOPgm::Write: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<T> *iImage = (static_cast<PGCore::Image < T > *>(iDataObject));
	if (!iImage) 
	{
		LOG0("IOPgm::Write: Invalid input image.");			
		return false;
	}

	PGCore::GaussianKernel<T, T> gausskernel(1.0f);

	if (GetSmoothFlag())
	{		
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*iImage, sImage);	
		*iImage = sImage;
		LOG1("IOPGM::Smooth Image [%s] DONE.", oFileName.c_str());			
	}

	long ioRows=0, ioCols=0;
	iImage->GetDimensions((long &)ioRows, (long &)ioCols);
	if (ioRows*ioCols <=0) 
	{	
		LOG0("IOPgm::Write: Invalid image dimensions.");			
		return false;
	}

	PGCore::PixelBase<T> tPixel;
	T rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
	iImage->GetDataRange(&rangeMin, &rangeMax);	
	double dataRange = (double)rangeMax - (double)rangeMin;

	if (dataRange<=0.0) {
		LOG0("IOPgm::Write: Invalid image data range. Using whole pixel range..");			
		//return false;
		//dataRange = (float) (tPixel.GetMaxValue() - tPixel.GetMinValue());
		//dataRange = 1.0f;
		/*if (dataRange<=0)
		{
			LOG0("IOPgm::Write: Invalid image data range and Invalid whole pixel range. Using 1.0...");			
			dataRange = 1.0f;
		}*/
	}

	T* iBuf = iImage->GetBuffer();
	if (!iBuf)
	{
		LOG0("IOPgm::Write: Invalid image buffer.");			
		return false;
	}


	FILE *fptr = fopen (oFileName.c_str(), "wb");
	if (!fptr) {			
		LOG1("IOPgm::Write: failed to open file %s to write.", oFileName.c_str());			
		return false;
	}

	try 
	{		
		const unsigned char kPgMaxNormalizedPixelValue = 255;
		fprintf(fptr, "P5 %ld %ld %d\n", ioCols, ioRows, (int)kPgMaxNormalizedPixelValue);

		long iRowIndex = 0, iColIndex = 0;
		double newVal=0.0f;
		for (iRowIndex=0; iRowIndex < ioRows; iRowIndex++) {
			for (iColIndex=0; iColIndex < ioCols; iColIndex++) {
				newVal = (double)(iImage->GetValue(iRowIndex, iColIndex));											

				if (dataRange>0.0f)
				{
					newVal = floor(255.0f*((newVal - (double)rangeMin)/(double) (dataRange)));	
				} else 
				{
					//newVal = 0;
					newVal = (newVal<0) ? 0 : newVal;
					newVal = (newVal>255) ? 255 : newVal;
				}
				
				//fprintf( fout, "%d ", ROUND (newVal ) );			
				unsigned char nValC = (unsigned char)((unsigned int)newVal);
				fwrite((void *)(&nValC), sizeof(unsigned char), 1, fptr);
			}
		}

	} 
	catch (...)
	{	
		fclose(fptr);
		LOG1("IOPgm::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
		return false;
	}
	
	LOG1("IOPGM::Write: Image [%s] DONE.", oFileName.c_str());			
	fclose(fptr);
	return true;
}


template <class T>
bool IOPgm<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject) {
		LOG0("IOPgm::Read: metadata object null");			
		return false;
	}

	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IOPgm::Read: failed to read metadata.");			
		return false;
	}

	// now read actual data
	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IOPgm::Read: Expecting single file path.");			
		return false;
	}

	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IOPgm::Read: Invalid input filename vector.");			
		return false;
	}
	const std::string &iFileName = iPaths[0];
	if (iFileName.empty())
	{
		LOG0("IOPgm::Read: Invalid input filename.");			
		return false;
	}


	if (!oDataObject) 
	{
		LOG0("IORaw::Read: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<T> *oImage = (static_cast<PGCore::Image < T > *>(oDataObject));
	if (!oImage) 
	{
		LOG0("IORaw::Read: Invalid input image.");			
		return false;
	}

	FILE *fptr = fopen (iFileName.c_str(), "rb");
	if (!fptr) {
		oImage->Clear();		
		LOG1("IOPgm::Read: failed to open file %s to read.", iFileName.c_str());			
		return false;
	}

	int maxPixelValue = 255;
	long ioRows = 0, ioCols = 0;		
	char imgTag[5] = {0};
	fscanf(fptr, "%s", imgTag);
	if (abs(strncmp(imgTag, "P5", 2))) 
	{
		LOG1("IOPgm::Read: not a raw PGM file: %s.", imgTag);			
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
		LOG2("IOPgm::Read: Invalid image dimensions: [%d] [%d].", ioRows, ioCols);			
		return false;
	}

	oImage->SetDimensions(ioRows, ioCols);	
	T* oBuf = oImage->GetBuffer();
	if (!oBuf)
	{
		LOG0("IOPgm::Read: Invalid image buffer.");			
		return false;
	}

	unsigned char *imgBuf = new unsigned char[ioRows*ioCols];
	if (!imgBuf) 
	{
		LOG0("IOPgm::Read: Invalid temporary image buffer.");	
		return false;
	}

	try 
	{
		
		fread((void *)imgBuf, sizeof(unsigned char), ioRows*ioCols, fptr);

	} 
	catch (...)
	{
		oImage->Clear();		
		fclose(fptr);
		LOG1("IOPgm::Read: failed to read file %s. Possible overrun.", iFileName.c_str());			
		return false;
	}
	
	

	if (GetSmoothFlag())
	{		
		PGCore::GaussianKernel<T, T> gausskernel(1.0f);
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*oImage, sImage);	
		*oImage = sImage;
		LOG1("IOPGM::Read: Smooth image [%s] DONE.", iFileName.c_str());			
	}

	//LOG1("IOPGM::Read: Image [%s] DONE.", iFileName.c_str());			

	fclose(fptr);


	

	// copy from temp
	long imgIter=0;
	while(imgIter<(ioRows*ioCols))
	{
		unsigned char iVal = *(imgBuf+imgIter);	
		*(oBuf+imgIter) = (T)(iVal);
		imgIter++;
	}

	if (imgBuf) 
	{
		delete [] imgBuf;
		imgBuf = 0;
	}

	return true;
}

template <class T>
bool IOPgm<T>::WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams)
{
	return false;
}

template <class T>
bool IOPgm<T>::ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams)
{
	const std::vector<std::string>& iPaths = iParams.Source();
	if (iPaths.empty())
	{
		LOG0("IOPgm::ReadMetaData: Warning: Invalid input filename vector.");			
		//return false;
	}
	const std::string &iFileName = iPaths[0];
	if (iFileName.empty())
	{
		LOG0("IOPgm::ReadMetaData: Warning: Invalid input filename.");			
		//return false;
	}
			
	//this is where to read the image in
	PGCore::MetaData<T> *oMetaData = (static_cast<PGCore::MetaData< T > *>(oMetaDataObject));
	if (!oMetaData) 
	{
		LOG0("IOBase::ReadMetaData: Invalid output container for metadata.");			
		return false;
	}

	return GetAttributes(*oMetaData, iFileName);
}



template <class T>
bool IOPgm<T>::GetAttributes(PGCore::MetaData<T>& oMetaDataObject, const std::string &iFilePath)
{
	// read the 

#ifdef _DEBUG
	LOG0("{ IOPgm::GetAttributes");			
#endif

	if (iFilePath.empty())
	{
		LOG0("IOPgm::GetAttributes Warning: Empty filename");			
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
			//LOG1("IODicom::Read: Cannot fetch pixel Y spacing (%s)", cond.text());			
		}

		float intercept = 0.0f;
		{ 
			oMetaDataObject.SetIntercept(intercept);
		}
	}

#ifdef _DEBUG
	LOG0("} IOPgm::GetAttributes");			
#endif

	return true;

}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class IOPgm<short>; 
	template class IOPgm<float>; 
	template class IOPgm<unsigned char>; 
#endif

};



