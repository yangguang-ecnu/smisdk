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
// Filename:	Vector3D.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/Vector3D.h"

namespace PGMath
{
	template <class T>
	inline bool Vector3D<T>::Normalize()
	{
		double len = (double)Length();
		if (len < 0.0f )
		{
			return false;
		}

		m_x = T((double)(m_x)/len);
		m_y = T((double)(m_y)/len);
		m_z = T((double)(m_z)/len);

		return true;
	}

	template <class T>
	const T& Vector3D<T>::X() const 
	{
		return m_x;
	}

	template <class T>
	const T& Vector3D<T>::Y() const 
	{
		return m_y;
	}

	template <class T>
	const T& Vector3D<T>::Z() const 
	{
		return m_z;
	}

	template <class T>
	const T& Vector3D<T>::W() const 
	{
		return m_w;
	}

	template <class T>
	T Vector3D<T>::Length() const 
	{
		double prod = double(m_x)*double(m_x) + double(m_y)*double(m_y) + double(m_z)*double(m_z);
		return (T)(sqrt((double)(prod)));
	}

	template <class T>
	T Vector3D<T>::LengthSquared() const 
	{
		double prod = double(m_x)*double(m_x) + double(m_y)*double(m_y) + double(m_z)*double(m_z);
		return (T)(prod);
	}

	template <class T>
	Vector3D<T>::~Vector3D(void) {};



	template <class T>
	Vector3D<T>::Vector3D()
	{
		m_x = (T)0;
		m_y = (T)0;
		m_z = (T)0;
		m_w = (T)0; // 0 for it to be a vector
	}


	template <class T>
	Vector3D<T>::Vector3D(const T& iX, const T& iY, const T& iZ)
	{
		m_x = iX;
		m_y = iY;
		m_z = iZ;
		m_w = (T)0; // 0 for it to be a vector
	}

	template <class T>
	Vector3D<T>::Vector3D(const Vector3D<T>& iVector)
	{
		*this = iVector;
	}

	template <class T>
	Vector3D<T> &Vector3D<T>::operator=(const Vector3D<T> &iVector) 
	{	
		m_x = iVector.X();
		m_y = iVector.Y();
		m_z = iVector.Z();
		m_w = iVector.W();
		return *this;
	}

	template <class T>
	Vector3D<T> Vector3D<T>::operator*(const float &iScalarValue) 
	{	
		T oX =  (T)(iScalarValue*m_x);
		T oY =  (T)(iScalarValue*m_y);
		T oZ =  (T)(iScalarValue*m_z);		
		return Vector3D<T>(oX, oY, oZ);
	}

	template <class T>
	Vector3D<T> Vector3D<T>::operator+(const Vector3D<T> &iVector) 
	{	
		T oX =  m_x + iVector.X();
		T oY =  m_y + iVector.Y();
		T oZ =  m_z + iVector.Z();	
		return Vector3D<T>(oX, oY, oZ);
	}

	template <class T>
	Vector3D<T> Vector3D<T>::operator/(const long double &iDivisor) 
	{	
		if ((long double)iDivisor<=0.00001f)
		{
			return *this;
		}

		T oX =  (T)((long double)(m_x)/iDivisor);
		T oY =  (T)((long double)(m_y)/iDivisor);
		T oZ =  (T)((long double)(m_z)/iDivisor);
		return Vector3D<T>(oX, oY, oZ);
	}

	template <class T>
	Vector3D<T> Vector3D<T>::operator-(const Vector3D<T> &iVector) 
	{	
		T oX =  m_x - iVector.X();
		T oY =  m_y - iVector.Y();
		T oZ =  m_z - iVector.Z();	
		return Vector3D<T>(oX, oY, oZ);
	}

	template <class T>
	T Vector3D<T>::operator*(const Vector3D<T>& iVector) // dot product
	{
		T oX =  m_x * iVector.X();
		T oY =  m_y * iVector.Y();
		T oZ =  m_z * iVector.Z();	
		return (oX + oY + oZ);
	}

	template <class T>
	Vector3D<T> Vector3D<T>::operator%(const Vector3D<T>& iVector) // dot product
	{
		T oX =  m_x * iVector.X();
		T oY =  m_y * iVector.Y();
		T oZ =  m_z * iVector.Z();	
		return Vector3D<T>(oX, oY, oZ);
	}


	template <class T>
	Vector3D<T> Vector3D<T>::operator^(const Vector3D<T>& iVector) // cross product
	{	
		T oX =  m_y * iVector.Z() - m_z * iVector.Y();
		T oY =  m_z * iVector.X() - m_x * iVector.Z();
		T oZ =  m_x * iVector.Y() - m_y * iVector.X();
		return Vector3D<T>(oX, oY, oZ);
	}

	template <class T>
	Vector3D<T> Vector3D<T>::operator-() 
	{	
		T oX =  -m_x;
		T oY =  -m_y;
		T oZ =  -m_z;	
		return Vector3D<T>(oX, oY, oZ);
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class Vector3D<unsigned char>; 
	template class Vector3D<short>; 
	template class Vector3D<char>; 
	template class Vector3D<unsigned short>; 	
	template class Vector3D<int>; 	
	template class Vector3D<long>; 	
	template class Vector3D<float>; 	
	template class Vector3D<double>; 
#endif

};


//////////////////////////////////////////////////////////////////////////
// EOF

