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

