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
