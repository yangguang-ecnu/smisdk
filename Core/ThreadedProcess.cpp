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
// Filename:	ThreadedProcess.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//
//
//////////////////////////////////////////////////////////////////////

#include "Core/ThreadedProcess.h"

namespace PGCore
{

	ThreadedProcess::ThreadedProcess()
	{		
		m_params = 0;
		m_asyncThreadedProcessingEnabled = false;
	}

	ThreadedProcess::~ThreadedProcess()
	{		
	}

	void ThreadedProcess::SetParams(void *iParams) 
	{ 
		m_params = iParams; 
	};

	void *ThreadedProcess::GetParams() 
	{ 
		return m_params; 
	}	

	void ThreadedProcess::SetAsyncEnabled(const bool iFlag) 
	{ 
		m_asyncThreadedProcessingEnabled = iFlag; 
	}

	bool ThreadedProcess::GetAsyncEnabled() const 
	{ 
		return m_asyncThreadedProcessingEnabled; 
	}



DWORD WINAPI ThreadEntryPoint(void *iParentThreadedProcess)
{
	if (!iParentThreadedProcess) return -1;

	PGCore::ThreadedProcess* targetThreadPtr = reinterpret_cast<PGCore::ThreadedProcess* >(iParentThreadedProcess);    

	void *params = targetThreadPtr->GetParams();

	if (!params) return -1;

	PGCore::ThreadedProcess &tProcess = (*targetThreadPtr);
	
	tProcess.StartExecution(params);

	return 0;
};


} ;
