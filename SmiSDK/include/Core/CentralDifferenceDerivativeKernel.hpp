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
// Filename:	CentralDifferenceDerivativeKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_CentralDifferenceDerivativeKernel_HPP_
#define _PG_CentralDifferenceDerivativeKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "CentralDifferenceDerivativeKernel.h"

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
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*sizeof(T));				
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
			
			SetValue(1, 0, (T)(-0.5f));
			SetValue(1, 2, (T)(0.5f));   
			

		} else 
		{
			// fill up the buffer here
			SetValue(0, 1, (T)(-0.5f));
			SetValue(2, 1, (T)(0.5f));
		}
		return true;
	}

	template <class T, class U>
	CentralDifferenceDerivativeKernel<T, U>::~CentralDifferenceDerivativeKernel(void)
	{	

	}




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_CentralDifferenceDerivativeKernel_HPP_