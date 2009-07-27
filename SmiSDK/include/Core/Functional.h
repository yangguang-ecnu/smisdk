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
// Filename:	Functional.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Functional_H_
#define _PG_Functional_H_
//////////////////////////////////////////////////////////////////////////

#include "ObjectBase.h"
#include "BaseDataObject.h"
#include "PixelBase.h"

namespace PGCore
{

	template <class T>
	class Functional
	{
	public:
		Functional() {};

		~Functional() {};

		static const T& Min(const T& iObjectA, const T& iObjectB);

		static const T& Max(const T& iObjectA, const T& iObjectB);		
	};
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Functional.hpp"
#endif
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Functional_H_