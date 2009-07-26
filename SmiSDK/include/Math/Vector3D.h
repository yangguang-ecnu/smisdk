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
