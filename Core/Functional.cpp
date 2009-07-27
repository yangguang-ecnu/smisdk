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
// Filename:	Functional.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Functional_HPP_
#define _PG_Functional_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/Functional.h"


namespace PGCore
{
	template <>
	inline const char& Functional<char>::Max(const char& iObjectA, const char& iObjectB)
	{
		return (iObjectA < iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const char& Functional<char>::Min(const char& iObjectA, const char& iObjectB)
	{
		return (iObjectA > iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const unsigned char& Functional<unsigned char>::Max(const unsigned char& iObjectA, const unsigned char& iObjectB)
	{
		return (iObjectA < iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const unsigned char& Functional<unsigned char>::Min(const unsigned char& iObjectA, const unsigned char& iObjectB)
	{
		return (iObjectA > iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const Point3D<unsigned char>& Functional<Point3D<unsigned char> >::Max(const Point3D<unsigned char>& iObjectA, const Point3D<unsigned char>& iObjectB)
	{
		return Point3D<unsigned char>( Functional<unsigned char>::Max(iObjectA.X(), iObjectB.X() ),
			Functional<unsigned char>::Max(iObjectA.Y(), iObjectB.Y() ),
			Functional<unsigned char>::Max(iObjectA.Z(), iObjectB.Z() ),
			Functional<unsigned char>::Max(iObjectA.W(), iObjectB.W() )
			);
	}

	template <>
	inline const Point3D<unsigned char>& Functional<Point3D<unsigned char> >::Min(const Point3D<unsigned char>& iObjectA, const Point3D<unsigned char>& iObjectB)
	{
		/*
		char avgValueA = iObjectA.X()/4 + iObjectA.Y()/4 + iObjectA.Z()/4 + iObjectA.W()/4;
		char avgValueB = iObjectB.X()/4 + iObjectB.Y()/4 + iObjectB.Z()/4 + iObjectB.W()/4;

		return (avgValueA > avgValueB) ? iObjectB : iObjectA;*/
		
		return Point3D<unsigned char>( Functional<unsigned char>::Min(iObjectA.X(), iObjectB.X() ),
			Functional<unsigned char>::Min(iObjectA.Y(), iObjectB.Y() ),
			Functional<unsigned char>::Min(iObjectA.Z(), iObjectB.Z() ),
			Functional<unsigned char>::Min(iObjectA.W(), iObjectB.W() )
			);
	}


	template <>
	inline const Point3D<unsigned short>& Functional<Point3D<unsigned short> >::Max(const Point3D<unsigned short>& iObjectA, const Point3D<unsigned short>& iObjectB)
	{
		return Point3D<unsigned short>( Functional<unsigned short>::Max(iObjectA.X(), iObjectB.X() ),
			Functional<unsigned short>::Max(iObjectA.Y(), iObjectB.Y() ),
			Functional<unsigned short>::Max(iObjectA.Z(), iObjectB.Z() ),
			Functional<unsigned short>::Max(iObjectA.W(), iObjectB.W() )
			);
	}

	template <>
	inline const Point3D<char>& Functional<Point3D<char> >::Max(const Point3D<char>& iObjectA, const Point3D<char>& iObjectB)
	{
		/*char avgValueA = iObjectA.X()/4 + 
			iObjectA.Y()/4 + 
			iObjectA.Z()/4 + 
			iObjectA.W()/4;
		char avgValueB = iObjectB.X()/4 + 
			iObjectB.Y()/4 + 
			iObjectB.Z()/4 + 
			iObjectB.W()/4;

		return (avgValueA < avgValueB) ? iObjectB : iObjectA;*/
		return Point3D<char>( Functional<char>::Max(iObjectA.X(), iObjectB.X() ),
			Functional<char>::Max(iObjectA.Y(), iObjectB.Y() ),
			Functional<char>::Max(iObjectA.Z(), iObjectB.Z() ),
			Functional<char>::Max(iObjectA.W(), iObjectB.W() )
			);
	}

	template <>
	inline const Point3D<char>& Functional<Point3D<char> >::Min(const Point3D<char>& iObjectA, const Point3D<char>& iObjectB)
	{
		/*
		char avgValueA = iObjectA.X()/4 + iObjectA.Y()/4 + iObjectA.Z()/4 + iObjectA.W()/4;
		char avgValueB = iObjectB.X()/4 + iObjectB.Y()/4 + iObjectB.Z()/4 + iObjectB.W()/4;

		return (avgValueA > avgValueB) ? iObjectB : iObjectA;*/
		
		return Point3D<char>( Functional<char>::Min(iObjectA.X(), iObjectB.X() ),
			Functional<char>::Min(iObjectA.Y(), iObjectB.Y() ),
			Functional<char>::Min(iObjectA.Z(), iObjectB.Z() ),
			Functional<char>::Min(iObjectA.W(), iObjectB.W() )
			);
	}

	template <>
	inline const Point3D<unsigned short>& Functional<Point3D<unsigned short> >::Min(const Point3D<unsigned short>& iObjectA, const Point3D<unsigned short>& iObjectB)
	{
		return Point3D<unsigned short>( Functional<unsigned short>::Min(iObjectA.X(), iObjectB.X() ),
			Functional<unsigned short>::Min(iObjectA.Y(), iObjectB.Y() ),
			Functional<unsigned short>::Min(iObjectA.Z(), iObjectB.Z() ),
			Functional<unsigned short>::Min(iObjectA.W(), iObjectB.W() )
			);
	}

	template <>
	inline const Vector3D<float>& Functional<Vector3D<float> >::Max(const Vector3D<float>& iObjectA, const Vector3D<float>& iObjectB)
	{
		/*char avgValueA = iObjectA.X()/4 + 
			iObjectA.Y()/4 + 
			iObjectA.Z()/4 + 
			iObjectA.W()/4;
		char avgValueB = iObjectB.X()/4 + 
			iObjectB.Y()/4 + 
			iObjectB.Z()/4 + 
			iObjectB.W()/4;

		return (avgValueA < avgValueB) ? iObjectB : iObjectA;*/
		return Vector3D<float>( Functional<float>::Max(iObjectA.X(), iObjectB.X() ),
			Functional<float>::Max(iObjectA.Y(), iObjectB.Y() ),
			Functional<float>::Max(iObjectA.Z(), iObjectB.Z() )
			);
	}

	template <>
	inline const Vector3D<float>& Functional<Vector3D<float> >::Min(const Vector3D<float>& iObjectA, const Vector3D<float>& iObjectB)
	{
		/*char avgValueA = iObjectA.X()/4 + 
			iObjectA.Y()/4 + 
			iObjectA.Z()/4 + 
			iObjectA.W()/4;
		char avgValueB = iObjectB.X()/4 + 
			iObjectB.Y()/4 + 
			iObjectB.Z()/4 + 
			iObjectB.W()/4;

		return (avgValueA < avgValueB) ? iObjectB : iObjectA;*/
		return Vector3D<float>( Functional<float>::Min(iObjectA.X(), iObjectB.X() ),
			Functional<float>::Min(iObjectA.Y(), iObjectB.Y() ),
			Functional<float>::Min(iObjectA.Z(), iObjectB.Z() )
			);
	}

	template <>
	inline const short& Functional<short>::Max(const short& iObjectA, const short& iObjectB)
	{
		return (iObjectA < iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const short& Functional<short>::Min(const short& iObjectA, const short& iObjectB)
	{
		return (iObjectA > iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const unsigned short& Functional<unsigned short>::Max(const unsigned short& iObjectA, const unsigned short& iObjectB)
	{
		return (iObjectA < iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const unsigned short& Functional<unsigned short>::Min(const unsigned short& iObjectA, const unsigned short& iObjectB)
	{
		return (iObjectA > iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const double& Functional<double>::Max(const double& iObjectA, const double& iObjectB)
	{
		return (iObjectA < iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const double& Functional<double>::Min(const double& iObjectA, const double& iObjectB)
	{
		return (iObjectA > iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const float& Functional<float>::Max(const float& iObjectA, const float& iObjectB)
	{
		return (iObjectA < iObjectB) ? iObjectB : iObjectA;
	}

	template <>
	inline const float& Functional<float>::Min(const float& iObjectA, const float& iObjectB)
	{
		return (iObjectA > iObjectB) ? iObjectB : iObjectA;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class Functional<double>; 
	template class Functional<short>; 
	template class Functional<unsigned short>; 
	template class Functional<char>; 
	template class Functional<unsigned char>; 
	template class Functional<float>; 
	template class Functional<PGMath::Point3D<char>>; 
	template class Functional<PGMath::Point3D<unsigned char>>; 
	template class Functional<PGMath::Point3D<unsigned short>>; 
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Functional_HPP_