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
#ifndef _PG_LaplacianKernel_HPP_
#define _PG_LaplacianKernel_HPP_
//////////////////////////////////////////////////////////////////////////

// A laplacian in n-dimensional Euclidean space is the divergence of the gradient
// Specifically, it is the sum of all unmixed partial second derivatives of a function
// It is thus the 'Trace' of the 'Hessian' matrix of a function
// The 'trace' of a square matrix is the sum of all its main-diagonal elements (top left to bottom right)
// A boundary exists where the laplacian is zero (maximum gradient)
// Laplacian is rotation-invariant

#include "Core/LaplacianKernel.h"

namespace PGCore
{

	template <class T, class U>
	LaplacianKernel<T, U>::LaplacianKernel()
	{		
		unsigned int iDimension = 3;
		
		m_separable = false;
		SetDimension(iDimension);	
		Initialize_();
	}

	template <class T, class U>
	LaplacianKernel<T, U>::LaplacianKernel(const LaplacianKernel<T, U>& iKernelObject) 
	{
		*this = iKernelObject;
	}

	template <class T, class U>
	LaplacianKernel<T, U> &LaplacianKernel<T, U>::operator=(const LaplacianKernel<T, U> &iKernelObject) 
	{	
		unsigned int iDimension = iKernelObject.GetDimension();		
		if(SetDimension(iDimension)) {
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(float));				
		}
		return *this;
	}


	/*
	template <class T, class U>
	bool LaplacianKernel<T, U>::initialize()
	{
		if (m_dimension<3) return false;

		// already called when initializing
		Reset(-1.0f);

		// 0 0 0
		// -0.5 0 0.5
		// 0 0 0		
		{
			// fill up the buffer here
			SetValue(1, 1, (T)(8.0f));
			
		}
		return true;
	}
*/

	template <class T, class U>
	LaplacianKernel<T, U>::~LaplacianKernel(void)
	{	

	}

	
	template <class T, class U>
	bool LaplacianKernel<T, U>::Initialize_()
	{
		return false;
	}
	

#ifdef _PG_GENERATE_SDK_LIBS_
	template class LaplacianKernel<unsigned short, unsigned short>; 
	template class LaplacianKernel<unsigned char, unsigned char>; 
	template class LaplacianKernel<short, short>; 
	template class LaplacianKernel<char, char>; 
	//template class LaplacianKernel<double, double>; 
	template class LaplacianKernel<char, short>; 
	template class LaplacianKernel<double, short>; 
	template class LaplacianKernel<char, PGMath::Point3D<unsigned char>>; 
#endif


};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_LaplacianKernel_HPP_