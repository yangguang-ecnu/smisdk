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
// Filename:	OptimizerBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Math/OptimizerBase.h"

namespace PGMath
{

	template <class T>
	OptimizerBase<T>::OptimizerBase() { m_sourceModule = 0; m_objectiveFn = 0; };
	
	template <class T>
	bool OptimizerBase<T>::SetInput(void * iSourceModule, ObjectiveFunction iFunction, const std::vector<T>& iInitParameters)
	{
		m_sourceModule  =   iSourceModule;
		m_parameters	=	iInitParameters;
		m_objectiveFn	=	iFunction;
		return (!m_parameters.empty() && m_objectiveFn!=0);
	}

	//template <class T>
	//bool OptimizerBase<T>::Optimize();

	template <class T>
	const std::vector<T>& OptimizerBase<T>::GetParameters() const 
	{
		return m_parameters;
	}
	
	template <class T>
	OptimizerBase<T>::~OptimizerBase(void) {};

};
//////////////////////////////////////////////////////////////////////////
// EOF

