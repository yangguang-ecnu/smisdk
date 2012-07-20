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
// Filename:	Thread.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//
//
//////////////////////////////////////////////////////////////////////

#include "Core/Thread.h"

namespace PGCore
{	

	Thread::Thread()
	{		
		m_alive = false;
	}

	Thread::~Thread()
	{		
		CloseThread();
	}

	bool Thread::IsAlive()
	{
		return m_alive;
	}

	HANDLE Thread::CreateThread(PGCore::ThreadedProcess *iParentProcessPtr)
	{		
		void *iPtr = static_cast<void *>(iParentProcessPtr);  
		m_threadHandle = ::CreateThread(NULL,	// default security
			0,							// default stack size
			&ThreadEntryPoint,			// thread function
			iPtr,						// thread function argument: parent process ptr
			0,							// default creation flags
			0);							// don't want thread id

		m_alive = true;
		return m_threadHandle;
	}


	void Thread::WaitToFinish()
	{
		WaitForSingleObject( m_threadHandle, INFINITE );
		CloseThread();	
	}

	void Thread::CloseThread()
	{
		if (m_alive)
		{
			::CloseHandle(m_threadHandle);
			m_alive = false;
		}
	}	



};

