// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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

