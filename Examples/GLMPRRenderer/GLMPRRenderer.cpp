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
// Filename		:	GLMPRRenderer.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read DICOM images and render MPR
//					images using OpenGL from the reconstructed volume.
//
// Usage		:	GLMPRRenderer.exe <InputDataFolder>
//						Where
//							<InputDataFolder>	:	Path to the folder that has test DICOM files.
//
// Test data	:	Please download DCM-CT.zip or DCM-MR.zip from 
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	GLMPRRenderer.exe C:\Data\DCM-CT
//					GLMPRRenderer.exe C:\Data\DCM-MR
//**************************************************************************/

#include "Core/Logger.h"		// for logging
#include "IO/IODicom3D.h"		// for reading DICOM images
#include "Algs/GLMPRRenderer.h"	// for rendering MPR image using OpenGL
#include <string>				// for std string
#include <vector>				// for stf vector

// define a logger
static PGCore::Logger mprLogger("C://Tmp//GLMPRRenderer.log");

// method to read DICOM images
bool ReadImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

// method to render MPR
bool RenderMPR(PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes, 
		int argc, char* argv[]);

// Entry point
int main(int argc, char* argv[])
{
	if (argc<2)
	{
		mprLogger.Log("Need at least one argument: GLMPRRenderer.exe <inputFolderPath>");
		return -1;
	}

	if (!argv[1])
	{		
		mprLogger.Log("No filenames to read");
		return -1;
	}

	LoggerBase::SetLogger(&mprLogger);

	// get folder name for input data
	std::vector<std::string> fileNames;
	fileNames.push_back(std::string(argv[1]));

	PGCore::Volume<short> ioVolume;
	PGCore::MetaData<short> ioVolumeAttributes;

	// read into volume structure
	bool rv = ReadImagesDcm(fileNames, ioVolume, ioVolumeAttributes);
	if (!rv)
	{
		mprLogger.Log("readImages failed");
		return -1;
	}

	// render MPR
	rv = RenderMPR(ioVolume, ioVolumeAttributes, argc, argv);
	if (!rv)
	{
		mprLogger.Log("RenderMPR failed");
		return -1;
	}

	return 0;
}

// method to read DICOM images
bool ReadImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes)
{
	PGIO::ePgIOSourceType srcType = PGIO::kPgIOSourceTypeFolder;		
	{			

		PGIO::IODicom3D<short> pDcmImgIO3D;

		PGIO::IOParams ioParamsDcm(
			iFileNames,
			std::string(""),
			srcType);

		bool iMTFlag = false;
		bool iAsynchronousLoad = false;

		ioParamsDcm.SetMultiThreadingFlag(iMTFlag);	
		ioParamsDcm.SetAsynchronousLoadFlag(iAsynchronousLoad);
		ioParamsDcm.SetAutoSkipFlag(true); // let the reader decide how much to skip
		ioParamsDcm.SetSourceFormat(PGIO::kPgIOSourceFormatDICOM);
		
		pDcmImgIO3D.SetByteLimitInKB(64000);// 64MB limit, beyond which data will be skipped
		pDcmImgIO3D.SetSmoothFlag(false);
						
		if (!pDcmImgIO3D.Read(&ioVolume, ioParamsDcm, &ioAttributes))
		{
			mprLogger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}

// method to render MPR
bool RenderMPR(PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes,
		int argc, char* argv[])
{
	PGCore::Voxel3D<short> voxelDataPrimary(ioAttributes, &ioVolume);						
	ioVolume.ResetBitVolume();

	PGCore::MultiDataBase<short> iMultiVolume; 
	iMultiVolume.AddDataObject(static_cast<PGCore::BaseDataObject *> (&voxelDataPrimary));
	
	// create renderer here
	PGAlgs::GLMPRRenderer<short, unsigned char> mprRenderer;
	bool rv = mprRenderer.SetInput(static_cast<PGCore::BaseDataObject *> (&iMultiVolume));
	if (!rv) { return false; };
	
	PGMath::Point3D<long> dims;
	ioVolume.GetDimensions(dims);
	
	PGCore::TransferFunctionLUT<unsigned char> mprLuT;
	mprLuT.SetType(PGCore::kPgLUTTypeGrayScale);
	mprLuT.SetWindow(0.1f, 0.55f);

	unsigned int sx=256, sy= 256;	
	rv = mprRenderer.SetImageSize(sx, sy);
	if (!rv) { return false; };

	rv = mprRenderer.PrepareRenderer();
	if (!rv) { return false; };
	
	rv = mprRenderer.SetTransferFunctionLUT(&mprLuT, 0);
	if (!rv) return 0;

	rv = mprRenderer.SetBlendWeight(1.0f, 0);
	if (!rv) { return false; };

	rv = mprRenderer.SetSlicerDirection(PGAlgs::kPgSlicingDirectionCoronal);
	if (!rv) { return false; };

	rv = mprRenderer.SetSlicerPosition(0.0f);
	if (!rv) { return false; };

	rv = mprRenderer.UpdateRender();	
	if (!rv)
	{
		return false;
	}	

	return true;
}