// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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
