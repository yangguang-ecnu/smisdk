// -*- C++ -*-
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
