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
// Filename		:	Thresholding.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read DICOM images and perform simple
//					Thresholding, given a threshold value. The result
//					is visualized using a GLMPRRenderer.
//
// Usage		:	Thresholding.exe <InputFilePath> <Threshold>
//						Where
//							<InputFilePath>		:	Path to the DICOM images folder.
//							<Threshold>			:	Threshold value
//
// Test data	:	Please download DCM-CT.zip or DCM-MR.zip from 
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	Thresholding.exe C:\Data\DCM-CT 200 C:\Tmp
//					Thresholding.exe C:\Data\DCM-MR 100 C:\Tmp
//**************************************************************************/
#include <string>
#include <vector>
#include "Core/Logger.h"
#include "IO/IODicom3D.h"
#include "Algs/ImageBinarizer.h"	// for rendering MPR image using OpenGL
#include "Algs/GLMPRRenderer.h"	// for rendering MPR image using OpenGL


static PGCore::Logger logger("C://Tmp//Thresholding.log");


//method to read images
bool ReadImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

// method to segment tissue
bool ThresholdImages(PGCore::Voxel3D<short>& ioVoxelData,		
		const short& iThreshold,
		const std::string& iFolderPath);

// method to render MPR
bool RenderMPR(PGCore::Voxel3D<short>& ioVoxelData,
		int argc, char* argv[]);


int main(int argc, char* argv[])
{
	if (argc<3)
	{
		logger.Log("Need at least 3 arguments: Thresholding.exe <inputFolderPath> <Threshold> <OutputFolder>");
		return -1;
	}

	if (!argv[1] || !argv[2])
	{		
		logger.Log("No filenames to read/write");
		return -1;
	}

	LoggerBase::SetLogger(&logger);

	// get folder name for input data
	std::vector<std::string> fileNames;
	fileNames.push_back(std::string(argv[1]));

	PGCore::Volume<short> ioVolume;
	PGCore::MetaData<short> ioVolumeAttributes;

	// read into volume structure
	bool rv = ReadImagesDcm(fileNames, ioVolume, ioVolumeAttributes);
	if (!rv)
	{
		logger.Log("readImages failed");
		return -1;
	}
	
	PGCore::Voxel3D<short> voxelData(ioVolumeAttributes, &ioVolume);						
	ioVolume.ResetBitVolume();

	// segment tissue
	std::string dstFolder(argv[3]);	
	short threshold(atoi(argv[2]));
	rv = ThresholdImages(voxelData, threshold, dstFolder);
	if (!rv)
	{
		logger.Log("ThresholdImages failed");
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
			logger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}


// method to segment tissue
bool ThresholdImages(PGCore::Voxel3D<short>& iVoxelData,
		const short& iThreshold,
		const std::string& iFolderPath)
{
	if (!iVoxelData.GetVolumeAccessor()) return false;

	PGMath::Point3D<long> dims;
	iVoxelData.GetVolumeAccessor()->GetDimensions(dims);
	if (dims.Length()<=0) return false;

	

	
	for (int z=0; z<dims.Z(); z++)
	{
		PGCore::Image<short> nextImage = iVoxelData.GetVolumeAccessor()->GetImage(z);

		PGCore::BitImage bImage;

		PGAlgs::ImageBinarizer<short, short> binarizer;

		binarizer.SetThresholdRange(iThreshold, iThreshold+1024);
		binarizer.SetInput(&nextImage);
		binarizer.Execute();
		binarizer.GetOutput(&bImage);

		PGCore::Image<short> oImage;
		bImage.UnPack(oImage);
		
		PGIO::IOPgm<short> pgmImgIOD;
		char fName[256] = {0};
		_snprintf(fName, 255, "%s/ThresholdedImg%04d.pgm", iFolderPath.c_str(), z); fName[255] = '\0';		
		std::string oPath = std::string(fName);			
		PGIO::IOParams oPgmParamsEd(std::vector<std::string>(1, oPath), oPath + ".txt", PGIO::kPgIOSourceTypeFile);
		bool rv = pgmImgIOD.Write(static_cast<PGCore::BaseDataObject *>(&oImage), oPgmParamsEd);		
		if (!rv) return false;
	}

	return true;
}


// method to render MPR
bool RenderMPR(PGCore::Voxel3D<short>& iVoxelData,
		int argc, char* argv[])
{
	PGCore::MultiDataBase<short> iMultiVolume; 
	iMultiVolume.AddDataObject(static_cast<PGCore::BaseDataObject *> (&iVoxelData));
	
	// create renderer here
	PGAlgs::GLMPRRenderer<short, unsigned char> mprRenderer;
	bool rv = mprRenderer.SetInput(static_cast<PGCore::BaseDataObject *> (&iMultiVolume));
	if (!rv) { return false; };
	
	PGCore::TransferFunctionLUT<unsigned char> mprLuT;
	mprLuT.SetType(PGCore::kPgLUTTypeGrayScale);
	mprLuT.SetWindow(0.1f, 0.55f);

	PGCore::TransferFunctionLUT<unsigned char> maskLuT;
	maskLuT.SetType(PGCore::kPgLUTTypeVIBGYOR);
	maskLuT.SetWindow(0.1f, 0.55f);
	
	unsigned int sx=256, sy= 256;	
	rv = mprRenderer.SetImageSize(sx, sy);
	if (!rv) { return false; };

	rv = mprRenderer.PrepareRenderer();
	if (!rv) { return false; };
	
	rv = mprRenderer.SetTransferFunctionLUT(&mprLuT, 0);
	if (!rv) return 0;

	rv = mprRenderer.SetMaskTransferFunctionLUT(&maskLuT, 0);
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

