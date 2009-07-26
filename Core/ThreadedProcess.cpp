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
