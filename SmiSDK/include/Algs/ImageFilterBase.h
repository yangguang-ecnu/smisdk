// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
