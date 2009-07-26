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
