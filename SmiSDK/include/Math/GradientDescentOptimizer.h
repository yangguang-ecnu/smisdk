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
