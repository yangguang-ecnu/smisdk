// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
// Filename		:	MPRRenderer.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read DICOM images and render MPR
//					images from the reconstructed volume.
//
// Usage		:	MPRRenderer.exe <InputDataFolder>   <OutputFolder>
//						Where
//							<InputDataFolder>	:	Path to the folder that has test DICOM files.
//							<OutputFolder>		:	Path to the folder where images will be saved.
//
// Test data	:	Please download DCM-CT.zip or DCM-MR.zip from 
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	MPRRenderer.exe C:\Data\DCM-CT C:\Tmp
//					MPRRenderer.exe C:\Data\DCM-MR C:\Tmp
//**************************************************************************/
#include "Core/Logger.h"
#include "IO/IODicom3D.h"
#include "Algs/VoxelToMPRRenderer.h"
#include <string>
#include <vector>

static PGCore::Logger logger("C://Tmp//MPRRenderer.log");

bool ReadImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool RenderMPR(PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes, 
		const std::string& oFolder);

int main(int argc, char* argv[])
{
	if (argc<3)
	{
		logger.Log("Need at least two arguments: MPRRenderer.exe <inputFolderPath> <outPutFolder>");
		return -1;
	}
	// read data	
	// read data	
	if (!argv[1] || !argv[2])
	{		
		logger.Log("No filenames to read/write");
		return -1;
	}

	// set common logger
	PGCore::LoggerBase::SetLogger(&logger);

	std::vector<std::string> fileNames;
	fileNames.push_back(std::string(argv[1]));

	PGCore::Volume<short> ioVolume;
	PGCore::MetaData<short> ioVolumeAttributes;

	bool rv = ReadImagesDcm(fileNames, ioVolume, ioVolumeAttributes);
	if (!rv)
	{
		logger.Log("readImages failed");
		return -1;
	}

	// render data
	std::string dstFolder(argv[2]);	
	rv = RenderMPR(ioVolume, ioVolumeAttributes, dstFolder);
	if (!rv)
	{
		logger.Log("RenderMPR failed");
		return -1;
	}

	return 0;
}

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
		ioParamsDcm.SetAutoSkipFlag(true);
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



bool RenderMPR(PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes,
		const std::string& oFolder)
{
	if (oFolder.empty()) return false;

	PGCore::Voxel3D<short> voxelDataPrimary(ioAttributes, &ioVolume);						
	ioVolume.ResetBitVolume();

	// create renderer here
	PGAlgs::VoxelToMPRRenderer<short, unsigned char> mprRenderer;
	bool rv = mprRenderer.SetInput(static_cast<PGCore::BaseDataObject *> (&voxelDataPrimary));
	if (!rv)
	{
		return false;
	}
	
	PGMath::Point3D<long> dims;
	ioVolume.GetDimensions(dims);

	unsigned int sx=256;		
	rv = mprRenderer.SetImageSize(sx, sx);
	
	rv = mprRenderer.PrepareRenderer();
	if (!rv)
	{
		return false;
	};

	rv = mprRenderer.SetBlendWeight(0.5f, 0);
	if (!rv)
	{
		return false;
	};

	PGCore::TransferFunctionLUT<unsigned char> mprLuT;
	mprLuT.SetWindow(0.1f, 0.35f);
	mprLuT.SetType(PGCore::kPgLUTTypeVIBGYOR);
	rv = mprRenderer.SetTransferFunctionLUT(&mprLuT, 0);
	if (!rv) return 0;

	rv = mprRenderer.Render();	
	if (!rv)
	{
		return false;
	}

	PGCore::Image<PGMath::Point3D<unsigned char>> renderedImage;

	rv = mprRenderer.GetOutput(&renderedImage);	
	if (!rv) return 0;

	char oPath[256] = {0};
	_snprintf(oPath, 255, "%s/MPRRenrererOutput.ppm", oFolder.c_str());
	oPath[255] = '\0';

	rv = PGAlgs::DumpImageAsPPM(renderedImage, std::string(oPath));
	if (!rv) return 0;

	return true;
}