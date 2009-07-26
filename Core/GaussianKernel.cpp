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
// Filename:	GaussianKernel.hpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_GaussianKernel_HPP_
#define _PG_GaussianKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/GaussianKernel.h"

namespace PGCore
{

	
/******public
 * NAME
 *    GaussianKernel
 * SYNOPSIS
 *    GaussianKernel<T, U>::GaussianKernel(const double iSigma, const int iDimension) 
 * DESCRIPTION
 *    Class constructor
 * PARAMETERS
 *	  iSigma		:	Standard deviation value. Double precision. Optional. Default value: 1.0f
 *	  iDimension	:	Spread of the kernel in one dimension. Integer. Optional. Default value: -1.
 *							If iDimension==-1, the dimension is computed off the given sigma value.
 * OUTPUT
 *    An instance of the class
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *    GaussianKernel<double, double> gKernel(1.5f, 64);    
 ***/
	template <class T, class U>
	GaussianKernel<T, U>::GaussianKernel(const double iSigma/*=1.0f*/, const int iDimension/*=-1*/)
	{	
		m_sigma = iSigma;
		m_separable = true;

		// compute default dimension
		unsigned int pDimension = 1 + 2 * ceil(2.5f * m_sigma);	

		// override with user provided value if acceptable
		if (iDimension>=3)
		{
			pDimension = iDimension;
		}

		// set dimensions
		SetDimension(pDimension);	

		// initialize the buffer, fill kernel values
		Initialize_();
	}

	
/******public
 * NAME
 *    ~GaussianKernel
 * SYNOPSIS
 *    GaussianKernel<T, U>::~GaussianKernel(void) 
 * DESCRIPTION
 *    Class destructor
 * PARAMETERS
 *	  None.
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *     
 ***/	
	template <class T, class U>
	GaussianKernel<T, U>::~GaussianKernel(void)
	{	
		// no memory allocated, nothing to delete
	}

	
/******public
 * NAME
 *    GaussianKernel
 * SYNOPSIS
 *    GaussianKernel<T, U>::GaussianKernel(const GaussianKernel<T, U>& iKernelObject) 
 * DESCRIPTION
 *    Copy constructor
 * PARAMETERS
 *	  iKernelObject	:	an instance of a GaussianKernel object
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *    GaussianKernel<double, double> gKernel1(1.5f, 32); 
 *	  GaussianKernel<double, double> gKernel2(gKernel1); 
 ***/	
	template <class T, class U>
	GaussianKernel<T, U>::GaussianKernel(const GaussianKernel<T, U>& iKernelObject) 
	{
		// use assignment operator
		*this = iKernelObject;
	}


/******public
 * NAME
 *    operator=
 * SYNOPSIS
 *    GaussianKernel<T, U> &GaussianKernel<T, U>::operator=(const GaussianKernel<T, U> &iKernelObject) 
 * DESCRIPTION
 *    Assignment operator
 * PARAMETERS
 *	  iKernelObject	:	an instance of a GaussianKernel object
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    An instance of the class.
 * EXAMPLE
 *    GaussianKernel<double, double> gKernel1(1.5f, 32); 
 *	  GaussianKernel<double, double> gKernel2 = gKernel1; 
 ***/	
	template <class T, class U>
	GaussianKernel<T, U> &GaussianKernel<T, U>::operator=(const GaussianKernel<T, U> &iKernelObject) 
	{	
		unsigned int iDimension = iKernelObject.GetDimension();		

		if(SetDimension(iDimension)) 
		{
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(T));		
			m_sigma = iKernelObject.GetSigma();
		}

		return *this;
	}


/******protected
 * NAME
 *    Initialize
 * SYNOPSIS
 *    bool GaussianKernel<T, U>::Initialize_()
 * DESCRIPTION
 *    Initialization method. It computes and fills in the kernel values.
 * PARAMETERS
 *	  iKernelObject	:	an instance of a GaussianKernel object
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    An instance of the class.
 * EXAMPLE
 *     
 ***/
	template <class T, class U>
	bool GaussianKernel<T, U>::Initialize_()
	{
		if (m_sigma<=0.0f)
		{
			return false;
		}

		// fill up the buffer here
		int i, center;
		double x, fx, sum=0.0, invSigmaSqFactor = -0.5f/(m_sigma*m_sigma);
		double invFactor = (m_sigma * sqrt(6.2831853f));

		center = (m_dimension) / 2;

		for(i=0; i < m_dimension; i++)
		{
			x = (double)(i - center);
			fx = pow((double)2.71828f, (double)(x*x*invSigmaSqFactor)) * invFactor ;
			SetValue(0, i, (fx));
			sum += fx;
		}

		// normalize
		if (sum>0.0f)
		{
			for (i=0;i<(m_dimension);i++) 
			{
				SetValue(0, i, GetValue(0, i)/sum);
			}
		}

#ifdef _DEBUG
		if (0)
		{
			LOG1("The filter coefficients are (sigma: %3.5f):", m_sigma);
			for(i=0;i<(m_dimension);i++)
				LOG2("Kernel[%d] = %f", i, (double)GetValue(0, i));
		}
#endif
		return true;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class GaussianKernel<unsigned short, unsigned short>; 
	template class GaussianKernel<unsigned char, unsigned char>; 
	template class GaussianKernel<short, short>; 
	template class GaussianKernel<char, char>; 
	template class GaussianKernel<double, unsigned char>; 
	template class GaussianKernel<double, double>; 
	template class GaussianKernel<float, float>;
	template class GaussianKernel<char, short>; 
	template class GaussianKernel<double, short>; 
	template class GaussianKernel<char, PGMath::Point3D<unsigned char>>; 
#endif




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_GaussianKernel_HPP_