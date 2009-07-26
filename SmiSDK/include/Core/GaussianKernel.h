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