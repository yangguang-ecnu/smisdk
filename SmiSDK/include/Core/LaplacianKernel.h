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
// Filename:	LaplacianKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_LaplacianKernel_H_
#define _PG_LaplacianKernel_H_
//////////////////////////////////////////////////////////////////////////

// A laplacian in n-dimensional Euclidean space is the divergence of the gradient
// Specifically, it is the sum of all unmixed partial second derivatives of a function
// It is thus the 'Trace' of the 'Hessian' matrix of a function
// The 'trace' of a square matrix is the sum of all its main-diagonal elements (top left to bottom right)
// A boundary exists where the laplacian is zero (maximum gradient)
// Laplacian is rotation-invariant

#include "Kernel.h"

namespace PGCore
{

	template <class T, class U>
	class LaplacianKernel : public Kernel<T, U>
	{
	public:

		LaplacianKernel();		

		~LaplacianKernel(void);

		LaplacianKernel(const LaplacianKernel<T, U>& iKernelObject);	

		LaplacianKernel<T, U>& operator=(const LaplacianKernel<T, U>& iKernelObject);

	protected: 	
		
	
		virtual bool Initialize_();

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "LaplacianKernel.hpp"
#endif
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_LaplacianKernel_H_