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
// Filename		:	DataIO.cpp
// Author		:	Prashant Chopra
// Created		:	Wednesday, August 13, 2008 at 5:11:33 PM
// Description	:	Example showing how to read DICOM images and dump them as PGM images
//
// Usage		:	DataIO.exe <InputDataFolder> ?OutputFolder?
//						Where
//							<InputDataFolder>	:	Path to the folder that has test DICOM files.
//							?OutputFolder?		:	Path to the folder where images will be saved (optional).
//
// Test data	:	Please download DCM-CT.zip or DCM-MR.zip from 
//					http://groups.google.com/group/medicalimagingscience/files
//
// Example		:	DataIO.exe C:\Data\DCM-CT C:\Tmp
//
//**************************************************************************/
#include "Core/Logger.h"
#include "IO/IODicom3D.h"

#include <string>
#include <vector>

static PGCore::Logger logger("C://Tmp//DataIO.log");

bool readImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool writeImagesPgm(const std::string& iFolderPath, 
		const PGCore::Volume<short>& iVolume);


int main(int argc, char* argv[])
{
	if (argc<2)
	{
		logger.Log("Need at least one argument: DataIO.exe <filePath> ?OutputFolder?");
		return -1;
	}

	// read data	
	if (!argv[1])
	{		
		logger.Log("No filenames to read");
		return -1;
	}
	
	std::string dstFolder;
	if (argc>2)
	{
		dstFolder = std::string(argv[2]);
	}

	// set common logger
	LoggerBase::SetLogger(&logger);

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
	if (argc>2 && !dstFolder.empty())
	{
		std::string dstFolder(argv[2]);
		bool rv = writeImagesPgm(dstFolder, ioVolume);
		if (!rv)
		{
			logger.Log("writeImagesPgm failed");
			return -1;
		}
	}

	return 0;
}

bool readImagesDcm(const std::vector<std::string>& iFileNames, 
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

		const int _MaxFilesForAUTOSKIP_ = 256;
		bool iMTFlag = false;
		bool iAsynchronousLoad = false;

		ioParamsDcm.SetMultiThreadingFlag(iMTFlag);	
		ioParamsDcm.SetAsynchronousLoadFlag(iAsynchronousLoad);
		ioParamsDcm.SetAutoSkipFlag(true);
		ioParamsDcm.SetSourceFormat(PGIO::kPgIOSourceFormatDICOM);

		pDcmImgIO3D.SetByteLimitInKB(128000);// 256MB limit, beyond which data will be skipped
		
		if (!pDcmImgIO3D.Read(&ioVolume, ioParamsDcm, &ioAttributes))
		{
			logger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}


bool writeImagesPgm(const std::string& iFolderPath, 
		const PGCore::Volume<short>& iVolume)
{
	PGMath::Point3D<long> dims;
	iVolume.GetDimensions(dims);
	if (dims.Length()<=0) return false;

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
	}

	return true;
}
