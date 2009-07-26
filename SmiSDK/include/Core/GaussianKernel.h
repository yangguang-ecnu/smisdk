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