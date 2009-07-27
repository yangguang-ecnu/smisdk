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