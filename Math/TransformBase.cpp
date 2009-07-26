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
// Filename:	TransformBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/TransformBase.h"

namespace PGMath
{

	template <class T>
	TransformBase<T>::TransformBase() 
	{
	};
	
	template <class T>
	bool TransformBase<T>::SetOrigin(const PGMath::Point3D<T>& iOrigin)
	{
		m_origin = iOrigin;
		return true;
	}

	template <class T>
	const PGMath::Point3D<T>& TransformBase<T>::GetOrigin() const 
	{
		return m_origin;
	}

	template <class T>
	TransformBase<T>::~TransformBase(void) 
	{
		
	};

	
#ifdef _PG_GENERATE_SDK_LIBS_
	template class TransformBase<float>; 	
#endif


};
//////////////////////////////////////////////////////////////////////////
// EOF

