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
