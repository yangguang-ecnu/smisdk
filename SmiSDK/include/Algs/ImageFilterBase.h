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
// Filename:	ImageFilterBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//
// Comments:	http://www.dspguide.com/ch24/7.htm

#ifndef _PGImageFilterBase_H_
#define _PGImageFilterBase_H_
//////////////////////////////////////////////////////////////////////////

#include "ProcessBase.h"
#include "Core/Image.h"
#include "IO/IOPpm.h"
#include "IO/IOPgm.h"

namespace PGAlgs
{
	
template<class T>
static bool DumpImageAsPGM(PGCore::Image<T>& iImage, const std::string &oPath);

template<class T>
static bool DumpImageAsPPM(PGCore::Image<T>& iImage, const std::string &oPath);
					
template <class T, class U>
class ImageFilterBase : public ProcessBase
{
public:
	ImageFilterBase(void);

	virtual bool Execute() = 0;
	
	bool GetOutput(PGCore::BaseDataObject *oDataObject);

public:	
	virtual ~ImageFilterBase(void);
};
	
template<class T>
static bool PGAlgs::DumpImageAsPGM(PGCore::Image<T>& iImage, const std::string &oPath)
{
	PGIO::IOPgm<T> pgmImgIOC;	
	PGIO::IOParams oPgmParamsC(std::vector<std::string>(1, oPath), 
		oPath + ".txt",
		PGIO::kPgIOSourceTypeFile);
	bool res = pgmImgIOC.Write(&iImage, oPgmParamsC);			
	return res;
}

template<class T>
static bool PGAlgs::DumpImageAsPPM(PGCore::Image<T>& iImage, const std::string &oPath)
{
	PGIO::IOPpm<T> ppmImgIOC;	
	PGIO::IOParams oPpmParamsC(std::vector<std::string>(1, oPath),  
		oPath + ".txt",
		PGIO::kPgIOSourceTypeFile);
	bool res = ppmImgIOC.Write(&iImage, oPpmParamsC);		
	return res;
}



};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageFilterBase_H_
