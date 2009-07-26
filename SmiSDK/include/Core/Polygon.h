// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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

