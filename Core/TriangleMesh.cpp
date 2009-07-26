// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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


