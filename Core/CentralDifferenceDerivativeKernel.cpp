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