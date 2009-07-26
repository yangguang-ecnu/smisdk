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