// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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
// Filename:	Matrix4x4.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGMatrix4x4_H_
#define _PGMatrix4x4_H_
//////////////////////////////////////////////////////////////////////////

#include "Point3D.h"

namespace PGMath
{

template <class T>
class Matrix4x4
{
public:
	Matrix4x4();		
	Matrix4x4(const T iMatrix[16]);		
	Matrix4x4(const Matrix4x4<T>& iMatrix4x4);	
	Matrix4x4<T>& operator=(const Matrix4x4<T>& iMatrix4x4);
	~Matrix4x4(void) {};

	bool		SetElement(const unsigned int iRow, const unsigned int iColumn, const T&iValue);
	const T		GetElement(const unsigned int iRow, const unsigned int iColumn) const;
	T& operator[](const unsigned int iIndex);  
	const T* Matrix() const; 

	bool IsIdentity() const;
	bool IsInvertible(T *oDeterminant =0) const;

	bool Invert(Matrix4x4<T>& oMatrix) const;
	bool Adjoint(Matrix4x4<T>& oMatrix) const;
	bool Transpose(Matrix4x4<T>& oMatrix) const;

	void Identity();

	const T Determinant() const;
	
	
	bool Multiply(const Matrix4x4<T>& iMatrix, Matrix4x4<T>& oMatrix) const;
	bool Multiply(const Vector3D<T>& iVector, Vector3D<T>& oVector) const;
	bool Multiply(const Point3D<T>& iPoint, Point3D<T>& oPoint) const;

	// eigen vector analysis?

private:	
	// To access a T[16] as a T[4][4] array
	typedef T	(*m_ptr4x4)[4];

	// row major
	T			m_matrix[16];

	T	det3x3(const T &a1, const T & a2, const T & a3, 
               const T & b1, const T & b2, const T & b3, 
               const T & c1, const T & c2, const T & c3) const; 

	T	det2x2(const T & a, const T & b, const T & c, const T & d) const; 
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Matrix4x4.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGMatrix4x4_H_
