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

