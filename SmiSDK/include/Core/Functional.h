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
// Filename:	Functional.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Functional_H_
#define _PG_Functional_H_
//////////////////////////////////////////////////////////////////////////

#include "ObjectBase.h"
#include "BaseDataObject.h"
#include "PixelBase.h"

namespace PGCore
{

	template <class T>
	class Functional
	{
	public:
		Functional() {};

		~Functional() {};

		static const T& Min(const T& iObjectA, const T& iObjectB);

		static const T& Max(const T& iObjectA, const T& iObjectB);		
	};
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Functional.hpp"
#endif
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Functional_H_