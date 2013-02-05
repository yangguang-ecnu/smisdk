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
// Filename:	PolygonMesh.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Core/PolygonMesh.h"


namespace PGCore
{
	template <class T, unsigned int Dim>
	inline PolygonMesh<T, Dim>::PolygonMesh()
	{		
		m_type = kPgDataObjectTypePolygonMesh;
		m_origin = PGMath::Point3D<float>(0, 0, 0);
		m_span = PGMath::Point3D<float>(0, 0, 0);
	}

	template <class T, unsigned int Dim>
	inline PolygonMesh<T, Dim>::PolygonMesh(const std::vector<Polygon<T, Dim>> &iPolygons)
	{		
		for (int i=0; i<iPolygons.size(); i++)
		{
			m_polygons.push_back(iPolygons[i]);	
		}
	}

	template <class T, unsigned int Dim>
	inline PolygonMesh<T, Dim>::~PolygonMesh()
	{

	}

	template <class T, unsigned int Dim>
	const unsigned int& PolygonMesh<T, Dim>::GetDimensions() const
	{
		return m_polygons.size();
	}

	// pointcloud ops
	template <class T, unsigned int Dim>
	std::vector<PGMath::Point3D<float> >& PolygonMesh<T, Dim>::GetPointCloud(const int iIndex/*=0*/) // in img/raw space
	{
		if (iIndex<0 || iIndex>1) return std::vector<PGMath::Point3D<float> >();

		return m_pointCloud[iIndex];
	}

	template <class T, unsigned int Dim>
	std::vector<PGMath::Point3D<float> >& PolygonMesh<T, Dim>::GetTargetCloud(const int iIndex/*=0*/) // in img/raw space
	{
		if (iIndex<0 || iIndex>1) return std::vector<PGMath::Point3D<float> >();

		return m_targetCloud[iIndex];
	}

	template <class T, unsigned int Dim>
	std::vector<PGMath::Point3D<float> >& PolygonMesh<T, Dim>::GetToolCloud(const int iIndex/*=0*/) // in img/raw space
	{
		if (iIndex<0 || iIndex>1) return std::vector<PGMath::Point3D<float> >();

		return m_toolCloud[iIndex];
	}

	template <class T, unsigned int Dim>
	bool PolygonMesh<T, Dim>::ResetPointCloud(const int iImageIndex/*=0*/)
	{
		if (iImageIndex<0 || iImageIndex>1) return false;

		m_pointCloud[iImageIndex].clear();
		return true;
	}

	template <class T, unsigned int Dim>
	bool PolygonMesh<T, Dim>::ResetTargetCloud(const int iImageIndex/*=0*/)
	{
		if (iImageIndex<0 || iImageIndex>1) return false;

		m_targetCloud[iImageIndex].clear();
		return true;
	}

	template <class T, unsigned int Dim>
	bool PolygonMesh<T, Dim>::ResetToolCloud(const int iImageIndex/*=0*/)
	{
		if (iImageIndex<0 || iImageIndex>1) return false;

		m_toolCloud[iImageIndex].clear();
		return true;
	}

	template <class T, unsigned int Dim>
	const unsigned int& PolygonMesh<T, Dim>::GetSkeletonDimensions(bool iRef/*=true*/) const
	{
		return iRef ? m_refSkeleton.size() : m_subSkeleton.size();
	}	

	template <class T, unsigned int Dim>
	void  PolygonMesh<T, Dim>::SetOrigin(const PGMath::Point3D<float>& iOrigin)
	{
		m_origin = iOrigin;
	}

	template <class T, unsigned int Dim>
	const PGMath::Point3D<float>& PolygonMesh<T, Dim>::GetOrigin() const
	{
		return m_origin;
	}
	
	template <class T, unsigned int Dim>
	void  PolygonMesh<T, Dim>::SetSpan(const PGMath::Point3D<float>& iSpan)
	{
		m_span = iSpan;
	}

	template <class T, unsigned int Dim>
	const PGMath::Point3D<float>& PolygonMesh<T, Dim>::GetSpan() const
	{
		return m_span;
	}

	template <class T, unsigned int Dim>
	bool PolygonMesh<T, Dim>::Clear(const int iIndex/*=-1*/)
	{
		m_polygons.clear();

		if (iIndex==-1)
		{
			m_refSkeleton.clear();
			m_subSkeleton.clear();
			for (int i=0; i<2; i++)
			{
				m_pointCloud[i].clear();
				m_targetCloud[i].clear();
				m_toolCloud[i].clear();
			}
		} else if (iIndex==0)
		{
			m_refSkeleton.clear();
			m_pointCloud[0].clear();
			m_targetCloud[0].clear();
			m_toolCloud[0].clear();
		} else if (iIndex==1)
		{
			m_subSkeleton.clear();
			m_pointCloud[1].clear();
			m_targetCloud[1].clear();
			m_toolCloud[1].clear();
		}	

		return true;
	}

	template <class T, unsigned int Dim>
	inline bool PolygonMesh<T, Dim>::Reserve(const unsigned long int& iMaxSize)
	{
		if (iMaxSize>0) m_polygons.reserve(iMaxSize);

		return true;
	}

	template <class T, unsigned int Dim>
	inline bool PolygonMesh<T, Dim>::AddPolygon(const Polygon<T, Dim>& iPolygon)
	{
		bool rv = true;
				
		m_polygons.push_back(iPolygon);			
		
		return rv;
	}

	template <class T, unsigned int Dim>
	inline const Polygon<T, Dim>& PolygonMesh<T, Dim>::GetPolygon(const unsigned int iIndex) const
	{
		if (iIndex <0 || iIndex> (m_polygons.size()-1))
		{		
			return Polygon<T, Dim>();
		}

		return m_polygons[iIndex];			
	}

	template <class T, unsigned int Dim>
	inline bool PolygonMesh<T, Dim>::AddSkeletonElement(const std::vector<PGMath::Point3D<T> > & iSkeletonElement, bool iRef/*=true*/)
	{
		bool rv = true;
				
		if (iRef)
			m_refSkeleton.push_back(iSkeletonElement); 
		else
			m_subSkeleton.push_back(iSkeletonElement);
		
		return rv;
	}

	template <class T, unsigned int Dim>
	inline const std::vector<PGMath::Point3D<T> > & PolygonMesh<T, Dim>::GetSkeletonElement(const unsigned int iIndex, bool iRef/*=true*/) const
	{
		if (iIndex <0)
		{		
			return std::vector<PGMath::Point3D<T> >();
		}

		if (iRef && iIndex> (m_refSkeleton.size()-1)) 
		{		
			return std::vector<PGMath::Point3D<T> >();
		}	
			
		if (!iRef && iIndex> (m_subSkeleton.size()-1))
		{		
			return std::vector<PGMath::Point3D<T> >();
		}

		return iRef ? m_refSkeleton[iIndex] : m_subSkeleton[iIndex];		
	}

	template <class T, unsigned int Dim>
	void  PolygonMesh<T, Dim>::SetSkeletonOrigin(const PGMath::Point3D<float>& iOrigin, bool iRef/*=true*/)
	{
		if (iRef)
		{
			m_refSkelOrigin = iOrigin;
		} else
		{
			m_subSkelOrigin = iOrigin;
		}
	}

	template <class T, unsigned int Dim>
	const PGMath::Point3D<float>& PolygonMesh<T, Dim>::GetSkeletonOrigin(bool iRef/*=true*/) const
	{
		return iRef ? m_refSkelOrigin :  m_subSkelOrigin;		
	}
	
	template <class T, unsigned int Dim>
	void  PolygonMesh<T, Dim>::SetSkeletonSpan(const PGMath::Point3D<float>& iSpan, bool iRef/*=true*/)
	{
		if (iRef)
		{
			m_refSkelSpan = iSpan;
		} else
		{
			m_subSkelSpan = iSpan;
		}		
	}

	template <class T, unsigned int Dim>
	const PGMath::Point3D<float>& PolygonMesh<T, Dim>::GetSkeletonSpan(bool iRef/*=true*/) const
	{
		return iRef ? m_refSkelSpan :  m_subSkelSpan;		
	}

	template <class T, unsigned int Dim>
	inline const Polygon<T, Dim>& PolygonMesh<T, Dim>::operator[](const int& iIndex) const
	{
		return GetPolygon(iIndex);
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class PolygonMesh<float, 3>; 	 	
#endif

};




//////////////////////////////////////////////////////////////////////////
// EOF



