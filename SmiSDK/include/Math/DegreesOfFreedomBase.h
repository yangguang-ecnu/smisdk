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
// Filename:	DegreesOfFreedomBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGDegreesOfFreedomBase_H_
#define _PGDegreesOfFreedomBase_H_
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include "Point3D.h"
#include "Core/BaseDataObject.h"

namespace PGMath
{
	typedef enum ePgDegreeOfFreedom {
			kPgDegreeOfFreedomTranslationX=0,
			kPgDegreeOfFreedomTranslationY=1,
			kPgDegreeOfFreedomTranslationZ=2,
			kPgDegreeOfFreedomRotationX=3,
			kPgDegreeOfFreedomRotationY=4,
			kPgDegreeOfFreedomRotationZ=5,
			kPgDegreeOfFreedomScalingX=6,
			kPgDegreeOfFreedomScalingY=7,
			kPgDegreeOfFreedomScalingZ=8,
			kPgDegreeOfFreedomShearXY=9,
			kPgDegreeOfFreedomShearYZ=10,
			kPgDegreeOfFreedomShearXZ=11,
			
			kPgDegreeOfFreedomTotal=12
	};

template <class T>
class DegreesOfFreedomBase : public PGCore::BaseDataObject
{
public:

	

	DegreesOfFreedomBase();	

	DegreesOfFreedomBase(const std::vector<T>& iParameters);	
	
	virtual bool SetParameter(unsigned int iIndex, const T& iParameter);
	
	virtual const T GetParameter(unsigned int iIndex) const;

	const PGMath::Point3D<T>& GetOrigin() const;

	void SetOrigin(const PGMath::Point3D<T>& iOrigin);

	virtual ~DegreesOfFreedomBase(void); 
	

protected:	

	PGMath::Point3D<T>	m_origin;
	// the derived class adds meaning to this vector
	std::vector<T>		m_parameters;
	
};




};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "DegreeOfFreedomBase.h"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGDegreesOfFreedomBase_H_
