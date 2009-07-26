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
// Filename:	IORaw.hpp
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IORaw.h: interface for the IORaw class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////


#include "IORaw.h"

namespace PGIO
{

template <class T>
bool IORaw<T>::Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject/* = 0*/)
{
	// write meta data first
/*	if (!iMetaDataObject) {
		LOG0("IORaw::Write: metadata object null");			
		return false;
	}
*/
	if (!WriteMetaData(iMetaDataObject, iParams)) {
		LOG0("IORaw::Write: failed to read metadata.");			
		return false;
	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IORaw::Write: Expecting single file path.");			
		return false;
	}

	const std::string &oFileName = iParams.Source();
	if (oFileName.empty())
	{
		LOG0("IORaw::Write: Invalid input filename.");			
		return false;
	}

	if (!iDataObject) 
	{
		LOG0("IORaw::Write: Invalid input image.");			
		return false;
	}

	// cast to image type here
	PGCore::Image<T> *iImage = (static_cast<PGCore::Image < T > *>(iDataObject));
	if (!iImage) 
	{
		LOG0("IORaw::Write: Invalid input image.");			
		return false;
	}

	long ioRows=0, ioCols=0;
	iImage->GetDimensions((long &)ioRows, (long &)ioCols);
	if (ioRows*ioCols <=0) 
	{	
		LOG0("IORaw::Write: Invalid image dimensions.");			
		return false;
	}

	PGCore::GaussianKernel<double, T> gausskernel(1.0f);
	if (GetSmoothFlag())
	{
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*iImage, sImage);	
		*iImage = sImage;		
		LOG1("IORaw::Smooth Image [%s] DONE.", oFileName.c_str());			
	}

	LOG1("IORaw::Write: Image [%s] DONE.", oFileName.c_str());			

	T* iBuf = iImage->GetBuffer();
	if (!iBuf)
	{
		LOG0("IORaw::Write: Invalid image buffer.");			
		return false;
	}


	FILE *fptr = fopen (oFileName.c_str(), "wb");
	if (!fptr) {			
		LOG1("IORaw::Write: failed to open file %s to write.", oFileName.c_str());			
		return false;
	}

	try 
	{		
		long rv = fwrite((const void *)iBuf, sizeof(T), ioRows*ioCols, fptr);
		if (rv!= ioRows*ioCols)
		{
			fclose(fptr);
			LOG1("IORaw::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
			return false;
		}

	} 
	catch (...)
	{	
		fclose(fptr);
		LOG1("IORaw::Write: failed to write file %s. Possible overrun.", oFileName.c_str());			
		return false;
	}
	
	fclose(fptr);
	return true;
}


template <class T>
bool IORaw<T>::Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject/* = 0*/)
{
	// read meta data first
	if (!oMetaDataObject) {
		LOG0("IORaw::Read: metadata object null");			
		return false;
	}

	if (!ReadMetaData(oMetaDataObject, iParams)) {
		LOG0("IORaw::Read: failed to read metadata.");			
		return false;
	}

	if (iParams.SourceType()!=kPgIOSourceTypeFile)
	{	
		LOG0("IORaw::Read: Expecting single file path.");			
		return false;
	}

	const std::string &iFileName = iParams.Source();
	if (iFileName.empty())
	{
		LOG0("IORaw::Read: Invalid input filename.");			
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

	long ioRows=0, ioCols=0;
	oImage->GetDimensions((long &)ioRows, (long &)ioCols);
	if (ioRows*ioCols <=0) 
	{	
		LOG0("IORaw::Read: Invalid input dimensions.");			
		return false;
	}	

	T* oBuf = oImage->GetBuffer();
	if (!oBuf)
	{
		LOG0("IORaw::Read: Invalid image buffer.");			
		return false;
	}

	PGCore::GaussianKernel<double, T> gausskernel(1.0f);

	FILE *fptr = fopen (iFileName.c_str(), "rb");
	if (!fptr) {
		oImage->Clear();		
		LOG1("IORaw::Read: failed to open file %s to read.", iFileName.c_str());			
		return false;
	}

	try 
	{		
		fread((void *)oBuf, sizeof(T), ioRows*ioCols, fptr);
	} 
	catch (...)
	{
		oImage->Clear();		
		fclose(fptr);
		LOG1("IORaw::Read: failed to read file %s. Possible overrun.", iFileName.c_str());			
		return false;
	}
		
	if (GetSmoothFlag())
	{		
		PGCore::Image<T> sImage;
		gausskernel.Convolve(*oImage, sImage);	
		*oImage = sImage;
		LOG1("IORaw::Smooth: Image [%s] DONE.", iFileName.c_str());			
	}

	//LOG1("IORaw::Read: Image [%s] DONE.", iFileName.c_str());			
	fclose(fptr);
	return true;
}

};



