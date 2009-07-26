// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	TransformBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/TransformBase.h"

namespace PGMath
{

	template <class T>
	TransformBase<T>::TransformBase() 
	{
	};
	
	template <class T>
	bool TransformBase<T>::SetOrigin(const PGMath::Point3D<T>& iOrigin)
	{
		m_origin = iOrigin;
		return true;
	}

	template <class T>
	const PGMath::Point3D<T>& TransformBase<T>::GetOrigin() const 
	{
		return m_origin;
	}

	template <class T>
	TransformBase<T>::~TransformBase(void) 
	{
		
	};

	
#ifdef _PG_GENERATE_SDK_LIBS_
	template class TransformBase<float>; 	
#endif


};
//////////////////////////////////////////////////////////////////////////
// EOF

