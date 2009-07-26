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


