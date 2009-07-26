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
// Filename:	AffineTransform.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGAffineTransform_H_
#define _PGAffineTransform_H_
//////////////////////////////////////////////////////////////////////////

#include "Constants.h"
#include "Core/BaseDataObject.h"
#include "TransformBase.h"
#include "Matrix4x4.h"

namespace PGMath
{

template <class T>
class AffineTransform : public TransformBase<T>, public PGCore::BaseDataObject
{
public:
	AffineTransform();
	AffineTransform(const PGMath::Point3D<T>& iOrigin);
	AffineTransform(const AffineTransform<T>& iAffineTransform);		
	AffineTransform<T>& operator=(const AffineTransform<T>& iAffineTransform);
	
	AffineTransform(const Matrix4x4<T>& iMatrix4x4);	
	AffineTransform(const PGMath::Point3D<T>& iOrigin, 
					const PGMath::DegreesOfFreedomBase<T>&	iDofs);	

	virtual bool ConcatPre(const TransformBase<T> *iTransform);
	virtual bool ConcatPost(const TransformBase<T> *iTransform);		
	virtual bool Invert(TransformBase<T> *oTransform=0);

	virtual bool Apply(const Vector3D<T>& iVector, Vector3D<T>& oVector) const;
	virtual bool Apply(const Point3D<T>& iPoint, Point3D<T>& oPoint) const;		

	virtual ~AffineTransform(void);
	
	const Matrix4x4<T>& ConstMatrix4x4() const;

	PGMath::Matrix4x4<T>& Matrix4x4();

private:	
	PGMath::Matrix4x4< T >	m_matrix4x4;

	bool	updateMatrix();
	
	bool	createMatrixTranslation	(const Point3D<T>&  iTranslationParams,
				PGMath::Matrix4x4<T>&	oMatrix4x4); // units ?
	bool	createMatrixRotation	(const Point3D<T>&  iRotationParams,
				PGMath::Matrix4x4<T>&	oMatrix4x4); // units : degrees 
	bool	createMatrixScaling		(const Point3D<T>&  iScalingParams,
				PGMath::Matrix4x4<T>&	oMatrix4x4); // units : none 
	bool	createMatrixShear		(const Point3D<T>&  iShearParams,
				PGMath::Matrix4x4<T>&	oMatrix4x4); // units : ?

};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "AffineTransform.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGAffineTransform_H_

