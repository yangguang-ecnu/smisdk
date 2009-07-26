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
// Filename:	CentralDifferenceDerivativeKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_CentralDifferenceDerivativeKernel_HPP_
#define _PG_CentralDifferenceDerivativeKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/CentralDifferenceDerivativeKernel.h"

namespace PGCore
{

	template <class T, class U>
	CentralDifferenceDerivativeKernel<T, U>::CentralDifferenceDerivativeKernel(bool iOrientY/*=false*/)
	{		
		unsigned int iDimension = 3;
		m_orientY = iOrientY;
		m_separable = false;
		SetDimension(iDimension);	
		Initialize_();
	}



	template <class T, class U>
	CentralDifferenceDerivativeKernel<T, U>::CentralDifferenceDerivativeKernel(const CentralDifferenceDerivativeKernel<T, U>& iKernelObject) 
	{
		*this = iKernelObject;
	}

	template <class T, class U>
	CentralDifferenceDerivativeKernel<T, U> &CentralDifferenceDerivativeKernel<T, U>::operator=(const CentralDifferenceDerivativeKernel<T, U> &iKernelObject) 
	{	
		unsigned int iDimension = iKernelObject.GetDimension();		
		if(SetDimension(iDimension)) {
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(float));				
		}
		return *this;
	}


	template <class T, class U>
	bool CentralDifferenceDerivativeKernel<T, U>::Initialize_()
	{
		if (m_dimension<3) return false;

		// already called when initializing
		//Reset();

		// 0 0 0
		// -0.5 0 0.5
		// 0 0 0

		if (!m_orientY)
		{
			// fill up the buffer here
			
			SetValue(1, 0, (-0.5f));
			SetValue(1, 2, (0.5f));   
			

		} else 
		{
			// fill up the buffer here
			SetValue(0, 1, (-0.5f));
			SetValue(2, 1, (0.5f));
		}
		return true;
	}

	template <class T, class U>
	CentralDifferenceDerivativeKernel<T, U>::~CentralDifferenceDerivativeKernel(void)
	{	

	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class CentralDifferenceDerivativeKernel<short, short>; 
	template class CentralDifferenceDerivativeKernel<unsigned char, unsigned char>; 
	template class CentralDifferenceDerivativeKernel<char, char>; 
	template class CentralDifferenceDerivativeKernel<char, short>; 
	template class CentralDifferenceDerivativeKernel<double, short>; 
	template class CentralDifferenceDerivativeKernel<float, float>; 
	template class CentralDifferenceDerivativeKernel<double, double>; 
	template class CentralDifferenceDerivativeKernel<char, PGMath::Point3D<unsigned char>>; 
#endif



};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_CentralDifferenceDerivativeKernel_HPP_