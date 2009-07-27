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
// Filename:	Polygon.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Core/Polygon.h"


namespace PGCore
{
	template <class T, unsigned int Dim>
	inline Polygon<T, Dim>::Polygon()
	{
		m_maxDimension = Dim;		
	}

	template <class T, unsigned int Dim>
	inline Polygon<T, Dim>::Polygon(const std::vector<PGMath::Point3D<T>> &iVertices)
	{
		m_maxDimension = Dim;

		int count = min(m_maxDimension, iVertices.size());

		for (int i=0; i<count; i++)
		{
			m_vertices.push_back(iVertices[i]);	
		}
	}

	template <class T, unsigned int Dim>
	inline Polygon<T, Dim>::~Polygon()
	{

	}

	template <class T, unsigned int Dim>
	bool Polygon<T, Dim>::Clear()
	{
		m_vertices.clear();
		return true;
	}

	template <class T, unsigned int Dim>
	inline bool Polygon<T, Dim>::AddVertex(const PGMath::Point3D<T>& iVertex)
	{
		bool rv = false;

		if (m_vertices.size() < m_maxDimension)
		{		
			m_vertices.push_back(iVertex);			
		}

		return rv;
	}

	template <class T, unsigned int Dim>
	inline const PGMath::Point3D<T>& Polygon<T, Dim>::GetVertex(const unsigned int iIndex) const
	{
		if (iIndex <0 || iIndex> (m_vertices.size()-1))
		{		
			return PGMath::Point3D<T>();
		}

		return m_vertices[iIndex];			
	}

	template <class T, unsigned int Dim>
	inline const PGMath::Point3D<T>& Polygon<T, Dim>::operator[](const int& iIndex) const
	{
		return GetVertex(iIndex);
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class Polygon<short, 3>; 	 
#endif

};




//////////////////////////////////////////////////////////////////////////
// EOF


