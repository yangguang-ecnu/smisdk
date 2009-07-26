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