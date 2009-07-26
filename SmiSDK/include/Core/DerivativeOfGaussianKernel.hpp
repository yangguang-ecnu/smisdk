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
#ifndef _PG_DerivativeOfGaussianKernel_HPP_
#define _PG_DerivativeOfGaussianKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "DerivativeOfGaussianKernel.h"

namespace PGCore
{

template <class T, class U>
DerivativeOfGaussianKernel<T, U>::DerivativeOfGaussianKernel(const double iSigma/*=1*/, const int iDimension/*=-1*/)
{	
	m_sigma = iSigma;
	m_separable = true;
	//m_kernelType = kPgKernelType1D;
	unsigned int pDimension = 1 + 2 * ceil(2.5f * m_sigma);	
	
	if (iDimension>=5)// && iDimension > pDimension)
	{
		pDimension = iDimension;
	}

	SetDimension(pDimension);	
	Initialize_();
}


template <class T, class U>
DerivativeOfGaussianKernel<T, U>::DerivativeOfGaussianKernel(const DerivativeOfGaussianKernel<T, U>& iKernelObject) 
{
	*this = iKernelObject;
}


template <class T, class U>
DerivativeOfGaussianKernel<T, U> &DerivativeOfGaussianKernel<T, U>::operator=(const DerivativeOfGaussianKernel<T, U> &iKernelObject) 
{	
	unsigned int iDimension = iKernelObject.GetDimension();		
	if(SetDimension(iDimension)) 
	{
		memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(T));		
		m_sigma = iKernelObject.GetSigma();
	}
	return *this;
}


template <class T, class U>
bool DerivativeOfGaussianKernel<T, U>::Initialize_()
{
   if (m_sigma<=0.0f)
   {
    	return false;
   }

   PGCore::Image<T> tempKernel(1, m_dimension);
   T* tempBuf = tempKernel.GetBuffer();
   if (!tempBuf)
   {
		return false;
   }

	// fill up the buffer here
   int i, center;
   double x, fx, sum=0.0;

   center = (m_dimension) / 2;

   //if(VERBOSE) printf("      The kernel has %d elements.\n", *windowsize);   

   for(i=0;i<(m_dimension);i++){
      x = (double)(i - center);
      fx = pow((double)2.71828f, (double)(-0.5f*x*x/(m_sigma*m_sigma))) / (m_sigma * sqrt(6.2831853f));
	  tempBuf[i] = (T)fx;
      //SetValue(0, i, (T)fx);
      sum += fx;
   }

   if (sum>0.0f)
   {
		for(i=0;i<(m_dimension);i++) 
		{
			tempBuf[i]/= sum;
			//SetValue(0, i, GetValue(0, i)/sum);
			//tempBuf[i]= (i==m_dimension/2);
		}
   }
   
    //LOG0("The filter coefficients before derivative are:");
    //for(i=0;i<(m_dimension);i++)
	//   LOG2("Kernel[%d] = %f", i, (double)tempBuf[i]);

   // compute derivative
   	PGCore::CentralDifferenceDerivativeKernel<T, T> cddkernelX;
   //PGCore::DiffOfGaussianDerivativeKernel<T, T> cddkernelX;
   
	PGCore::Image<T> oImageDeriv = tempKernel;
	cddkernelX.Convolve(tempKernel, oImageDeriv);		
	

	T *derivKernel = oImageDeriv.GetBuffer();
	if (!derivKernel)
	{
		return false;
	}

   memcpy(m_buffer, derivKernel, m_dimension*sizeof(T));

	//LOG1("The filter coefficients after derivative are (Sigma: %3.4f):", m_sigma);
	//for(i=0;i<(m_dimension);i++)
	//   LOG2("Kernel[%d] = %f", i, (double)GetValue(0, i));

   // compute derivative
   if (0)
   {
	   PGCore::GaussianKernel<T, T> gkernel;
	   gkernel.Convolve(oImageDeriv, tempKernel);		


	   derivKernel = tempKernel.GetBuffer();
	   if (!derivKernel)
	   {
		   return false;
	   }

	   memcpy(m_buffer, derivKernel, m_dimension*sizeof(T));

	   //LOG1("The filter coefficients after derivative and smoothing are (Sigma: %3.4f):", m_sigma);
	   //for(i=0;i<(m_dimension);i++)
		//   LOG2("Kernel[%d] = %f", i, (double)GetValue(0, i));
   }

   return true;
}


template <class T, class U>
DerivativeOfGaussianKernel<T, U>::~DerivativeOfGaussianKernel(void)
{	
	
}




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DerivativeOfGaussianKernel_HPP_