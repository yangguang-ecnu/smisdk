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

