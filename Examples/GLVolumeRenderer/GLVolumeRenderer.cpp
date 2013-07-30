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
// Filename		:	GLVolumeRenderer.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read DICOM images and volume render and
//					reconstruct the volumetric data. The current version only works
//					with square images (e.g. 512x512 pixels).
//
// Usage		:	GLVolumeRenderer.exe <InputDataFolder>
//						Where
//							<InputDataFolder>	:	Path to the folder that has test DICOM files.
//
// Test data	:	Please download DCM-CT.zip from 
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	GLVolumeRenderer.exe C:\Data\DCM-CT
//**************************************************************************/
#include "Core/Logger.h"
#include "IO/IODicom3D.h"
#include "Algs/GLTextureVolumeRenderer.h"
#include <string>
#include <vector>

static PGCore::Logger volumeLogger("C://Temp//VolumeRenderer.log");

bool ReadImages(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool RenderVolume(PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes, 
		int argc, char* argv[]);

int main(int argc, char* argv[])
{
	if (argc<2) 
	{
		volumeLogger.Log("Need at least one argument: GLVolumeRenderer.exe <inputFolderPath>");
		return -1;
	}

	// read data
	
	if (!argv[1])
	{		
		volumeLogger.Log("No filenames to read");
		return -1;
	}

	PGCore::LoggerBase::SetLogger(&volumeLogger);

	std::vector<std::string> fileNames;
	fileNames.push_back(std::string(argv[1]));

	PGCore::Volume<short> ioVolume;
	PGCore::MetaData<short> ioVolumeAttributes;

	bool rv = ReadImages(fileNames, ioVolume, ioVolumeAttributes);
	if (!rv)
	{
		volumeLogger.Log("ReadImages failed");
		return -1;
	}

	// render data
	rv = RenderVolume(ioVolume, ioVolumeAttributes, argc, argv);
	if (!rv)
	{
		volumeLogger.Log("RenderVolume failed");
		return -1;
	}

	return 0;
}


bool ReadImages(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes)
{
	PGIO::ePgIOSourceType srcType = PGIO::kPgIOSourceTypeFolder;		
	
	{			

		PGIO::IODicom3D<short> pImgIO3D;

		PGIO::IOParams ioParams(
			iFileNames,
			std::string(""),
			srcType);

		bool iMTFlag = false;
		bool iAsynchronousLoad = false;

		ioParams.SetMultiThreadingFlag(iMTFlag);	
		ioParams.SetAsynchronousLoadFlag(iAsynchronousLoad);
		ioParams.SetAutoSkipFlag(false);//true); // let the reader decide how much to skip
		ioParams.SetSourceFormat(PGIO::kPgIOSourceFormatDICOM);

		pImgIO3D.SetByteLimitInKB(4*64000);// 64MB limit, beyond which data will be skipped
		
		if (!pImgIO3D.Read(&ioVolume, ioParams, &ioAttributes))
		{
			volumeLogger.Log("ReadImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}

bool RenderVolume(PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes,
		int argc, char* argv[])
{
	bool rv = false;

	// Add data
	PGMath::Point3D<long> dims;
	ioVolume.GetDimensions(dims);
	
	PGCore::Voxel3D<short> voxelDataPrimary(ioAttributes, &ioVolume);						

	// reset bitmask
	ioVolume.ResetBitVolume();

	PGCore::MultiDataBase<short> iMultiVolume; 
	iMultiVolume.AddDataObject(static_cast<PGCore::BaseDataObject *> (&voxelDataPrimary));
	
	// create renderer here
	PGAlgs::GLTextureVolumeRenderer<short, unsigned char> VolumeRenderer;
	rv = VolumeRenderer.SetInput(static_cast<PGCore::BaseDataObject *> (&iMultiVolume));
	if (!rv) return false;
	
	// how much to skip in sparse mode
	VolumeRenderer.SetSkipFactor(1);

	// create lookup table
	PGCore::TransferFunctionLUT<unsigned char> volLuT;
	volLuT.SetType(PGCore::kPgLUTTypeVIBGYOR);
	volLuT.SetWindow(0.15f, 0.45f);

	unsigned int sx = dims.X(), sy = dims.Y();
	rv = VolumeRenderer.SetImageSize(sx, sy);
	
	rv = VolumeRenderer.PrepareRenderer();
	if (!rv) return false;

	rv = VolumeRenderer.SetBlendWeight(0.05f, 0);
	if (!rv) return false;

	rv = VolumeRenderer.SetTransferFunctionLUT(&volLuT, 0);
	if (!rv) return false;

	rv = VolumeRenderer.UpdateRender();	
	if (!rv) return false;

	return true;
}