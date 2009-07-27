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
// Filename:	GaussianKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_GaussianKernel_H_
#define _PG_GaussianKernel_H_
//////////////////////////////////////////////////////////////////////////

#include "Kernel.h"

namespace PGCore
{

	template <class T, class U>
	class GaussianKernel : public Kernel<T, U>
	{
	public:
		GaussianKernel(const double iSigma=1, const int iDimension=-1);		

		~GaussianKernel(void);

		GaussianKernel(const GaussianKernel<T, U>& iKernelObject);	

		GaussianKernel<T, U>& operator=(const GaussianKernel<T, U>& iKernelObject);

		const double& GetSigma() const { return m_sigma; }

	protected: 	
	
		virtual bool Initialize_(); // derived and overridden as required

	private:
		double	m_sigma;

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GaussianKernel.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_GaussianKernel_H_