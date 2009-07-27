// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
