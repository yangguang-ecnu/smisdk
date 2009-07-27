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
// Filename:	DegreesOfFreedomBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/DegreesOfFreedomBase.h"

namespace PGMath
{
	

	template <class T>
	DegreesOfFreedomBase<T>::DegreesOfFreedomBase() 
	{
		m_parameters.reserve(kPgDegreeOfFreedomTotal);
		int i=0;
		for (i=0; i<6; i++)
		{
			m_parameters.push_back(0.0f);
		}
		
		// scaling
		for (i=6; i<9; i++)
		{
			m_parameters.push_back(1.0f);
		}
		
		// shear
		for (i=9; i<kPgDegreeOfFreedomTotal; i++)
		{
			m_parameters.push_back(0.0f);
		}		
		
	};	

	template <class T>
	DegreesOfFreedomBase<T>::DegreesOfFreedomBase(const std::vector<T>& iParameters) 
	{
		if (iParameters.size()==kPgDegreeOfFreedomTotal)
		{
			m_parameters = iParameters;	
		}			
	};	

	
	template <class T>
	bool DegreesOfFreedomBase<T>::SetParameter(unsigned int iIndex, const T& iParameter)
	{
		if (iIndex> (m_parameters.size()-1)) {
			return false;
		}
		m_parameters[iIndex] = iParameter;
		return true;
	}
	
	template <class T>
	const T DegreesOfFreedomBase<T>::GetParameter(unsigned int iIndex) const
	{
		if (iIndex> (m_parameters.size()-1)) {
			return (T)0;
		}
		return m_parameters[iIndex];
	}

	template <class T>
	const PGMath::Point3D<T>& DegreesOfFreedomBase<T>::GetOrigin() const 
	{
		return m_origin;
	}

	template <class T>
	void DegreesOfFreedomBase<T>::SetOrigin(const PGMath::Point3D<T>& iOrigin) 
	{
		m_origin = iOrigin;
	}

	template <class T>
	DegreesOfFreedomBase<T>::~DegreesOfFreedomBase(void) {}; 
	
#ifdef _PG_GENERATE_SDK_LIBS_
	template class DegreesOfFreedomBase<float>; 	
#endif

};


//////////////////////////////////////////////////////////////////////////
// EOF

