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

#include "LaplacianKernel.h"

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
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(T));				
		}
		return *this;
	}


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

	template <class T, class U>
	LaplacianKernel<T, U>::~LaplacianKernel(void)
	{	

	}




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_LaplacianKernel_HPP_