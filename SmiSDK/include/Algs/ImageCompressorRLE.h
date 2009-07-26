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
// Filename:	ImageCompressorRLE.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorRLE_H_
#define _PGImageCompressorRLE_H_
//////////////////////////////////////////////////////////////////////////

#include "ImageCompressorBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageCompressorRLE : public ImageCompressorBase<T, U>
{
public:
	ImageCompressorRLE(void);

	virtual bool Execute();
	
	virtual ~ImageCompressorRLE(void);

	void SetQuantizationRange(const float& iRange); // [0...1]

	const float& GetQuantizationRange() const;

protected:
	
private:
	float				m_range;	

	bool	compress(PGCore::Image<T> *inImage);
	bool	decompress(PGCore::Image<T> *inImage);
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageCompressorRLE.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorRLE_H_
