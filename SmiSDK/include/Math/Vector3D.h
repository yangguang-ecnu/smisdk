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
// Filename:	Vector3D.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGVector3D_H_
#define _PGVector3D_H_
//////////////////////////////////////////////////////////////////////////

#include "Core/Core.h"

namespace PGMath
{

template <class T>
class Vector3D
{
public:
	Vector3D();	
	Vector3D(const T& iX, const T& iY, const T& iZ);	
	Vector3D(const Vector3D<T>& iVector);	
	Vector3D<T>& operator=(const Vector3D<T>& iVector);

	Vector3D<T> operator+(const Vector3D<T>& iVector);
	Vector3D<T> operator-(const Vector3D<T>& iVector);
	Vector3D<T> operator/(const long double &iDivisor);
	Vector3D<T> operator*(const float& iScalarValue);
	Vector3D<T> operator-(); // negation
	Vector3D<T> operator^(const Vector3D<T>& iVector); // cross product
	T operator*(const Vector3D<T>& iVector); // dot product
	Vector3D<T> operator%(const Vector3D<T>& iVector); // element wise product

	class BinPred : public std::binary_function< Vector3D<T>, Vector3D<T>, bool >
	{
	public:
		bool operator() ( const Vector3D<T>& iA , const Vector3D<T>& iB )
		{
			return iA.Length() < iB.Length();
		}
	};

	inline bool Normalize();
	
	const T& X() const;

	const T& Y() const;

	const T& Z() const;

	const T& W() const;

	T Length() const;

	T LengthSquared() const;

	~Vector3D(void);

protected:	
	
	T			m_x;
	T			m_y;
	T			m_z;
	T			m_w;

};


};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Vector3D.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGVector3D_H_
