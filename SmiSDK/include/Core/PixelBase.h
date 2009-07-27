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
// Filename:	PixelBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGPixelBase_H_
#define _PGPixelBase_H_
//////////////////////////////////////////////////////////////////////////


#include "BaseDataObject.h"
#include "Math/Point3D.h"

using namespace PGMath;

namespace PGCore
{

typedef Point3D<char> PgChar_RGBAPixel;

typedef Point3D<unsigned char> PgUChar_RGBAPixel;

typedef Point3D<unsigned short> PgUShort_RGBAPixel;

template <class T>
class PixelBase: public BaseDataObject
{
public:
	PixelBase();	
	PixelBase(const PixelBase<T>& iObject);	
	PixelBase<T>& operator=(const PixelBase<T>& iPixel);
	
	const T& GetValue() const 
	{
		return m_value;
	}

	void SetValue(const T& iValue)  
	{
		m_value = iValue;
	}

	const T GetMinValue() const; 
	
	const T GetMaxValue() const; 
	

	virtual ~PixelBase(void);
private:
	T	m_value;	
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "PixelBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGPixelBase_H_
