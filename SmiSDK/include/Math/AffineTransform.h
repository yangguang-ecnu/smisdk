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

