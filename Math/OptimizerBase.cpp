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
// Filename:	OptimizerBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/OptimizerBase.h"

namespace PGMath
{

	template <class T>
	OptimizerBase<T>::OptimizerBase() { m_sourceModule = 0; m_objectiveFn = 0; };
	
	template <class T>
	bool OptimizerBase<T>::SetInput(void * iSourceModule, ObjectiveFunction iFunction, const std::vector<T>& iInitParameters)
	{
		m_sourceModule  =   iSourceModule;
		m_parameters	=	iInitParameters;
		m_objectiveFn	=	iFunction;
		return (!m_parameters.empty() && m_objectiveFn!=0);
	}

	//template <class T>
	//bool OptimizerBase<T>::Optimize();

	template <class T>
	const std::vector<T>& OptimizerBase<T>::GetParameters() const 
	{
		return m_parameters;
	}
	
	template <class T>
	OptimizerBase<T>::~OptimizerBase(void) {};

};
//////////////////////////////////////////////////////////////////////////
// EOF

