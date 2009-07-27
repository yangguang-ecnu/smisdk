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
// Filename		:	EdgeDetection.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read images and estimate edges in them.
//
// Usage		:	EdgeDetection.exe <InputFilePath> <OutputFolder>
//						Where
//							<InputFilePath>		:	Path to the DICOM image file.
//							<OutputFolder>		:	Path to the folder where images will be saved.
//
// Test data	:	Please download DCM-CT.zip or DCM-MR.zip from
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	EdgeDetection.exe C:\Data\DCM-CT\IM_00032.dcm C:\Tmp
//
//**************************************************************************/
#include "Core/Logger.h"
#include "IO/IODicom3D.h"
#include "Algs/ImageEdgeDetectionBase.h"
#include <string>
#include <vector>

static PGCore::Logger logger("C://Tmp//EdgeDetection.log");

bool readImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool detectEdgesAndWriteImagesPgm(const std::string& iFolderPath, 
		const PGCore::Volume<short>& iVolume);

int main(int argc, char* argv[])
{
	if (argc<3)
	{
		logger.Log("Need at least two arguments: EdgeDetection.exe <inputFilePath> <outPutFolder>");
		return -1;
	}

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

	bool rv = readImagesDcm(fileNames, ioVolume, ioVolumeAttributes);
	if (!rv)
	{
		logger.Log("readImages failed");
		return -1;
	}

	// write data back as PGMs
	// smooth and write data
	std::string dstFolder(argv[2]);	
	rv = detectEdgesAndWriteImagesPgm(dstFolder, ioVolume);
	if (!rv)
	{
		logger.Log("detectEdgesAndWriteImagesPgm failed");
		return -1;
	}

	return 0;
}

bool readImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes)
{
	PGIO::ePgIOSourceType srcType = PGIO::kPgIOSourceTypeFile;		
	{			

		PGIO::IODicom3D<short> pDcmImgIO3D;

		PGIO::IOParams ioParamsDcm(
			iFileNames,
			std::string(""),
			srcType);

		const int _MaxFilesForAUTOSKIP_ = 256;
		bool iMTFlag = false;
		bool iAsynchronousLoad = false;

		ioParamsDcm.SetMultiThreadingFlag(iMTFlag);	
		ioParamsDcm.SetAsynchronousLoadFlag(iAsynchronousLoad);
		ioParamsDcm.SetAutoSkipFlag(false);
		ioParamsDcm.SetSourceFormat(PGIO::kPgIOSourceFormatDICOM);

		pDcmImgIO3D.SetSmoothFlag(false);
		
		if (!pDcmImgIO3D.Read(&ioVolume, ioParamsDcm, &ioAttributes))
		{
			logger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}


bool detectEdgesAndWriteImagesPgm(const std::string& iFolderPath, 
		const PGCore::Volume<short>& iVolume)
{
	PGMath::Point3D<long> dims;
	iVolume.GetDimensions(dims);
	if (dims.Length()<=0) return false;

	// Canny edge detector
	PGAlgs::ImageEdgeDetectionBase<short, short> edgeDetector(0.75f);
	
	edgeDetector.SetHysterisisThresholds(0.1f, 0.7f);// normalized between 0.0f and 1.0f
	
	for (int z=0; z<dims.Z(); z++)
	{
		PGCore::Image<short> nextImage = iVolume.GetImage(z);
		char fName[256] = {0};

		_snprintf(fName, 255, "%s/Slice%04d.pgm", iFolderPath.c_str(), z); fName[255] = '\0';		
		std::string oPath(fName);		
		
		PGIO::IOPgm<short> pgmImgIO;	
		PGIO::IOParams oPgmParams(std::vector<std::string>(1, oPath), oPath + ".txt", PGIO::kPgIOSourceTypeFile);
		bool rv = pgmImgIO.Write(static_cast<PGCore::BaseDataObject *>(&nextImage), oPgmParams);	
		if (!rv) return false;
	
		// detect edges here
		PGCore::Image<short> edgeImage;
		edgeDetector.SetInput(&nextImage);
		edgeDetector.Execute();
		edgeDetector.GetOutput(&edgeImage);

		// write
		PGIO::IOPgm<short> pgmImgIOD;
		_snprintf(fName, 255, "%s/CannyEdgeSlice%04d.pgm", iFolderPath.c_str(), z); fName[255] = '\0';		
		oPath = std::string(fName);		
		PGIO::IOParams oPgmParamsEd(std::vector<std::string>(1, oPath), oPath + ".txt", PGIO::kPgIOSourceTypeFile);
		rv = pgmImgIOD.Write(static_cast<PGCore::BaseDataObject *>(&edgeImage), oPgmParamsEd);		
		if (!rv) return false;
	}

	return true;
}