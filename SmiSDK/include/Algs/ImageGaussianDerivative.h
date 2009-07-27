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
// Filename:	ImageGaussianDerivative.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageGaussianDerivative_H_
#define _PGImageGaussianDerivative_H_
//////////////////////////////////////////////////////////////////////////

#include "ImageFilterBase.h"
#include "Core/GaussianKernel.h"
#include "Core/DiffOfGaussianDerivativeKernel.h"


namespace PGAlgs
{

template <class T, class U>
class ImageGaussianDerivative : public ImageFilterBase<T, U>
{
public:
	
	ImageGaussianDerivative(double iSigma=1.0f);

	bool Execute();

public:	
	
	virtual ~ImageGaussianDerivative(void);

protected:
	

private:
	PGCore::Image<U>	*m_oImageObject;
	double				m_sigma;
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageGaussianDerivative.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageGaussianDerivative_H_
