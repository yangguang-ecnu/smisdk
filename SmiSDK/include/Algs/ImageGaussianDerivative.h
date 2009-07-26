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
