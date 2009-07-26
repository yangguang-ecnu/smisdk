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
// Filename:	ImageCasterBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGIMAGECASTERBASE_H_
#define _PGIMAGECASTERBASE_H_
//////////////////////////////////////////////////////////////////////////


#include "ProcessBase.h"

//#include "../Core/MultiDataBase.h"
#include "../Core/Image.h"

namespace PGAlgs
{

template <class T, class U>
class ImageCasterBase : public ProcessBase//<T, U>
{
public:
	ImageCasterBase(void);

	virtual bool Execute() = 0;
	
	bool GetOutput(PGCore::BaseDataObject *oDataObject) ;
	
public:	
	virtual ~ImageCasterBase(void);
};


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGIMAGECASTERBASE_H_
