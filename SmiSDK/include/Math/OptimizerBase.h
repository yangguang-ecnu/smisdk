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
// Filename:	OptimizerBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGOptimizerBase_H_
#define _PGOptimizerBase_H_
//////////////////////////////////////////////////////////////////////////

#include "../Core/Core.h"
#include "../Core/BaseDataObject.h"
#include "../Algs/ProcessBase.h"

namespace PGMath
{

template <class T>
class OptimizerBase : public PGCore::BaseDataObject
{
public:
	OptimizerBase();
	
	bool SetInput(void * iSourceModule, ObjectiveFunction iFunction, const std::vector<T>& iInitParameters);	

	virtual bool Optimize() = 0;

	const std::vector<T>& GetParameters() const;
	
	virtual ~OptimizerBase(void);

protected:
	void				*m_sourceModule;
	std::vector<T>		m_parameters;	
	ObjectiveFunction	m_objectiveFn;
	
};

};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "OptimizerBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGOptimizerBase_H_
