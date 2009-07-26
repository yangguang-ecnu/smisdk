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

