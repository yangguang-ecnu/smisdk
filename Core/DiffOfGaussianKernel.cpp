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
// Filename:	DiffOfGaussianKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_DiffOfGaussianKernel_HPP_
#define _PG_DiffOfGaussianKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/DiffOfGaussianKernel.h"
#include "Core/GaussianKernel.h"

namespace PGCore
{
	template <class T, class U>
	const double& DiffOfGaussianKernel<T, U>::GetSigmaOuter() const 
	{ 
		return m_sigmaOuter; 
	}

	template <class T, class U>
	const double& DiffOfGaussianKernel<T, U>::GetSigmaInner() const 
	{ 
		return m_sigmaInner; 
	}

	template <class T, class U>
	DiffOfGaussianKernel<T, U>::DiffOfGaussianKernel(const double iSigmaOuter/*=2*/, const double iSigmaInner/*=1*/, const int iDimension/*=-1*/)
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

		SetDimension(pDimension);	
		Initialize_();
	}



	template <class T, class U>
	DiffOfGaussianKernel<T, U>::DiffOfGaussianKernel(const DiffOfGaussianKernel<T, U>& iKernelObject) 
	{
		*this = iKernelObject;
	}

	template <class T, class U>
	DiffOfGaussianKernel<T, U> &DiffOfGaussianKernel<T, U>::operator=(const DiffOfGaussianKernel<T, U> &iKernelObject) 
	{	
		unsigned int iDimension = iKernelObject.GetDimension();		
		if(SetDimension(iDimension)) {
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(float));				
		}
		return *this;
	}


	template <class T, class U>
	bool DiffOfGaussianKernel<T, U>::Initialize_()
	{
		//if (m_dimension<5) return false;

		// create two gaussian deriv kernels

		PGCore::GaussianKernel<T, U> kernelOuter(m_sigmaOuter, m_dimension);
		PGCore::GaussianKernel<T, U> kernelInner(m_sigmaInner, m_dimension);
	
		

		kernelOuter-= kernelInner;
		
		memcpy(m_buffer, kernelOuter.GetBuffer(), m_dimension*sizeof(float));
		
		for(int i=0;i<(m_dimension);i++)
		{
			m_buffer[i]*=10;//m_dimension;		
		}
		

		/*
		float sum=0;
		for(int i=0;i<(m_dimension);i++)
		{
			m_buffer[i]*=50;//m_dimension;
			sum += m_buffer[i];			
		}
		
		if (sum<0)
		{
			float offset=abs(sum)/m_dimension;
			for (int i=0; i<(m_dimension); i++)
			{
				m_buffer[i] = offset;
			}			
		}		
		*/

#ifdef _DEBUG
		LOG0("{ The filter coefficients difference of gaussian are:");
		for(int i=0;i<(m_dimension);i++)
		{
			LOG2("Kernel[%d] = %f", i, (double)m_buffer[i]);
		}
		LOG0("} The filter coefficients.");
#endif

		//Reset()

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
	DiffOfGaussianKernel<T, U>::~DiffOfGaussianKernel(void)
	{	

	}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class DiffOfGaussianKernel<unsigned short, unsigned short>; 
	template class DiffOfGaussianKernel<short, short>; 
	template class DiffOfGaussianKernel<unsigned char, unsigned char>; 
	template class DiffOfGaussianKernel<char, char>; 
	template class DiffOfGaussianKernel<char, short>; 
	template class DiffOfGaussianKernel<double, short>; 
	template class DiffOfGaussianKernel<float, float>; 
	template class DiffOfGaussianKernel<double, double>; 
	template class DiffOfGaussianKernel<char, PGMath::Point3D<unsigned char>>; 
#endif


};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DiffOfGaussianKernel_HPP_