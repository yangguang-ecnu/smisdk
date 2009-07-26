// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
// 
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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

