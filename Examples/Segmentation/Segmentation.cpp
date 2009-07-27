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
// Filename		:	Segmentation.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read DICOM images and perform simple
//					region grow based segmentation, given a seed point. The result
//					is visualized using a GLMPRRenderer.
//
// Usage		:	Segmentation.exe <InputFolderPath> <seed_x> <seed_y> <seed_z>
//						Where
//							<InputFolderPath>	:	Path to the DICOM image folder.
//							<seed_x>			:	Seed X co-ordinate in image space
//							<seed_y>			:	Seed Y co-ordinate in image space
//							<seed_z>			:	Seed Z co-ordinate in image space
//
// Test data	:	Please download DCM-CT.zip or DCM-MR.zip from 
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	Segmentation.exe C:\Data\DCM-CT 31 71 256
//						DCM-CT: (seed in liver) 31 71 256
//						DCM-CT: (seed in aorta)	67 70 145
//					Segmentation.exe C:\Data\DCM-MR 127 97 9
//						DCM-MR: (seed in aorta)	127 97 9
//**************************************************************************/
#include <string>
#include <vector>
#include "Core/Logger.h"
#include "IO/IODicom3D.h"
#include "Algs/RegionGrowSegmentation.h" // for simple region grow based segmentation
#include "Algs/GLMPRRenderer.h"	// for rendering MPR image using OpenGL


static PGCore::Logger logger("C://Tmp//Segmentation.log");


//method to read images
bool ReadImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

// method to segment tissue
bool SegmentTissue(PGCore::Voxel3D<short>& ioVoxelData,		
		const PGMath::Point3D<float>& iSeed);

// method to render MPR
bool RenderMPR(PGCore::Voxel3D<short>& ioVoxelData,
		int argc, char* argv[]);


int main(int argc, char* argv[])
{
	if (argc<5)
	{
		logger.Log("Need at least 4 arguments: Segmentation.exe <inputFolderPath> <x> <y> <z>");
		return -1;
	}

	if (!argv[1])
	{		
		logger.Log("No filenames to read");
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
	PGMath::Point3D<float> seed(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	rv = SegmentTissue(voxelData, seed);
	if (!rv)
	{
		logger.Log("SegmentTissue failed");
		return -1;
	}

	// render MPR
	rv = RenderMPR(voxelData, argc, argv);
	if (!rv)
	{
		logger.Log("RenderMPR failed");
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
bool SegmentTissue(PGCore::Voxel3D<short>& iVoxelData,
		const PGMath::Point3D<float>& iSeed)
{
	
	PGCore::MultiDataBase<short> iMultiVolume; 
	iMultiVolume.AddDataObject(static_cast<PGCore::BaseDataObject *> (&iVoxelData));
	
	// create RegionGrowSegmentation here
	PGAlgs::RegionGrowSegmentation<short, short> segmentationEngine;
	bool rv = segmentationEngine.SetInput(static_cast<PGCore::BaseDataObject *> (&iMultiVolume));
	if (!rv) { return false; };

	float rangeFac = 0.1f;
	int gradientHigh = 75;
	
	switch (iVoxelData.GetMetaData().GetModality())
	{
	case PGCore::kPGModalityCT : rangeFac = 0.2f;
		gradientHigh = 175;
		break;

	case PGCore::kPGModalityMR : rangeFac = 0.2f;
		gradientHigh = 150;
		break;

	case PGCore::kPGModalityPT : rangeFac = 0.3f;
		gradientHigh = 100;
		break;

	default: break;
	}

	// transform seed from image to voxel space
	float x=0, y=0, z=0;
	rv = iVoxelData.GetImgToStdCoord(iSeed.X(), iSeed.Y(), iSeed.Z(), x, y, z);
	if (!rv) return false;

	std::vector<PGMath::Point3D<float> > seeds;
	seeds.push_back(PGMath::Point3D<float>(x, y, z));

	//  mask will be implicit to the volume
	segmentationEngine.SetAutoDilationCount(1); // after segmentation, dilate result by 1 voxel kernel
	segmentationEngine.SetSeeds(seeds); // set seeds
	segmentationEngine.SetWindow(rangeFac); // window around the seed point's value
	segmentationEngine.SetGradientHigh(gradientHigh); // high gradient limit
	segmentationEngine.SetAutoAdjustConditions(false); // dont auto adjust conditions
	segmentationEngine.SetAsyncEnabled(false); // dont separate into another thread

	return segmentationEngine.Execute();
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

