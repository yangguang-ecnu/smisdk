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
// Filename:	Point3D.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////
#include "Math/Point3D.h"

namespace PGMath
{
template <class T>
Point3D<T>::Point3D()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 0;
}

template <class T>
Point3D<T>::Point3D(const T& iXYZW)
{
	m_x = iXYZW;
	m_y = iXYZW;
	m_z = iXYZW;
	m_w = (T)iXYZW;
}

template <class T>
Point3D<T>::Point3D(const T& iX, const T& iY, const T& iZ, const T iW/*=1*/)
{
	m_x = iX;
	m_y = iY;
	m_z = iZ;
	m_w = (T)iW;
}

template <class T>
Point3D<T>::Point3D(const Point3D<T>& iPoint)
{
	*this = iPoint;
}

template <class T>
Point3D<T> &Point3D<T>::operator=(const Point3D<T> &iPoint) 
{	
	m_x = iPoint.X();
	m_y = iPoint.Y();
	m_z = iPoint.Z();
	m_w = iPoint.W();
	return *this;
}

template <class T>
bool Point3D<T>::operator>(const Point3D<T> &iPoint)
{
	return Length() > iPoint.Length();
	//return ((m_x-iPoint.X()) + (m_y -iPoint.Y()) + (m_z - iPoint.Z()) >0);
}

template <class T>
bool Point3D<T>::operator>=(const Point3D<T> &iPoint)
{
	return Length() >= iPoint.Length();
	//return ((m_x-iPoint.X()) + (m_y -iPoint.Y()) + (m_z - iPoint.Z()) >=0);
}

template <class T>
bool Point3D<T>::operator==(const Point3D<T> &iPoint)
{
	return (m_x == iPoint.X() 
			&& m_y == iPoint.Y()
			&& m_z == iPoint.Z()
			&& m_w == iPoint.W()
			);
}

template <class T>
bool Point3D<T>::operator<(const Point3D<T> &iPoint)
{
	return Length() < iPoint.Length();
	//return ((-m_x+iPoint.X()) + (-m_y + iPoint.Y()) + (-m_z + iPoint.Z()) >0);
}

template <class T>
bool Point3D<T>::operator<=(const Point3D<T> &iPoint)
{
	return Length() <= iPoint.Length();
	//return ((-m_x+iPoint.X()) + (-m_y + iPoint.Y()) + (-m_z + iPoint.Z()) >=0);
}


template <class T>
Point3D<T> Point3D<T>::operator*(const float &iScalarValue) 
{	
	T oX =  (T)(iScalarValue*m_x);
	T oY =  (T)(iScalarValue*m_y);
	T oZ =  (T)(iScalarValue*m_z);	
	T oW =  (T)(iScalarValue*m_w);	
	return Point3D<T>(oX, oY, oZ, oW);
}

template <class T>
Point3D<T> Point3D<T>::operator/(const Point3D<T> &iPoint) 
{	
	T oX =  iPoint.X() ? (T)(m_x/iPoint.X()) : m_x;
	T oY =  iPoint.Y() ? (T)(m_y/iPoint.Y()) : m_y;
	T oZ =  iPoint.Z() ? (T)(m_z/iPoint.Z()) : m_z;
	T oW =  iPoint.W() ? (T)(m_w/iPoint.W()) : m_w;
	return Point3D<T>(oX, oY, oZ, oW);
}


template <class T>
Point3D<T> Point3D<T>::operator>>(const int& iPositions) 
{	
	T oX =  (T)((long int)(m_x)>>iPositions);
	T oY =  (T)((long int)(m_y)>>iPositions);
	T oZ =  (T)((long int)(m_z)>>iPositions);
	T oW =  (T)((long int)(m_w)>>iPositions);
	return Point3D<T>(oX, oY, oZ, oW);
}

template <>
Point3D<float> Point3D<float>::operator>>(const int& iPositions) 
{	
	float oX =  (float)(int(m_x)>>iPositions);
	float oY =  (float)(int(m_y)>>iPositions);
	float oZ =  (float)(int(m_z)>>iPositions);
	float oW =  (float)(int(m_w)>>iPositions);
	return Point3D<float>(oX, oY, oZ, oW);
}

template <class T>
Point3D<T> Point3D<T>::operator/=(const float &iScalarValue) 
{	
	if (iScalarValue<=0)
	{
		return Point3D<T>();
	}

	m_x = T((float)m_x/iScalarValue);
	m_y = T((float)m_y/iScalarValue);
	m_z = T((float)m_z/iScalarValue);
	m_w = T((float)m_w/iScalarValue);

	return *this;
}

template <class T>
Point3D<T> Point3D<T>::operator*(const Point3D<T> &iPoint) 
{	
	T oX =  (T)(iPoint.X()*m_x);
	T oY =  (T)(iPoint.Y()*m_y);
	T oZ =  (T)(iPoint.Z()*m_z);	
	T oW =  (T)(iPoint.W()*m_w);	
	return Point3D<T>(oX, oY, oZ, oW);
}

template <class T>
Point3D<T> Point3D<T>::operator+(const Point3D<T> &iVector) 
{	
	T oX =  m_x + iVector.X();
	T oY =  m_y + iVector.Y();
	T oZ =  m_z + iVector.Z();	
	T oW =  m_w + iVector.W();	
	return Point3D<T>(oX, oY, oZ, oW);
}

template <class T>
Point3D<T> Point3D<T>::operator+=(const Point3D<T> &iVector) 
{	
	m_x += iVector.X();
	m_y += iVector.Y();
	m_z += iVector.Z();	
	m_w += iVector.W();	
	return *this;
}

template <class T>
Point3D<T> Point3D<T>::operator-(const Point3D<T> &iVector) 
{	
	T oX =  m_x - iVector.X();
	T oY =  m_y - iVector.Y();
	T oZ =  m_z - iVector.Z();	
	T oW =  m_w - iVector.W();	
	return Point3D<T>(oX, oY, oZ, oW);
}

template <class T>
T& Point3D<T>::operator[](const int& iIndex)
{
	switch (iIndex)
	{
		case 0: return m_x; 
		case 1: return m_y; 
		case 2: return m_z; 
		case 3: return m_w; 
		default: break;
	}

	return m_x;
}

template <class T>
Point3D<T> Point3D<T>::operator-() 
{	
	T oX =  -m_x;
	T oY =  -m_y;
	T oZ =  -m_z;	
	T oW =  -m_w;	
	return Point3D<T>(oX, oY, oZ, oW);
}


#ifdef _PG_GENERATE_SDK_LIBS_	
	template class Point3D<unsigned char>; 	
	template class Point3D<char>; 	
	template class Point3D<unsigned char>; 	
	template class Point3D<unsigned short>; 	
	template class Point3D<int>; 	
	template class Point3D<long>; 	
	template class Point3D<float>; 		
	template class Point3D<short>; 		
#endif

};


//////////////////////////////////////////////////////////////////////////
// EOF


