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
