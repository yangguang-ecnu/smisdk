#include "Core/Logger.h"
#include "IO/IOPpm3D.h"
#include "IO/IODef3D.h"
#include "IO/IODicom3D.h"
#include "Algs/VoxelToMPRRenderer.h"
#include <string>
#include <vector>

static PGCore::Logger logger("C://Tmp//Convolution.log");



bool readImagesDcm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool readImagesDef(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool readImagesPpm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);

bool readImagesMhd(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes);


int main(int argc, char* argv[])
{
	if (argc<2)
	{
		logger.Log("Need at least one argument: Subtraction.exe <filePath>");
		return -1;
	}

	// read data
	
	if (!argv[1])
	{		
		logger.Log("No filenames to read");
		return -1;
	}

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

	return 0;
}

bool readImagesDef(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes)
{
	PGIO::ePgIOSourceType srcType = iFileNames.size()==1 ? PGIO::kPgIOSourceTypeFile
		: PGIO::kPgIOSourceTypeFolder;		
	{			

		PGIO::IODef3D<short> pDefImgIO3D;

		PGIO::IOParams ioParamsDef(
			iFileNames,
			std::string(""),
			srcType);

		const int _MaxFilesForAUTOSKIP_ = 256;
		bool iMTFlag = false;
		bool iAsynchronousLoad = false;

		ioParamsDef.SetMultiThreadingFlag(iMTFlag);	
		ioParamsDef.SetAsynchronousLoadFlag(iAsynchronousLoad);
		ioParamsDef.SetAutoSkipFlag(iFileNames.size()>_MaxFilesForAUTOSKIP_);
		ioParamsDef.SetSourceFormat(PGIO::kPgIOSourceFormatDEF);
		
		if (!pDefImgIO3D.Read(&ioVolume, ioParamsDef, &ioAttributes))
		{
			logger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
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
		ioParamsDcm.SetAutoSkipFlag(false);//true);//iFileNames.size()>_MaxFilesForAUTOSKIP_);
		ioParamsDcm.SetSourceFormat(PGIO::kPgIOSourceFormatDICOM);

		pDcmImgIO3D.SetSkipFactors(1, 16);
		pDcmImgIO3D.SetSmoothFlag(false);
		
		if (!pDcmImgIO3D.Read(&ioVolume, ioParamsDcm, &ioAttributes))
		{
			logger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}



bool readImagesPpm(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes)
{
	PGIO::ePgIOSourceType srcType = iFileNames.size()==1 ? PGIO::kPgIOSourceTypeFile
		: PGIO::kPgIOSourceTypeFolder;		
	{			

		PGIO::IOPpm3D<short> pPPMImgIO3D;

		PGIO::IOParams ioParamsPpm(
			iFileNames,
			std::string(""),
			srcType);

		const int _MaxFilesForAUTOSKIP_ = 256;
		bool iMTFlag = false;
		bool iAsynchronousLoad = false;

		ioParamsPpm.SetMultiThreadingFlag(iMTFlag);	
		ioParamsPpm.SetAsynchronousLoadFlag(iAsynchronousLoad);
		ioParamsPpm.SetAutoSkipFlag(iFileNames.size()>_MaxFilesForAUTOSKIP_);
		ioParamsPpm.SetSourceFormat(PGIO::kPgIOSourceFormatPGM);
		
		if (!pPPMImgIO3D.Read(&ioVolume, ioParamsPpm, &ioAttributes))
		{
			logger.Log("readImages: Error: read failed");				
			return false;
		}			
	}
	
	return true;
}



bool readImagesMhd(const std::vector<std::string>& iFileNames, 
		PGCore::Volume<short>& ioVolume,
		PGCore::MetaData<short>& ioAttributes)
{

	return false;
}