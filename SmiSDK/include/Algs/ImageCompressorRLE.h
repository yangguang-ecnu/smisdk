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
