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
// Filename:	PixelBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGPixelBase_H_
#define _PGPixelBase_H_
//////////////////////////////////////////////////////////////////////////


#include "BaseDataObject.h"
#include "Math/Point3D.h"

using namespace PGMath;

namespace PGCore
{

typedef Point3D<char> PgChar_RGBAPixel;

typedef Point3D<unsigned char> PgUChar_RGBAPixel;

typedef Point3D<unsigned short> PgUShort_RGBAPixel;

template <class T>
class PixelBase: public BaseDataObject
{
public:
	PixelBase();	
	PixelBase(const PixelBase<T>& iObject);	
	PixelBase<T>& operator=(const PixelBase<T>& iPixel);
	
	const T& GetValue() const 
	{
		return m_value;
	}

	void SetValue(const T& iValue)  
	{
		m_value = iValue;
	}

	const T GetMinValue() const; 
	
	const T GetMaxValue() const; 
	

	virtual ~PixelBase(void);
private:
	T	m_value;	
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "PixelBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGPixelBase_H_
