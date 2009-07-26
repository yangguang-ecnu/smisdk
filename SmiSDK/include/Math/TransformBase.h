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
// Filename:	TransformBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGTransformBase_H_
#define _PGTransformBase_H_
//////////////////////////////////////////////////////////////////////////

#include "Point3D.h"
#include "DegreesOfFreedomBase.h"

namespace PGMath
{

template <class T>
class TransformBase
{
public:
	TransformBase();
	
	bool SetOrigin(const PGMath::Point3D<T>& iOrigin);

	const PGMath::Point3D<T>& GetOrigin() const;

	virtual bool ConcatPre(const TransformBase *iTransform) = 0;
	virtual bool ConcatPost(const TransformBase *iTransform) = 0;		
	virtual bool Invert(TransformBase *oTransform=0) = 0;

	virtual bool Apply(const Vector3D<T>& iVector, Vector3D<T>& oVector) const = 0;
	virtual bool Apply(const Point3D<T>& iPoint, Point3D<T>& oPoint) const = 0;		

	virtual ~TransformBase(void);

protected:
	PGMath::DegreesOfFreedomBase<T>	m_dofs;	
	PGMath::Point3D<T>	m_origin;
	
};




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGTransformBase_H_
