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
// Filename:	DiffOfGaussianDerivativeKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_DiffOfGaussianDerivativeKernel_H_
#define _PG_DiffOfGaussianDerivativeKernel_H_
//////////////////////////////////////////////////////////////////////////

#include "Kernel.h"
#include "DerivativeOfGaussianKernel.h"
// similar to the laplacian of a function
// more so because it approxmates the divergence of gradient of teh function by taking differences
// in gaussian derivatives at a given point

namespace PGCore
{

	template <class T, class U>
	class DiffOfGaussianDerivativeKernel : public Kernel<T, U>
	{
	public:

		DiffOfGaussianDerivativeKernel(const double iSigmaOuter=2, const double iSigmaInner=1, const int iDimension=-1);		

		~DiffOfGaussianDerivativeKernel(void);

		DiffOfGaussianDerivativeKernel(const DiffOfGaussianDerivativeKernel<T, U>& iKernelObject);	

		DiffOfGaussianDerivativeKernel<T, U>& operator=(const DiffOfGaussianDerivativeKernel<T, U>& iKernelObject);

		const double& GetSigmaOuter() const;

		const double& GetSigmaInner() const;

	protected: 	
		
		double	m_sigmaOuter, m_sigmaInner;
	
		virtual bool Initialize_();
	};	
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "DiffOfGaussianDerivativeKernel.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DiffOfGaussianDerivativeKernel_H_