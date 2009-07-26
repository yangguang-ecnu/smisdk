// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
