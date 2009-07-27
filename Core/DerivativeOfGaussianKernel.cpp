// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
// Filename:	DerivativeOfGaussianKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_DerivativeOfGaussianKernel_HPP_
#define _PG_DerivativeOfGaussianKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/DerivativeOfGaussianKernel.h"
#include "Core/GaussianKernel.h"

namespace PGCore
{

template <class T, class U>
const double& DerivativeOfGaussianKernel<T, U>::GetSigma() const
{ 
	return m_sigma; 
}

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
		memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(float));		
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

   PGCore::Image<float> tempKernel(1, m_dimension);
   float* tempBuf = tempKernel.GetBuffer();
   if (!tempBuf)
   {
		return false;
   }

	// fill up the buffer here
   int i, center;
   double x, fx, sum=0.0;

   center = (m_dimension) / 2;

   //if(VERBOSE) printf("      The kernel has %d elements.\n", *windowsize);   

   for(i=0;i<(m_dimension);i++)
   {
      x = (double)(i - center);
      fx = pow((double)2.71828f, (double)(-0.5f*x*x/(m_sigma*m_sigma))) / (m_sigma * sqrt(6.2831853f));
	  tempBuf[i] = fx;
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
   
#ifdef _DEBUG
   LOG0("The DerivativeOfGaussianKernel filter coefficients before derivative are:");
   for(i=0;i<(m_dimension);i++)
	   LOG2("Kernel[%d] = %f", i, (double)tempBuf[i]);
#endif

   // compute derivative
   	PGCore::CentralDifferenceDerivativeKernel<float, float> cddkernelX;
   //PGCore::DiffOfGaussianDerivativeKernel<T, T> cddkernelX;
   
	PGCore::Image<float> oImageDeriv = tempKernel;
	cddkernelX.Convolve(tempKernel, oImageDeriv);		
	

	float *derivKernel = oImageDeriv.GetBuffer();
	if (!derivKernel)
	{
		return false;
	}

   memcpy(m_buffer, derivKernel, m_dimension*sizeof(float));

   /*for(int i=0;i<(m_dimension);i++)
   {
		m_buffer[i]*=5;//m_dimension;		
   }*/
		


#ifdef _DEBUG
	LOG1("The DerivativeOfGaussianKernel filter coefficients after derivative are (Sigma: %3.4f):", m_sigma);
	for(i=0;i<(m_dimension);i++)
	   LOG2("Kernel[%d] = %f", i, (double)m_buffer[i]);
#endif

   // compute derivative
   if (0)
   {
	   PGCore::GaussianKernel<float, float> gkernel;
	   gkernel.Convolve(oImageDeriv, tempKernel);		


	   derivKernel = tempKernel.GetBuffer();
	   if (!derivKernel)
	   {
		   return false;
	   }

	   memcpy(m_buffer, derivKernel, m_dimension*sizeof(float));

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


#ifdef _PG_GENERATE_SDK_LIBS_
	template class DerivativeOfGaussianKernel<short, short>; 
	template class DerivativeOfGaussianKernel<unsigned char, unsigned char>; 
	template class DerivativeOfGaussianKernel<unsigned short, unsigned short>; 
	template class DerivativeOfGaussianKernel<char, char>; 
	template class DerivativeOfGaussianKernel<double, double>; 
	template class DerivativeOfGaussianKernel<double, unsigned char>; 
	template class DerivativeOfGaussianKernel<char, short>; 
	template class DerivativeOfGaussianKernel<float, float>; 
	template class DerivativeOfGaussianKernel<double, short>; 
	template class DerivativeOfGaussianKernel<char, PGMath::Point3D<unsigned char>>; 
#endif



};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DerivativeOfGaussianKernel_HPP_