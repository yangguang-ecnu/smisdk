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
// Filename:	PolygonMesh.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_PolygonMesh_H_
#define _PG_PolygonMesh_H_
//////////////////////////////////////////////////////////////////////////


#include "BaseDataObject.h"
#include "Polygon.h"
#include "Math/Point3D.h"


namespace PGCore
{
	template <class T, unsigned int Dim=2>
	class PolygonMesh : public BaseDataObject
	{

	public:

		PolygonMesh();	

		PolygonMesh(const std::vector<Polygon<T, Dim>> &iPolygons);	

		virtual ~PolygonMesh(void);

		bool Clear(const int iIndex=-1);

		bool Reserve(const unsigned long int& iMaxSize);

		bool AddPolygon(const Polygon<T, Dim>& iPolygon);

		const Polygon<T, Dim>& GetPolygon(const unsigned int iIndex) const;

		const Polygon<T, Dim>& operator[](const int& iIndex) const;

		const unsigned int& GetDimensions() const;

		void  SetOrigin(const PGMath::Point3D<float>& iOrigin);
		const PGMath::Point3D<float>& GetOrigin() const;

		void  SetSpan(const PGMath::Point3D<float>& iOrigin);
		const PGMath::Point3D<float>& GetSpan() const;

		// skeleton		
		bool AddSkeletonElement(const std::vector<PGMath::Point3D<T> > & iSkeletonElement, bool iRef=true);		
		const std::vector<PGMath::Point3D<T> > & GetSkeletonElement(const unsigned int iIndex, bool iRef=true) const;

		const unsigned int& GetSkeletonDimensions(bool iRef=true) const;

		void  SetSkeletonOrigin(const PGMath::Point3D<float>& iOrigin, bool iRef=true);
		const PGMath::Point3D<float>& GetSkeletonOrigin(bool iRef=true) const;
		
		void  SetSkeletonSpan(const PGMath::Point3D<float>& iOrigin, bool iRef=true);
		const PGMath::Point3D<float>& GetSkeletonSpan(bool iRef=true) const;



	private: 		
		std::vector<std::vector<PGMath::Point3D<T> > >		m_refSkeleton;	
		std::vector<std::vector<PGMath::Point3D<T> > >		m_subSkeleton;	

		std::vector<Polygon<T, Dim> >						m_polygons;	
		PGMath::Point3D<float>								m_origin;
		PGMath::Point3D<float>								m_span;

		PGMath::Point3D<float>								m_refSkelOrigin;
		PGMath::Point3D<float>								m_refSkelSpan;

		PGMath::Point3D<float>								m_subSkelOrigin;
		PGMath::Point3D<float>								m_subSkelSpan;
	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "PolygonMesh.hpp"
#endif



//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_PolygonMesh_H_

