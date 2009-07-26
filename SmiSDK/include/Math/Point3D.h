// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
