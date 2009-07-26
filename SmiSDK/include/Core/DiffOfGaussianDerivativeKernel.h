// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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