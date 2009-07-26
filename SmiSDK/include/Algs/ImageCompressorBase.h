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
// Filename:	ImageCompressorBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorBase_H_
#define _PGImageCompressorBase_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"
#include "../Core/Image.h"

namespace PGAlgs
{

template <class T, class U>
class ImageCompressorBase : public ImageFilterBase<T, U>
{
public:
	ImageCompressorBase();

	void SetCompressionFlag(bool iForward);

	virtual bool Execute() = 0;
	
public:	
	virtual ~ImageCompressorBase(void);

protected:
	bool				m_Forward;

	PGCore::Image<U>   *m_oImageObject;		
	
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageCompressorBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorBase_H_
