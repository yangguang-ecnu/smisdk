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
// Filename:	TriangleMesh.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Core/TriangleMesh.h"


namespace PGCore
{
	template <class T>
	TriangleMesh<T>::TriangleMesh()
	{

	}

	template <class T>
	TriangleMesh<T>::~TriangleMesh()
	{

	}

	template <class T>
	unsigned int TriangleMesh<T>::Size()
	{
		return m_connectivity.size();		
	}

	template <class T>
	bool TriangleMesh<T>::Clear()
	{
		m_points.clear();
		m_connectivity.clear();

		return true;
	}

	template <class T>
	bool TriangleMesh<T>::AddTriangle(const PGCore::Polygon<T, 3>& iTriangle)
	{
		int firstIndex = m_points.size();
		for (int i=0; i<3; i++)
		{
			m_points.push_back(iTriangle[i]);			
		}

		m_connectivity.push_back(PGMath::Point3D<int>(firstIndex, firstIndex+1, firstIndex+2));

		return true;
	}

	template <class T>
	const PGCore::Polygon<T, 3>& TriangleMesh<T>::GetTriangle(const unsigned int iIndex) const
	{
		PGCore::Polygon<T, 3> triad;

		if (iIndex<0 || iIndex>m_points.size()-1) return triad;
		
		PGMath::Point3D<int> connectivity = m_connectivity[iIndex];

		for (int i=0; i<3; i++)
		{
			int index = connectivity[i];
			if (index<0 || index>m_points.size()-1) continue;

			triad.AddVertex(m_points[index]);
		}

		return triad;
	}

	template <class T>
	const PGCore::Polygon<T, 3>& TriangleMesh<T>::operator[](const int& iIndex) const
	{
		return GetTriangle(iIndex);
	}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class TriangleMesh<short>; 	 
#endif

};




//////////////////////////////////////////////////////////////////////////
// EOF


