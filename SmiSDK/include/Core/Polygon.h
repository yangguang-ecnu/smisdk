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
// Filename:	Polygon.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Polygon_H_
#define _PG_Polygon_H_
//////////////////////////////////////////////////////////////////////////


#include "BaseDataObject.h"
#include "Math/Point3D.h"


namespace PGCore
{
	template <class T, unsigned int Dim=2>
	class Polygon : public BaseDataObject
	{

	public:

		Polygon();	

		Polygon(const std::vector<PGMath::Point3D<T>> &iVertices);	

		virtual ~Polygon(void);

		bool Clear();

		bool AddVertex(const PGMath::Point3D<T>& iVertex);

		const PGMath::Point3D<T>& GetVertex(const unsigned int iIndex) const;

		const PGMath::Point3D<T>& operator[](const int& iIndex) const;

	private: 
		
		int										m_maxDimension;
		std::vector<PGMath::Point3D<T>>			m_vertices;		

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Polygon.hpp"
#endif



//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Polygon_H_

