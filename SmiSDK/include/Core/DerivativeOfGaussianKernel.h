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