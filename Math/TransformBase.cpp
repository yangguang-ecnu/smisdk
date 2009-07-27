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

