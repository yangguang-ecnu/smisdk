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
// Filename:	TriangleMesh.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_TriangleMesh_H_
#define _PG_TriangleMesh_H_
//////////////////////////////////////////////////////////////////////////


#include "BaseDataObject.h"
#include "Math/Point3D.h"
#include "Polygon.h"


namespace PGCore
{
	template <class T>
	class TriangleMesh : public BaseDataObject
	{

	public:

		TriangleMesh();	

		virtual ~TriangleMesh(void);

		bool Clear();

		unsigned int Size();

		bool AddTriangle(const PGCore::Polygon<T, 3>& iTriangle);

		const PGCore::Polygon<T, 3>& GetTriangle(const unsigned int iIndex) const;

		const PGCore::Polygon<T, 3>& operator[](const int& iIndex) const;

	private: 	
		
		std::vector<PGMath::Point3D<T>>			m_points;

		std::vector<PGMath::Point3D<int>>		m_connectivity;	

	};

};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "TriangleMesh.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_TriangleMesh_H_

