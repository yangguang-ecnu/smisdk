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
#ifndef _PG_DiffOfGaussianDerivativeKernel_HPP_
#define _PG_DiffOfGaussianDerivativeKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "DiffOfGaussianDerivativeKernel.h"
// similar to the laplacian of a function
// more so because it approxmates the divergence of gradient of teh function by taking differences
// in gaussian derivatives at a given point

namespace PGCore
{
	template <class T, class U>
	DiffOfGaussianDerivativeKernel<T, U>::DiffOfGaussianDerivativeKernel(const double iSigmaOuter/*=2*/, const double iSigmaInner/*=1*/, const int iDimension/*=-1*/)
	{	
		if (iSigmaInner>=iSigmaOuter)
			return;

		m_sigmaOuter = iSigmaOuter;
		m_sigmaInner = iSigmaInner;		
		
		m_separable = true;//false;
		//m_orientY = iOrientY;

		unsigned int pDimension = 1 + 2 * ceil(2.5f * m_sigmaOuter);	

		if (iDimension>=5)
		{
			pDimension = iDimension;
		}

		SetDimension(iDimension);	
		Initialize_();
	}



	template <class T, class U>
	DiffOfGaussianDerivativeKernel<T, U>::DiffOfGaussianDerivativeKernel(const DiffOfGaussianDerivativeKernel<T, U>& iKernelObject) 
	{
		*this = iKernelObject;
	}

	template <class T, class U>
	DiffOfGaussianDerivativeKernel<T, U> &DiffOfGaussianDerivativeKernel<T, U>::operator=(const DiffOfGaussianDerivativeKernel<T, U> &iKernelObject) 
	{	
		unsigned int iDimension = iKernelObject.GetDimension();		
		if(SetDimension(iDimension)) {
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(T));				
		}
		return *this;
	}


	template <class T, class U>
	bool DiffOfGaussianDerivativeKernel<T, U>::Initialize_()
	{
		//if (m_dimension<5) return false;

		// create two gaussian deriv kernels

		PGCore::DerivativeOfGaussianKernel<T, U> kernelOuter(m_sigmaOuter, m_dimension);
		PGCore::DerivativeOfGaussianKernel<T, U> kernelInner(m_sigmaInner, m_dimension);
	
		/*
		LOG0("The filter coefficients for BEFORE difference of gaussian derivatives are:");
		for(int i=0;i<(m_dimension);i++)
		{
			m_buffer[i]*= 100.0f;
			LOG2("Kernel[%d] = %f", i, 100.0f*(double)kernelOuter.GetBuffer()[i]);
		}*/

		kernelOuter-= kernelInner;
		
		memcpy(m_buffer, kernelOuter.GetBuffer(), m_dimension*sizeof(T));
		
		double scale = m_dimension/3.0f;
		//LOG0("The filter coefficients for AFTER difference of gaussian derivatives are:");
		for(int i=0;i<(m_dimension);i++)
		{
			m_buffer[i]*= scale;
			//LOG2("Kernel[%d] = %f", i, (double)m_buffer[i]);
		}

		//Reset();


		// experimental
		//  0	-1	-2	 -1	 0
		// -1	-2	-4	 -2	 1
		// -2	-4	 0	  4	 2
		// -1	 2	 4	  2	 1
		//  0	 1	 2	  1	 0
/*
		if (!m_orientY)
		{

			// fill up the buffer here
			SetValue(1, 0, (T)(-1.0f)); SetValue(3, 0, (T)(-1.0f));
			SetValue(1, 1, (T)(-2.0f)); SetValue(3, 1, (T)(-2.0f));
			SetValue(1, 3, (T)(2.0f));  SetValue(3, 3, (T)(2.0f));
			SetValue(1, 4, (T)(1.0f));  SetValue(3, 4, (T)(1.0f));

			SetValue(2, 0, (T)(-2.0f)); 
			SetValue(2, 1, (T)(-4.0f)); 
			SetValue(2, 3, (T)(4.0f));  
			SetValue(2, 4, (T)(2.0f));  

		} else
		{
			// fill up the buffer here
			SetValue(0, 1, (T)(-1.0f)); SetValue(0, 3, (T)(-1.0f));
			SetValue(1, 1, (T)(-2.0f)); SetValue(1, 3, (T)(-2.0f));
			SetValue(3, 1, (T)(2.0f));  SetValue(3, 3, (T)(2.0f));
			SetValue(4, 1, (T)(1.0f));  SetValue(4, 3, (T)(1.0f));

			SetValue(0, 2, (T)(-2.0f)); 
			SetValue(1, 2, (T)(-4.0f)); 
			SetValue(3, 2, (T)(4.0f));  
			SetValue(4, 2, (T)(2.0f));  

		}*/
		return true;
	}

	template <class T, class U>
	DiffOfGaussianDerivativeKernel<T, U>::~DiffOfGaussianDerivativeKernel(void)
	{	

	}




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DiffOfGaussianDerivativeKernel_HPP_