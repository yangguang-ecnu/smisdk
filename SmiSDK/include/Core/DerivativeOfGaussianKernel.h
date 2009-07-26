// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
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
// Filename:	DerivativeOfGaussianKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_DerivativeOfGaussianKernel_H_
#define _PG_DerivativeOfGaussianKernel_H_
//////////////////////////////////////////////////////////////////////////

#include "Kernel.h"
#include "Core/CentralDifferenceDerivativeKernel.h"

namespace PGCore
{

template <class T, class U>
class DerivativeOfGaussianKernel : public Kernel<T, U>
{
public:

	DerivativeOfGaussianKernel(const double iSigma=1, const int iDimension=-1);		

	~DerivativeOfGaussianKernel(void);

	DerivativeOfGaussianKernel(const DerivativeOfGaussianKernel<T, U>& iKernelObject);	

	DerivativeOfGaussianKernel<T, U>& operator=(const DerivativeOfGaussianKernel<T, U>& iKernelObject);
	
	const double& GetSigma() const;

protected: 	

	double	m_sigma;

	virtual bool Initialize_();
	
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "DerivativeOfGaussianKernel.hpp"
#endif
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DerivativeOfGaussianKernel_H_