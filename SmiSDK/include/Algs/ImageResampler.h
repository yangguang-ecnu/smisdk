// -*- C++ -*-
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
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
// Filename:	ImageResampler.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageResampler_H_
#define _PGImageResampler_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageCasterBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageResampler : public ImageCasterBase<T, U>
{
public:
	ImageResampler(void);

	bool Execute();

	void SetScaleFactor(const float& iScale);

	const float& GetScaleFactor() const;


public:	
	virtual ~ImageResampler(void);

protected:

	

private:
	PGCore::Image<U>	*m_oImageObject;

	float m_sc;
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageResampler.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageResampler_H_
