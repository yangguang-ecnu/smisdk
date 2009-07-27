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
// Filename:	Point3D.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGPoint3D_H_
#define _PGPoint3D_H_
//////////////////////////////////////////////////////////////////////////
#include "Vector3D.h"

namespace PGMath
{

template <class T>
class Point3D : public Vector3D<T>
{
public:
	Point3D();	
	Point3D(const T& iXYZW);
	Point3D(const T& iX, const T& iY, const T& iZ, const T iW=1);	
	Point3D(const Point3D<T>& iPoint);	
	Point3D<T>& operator=(const Point3D<T>& iPoint);	

	T& operator[](const int& iIndex);
	Point3D<T> operator*(const float& iScalarValue);
	Point3D<T> operator/(const Point3D<T>& iVector);
	Point3D<T> operator/=(const float& iScalarValue);
	Point3D<T> operator*(const Point3D<T>& iVector);// element wise product
	Point3D<T> operator+(const Point3D<T>& iVector);
	Point3D<T> operator+=(const Point3D<T>& iVector);
	Point3D<T> operator-(const Point3D<T>& iVector);
	bool	   operator>(const Point3D<T>& iVector);
	Point3D<T> operator>>(const int& iPositions);// bit shift
	bool	   operator>=(const Point3D<T>& iVector);
	bool       operator<(const Point3D<T>& iVector);
	bool       operator<=(const Point3D<T>& iVector);
	bool       operator==(const Point3D<T>& iVector);
	Point3D<T> operator-(); // negation

	~Point3D(void) {};

};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Point3D.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGPoint3D_H_
