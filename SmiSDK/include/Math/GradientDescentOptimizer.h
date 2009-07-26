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
// Filename:	GradientDescentOptimizer.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGGradientDescentOptimizer_H_
#define _PGGradientDescentOptimizer_H_
//////////////////////////////////////////////////////////////////////////

#include "OptimizerBase.h"

namespace PGMath
{

	template <class T>
	class GradientDescentOptimizer : public OptimizerBase<T>
	{
	public:
		GradientDescentOptimizer(const float iGradientStep=0.1f);

		bool SetGradientFunction(const ObjectiveGradientFunction iGradientFunction);

		virtual bool Optimize();

		virtual ~GradientDescentOptimizer(void);

	protected:
		ObjectiveGradientFunction	m_gradientFunction;
		float						m_gradientStep;
		int							m_iterations;
		float						m_initGradientStep;

		bool LineSearch(const std::vector<float>& iGradientDescentValues, float& ioCurrentFnValue);

	};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GradientDescentOptimizer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGradientDescentOptimizer_H_
