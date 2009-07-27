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
// Filename:	Matrix4x4.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/Matrix4x4.h"

namespace PGMath
{

template <class T>
Matrix4x4<T>::Matrix4x4()
{
	Identity();
}


template <class T>
Matrix4x4<T>::Matrix4x4(const T iMatrix[16])	
{
	memcpy(m_matrix, iMatrix, 16*sizeof(T));	
}


template <class T>
Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& iMatrix4x4)	
{
	*this = iMatrix4x4;	
}

template <class T>
Matrix4x4<T> &Matrix4x4<T>::operator=(const Matrix4x4<T> &iMatrix4x4) 
{	
	memcpy(m_matrix, iMatrix4x4.Matrix(), 16*sizeof(T));
	return *this;
}

template <class T>
void Matrix4x4<T>::Identity()
{
	// row major
	memset(m_matrix, 0, 16*sizeof(T));	
	m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = (T)1;
}

template <class T>
const T* Matrix4x4<T>::Matrix() const
{
	return m_matrix;
}

template <class T>
T &Matrix4x4<T>::operator[](const unsigned int iIndex)
{	
	if (iIndex>15) return m_matrix[0];
	return m_matrix[iIndex];
};

template <class T>
bool Matrix4x4<T>::SetElement(const unsigned int iRow, const unsigned int iColumn, const T& iValue)
{
	unsigned int index = iRow*4 + iColumn;
	if (index>15) 
	{
		return false;
	}
	m_matrix[index] = iValue;
	return true;
}

template <class T>
const T Matrix4x4<T>::GetElement(const unsigned int iRow, const unsigned int iColumn) const
{
	unsigned int index = iRow*4 + iColumn;
	if (index>15) return T(0);
	return m_matrix[index];
}

template <class T>
bool Matrix4x4<T>::IsIdentity() const
{
	T iValue = (T)1, zValue = (T)0;
	bool diagTest = (m_matrix[0]==iValue &&
		m_matrix[5]==iValue &&
		m_matrix[10]==iValue &&
		m_matrix[15]==iValue);

	bool otherTest = (m_matrix[1]==zValue &&
		m_matrix[2]==zValue &&
		m_matrix[3]==zValue &&
		m_matrix[4]==zValue &&
		m_matrix[6]==zValue &&
		m_matrix[7]==zValue &&
		m_matrix[8]==zValue &&
		m_matrix[9]==zValue &&
		m_matrix[11]==zValue &&
		m_matrix[12]==zValue &&
		m_matrix[13]==zValue &&
		m_matrix[14]==zValue);

	return (otherTest && diagTest);
}

template <class T>
bool Matrix4x4<T>::IsInvertible(T *oDeterminant /*=0*/) const
{
	T determinant = Determinant();
	if (oDeterminant) {
		*oDeterminant = determinant;
	}

	// if the determinent is zero, then the inverse matrix is not unique and is singular OR non-invertible.
	if ( (double)fabs(determinant) <=0.0f ) {//< kPgEpsilon )  {		
		return false;
	}

	return true;
}

template <class T>
const T Matrix4x4<T>::Determinant() const
{
  m_ptr4x4 elements = (m_ptr4x4)m_matrix;

  T a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;
  T detResult= (T)0;
  
  a1 = elements[0][0]; b1 = elements[0][1]; 
  c1 = elements[0][2]; d1 = elements[0][3];

  a2 = elements[1][0]; b2 = elements[1][1]; 
  c2 = elements[1][2]; d2 = elements[1][3];

  a3 = elements[2][0]; b3 = elements[2][1]; 
  c3 = elements[2][2]; d3 = elements[2][3];

  a4 = elements[3][0]; b4 = elements[3][1]; 
  c4 = elements[3][2]; d4 = elements[3][3];

  detResult = a1 * det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4)
       - b1 * det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4)
       + c1 * det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4)
       - d1 * det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
  return detResult;
}


template <class T>
bool Matrix4x4<T>::Transpose(Matrix4x4<T>& oMatrix4x4) const
{
  m_ptr4x4 inMatrix = (m_ptr4x4)m_matrix;
  m_ptr4x4 outMatrix = (m_ptr4x4)oMatrix4x4.Matrix();
  unsigned int i=0, j=0;
  T temp;

  for (i=0; i<4; i++) {
    for(j=i; j<4; j++) {
      temp = inMatrix[i][j];
      outMatrix[i][j] = inMatrix[j][i];
      outMatrix[j][i] = temp;
    }
  }
  return true;
}

template <class T>
bool Matrix4x4<T>::Invert(Matrix4x4<T>& oMatrix4x4) const
{
	T determinant;
	if (!IsInvertible(&determinant))
	{
		return false;
	}	
	
	//      -1     
	//     A    = adjoint (A) / determinant (A)
	// 
	
	// calculate the adjoint matrix
	Adjoint(oMatrix4x4);

	m_ptr4x4 outMatrix = (m_ptr4x4)oMatrix4x4.Matrix();
	unsigned int i=0, j=0;	
	
	// scale the adjoint matrix to get the inverse
	for (i=0; i<4; i++) {
		for(j=0; j<4; j++) {
			outMatrix[i][j] /= determinant;
		}
	}
	return true;
}

template <class T>
bool Matrix4x4<T>::Multiply(const Matrix4x4<T>& iMatrix, Matrix4x4<T>& oMatrix) const
{
	m_ptr4x4 aMatrix = (m_ptr4x4) m_matrix;
	m_ptr4x4 bMatrix = (m_ptr4x4) iMatrix.Matrix();
	m_ptr4x4 cMatrix = (m_ptr4x4) oMatrix.Matrix();
	unsigned int i, k;
	T temp[4][4];
	
	for (i = 0; i < 4; i++) {
		for (k = 0; k < 4; k++)  {
			temp[i][k] =  aMatrix[i][0] * bMatrix[0][k] +
				aMatrix[i][1] * bMatrix[1][k] +
				aMatrix[i][2] * bMatrix[2][k] +
				aMatrix[i][3] * bMatrix[3][k];
		}
	}
	
	for (i = 0; i < 4; i++) {
		cMatrix[i][0] = temp[i][0];
		cMatrix[i][1] = temp[i][1];
		cMatrix[i][2] = temp[i][2];
		cMatrix[i][3] = temp[i][3];
    }
	return true;
}

template <class T>
bool Matrix4x4<T>::Multiply(const Vector3D<T>& iVector, Vector3D<T>& oVector) const
{
	// Multiply a homogeneous vector (x,y,z,w) by this row major matrix, 
	// p' = A*p.	
	T e1 = iVector.X();
	T e2 = iVector.Y();
	T e3 = iVector.Z();	
	T e4 = iVector.W();	// is 0 for a vector. Hence no translations

	T oX = e1*m_matrix[0]  + e2*m_matrix[1]  + e3*m_matrix[2];//  + e4*m_matrix[3];
	T oY = e1*m_matrix[4]  + e2*m_matrix[5]  + e3*m_matrix[6];//  + e4*m_matrix[7];
	T oZ = e1*m_matrix[8]  + e2*m_matrix[9]  + e3*m_matrix[10];// + e4*m_matrix[11];	
	
	oVector = Vector3D<T>(oX, oY, oZ);
	return true;
}

template <class T>
bool Matrix4x4<T>::Multiply(const Point3D<T>& iPoint, Point3D<T>& oPoint) const
{
	// Multiply a point (x,y,z) by this this row major matrix, 
	// p' = A*p.		
	T e1 = iPoint.X();
	T e2 = iPoint.Y();
	T e3 = iPoint.Z();	
	T e4 = iPoint.W();	// 1. hence translation can be applied

	T oX = e1*m_matrix[0]  + e2*m_matrix[1]  + e3*m_matrix[2]  + e4*m_matrix[3];
	T oY = e1*m_matrix[4]  + e2*m_matrix[5]  + e3*m_matrix[6]  + e4*m_matrix[7];
	T oZ = e1*m_matrix[8]  + e2*m_matrix[9]  + e3*m_matrix[10] + e4*m_matrix[11];	
	
	oPoint = Point3D<T>(oX, oY, oZ);
	return true;
}


template <class T>
bool Matrix4x4<T>::Adjoint(Matrix4x4<T> &oMatrix4x4) const
{
  m_ptr4x4 inData = (m_ptr4x4) m_matrix;
  m_ptr4x4 outData = (m_ptr4x4) oMatrix4x4.Matrix();

  T a1, a2, a3, a4, b1, b2, b3, b4;
  T c1, c2, c3, c4, d1, d2, d3, d4;
  
  a1 = inData[0][0]; b1 = inData[0][1]; 
  c1 = inData[0][2]; d1 = inData[0][3];

  a2 = inData[1][0]; b2 = inData[1][1]; 
  c2 = inData[1][2]; d2 = inData[1][3];

  a3 = inData[2][0]; b3 = inData[2][1];
  c3 = inData[2][2]; d3 = inData[2][3];

  a4 = inData[3][0]; b4 = inData[3][1]; 
  c4 = inData[3][2]; d4 = inData[3][3];

  // reversal of row column labels indicates that
  // rows and columns have been transposed
  outData[0][0]  =   
    det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4);
  outData[1][0]  = 
    - det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4);
  outData[2][0]  =   
    det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4);
  outData[3][0]  = 
    - det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);

  outData[0][1]  = 
    - det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4);
  outData[1][1]  =   
    det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4);
  outData[2][1]  = 
    - det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4);
  outData[3][1]  =   
    det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4);
        
  outData[0][2]  =   
    det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4);
  outData[1][2]  = 
    - det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4);
  outData[2][2]  =   
    det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4);
  outData[3][2]  = 
    - det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4);
        
  outData[0][3]  = 
    - det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3);
  outData[1][3]  =   
    det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3);
  outData[2][3]  = 
    - det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3);
  outData[3][3]  =   
    det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3);

  return true;
}


// private
template <class T>
T Matrix4x4<T>::det3x3(const T &a1, const T & a2, const T & a3, 
               const T & b1, const T & b2, const T & b3, 
               const T & c1, const T & c2, const T & c3) const
{
	return ( a1 * det2x2( b2, b3, c2, c3 )
           - b1 * det2x2( a2, a3, c2, c3 )
           + c1 * det2x2( a2, a3, b2, b3 ) );
}


template <class T>
T Matrix4x4<T>::det2x2(const T & a, const T & b, const T & c, const T & d) const
{
	return (a * d - b * c);
}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class Matrix4x4<float>; 	
#endif

};



//////////////////////////////////////////////////////////////////////////
// EOF
