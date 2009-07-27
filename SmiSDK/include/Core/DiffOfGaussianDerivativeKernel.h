// -*- C++ -*-
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